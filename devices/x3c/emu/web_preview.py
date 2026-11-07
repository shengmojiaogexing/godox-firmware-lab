"""X3-C 固件调试模拟机 · 浏览器可视化预览

启动方式:
    tools/python/python.exe devices/x3c/emu/web_preview.py --port 18080

然后浏览器打开 http://127.0.0.1:18080
页面内容:
  * 实时显示模拟机从 LCD 总线 (XMC 8080 @0x6C000000) 还原的 368x448 屏幕
  * 指令数 / PC / LR / LCD 命令数 / 像素数 实时状态
  * 控制: 暂停 / 继续 / 重置 / 单步(5万条指令) / 快进到开机画面 / 1ms 节拍注入(实验)
页面 400ms 刷新一次画面。

架构: 单线程 Unicorn 执行 + 一个 HTTP 线程只读取已发布的快照。
"""
import argparse
import json
import os
import struct
import sys
import threading
import time
import zlib
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

from machine import Machine  # noqa: E402

IMAGE = os.path.join(HERE, "..", "firmware", "X3_C_v1.38_app.bin")
OUT = os.path.join(HERE, "lcd_out")

from demo_x3 import LcdBus, patch_option_bytes, patch_adc, patch_delays, map_xmc  # noqa: E402

from unicorn import UC_HOOK_MEM_WRITE  # noqa: E402


class Simulator:
    def __init__(self, raw_adc=2600, tick_mode=False):
        self.lock = threading.Lock()
        self.raw_adc = raw_adc
        self.tick_mode = tick_mode
        self.running = True
        self.insn = 0
        self.pc = 0
        self.lr = 0
        self.cmds = []
        self.n_pixels = 0
        self.n_data = 0
        self.window = (0, 0, 0, 0)
        self.frame_png = b""
        self.frame_seq = 0
        self.last_frame_ts = 0.0
        self.stop = False
        self.reset_pending = False
        self.tick = 0
        self.status = "初始化"
        self._machine = None
        self._bus = None
        self._build()
        self.thread = threading.Thread(target=self._worker, daemon=True)
        self.thread.start()

    # ------------------------------------------------------------------ setup
    def _lcd_hook(self):
        bus = self._bus

        def wr(uc, access, address, size, value, user):
            if address == 0x6C000000:
                bus.write_addr0(value & 0xFF)
            elif address == 0x6C000001:
                bus.write_addr1(value & 0xFF)
        return wr

    def _build(self):
        m = Machine(image=IMAGE, log_fn=lambda s: None, trace=True)
        patch_option_bytes(m)
        patch_adc(m, self.raw_adc)
        patch_delays(m)
        map_xmc(m)
        self._bus = LcdBus()
        m.uc.hook_add(UC_HOOK_MEM_WRITE, self._lcd_hook(),
                      begin=0x6C000000, end=0x6C000002)
        m.boot()
        self._machine = m
        with self.lock:
            self.insn = 0
            self.pc = m.reg("PC")
            self.lr = m.reg("LR")
            self.cmds = []
            self.n_pixels = 0
            self.n_data = 0
            self.tick = 0
            self.status = "已复位，正在启动"
            self._publish_frame(force=True)

    # ------------------------------------------------------------------ worker
    def _worker(self):
        chunk = 200_000
        while not self.stop:
            if self.reset_pending:
                self.reset_pending = False
                self._build()
                self.running = True
            if not self.running:
                time.sleep(0.05)
                continue
            try:
                self._machine.run(chunk)
            except Exception as e:  # noqa: BLE001 - surface to UI
                with self.lock:
                    self.status = "模拟异常: %s" % e
                    self.running = False
                continue
            bus = self._bus
            with self.lock:
                self.insn = self._machine.insn_count
                self.pc = self._machine.reg("PC")
                self.lr = self._machine.reg("LR")
                self.cmds = list(bus.cmds[-64:])
                self.n_pixels = bus.n_pixels
                self.n_data = bus.n_data
                self.window = (bus.xs, bus.xe, bus.ys, bus.ye)
            if self.tick_mode:
                # 实验性: 手动推进固件的 1ms 节拍 (TMR7/IRQ55 尚未建模)
                self.tick += 1
                try:
                    self._machine.uc.mem_write(0x20000310, struct.pack("<I", self.tick))
                    self._machine.uc.mem_write(0x2000030C, b"\x00")
                except Exception:
                    pass
                with self.lock:
                    self.status = "运行中（节拍注入 tick=%d）" % self.tick
            elif self.insn > 7_000_000 and self.n_pixels:
                with self.lock:
                    self.status = "已到开机画面（真实固件执行 7M+ 指令）"
            self._publish_frame()

    def _publish_frame(self, force=False):
        bus = self._bus
        now = time.time()
        if not force and now - self.last_frame_ts < 0.4:
            return
        png = png_bytes(bus, self.insn)
        with self.lock:
            self.frame_png = png
            self.frame_seq += 1
            self.last_frame_ts = now

    # ------------------------------------------------------------------ control
    def snapshot(self):
        with self.lock:
            return {
                "insn": self.insn,
                "pc": self.pc,
                "lr": self.lr,
                "cmds": self.cmds,
                "n_cmds": len(self.cmds),
                "n_pixels": self.n_pixels,
                "n_data": self.n_data,
                "window": self.window,
                "running": self.running,
                "status": self.status,
                "tick_mode": self.tick_mode,
                "tick": self.tick,
                "raw_adc": self.raw_adc,
                "frame_seq": self.frame_seq,
            }

    def control(self, action):
        if action == "pause":
            self.running = False
        elif action == "play":
            self.running = True
        elif action == "step":
            self._machine.run(50_000)
        elif action == "reset":
            self.reset_pending = True
        elif action == "fast":
            # run ahead until the boot screen has been painted (bounded)
            self.running = True
        elif action == "tick_on":
            self.tick_mode = True
        elif action == "tick_off":
            self.tick_mode = False
        elif action.startswith("adc_"):
            try:
                self.raw_adc = int(action[4:])
                self.reset_pending = True
            except ValueError:
                pass


