# -*- coding: utf-8 -*-
"""AD200 Pro 固件验证脚手架 —— 公共库（整机模拟器 sim.machine 之上）。

本目录只放验证脚本，不改 devices/ad200pro 源码；需要模拟器时：
    sys.path.insert(0, r'devices/ad200pro')
    from sim.machine import Machine

统一约定（地址来自 keypatch/dev/ad200_ui/BRIEF.md 与 devices/ad200pro/docs）：
  * 镜像基址 0x08003000；复位 PC 0x080030D4；主循环区 [0x0800CE40,0x0800CEB0)
  * UI 结构 0x20000150；UI+0x12(0x20000162)=C.Fn 打开标志；UI+0x13(0x20000163)=编辑/渲染态；
    UI+0x14(0x20000164)=C.Fn 当前项 0..7；UI+0x15/0x16 = 计数/编辑态
  * settings[0..7] = 0x20000418..0x2000041F；settings[7] = 0x2000041F = F8
  * 段码位图 0x20000408 起 16 字节（0x0800CCAC(value,bitpos) 逐段置位）
  * C.Fn 渲染函数 0x08003FD0
  * 参数记录页 0x0801F000 + slot*100，记录 32 字节，字节 0=magic 0x5C，字节 24..31=settings[0..7]
  * 保存 0x0800B820(slot)；加载 0x0800AC58；记录构建 0x080043E4；flash 写 0x08005974

所有脚本仅依赖标准库 + devices/ad200pro 自带的 unicorn/capstone（用其 python.exe）。
"""
import os
import struct
import sys
import time

# ---------------------------------------------------------------- 路径/常量
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))))
ADROOT = os.path.join(ROOT, "devices/ad200pro")
SIM_DIR = os.path.dirname(os.path.abspath(__file__))
if ADROOT not in sys.path:
    sys.path.insert(0, ADROOT)

ORIG_CANDIDATES = (
    os.path.join(ROOT, "devices", "x3c", "firmware", "AD200pro_V1.60.bin"),
    os.path.join(ADROOT, "samples", "AD200pro_V1.60.bin"),
)
PATCHED_DEFAULT = os.path.join(ROOT, "keypatch", "out", "AD200pro_V1.60_keyD798.bin")

BASE = 0x08003000
RESET_PC = 0x080030D4
LOOP_LO, LOOP_HI = 0x0800CE40, 0x0800CEB0
MAIN = 0x0800CDB0

UI = 0x20000150
UI_OPEN = UI + 0x12          # 0x20000162
UI_EDIT = UI + 0x13          # 0x20000163
UI_INDEX = UI + 0x14         # 0x20000164
UI_COUNT = UI + 0x15         # 0x20000165
UI_EDIT2 = UI + 0x16         # 0x20000166
SETTINGS = 0x20000418
SETTINGS7 = SETTINGS + 7
BITMAP = 0x20000408
PARAM_MIRROR = 0x20000464
RECORD_SRC = 0x20000484
FLASH_PARAM_PAGE = 0x0801F000
FLASH_FLAG_PAGE = 0x0801F800
FLASH_SIZE = 0x20000
RAM_SIZE = 0x2000

FUNC_RENDER = 0x08003FD0
FUNC_BITMAP_SET = 0x0800CCAC
FUNC_BUILD_RECORD = 0x080043E4
FUNC_STORE = 0x0800B820
FUNC_FLASH_WRITE = 0x08005974
FUNC_FLASH_READ = 0x0800590C
FUNC_LOAD = 0x0800AC58
FUNC_EXPAND = 0x0800CCC8
FUNC_KEYSCAN = 0x08008818
FUNC_SYNC = 0x0800B9AC
FUNC_TIM14_IRQ = 0x0800BDE0

FONT_TABLE = 0x0800D01C

# 10 个字段渲染函数（均渲染一个字符到 16 字节位图；调用约定 (code, bitpos)）
FIELD_FUNCS = (
    0x08008DEC, 0x08008E58, 0x08008EC4, 0x08008F34, 0x08008FA0,
    0x0800900C, 0x08009078, 0x080090E4, 0x08009150, 0x080091BC,
)

