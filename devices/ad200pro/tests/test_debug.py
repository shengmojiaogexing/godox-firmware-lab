#!/usr/bin/env python3
"""验收测试 2：调试能力（断点/单步/内存/反汇编/trace/调用栈/虚拟整机）。

对应 SIM-SPEC §4 的 Debugger API 契约。不依赖实现内部细节：
  - regs / stats / read / write / step / run / set_breakpoint / clear_breakpoint
  - disasm / trace / reset_trace / backtrace
  - device_state / device_set / device_event
运行：  python tests/test_debug.py
"""
import sys

sys.path.insert(0, __file__.rsplit("\\", 1)[0] if "\\" in __file__ else ".")
import _util as U  # noqa: E402


def _call_any(obj, names, *args, **kw):
    """按契约名字调用；实现里是属性或方法都兼容。"""
    for n in names:
        if hasattr(obj, n):
            attr = getattr(obj, n)
            return attr(*args, **kw) if callable(attr) else attr
    raise AttributeError("缺少 API：%s" % "/".join(names))


def test_reset_and_step(m):
    m.reset()
    r = U.regs(m)
    assert r["pc"] == (U.RESET_PC & ~1), "reset PC=0x%08X" % r["pc"]
    assert U.insn_count(m) == 0

    n0 = U.insn_count(m)
    p0 = r["pc"]
    m.step(1)
    r1 = U.regs(m)
    assert U.insn_count(m) == n0 + 1, "step(1) 后 insn_count 应 +1"
    # reset 处 0x080030D4: ldr r0,[pc,#0x10] (2 字节) -> 下一条 0x080030D6
    assert r1["pc"] == 0x080030D6, \
        "step(1) 后 PC 应为 0x080030D6（0x%08X -> 0x%08X）" % (p0, r1["pc"])

    n1 = U.insn_count(m)
    m.step(5)
    assert U.insn_count(m) == n1 + 5, "step(5) 后 insn_count 应 +5"
    print("  step: pc=0x%08X -> 0x%08X, insns=%d" % (
        p0, U.pc(m), U.insn_count(m)))


def test_breakpoints(m):
    m.reset()
    bp = U.RESET_PC & ~1
    U.set_breakpoint(m, bp)
    assert bp in [a & ~1 for a in U.breakpoints(m)], "set_breakpoint 后列表里没有该地址"
    reason = U.run(m, max_insns=1000)
    assert "breakpoint" in str(reason).lower(), \
        "run 应停在断点，实际 reason=%r pc=0x%08X" % (reason, U.pc(m))
    assert U.pc(m) == bp, "停在断点应 pc=0x%08X，实际 0x%08X" % (bp, U.pc(m))
    U.clear_breakpoint(m, bp)
    assert bp not in [a & ~1 for a in U.breakpoints(m)], "clear_breakpoint 未生效"
    # 清除后应能继续向前执行
    n0 = U.insn_count(m)
    U.run(m, max_insns=500)
    assert U.insn_count(m) > n0, "清除断点后 run 应继续执行"
    print("  breakpoint: 停在 0x%08X，清除后继续执行 OK" % bp)


def test_memory(m):
    m.reset()
    addr = 0x20001000                      # RAM 0x20000000+0x2000 内
    data = b"\xde\xad\xbe\xef"
    m.write(addr, data)
    got = bytes(m.read(addr, 4))
    assert got == data, "内存读写不一致：%r != %r" % (got, data)
    print("  mem: 0x%08X 写入/读回 %s OK" % (addr, got.hex()))


def test_disasm(m):
    insns = _call_any(m, ["disasm"], U.MAIN, 8)
    assert isinstance(insns, list) and len(insns) >= 8, \
        "disasm 应返回 >=8 条，实际 %r" % (len(insns) if isinstance(insns, list) else insns)
    first = insns[0]
    assert first["addr"] & ~1 == U.MAIN, \
        "disasm 首条地址应 0x%08X，实际 %r" % (U.MAIN, first.get("addr"))
    for ins in insns:
        assert "addr" in ins and "text" in ins and "bytes" in ins, \
            "disasm 条目缺少 addr/bytes/text：%r" % ins
        assert ins["text"].strip(), "disasm 条目 text 为空：%r" % ins
    print("  disasm: 0x%08X %s ..." % (insns[0]["addr"], insns[0]["text"]))


def test_trace_and_stats(m):
    m.reset()
    U.reset_trace(m)
    m.run(max_insns=3000)
    ev = U.trace_events(m, since=0, limit=500)
    assert ev, "运行后 trace 为空"
    for e in ev:
        assert "seq" in e and "type" in e, "trace 事件缺少 seq/type：%r" % e
    st = U.stats(m)
    assert ("insn_count" in st or "insns" in st), "stats 缺少 insn_count/insns"
    assert ("uptime_ms" in st or "ticks" in st or "cycle_estimate" in st or "cycles" in st), \
        "stats 缺少 uptime_ms/cycle_estimate"
    print("  trace: %d 条；stats insns=%d uptime=%.1fms" % (
        len(ev), U.insn_count(m), U.uptime_ms(m)))


def test_backtrace(m):
    m.reset()
    m.run(max_insns=2000)
    bt = _call_any(m, ["backtrace"], 8)
    assert isinstance(bt, list) and bt, "backtrace 应返回非空列表"
    for fr in bt:
        assert "addr" in fr, "调用栈帧缺少 addr：%r" % fr
    print("  backtrace: %d 帧，top=0x%08X" % (len(bt), bt[0]["addr"]))


def test_devices(m):
    st = _call_any(m, ["device_state"])
    assert isinstance(st, dict) and st, "device_state 应返回 dict"
    keys = set(st)
    assert ("battery" in keys or "battery_mv" in keys), "device_state 缺 battery/battery_mv"
    assert "cap_voltage" in keys, "device_state 缺 cap_voltage"
    assert ("temp" in keys or "temp_c" in keys), "device_state 缺 temp/temp_c"

    # 设置输入量（按 webui 的两种单位都试）
    _call_any(m, ["device_set"], battery=3.7, battery_mv=3700,
              cap_voltage=330.0, temp=25.0, temp_c=25.0)
    st2 = _call_any(m, ["device_state"])
    b = st2.get("battery", (st2.get("battery_mv") or 0) / 1000.0)
    assert abs(float(b) - 3.7) < 0.2, "device_set battery 未生效：%r" % b
    assert abs(float(st2.get("cap_voltage", -1)) - 330.0) < 1.0, \
        "device_set cap_voltage 未生效：%r" % st2.get("cap_voltage")

    # 事件注入：按键 / RF
    _call_any(m, ["device_event"], "button", {"name": "POWER", "pressed": True})
    _call_any(m, ["device_event"], "rf_rx", {"bytes": [0x01, 0x23, 0x45], "hex": "012345"})
    st3 = _call_any(m, ["device_state"])
    print("  device: battery=%r cap=%r temp=%r" % (
        st3.get("battery", st3.get("battery_mv")),
        st3.get("cap_voltage"), st3.get("temp", st3.get("temp_c"))))


def main():
    print("== test_debug: debugger API ==")
    Machine, m = U.load_machine()
    for fn in (test_reset_and_step, test_breakpoints, test_memory, test_disasm,
               test_trace_and_stats, test_backtrace, test_devices):
        print("[%s]" % fn.__name__)
        fn(m)
    print("PASS test_debug")
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except AssertionError as e:
        print("FAIL test_debug:", e)
        sys.exit(1)
    except Exception as e:
        import traceback
        traceback.print_exc()
        print("ERROR test_debug: %s: %s" % (type(e).__name__, e))
        sys.exit(1)
