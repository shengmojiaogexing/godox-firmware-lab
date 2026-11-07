"""X3-C 固件模拟机 · 浏览器调试界面后端（HTTP + 静态 webui，单端口）

用法:
    tools/python/python.exe devices/x3c/emu/web_server.py --port 18765
浏览器打开: http://127.0.0.1:18765

接口（与 webui/app.js / API.md 契约一致）:
    GET  /api/state    /api/disasm?addr=&n=   /api/mem?addr=&len=
    GET  /api/screen   /api/functions         /api/periph /api/trace /api/health
    POST /api/cmd      {cmd: run|pause|step|step_over|reset|bp_add|bp_del}
    GET  /             静态 webui
"""
import argparse
import csv
import json
import os
import struct
import sys
import threading
import time
import traceback
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import parse_qs, urlparse

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

from machine import Machine  # noqa: E402
from demo_x3 import LcdBus, patch_option_bytes, patch_adc, patch_delays, map_xmc  # noqa: E402

from unicorn import UC_HOOK_MEM_WRITE  # noqa: E402
from unicorn.arm_const import UC_ARM_REG_XPSR  # noqa: E402

IMAGE = os.path.join(HERE, "..", "firmware", "X3_C_v1.38_app.bin")
FUNCTIONS_CSV = os.path.join(HERE, "..", "re", "ghidra_exports", "main", "functions.csv")
WEBUI = os.path.join(HERE, "webui")

W, H = 368, 448              # 面板原生 448x368，固件按 368x448 竖屏开窗
CHUNK = 100_000
MAX_LOG = 200


def load_functions():
    out = []
    try:
        with open(FUNCTIONS_CSV, newline="", encoding="utf-8") as f:
            for row in csv.DictReader(f):
                a = int(row["addr"], 0)
                size = int(row.get("size") or 0)
                out.append((a, row.get("name") or ("sub_%08x" % a), a + max(size, 2)))
    except Exception as e:  # noqa: BLE001
        print("functions.csv load failed:", e, flush=True)
    out.sort()
    return out


FUNCS = load_functions()
FUNC_ADDRS = [f[0] for f in FUNCS]


def func_at(addr):
    import bisect
    a = addr & ~1
    i = bisect.bisect_right(FUNC_ADDRS, a) - 1
    if 0 <= i < len(FUNCS):
        return FUNCS[i]
    return None