# 调用固件函数时用的返回哨兵（RAM 高端空闲区；只作 until_pc 停点，不执行）
SENTINEL = 0x20001F00
_SENTINEL_CODE = b"\xfe\xe7"      # b .（占位）

# 按键矩阵：3x3，行 PB3/PB4/PB5，列 PC10/PC11/PC12（docs/05-pinmap.md §3）
KEY_ROW_PINS = (3, 4, 5)
KEY_COL_PINS = (10, 11, 12)
GPIOB_BASE = 0x48000400
GPIOC_BASE = 0x48000800
# 9 个键位码 bit = 3*row + col（ui_key_scan_task 拼装）
KEY_CODES = {(r, c): 1 << (3 * r + c) for r in range(3) for c in range(3)}
# 实测（本脚手架 key_probe / cfn_probe --keys）：row2,col2 (code 0x100) = 进入 C.Fn 的键
KEY_CFN = (2, 2)
KEY_CFN_CODE = 0x100
# 实测 row1,col2 (code 0x20) 在 C.Fn 打开时切换 UI+0x13（编辑态）
KEY_EDIT = (1, 2)
KEY_EDIT_CODE = 0x20


def ensure_stdout_utf8():
    """Windows 控制台/重定向统一 UTF-8，避免 cp936 报错。"""
    for stream in (sys.stdout, sys.stderr):
        try:
            stream.reconfigure(encoding="utf-8")
        except Exception:  # noqa: BLE001
            pass


def resolve_orig(path=None):
    if path:
        return os.path.abspath(path)
    for p in ORIG_CANDIDATES:
        if os.path.isfile(p):
            return p
    raise FileNotFoundError("找不到原厂固件，候选：%r" % (ORIG_CANDIDATES,))


def sha256_hex(data):
    import hashlib
    return hashlib.sha256(data).hexdigest()


def read_file_bytes(path):
    with open(path, "rb") as f:
        return f.read()


def check_machine_deps():
    """给用户一个清晰的错误提示（而不是 ImportError traceback）。"""
    try:
        from sim.machine import Machine  # noqa: F401
    except Exception as exc:  # noqa: BLE001
        raise SystemExit(
            "无法 import sim.machine：%s\n请用 devices/ad200pro 自带解释器运行：\n"
            "    devices\ad200pro\\python\\python.exe %s"
            % (exc, os.path.abspath(sys.argv[0])))


# ---------------------------------------------------------------- 基本工具
def load_machine(path=None):
    check_machine_deps()
    from sim.machine import Machine
    return Machine(resolve_orig(path) if path is None else os.path.abspath(path))


def read_bytes(m, addr, size):
    return bytes(m.read(addr, size))


def read_u8(m, addr):
    return read_bytes(m, addr, 1)[0]


def hexs(data, sep=" "):
    return sep.join("%02X" % b for b in data)


def read_symbol_name(m, addr):
    n = m.fw.name(addr)
    return n or ("0x%08X" % addr)


def item_label_text(index):
    """C.Fn 第 index 项（0 基）→ 显示名 F1..F8。"""
    return "F%d" % (index + 1)


# ---------------------------------------------------------------- 字体/段码
# 字体表 0x0800D01C 的字节位 → 7 段（由数字 0..9 字模反推；bit4 未使用）
_FONT_BIT_SEG = {0: "d", 1: "c", 2: "b", 3: "a", 5: "e", 6: "g", 7: "f"}
# 段集合 → 可读字符（用于把 field 渲染调用翻译成人能读的字符）
_GLYPHS = {
    "abcdef": "0", "bc": "1", "abdeg": "2", "abcdg": "3", "bcfg": "4",
    "acdfg": "5", "acdefg": "6", "abc": "7", "abcdefg": "8", "abcdfg": "9",
    "abcefg": "A", "cdefg": "b", "adef": "C", "bcdeg": "d", "adefg": "E",
    "aefg": "F", "ceg": "n", "cdeg": "o", "def": "L", "abcef": "N",
    "g": "-", "": " ",
}