def png_bytes(bus, tag=0):
    w, h = bus.w, bus.h
    rgb = bytearray(w * h * 3)
    fb = bus.fb
    for i in range(0, len(fb), 2):
        v = fb[i] | (fb[i + 1] << 8)
        o = (i // 2) * 3
        rgb[o] = ((v >> 11) & 0x1F) << 3
        rgb[o + 1] = ((v >> 5) & 0x3F) << 2
        rgb[o + 2] = (v & 0x1F) << 3
    raw = bytearray()
    for y in range(h):
        raw.append(0)
        raw += rgb[y * w * 3:(y + 1) * w * 3]

    def chunk(tag_, data):
        return (struct.pack(">I", len(data)) + tag_ + data
                + struct.pack(">I", zlib.crc32(tag_ + data) & 0xFFFFFFFF))

    return (b"\x89PNG\r\n\x1a\n"
            + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0))
            + chunk(b"IDAT", zlib.compress(bytes(raw), 3))
            + chunk(b"IEND", b""))


PAGE = """<!DOCTYPE html>
<html lang="zh-CN"><head><meta charset="utf-8">
<title>Godox X3-C v1.38 固件模拟机 · 预览</title>
<style>
:root { color-scheme: dark; }
body { margin:0; background:#0d0f12; color:#e6e6e6; font:14px/1.5 "Segoe UI",system-ui,sans-serif; }
.wrap { display:flex; gap:24px; padding:20px 26px; max-width:1180px; margin:0 auto; }
.screen { flex:0 0 auto; background:#000; border:1px solid #2a2f37; border-radius:14px; padding:10px;
          box-shadow:0 10px 40px #0008; }
.screen img { display:block; width:368px; height:448px; border-radius:8px; image-rendering:pixelated; background:#000; }
.panel { flex:1; min-width:340px; }
h1 { font-size:18px; margin:0 0 4px; letter-spacing:.5px; }
.sub { color:#8b93a1; font-size:12px; margin-bottom:16px; }
.grid { display:grid; grid-template-columns:1fr 1fr; gap:10px; margin-bottom:16px; }
.card { background:#151a21; border:1px solid #232a34; border-radius:10px; padding:10px 12px; }
.card .k { color:#7f8999; font-size:11px; text-transform:uppercase; letter-spacing:.08em; }
.card .v { font-size:16px; font-family:Consolas,monospace; margin-top:2px; word-break:break-all; }
.btns { display:flex; flex-wrap:wrap; gap:8px; margin-bottom:14px; }
button { background:#1d2530; color:#dfe6f0; border:1px solid #2c3644; padding:8px 14px; border-radius:8px;
         cursor:pointer; font-size:13px; }
button:hover { background:#273241; }
button.primary { background:#e8641f; border-color:#e8641f; color:#120b06; font-weight:600; }
button.ghost { background:transparent; }
.log { background:#0a0d11; border:1px solid #1d232b; border-radius:10px; padding:10px 12px;
       font:12px/1.6 Consolas,monospace; color:#9fb4c8; max-height:170px; overflow:auto; white-space:pre-wrap; }
.badge { display:inline-block; padding:2px 8px; border-radius:20px; font-size:11px; margin-left:8px; }
.run { background:#173d24; color:#69d58c; }
.stop { background:#3d1717; color:#e88; }
a { color:#e8a05f; }
</style></head>
<body><div class="wrap">
  <div class="screen"><img id="shot" src="/frame.png" alt="LCD"></div>
  <div class="panel">
    <h1>Godox X3-C (X3 C) v1.38 · 固件调试模拟机</h1>
    <div class="sub">AT32F435 · 应用基址 0x08004800 · LCD 448&times;368 RGB565 · XMC 8080 @0x6C000000
      <span id="state" class="badge stop">…</span></div>
    <div class="grid">
      <div class="card"><div class="k">已执行指令</div><div class="v" id="insn">0</div></div>
      <div class="card"><div class="k">PC / LR</div><div class="v" id="pclr">-</div></div>
      <div class="card"><div class="k">LCD 命令数</div><div class="v" id="ncmds">0</div></div>
      <div class="card"><div class="k">已接收像素</div><div class="v" id="npix">0</div></div>
      <div class="card"><div class="k">显示窗口</div><div class="v" id="win">-</div></div>
      <div class="card"><div class="k">状态</div><div class="v" id="status" style="font-size:13px">初始化</div></div>
    </div>
    <div class="btns">
      <button class="primary" onclick="act('play')">▶ 继续</button>
      <button onclick="act('pause')">⏸ 暂停</button>
      <button onclick="act('step')">⏭ 单步 5 万条</button>
      <button onclick="act('reset')">⟲ 重置</button>
      <button class="ghost" onclick="act('tick_on')">注入 1ms 节拍(实验)</button>
      <button class="ghost" onclick="act('tick_off')">关闭节拍注入</button>
      <button class="ghost" onclick="act('adc_2600')">电池 ADC=2600</button>
      <button class="ghost" onclick="act('adc_3300')">电池 ADC=3300</button>
    </div>
    <div class="log" id="log">LCD 命令: (等待中)</div>
    <div class="sub" style="margin-top:12px">
      画面为模拟机解码 LCD 8080 总线后的真实还原（非示意图）。
      1ms 节拍注入用于绕过尚未建模的 TMR7(IRQ55)，实验性。
      截图: <a href="/frame.png" target="_blank">打开当前帧 PNG</a>
    </div>
  </div>
</div>
<script>
let lastSeq = -1;
async function tick() {
  try {
    const s = await (await fetch('/state.json')).json();
    document.getElementById('insn').textContent = s.insn.toLocaleString();
    document.getElementById('pclr').textContent = '0x' + s.pc.toString(16) + ' / 0x' + s.lr.toString(16);
    document.getElementById('ncmds').textContent = s.n_cmds;
    document.getElementById('npix').textContent = s.n_pixels.toLocaleString();
    document.getElementById('win').textContent = 'x ' + s.window[0] + '..' + s.window[1] + '  y ' + s.window[2] + '..' + s.window[3];
    document.getElementById('status').textContent = s.status;
    const b = document.getElementById('state');
    b.textContent = s.running ? '运行中' : '已暂停';
    b.className = 'badge ' + (s.running ? 'run' : 'stop');
    if (s.cmds && s.cmds.length) {
      document.getElementById('log').textContent = 'LCD 命令: ' + s.cmds.map(c => c.toString(16).padStart(2,'0')).join(' ');
    }
    if (s.frame_seq !== lastSeq) {
      lastSeq = s.frame_seq;
      document.getElementById('shot').src = '/frame.png?t=' + Date.now();
    }
  } catch (e) {}
  setTimeout(tick, 500);
}
tick();
async function act(a) { await fetch('/control?action=' + a); }
</script></body></html>
"""