class X3Service:
    def __init__(self):
        self.lock = threading.RLock()
        self.running = False
        self.stop_flag = False
        self.worker = None
        self.run_started = 0.0
        self.log = []
        self._new_machine()

    # ---------------------------------------------------------------- setup
    def _new_machine(self):
        m = Machine(image=IMAGE, log_fn=lambda s: None, trace=True)
        patch_option_bytes(m)
        patch_adc(m, 2600)
        patch_delays(m)
        map_xmc(m)
        bus = LcdBus(W, H)
        self.machine = m
        self.bus = bus

        def wr(uc, access, address, size, value, user):
            if address == 0x6C000000:
                bus.write_addr0(value & 0xFF)
            elif address == 0x6C000001:
                bus.write_addr1(value & 0xFF)

        m.uc.hook_add(UC_HOOK_MEM_WRITE, wr, begin=0x6C000000, end=0x6C000002)
        m.boot()
        self.frames = 0
        self.log_line("reset: pc=0x%08X sp=0x%08X (app base 0x08004800)"
                      % (m.reg("PC"), m.reg("SP")))

    def log_line(self, text):
        self.log.append("[%s] %s" % (time.strftime("%H:%M:%S"), text))
        if len(self.log) > MAX_LOG:
            del self.log[:-MAX_LOG]

    # ---------------------------------------------------------------- state
    def state(self):
        with self.lock:
            m = self.machine
            bus = self.bus
            try:
                xpsr = m.uc.reg_read(UC_ARM_REG_XPSR)
            except Exception:
                xpsr = 0
            f = func_at(m.reg("PC"))
            cfsr = hfsr = 0
            try:
                cfsr = int.from_bytes(bytes(m.uc.mem_read(0xE000ED28, 4)), "little")
                hfsr = int.from_bytes(bytes(m.uc.mem_read(0xE000ED2C, 4)), "little")
            except Exception:
                pass
            faults = []
            if cfsr or hfsr:
                faults.append({"type": "HardFault", "pc": m.reg("PC"),
                               "text": "CFSR=0x%08X HFSR=0x%08X" % (cfsr, hfsr)})
            frames = bus.n_pixels // (W * H)
            return dict(
                pc=m.reg("PC"), sp=m.reg("SP"), lr=m.reg("LR"), xpsr=xpsr,
                regs=[m.reg("R%d" % i) for i in range(13)] + [m.reg("SP"), m.reg("LR"), m.reg("PC")],
                insnCount=m.insn_count,
                running=self.running,
                breakpoints=sorted(m.breakpoints),
                log=self.log[-60:],
                faults=faults,
                uart="", uartLen=0,
                func=(dict(addr=f[0], name=f[1]) if f else None),
                callstack=[dict(addr=f[0], name=f[1], return_pc=m.reg("LR"))] if f else [],
                lcd=dict(w=W, h=H, format="rgb565", addr=None, bytes_written=bus.n_data,
                         frames=frames),
                stats=dict(executed=m.insn_count,
                           runTime=round(time.time() - self.run_started, 3) if self.running else 0.0),
            )

    def screen(self):
        with self.lock:
            bus = self.bus
            # UI 按 RGB565 大端 (hi,lo) 解释帧数据；LcdBus 内部按小端存放，这里换序。
            fb = bytes(bus.fb)
            be = bytearray(len(fb))
            be[0::2] = fb[1::2]
            be[1::2] = fb[0::2]
            frames = bus.n_pixels // (W * H)
            return dict(
                w=W, h=H, format="rgb565", addr=0,
                hex=bytes(be).hex(),
                info=dict(w=W, h=H, format="rgb565", bytes_written=bus.n_data, frames=frames),
            )

    def disasm(self, addr, n=61):
        n = max(4, min(int(n), 200))
        addr &= ~1
        try:
            data = m_read(self.machine, addr, n * 4 + 8)
        except Exception as e:  # noqa: BLE001
            return dict(addr=addr, lines=[], error=str(e))
        import capstone
        md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)
        lines = []
        for ins in md.disasm(bytes(data), addr):
            lines.append(dict(addr=ins.address, bytes=ins.bytes.hex(),
                              mnem=ins.mnemonic, op=ins.op_str))
            if len(lines) >= n:
                break
        return dict(addr=addr, lines=lines)

    def mem(self, addr, length=256):
        length = max(1, min(int(length), 0x10000))
        try:
            data = m_read(self.machine, addr, length)
        except Exception as e:  # noqa: BLE001
            return dict(addr=addr, hex="", len=0, error=str(e))
        return dict(addr=addr, hex=data.hex(), len=length,
                    ascii="".join(chr(c) if 32 <= c < 127 else "." for c in data))

    def functions(self):
        return [dict(addr=a, end=b, size=b - a, name=n) for a, n, b in FUNCS]

    # ---------------------------------------------------------------- commands
    def cmd(self, payload):
        cmd = (payload.get("cmd") or "").strip().lower()
        with self.lock:
            if cmd == "run":
                count = payload.get("count") or payload.get("n") or None
                return self.start_run(int(count) if count else None)
            if cmd == "pause":
                return self.pause()
            if cmd == "step":
                n = int(payload.get("n") or payload.get("count") or 1)
                self.stop_worker()
                m = self.machine
                m.run(min(max(n, 1), 200000))
                self.log_line("step %d -> pc=0x%08X insn=%d" % (n, m.reg("PC"), m.insn_count))
                return dict(ok=True, pc=m.reg("PC"), insn=m.insn_count)
            if cmd == "step_over":
                self.stop_worker()
                self.machine.step_over()
                return dict(ok=True, pc=self.machine.reg("PC"), insn=self.machine.insn_count)
            if cmd == "reset":
                self.stop_worker()
                self._new_machine()
                self.log_line("reset done")
                return dict(ok=True, pc=self.machine.reg("PC"), insn=0)
            if cmd in ("bp_add", "bp_del", "breakpoint_add", "breakpoint_del"):
                addrs = payload.get("addrs") or payload.get("addr")
                if isinstance(addrs, (int, str)):
                    addrs = [addrs]
                elif addrs is None and payload.get("bp") is not None:
                    addrs = [payload["bp"]]
                out = []
                for a in addrs or []:
                    a = int(a, 0) if isinstance(a, str) else int(a)
                    a &= ~1
                    if cmd in ("bp_add", "breakpoint_add"):
                        self.machine.breakpoints[a] = "bp"
                    else:
                        self.machine.breakpoints.pop(a, None)
                    out.append(a)
                self.log_line("%s %s" % (cmd, ["0x%08X" % a for a in out]))
                return dict(ok=True, breakpoints=sorted(self.machine.breakpoints), changed=out)
        return dict(ok=False, error="unknown cmd: %r" % cmd)

    # ---------------------------------------------------------------- run loop
    def start_run(self, count=None):
        self.stop_worker()
        self.stop_flag = False
        self.running = True
        self.run_started = time.time()
        self.log_line("run start count=%s pc=0x%08X" % (count, self.machine.reg("PC")))
        self.worker = threading.Thread(target=self._run_loop, args=(count,), daemon=True)
        self.worker.start()
        return dict(ok=True, running=True, count=count, pc=self.machine.reg("PC"))

    def stop_worker(self):
        self.stop_flag = True
        w = self.worker
        if w is not None and w.is_alive():
            w.join(timeout=10)
        self.worker = None
        self.running = False

    def pause(self):
        self.stop_worker()
        self.log_line("paused at pc=0x%08X insn=%d"
                      % (self.machine.reg("PC"), self.machine.insn_count))
        return dict(ok=True, running=False, pc=self.machine.reg("PC"),
                    insn=self.machine.insn_count)

    def _run_loop(self, count):
        remaining = count
        try:
            while not self.stop_flag:
                n = CHUNK if remaining is None else min(CHUNK, remaining)
                with self.lock:
                    before = self.machine.insn_count
                    self.machine.run(n)
                    executed = self.machine.insn_count - before
                if executed <= 0:
                    break
                if remaining is not None:
                    remaining -= executed
                    if remaining <= 0:
                        break
        except Exception as e:  # noqa: BLE001
            traceback.print_exc()
            self.log_line("run error: %s" % e)
        finally:
            self.running = False
            self.log_line("run end pc=0x%08X insn=%d"
                          % (self.machine.reg("PC"), self.machine.insn_count))