def decode_font_codes(m):
    """读固件字体表，返回 {code: 可读字符/None}。"""
    out = {}
    for code in range(32):
        font = read_u8(m, FONT_TABLE + code)
        segs = "".join(_FONT_BIT_SEG[b] for b in range(8) if (font >> b) & 1)
        out[code] = _GLYPHS.get("".join(sorted(segs)))
    return out


# ---------------------------------------------------------------- 启动到主循环
def boot_to_main(m, budget=5_000_000, exact_insns=False, until_pc=LOOP_LO,
                 do_reset=True):
    """复位并跑到主循环入口 `until_pc`（默认 0x0800CE40）。

    exact_insns=True 时把 max_slice_rate 临时设为 1（单步）以获得精确指令数，
    与 keypatch/dev/ad200/sim_regress.py 的做法一致；会慢一些（~8 万次单步）。
    do_reset=False 用于"上电前预置 flash 内容"的掉电重上电测试（不再清 flash）。
    返回 dict：machine / insns / pc / state / reason / wall。
    """
    if do_reset:
        m.reset()
    t0 = time.time()
    saved_rate = m.max_slice_rate
    if exact_insns:
        # 单步 + breakpoint：精确计数（~8 万次单步，约 0.8s）
        m.max_slice_rate = 1
        m.set_breakpoint(until_pc)
        try:
            reason = m.run(max_insns=budget)
        finally:
            m.clear_breakpoint(until_pc)
            m.max_slice_rate = saved_rate
    else:
        # 分片 + 主循环区间判定（同 test_boot.py 口径），快很多
        reason = None
        total = 0
        while total < budget:
            prev = m.insn_count
            reason = m.run(max_insns=min(5000 if total < 200_000 else 2000,
                                         budget - total))
            delta = m.insn_count - prev
            total += delta if delta > 0 else 2000
            pc = m.regs()["pc"] & ~1
            if until_pc <= pc < until_pc + (LOOP_HI - LOOP_LO):
                reason = "loop_zone"
                break
            if m.state in ("fault", "stalled"):
                break
    wall = time.time() - t0
    pc = m.regs()["pc"] & ~1
    info = {
        "machine": m,
        "insns": m.insn_count,
        "pc": pc,
        "state": m.state,
        "reason": reason,
        "wall": wall,
        "step_mode": exact_insns,
    }
    return info


def run_slices(m, budget, slice_insns=5000, cond=None):
    """分片跑 budget 条指令；每片后调用 cond()（可为 None）。返回 (ok, 已跑, 停止原因)。

    ok=False 表示遇到 fault/stall（或 cond 在异常状态返回 True 也不影响）。
    """
    total = 0
    reason = None
    while total < budget:
        prev = m.insn_count
        reason = m.run(max_insns=min(slice_insns, budget - total))
        delta = m.insn_count - prev
        total += delta if delta > 0 else slice_insns
        if m.state in ("fault", "stalled"):
            return False, total, m.state
        if cond is not None and cond():
            return True, total, reason
    return True, total, "ok"


