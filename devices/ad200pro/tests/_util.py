"""tests 公共工具：路径、契约兼容访问器。

测试以 docs/SIM-SPEC.md §4 的 Machine API 为准；这里只做少量「属性/方法」
兼容（例如 `regs` 在契约里是 property，实现里可能是方法），以及 stats()
字段别名（insn_count/insns、cycle_estimate/cycles），不改变验收语义。
"""
import os
import sys

# Windows 控制台默认 cp936，测试输出含中文；尽量切到 UTF-8，失败不影响测试
try:
    sys.stdout.reconfigure(encoding="utf-8")
    sys.stderr.reconfigure(encoding="utf-8")
except Exception:  # noqa: BLE001
    pass

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BIN = os.path.join(ROOT, "firmware", "AD200pro_V1.60.bin")
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

# 固件事实（docs/01-static-notes.md / SIM-SPEC §3）
IMAGE_BASE = 0x08003000
RESET_PC = 0x080030D4
MAIN = 0x0800CDB0
LOOP_LO, LOOP_HI = 0x0800CE40, 0x0800CEB0
_MAX_INSN_TO_LOOP = 5_000_000


def load_machine():
    """导入并实例化 Machine；返回 (Machine 类, m 实例)。"""
    from sim.machine import Machine
    return Machine, Machine(BIN)


def regs(m):
    r = getattr(m, "regs")
    return dict(r() if callable(r) else r)


def pc(m):
    return regs(m)["pc"] & ~1


def stats(m):
    return dict(m.stats())


def insn_count(m):
    s = stats(m)
    return int(s.get("insn_count", s.get("insns", 0)))


def uptime_ms(m):
    s = stats(m)
    if "uptime_ms" in s:
        return float(s["uptime_ms"])
    return float(s.get("ticks", 0))


def trace_events(m, since=0, limit=5000):
    """按契约 m.trace(since, limit)；兼容实现里的 tracer.since/last。"""
    if hasattr(m, "trace"):
        try:
            ev = m.trace(since=since, limit=limit)
            return list(ev)
        except TypeError:
            return list(m.trace(since, limit))
    if hasattr(m, "tracer"):
        try:
            return list(m.tracer.since(since, limit))
        except Exception:
            return list(m.tracer.last(limit))
    return []


def reset_trace(m):
    if hasattr(m, "reset_trace"):
        m.reset_trace()
    elif hasattr(m, "tracer"):
        m.tracer.clear()


def set_breakpoint(m, addr):
    if hasattr(m, "set_breakpoint"):
        return m.set_breakpoint(addr)
    # 容错：直接操作集合
    m.breakpoints.add(addr & ~1)


def clear_breakpoint(m, addr):
    if hasattr(m, "clear_breakpoint"):
        return m.clear_breakpoint(addr)
    m.breakpoints.discard(addr & ~1)


def breakpoints(m):
    if hasattr(m, "breakpoints"):
        b = getattr(m, "breakpoints")
        if callable(b):
            return list(b())
        if hasattr(b, "__iter__"):
            return list(b)
    return []


def run(m, max_insns=1000, **kw):
    return m.run(max_insns=max_insns, **kw)


def is_fault_reason(reason):
    return reason is not None and "fault" in str(reason).lower()


def fmt_hex(v):
    return "0x%08X" % (v & 0xFFFFFFFF)