def m_read(machine, addr, n):
    """Read memory through the emulator (works for flash/RAM, auto-maps gaps)."""
    try:
        return bytes(machine.uc.mem_read(addr, n))
    except Exception:
        out = bytearray()
        for i in range(n):
            try:
                out += bytes(machine.uc.mem_read(addr + i, 1))
            except Exception:
                out.append(0)
        return bytes(out)


MIME = {".html": "text/html; charset=utf-8", ".js": "application/javascript; charset=utf-8",
        ".css": "text/css; charset=utf-8", ".json": "application/json; charset=utf-8",
        ".png": "image/png", ".svg": "image/svg+xml", ".ico": "image/x-icon"}

SERVICE = None


class Handler(BaseHTTPRequestHandler):
    server_version = "X3C-Sim/1.0"
    protocol_version = "HTTP/1.1"

    def _cors(self):
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type, Authorization")
        self.send_header("Access-Control-Max-Age", "86400")

    def _json(self, obj, status=200):
        body = json.dumps(obj, ensure_ascii=False, default=str).encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-store")
        self._cors()
        self.end_headers()
        try:
            self.wfile.write(body)
        except (BrokenPipeError, ConnectionResetError):
            pass

    def _static(self, path):
        if path in ("/", ""):
            path = "/index.html"
        rel = path.lstrip("/")
        full = os.path.normpath(os.path.join(WEBUI, rel))
        if not full.startswith(WEBUI) or not os.path.isfile(full):
            return False
        data = open(full, "rb").read()
        ext = os.path.splitext(full)[1].lower()
        self.send_response(200)
        self.send_header("Content-Type", MIME.get(ext, "application/octet-stream"))
        self.send_header("Content-Length", str(len(data)))
        self.send_header("Cache-Control", "no-store")
        self._cors()
        self.end_headers()
        try:
            self.wfile.write(data)
        except (BrokenPipeError, ConnectionResetError):
            pass
        return True

    def log_message(self, fmt, *args):
        if os.environ.get("X3_SIM_HTTP_LOG"):
            sys.stderr.write("[http] %s\n" % (fmt % args))

    def do_OPTIONS(self):  # noqa: N802
        self.send_response(204)
        self._cors()
        self.send_header("Content-Length", "0")
        self.end_headers()

    def _params(self):
        q = parse_qs(urlparse(self.path).query)
        return {k: (v[0] if len(v) == 1 else v) for k, v in q.items()}

    def do_GET(self):  # noqa: N802
        u = urlparse(self.path)
        p = u.path.rstrip("/") or "/"
        q = self._params()
        try:
            if p == "/api/health":
                return self._json(dict(ok=True, service="X3C-Sim", ts=time.time()))
            if p == "/api/state":
                return self._json(SERVICE.state())
            if p == "/api/screen":
                return self._json(SERVICE.screen())
            if p == "/api/disasm":
                addr = int(q.get("addr", "0"), 0)
                n = int(q.get("n", q.get("count", 61)))
                return self._json(SERVICE.disasm(addr, n))
            if p == "/api/mem":
                addr = int(q.get("addr", "0"), 0)
                length = int(q.get("len", q.get("n", 256)))
                return self._json(SERVICE.mem(addr, length))
            if p == "/api/functions":
                return self._json(SERVICE.functions())
            if p == "/api/periph":
                return self._json(dict(state={}, summary={}))
            if p == "/api/trace":
                return self._json(dict(entries=[]))
            if p.startswith("/api/"):
                return self._json(dict(error="not found", path=p), 404)
            if self._static(u.path):
                return
            return self._json(dict(error="not found"), 404)
        except Exception as e:  # noqa: BLE001
            traceback.print_exc()
            return self._json(dict(error="%s: %s" % (type(e).__name__, e)), 500)

    def do_POST(self):  # noqa: N802
        u = urlparse(self.path)
        p = u.path.rstrip("/") or "/"
        try:
            n = int(self.headers.get("Content-Length") or 0)
            raw = self.rfile.read(n) if n else b"{}"
            payload = json.loads(raw.decode("utf-8") or "{}")
        except Exception:
            payload = {}
        try:
            if p in ("/api/cmd", "/api/command"):
                return self._json(SERVICE.cmd(payload))
            if p == "/api/pause":
                return self._json(SERVICE.pause())
            return self._json(dict(ok=False, error="not found"), 404)
        except Exception as e:  # noqa: BLE001
            traceback.print_exc()
            return self._json(dict(ok=False, error="%s: %s" % (type(e).__name__, e)), 500)


def main():
    global SERVICE
    ap = argparse.ArgumentParser()
    ap.add_argument("--host", default="127.0.0.1")
    ap.add_argument("--port", type=int, default=18765)
    ap.add_argument("--no-autorun", action="store_true")
    args = ap.parse_args()
    SERVICE = X3Service()
    if not args.no_autorun:
        # 自动跑到开机画面：约 8M 条指令后 LCD 完成一帧
        SERVICE.start_run(8_000_000)
    httpd = ThreadingHTTPServer((args.host, args.port), Handler)
    httpd.daemon_threads = True
    print("X3-C simulator web UI: http://%s:%d" % (args.host, args.port), flush=True)
    print("  webui: %s" % WEBUI, flush=True)
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