# ---------------------------------------------------------------- 固件函数调用
def call_func(m, addr, args=(), max_insns=2_000_000, keep_ints=True):
    """在暂停的整机状态里调用一个固件函数（模拟一次 bl + 返回）。

    - 用 RAM 哨兵 SENTINEL 作为返回地址，`run(until_pc=SENTINEL)` 在函数 pop 返回后停下；
    - 调用前保存并屏蔽中断（PRIMASK=1），返回后恢复 r0-r12/sp/lr/pc/PRIMASK；
    - 函数对 RAM/flash 的修改会保留（渲染、保存等就是要这些副作用）。
    返回 dict：reason / ret_pc / insns / fault。
    """
    uc = m.uc
    saved = dict(m.regs())
    m.write(SENTINEL, _SENTINEL_CODE)
    try:
        primask = uc.reg_read(_uc_reg("PRIMASK"))
    except Exception:  # noqa: BLE001
        primask = 0
    uc.reg_write(_uc_reg("PRIMASK"), 1 if keep_ints else primask)
    uc.reg_write(_uc_reg("SP"), (saved["sp"] - 64) & ~7)
    for i, a in enumerate(args):
        uc.reg_write(_uc_reg("R0") + i, a & 0xFFFFFFFF)
    uc.reg_write(_uc_reg("LR"), (SENTINEL | 1) & 0xFFFFFFFF)
    uc.reg_write(_uc_reg("PC"), addr & ~1)
    start = m.insn_count
    reason = m.run(max_insns=max_insns, until_pc=SENTINEL)
    ret_pc = m.regs()["pc"] & ~1
    used = m.insn_count - start
    fault = m.state == "fault"
    # 恢复现场
    for i in range(13):
        uc.reg_write(_uc_reg("R0") + i, saved["r%d" % i])
    uc.reg_write(_uc_reg("SP"), saved["sp"])
    uc.reg_write(_uc_reg("LR"), saved["lr"])
    uc.reg_write(_uc_reg("PC"), saved["pc"])
    try:
        uc.reg_write(_uc_reg("PRIMASK"), primask)
    except Exception:  # noqa: BLE001
        pass
    if m.state == "fault":
        m.state = "paused"
        m.stop_reason = None
    return {"reason": reason, "ret_pc": ret_pc, "insns": used, "fault": fault}


def _uc_reg(name):
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_SP, UC_ARM_REG_LR,
                                   UC_ARM_REG_PC, UC_ARM_REG_PRIMASK)
    return {"R0": UC_ARM_REG_R0, "SP": UC_ARM_REG_SP, "LR": UC_ARM_REG_LR,
            "PC": UC_ARM_REG_PC, "PRIMASK": UC_ARM_REG_PRIMASK}[name]


# ---------------------------------------------------------------- C.Fn 渲染
class FieldTrace:
    """捕获 10 个字段渲染函数的调用 (func_addr, code, bitpos)。"""

    def __init__(self, m):
        from unicorn import UC_HOOK_CODE
        from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_R1
        self.m = m
        self.events = []
        self._handles = []
        self._by_addr = {a: i for i, a in enumerate(FIELD_FUNCS)}

        def _hook(uc, address, size, _ud):
            self.events.append((address,
                                uc.reg_read(UC_ARM_REG_R0),
                                uc.reg_read(UC_ARM_REG_R1)))

        for a in FIELD_FUNCS:
            self._handles.append(m.uc.hook_add(UC_HOOK_CODE, _hook, begin=a, end=a))

    def clear(self):
        self.events = []

    def close(self):
        for h in self._handles:
            try:
                self.m.uc.hook_del(h)
            except Exception:  # noqa: BLE001
                pass
        self._handles = []

    def transcript(self, glyphs=None):
        """把事件翻译成 [(field_idx, code, 可读字符, char_idx, parity)]。"""
        out = []
        for addr, code, pos in self.events:
            idx = self._by_addr.get(addr)
            ch = glyphs.get(code) if glyphs else None
            out.append({"field": (idx + 1) if idx is not None else None,
                        "func": addr, "code": code, "glyph": ch,
                        "char": (pos >> 1), "parity": pos & 1, "bitpos": pos})
        return out

    def text_row(self, glyphs, width=16):
        """按 char 索引拼出可读的显示文本行（未被字段渲染的位置用 '.'）。"""
        row = ["."] * width
        for ev in self.transcript(glyphs):
            if 0 <= ev["char"] < width:
                row[ev["char"]] = ev["glyph"] or "?"
        return "".join(row)


def clear_bitmap(m, n=16):
    m.write(BITMAP, b"\x00" * n)


def render_cfn(m, index, settings7=None, field_trace=None, glyphs=None):
    """进入 C.Fn 状态并渲染第 index 项（0..7），返回 (bitmap_bytes, transcript, text_row)。

    settings7 不为 None 时先写 settings[7]（F8 值）。
    """
    m.write(UI_OPEN, b"\x01")
    m.write(UI_INDEX, bytes([index & 0xFF]))
    if settings7 is not None:
        m.write(SETTINGS7, bytes([settings7 & 0xFF]))
    clear_bitmap(m)
    if field_trace is not None:
        field_trace.clear()
    res = call_func(m, FUNC_RENDER)
    bitmap = read_bytes(m, BITMAP, 16)
    transcript = field_trace.transcript(glyphs) if field_trace is not None else []
    text = field_trace.text_row(glyphs) if (field_trace is not None and glyphs) else ""
    return bitmap, transcript, text, res