class Handler(BaseHTTPRequestHandler):
    sim: Simulator = None  # set in main

    def log_message(self, fmt, *args):  # quiet
        pass

    def _send(self, code, body, ctype="text/plain; charset=utf-8", extra=None):
        self.send_response(code)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-store")
        for k, v in (extra or {}).items():
            self.send_header(k, v)
        self.end_headers()
        self.wfile.write(body)

    def do_GET(self):  # noqa: N802
        path = self.path.split("?")[0]
        if path == "/":
            return self._send(200, PAGE.encode("utf-8"), "text/html; charset=utf-8")
        if path == "/frame.png":
            with self.sim.lock:
                png = self.sim.frame_png
                seq = self.sim.frame_seq
            return self._send(200, png, "image/png", {"X-Frame-Seq": str(seq)})
        if path == "/state.json":
            body = json.dumps(self.sim.snapshot(), ensure_ascii=False).encode("utf-8")
            return self._send(200, body, "application/json; charset=utf-8")
        if path == "/control":
            from urllib.parse import parse_qs, urlparse
            q = parse_qs(urlparse(self.path).query)
            action = (q.get("action") or [""])[0]
            self.sim.control(action)
            return self._send(200, b"ok")
        return self._send(404, b"not found")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--port", type=int, default=18080)
    ap.add_argument("--host", default="127.0.0.1")
    ap.add_argument("--adc-raw", type=int, default=2600)
    args = ap.parse_args()
    os.makedirs(OUT, exist_ok=True)
    sim = Simulator(raw_adc=args.adc_raw)
    Handler.sim = sim
    srv = ThreadingHTTPServer((args.host, args.port), Handler)
    print("X3-C firmware simulator preview: http://%s:%d" % (args.host, args.port), flush=True)
    try:
        srv.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        sim.stop = True
        sim.running = False


if __name__ == "__main__":
    main()