# ---------------------------------------------------------------- 按键注入（3x3 矩阵）
def _active_key_row(m):
    """当前被拉低的行（MODER=输出 且 ODR=0）。没有则返回 None。"""
    moder = m.read32(GPIOB_BASE)
    odr = m.read32(GPIOB_BASE + 0x14)
    for r, pin in enumerate(KEY_ROW_PINS):
        if ((moder >> (2 * pin)) & 3) == 1 and ((odr >> pin) & 1) == 0:
            return r
    return None


class KeyMatrix:
    """在模拟器里给 3x3 键盘建模并注入按键。

    模拟器本身不做"按键 -> 矩阵电平"的映射（Device.press 只改 Device.buttons），
    所以这里包一层 GPIO IDR 刷新：读 GPIOC IDR 时，按当前被拉低的行，把对应列
    电平改成 0（按下）或保持上拉 1。行拉低/列上拉时序完全由固件 ui_key_scan_task
    自己驱动，因此按键路径仍是真实的固件扫描+去抖+分派路径。

    用法：
        keys = KeyMatrix(m); keys.press(2, 2)      # MENU -> 打开 C.Fn
        run_slices(m, 600_000, 2000, lambda: read_u8(m, UI_OPEN) == 1)
        keys.release_all()
    """

    def __init__(self, m):
        self.m = m
        self.pressed = set()
        self.column_reads = 0
        orig = m.bus._refresh_idr
        self._orig = orig

        def patched(port, base):
            orig(port, base)
            if port != "C":
                return
            row = _active_key_row(m)
            idr = m.bus.rd32(base + 0x10)
            for col, pin in enumerate(KEY_COL_PINS):
                low = row is not None and (row, col) in self.pressed
                level = 0 if low else 1
                idr = (idr & ~(1 << pin)) | (level << pin)
            m.bus.wr32(base + 0x10, idr)
            self.column_reads += 1

        m.bus._refresh_idr = patched

    def press(self, row, col):
        self.pressed.add((int(row), int(col)))

    def release(self, row, col):
        self.pressed.discard((int(row), int(col)))

    def release_all(self):
        self.pressed.clear()

    def press_code(self, code):
        for (r, c), v in KEY_CODES.items():
            if v == code:
                self.press(r, c)
                return (r, c)
        raise ValueError("未知键码 0x%X" % code)

    def restore(self):
        self.m.bus._refresh_idr = self._orig


def open_cfn_via_key(m, keys, attempts=4, budget_per_try=2_500_000):
    """用真实按键路径打开 C.Fn：注入 MENU 键（row2,col2），检测 UI+0x12==1。

    注意固件长按会再次切换（关闭），所以检测到打开后要立刻松开；这里每 500 条指令
    检测一次，尽量贴近"按下即确认"。返回 dict：ok / attempts / insns。
    """
    total = 0
    for attempt in range(attempts):
        keys.press(*KEY_CFN)
        ok, used, _ = run_slices(m, budget_per_try, slice_insns=500,
                                 cond=lambda: read_u8(m, UI_OPEN) == 1)
        total += used
        keys.release_all()
        if ok and read_u8(m, UI_OPEN) == 1:
            # 松开后的去抖/收尾，少量推进即可（不要长按）
            run_slices(m, 100_000, slice_insns=2000)
            if read_u8(m, UI_OPEN) == 1:
                return {"ok": True, "attempts": attempt + 1, "insns": total}
        # 没打开或打开后又被长按关掉：清一下 UI 标志再试
        m.write(UI_OPEN, b"\x00")
        run_slices(m, 50_000, slice_insns=2000)
    return {"ok": False, "attempts": attempts, "insns": total}
