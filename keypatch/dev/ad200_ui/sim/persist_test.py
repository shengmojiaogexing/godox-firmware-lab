#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""persist_test.py —— 用整机模拟器验证 F8（settings[7]）的保存/加载持久化 roundtrip。

验证链路（全部在模拟器里真跑固件函数，不是纯内存伪造）：
    settings[7]=值
      -> 0x0800B820(slot)  flash_store_params：记录构建 0x080043E4 + flash_write_params 0x08005974
      -> flash 0x0801F000+slot*100 的 32 字节记录：字节0=magic 0x5C，字节31=settings[7]
      -> 清空 settings RAM
      -> 0x0800AC58  settings_load_task：扫描 slot19..0 找 magic -> 0x0800590C 读回
         -> 0x0800CCC8 展开记录 -> settings[0..7]
      -> settings[7] 必须等于保存值（再验证 =0 的方向）

另外做一次"掉电重上电"：把模拟器里真实的 flash 参数页内容塞进新 Machine 的 flash，
再冷启动到主循环，检查上电加载后 settings[7] 仍然是保存值。
（devices/ad200pro 的 Machine.reset() 会用镜像覆盖 flash，所以这里用 do_reset=False 保留预置 flash。）

用法：
    devices\ad200pro\\python\\python.exe keypatch\\dev\\ad200_ui\\sim\\persist_test.py
    ... persist_test.py --bin 补丁.bin --slot 19 --json out.json

退出码：0 = PASS；1 = FAIL。
日志：`> log_persist_test.txt`。
"""
import argparse
import json
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import _sim_common as C  # noqa: E402

RECORD_SIZE = 32
SLOT_STRIDE = 100


def record_addr(slot):
    return C.FLASH_PARAM_PAGE + slot * SLOT_STRIDE


def read_settings(m):
    return C.read_bytes(m, C.SETTINGS, 8)


def show_record(m, slot):
    addr = record_addr(slot)
    return addr, C.read_bytes(m, addr, RECORD_SIZE)


def store(m, slot):
    return C.call_func(m, C.FUNC_STORE, (slot,))


def load(m):
    return C.call_func(m, C.FUNC_LOAD, ())


def clear_settings(m, fill=0xAA):
    m.write(C.SETTINGS, bytes([fill]) * 8)


def power_cycle_flash(m, slot, expect_byte=None):
    """把 m 当前 0x0801F000 整页 flash 预置进一个新 Machine，冷启动到主循环。

    返回 (new_machine, boot_info, settings_after_boot)。
    """
    page = C.read_bytes(m, C.FLASH_PARAM_PAGE, 0x800)
    m2 = C.load_machine(getattr(m.fw, "path", None))
    # Machine.__init__ 已 reset（flash=镜像 0xFF），这里在启动前写入真实参数页
    m2.write(C.FLASH_PARAM_PAGE, page)
    boot = C.boot_to_main(m2, exact_insns=False, do_reset=False)
    return m2, boot, read_settings(m2)


def roundtrip(m, slot, value, log):
    """settings[7]=value -> store -> 校验 flash -> 清 RAM -> load -> 校验 settings[7]。"""
    ok = True

    def need(cond, msg):
        nonlocal ok
        ok = ok and cond
        log.append("  [%s] %s" % ("PASS" if cond else "FAIL", msg))

    before = bytearray(read_settings(m))
    log.append("  settings(保存前) = %s" % C.hexs(before))
    m.write(C.SETTINGS7, bytes([value & 0xFF]))
    want = read_settings(m)

    res = store(m, slot)
    need(not res["fault"], "0x0800B820(%d) 正常返回（reason=%s pc=0x%08X）"
         % (slot, res["reason"], res["ret_pc"]))
    addr, rec = show_record(m, slot)
    log.append("  flash 记录 @0x%08X: %s" % (addr, C.hexs(rec)))
    need(rec[0] == 0x5C, "记录字节0 = 0x%02X（magic，期望 0x5C）" % rec[0])
    need(rec[24:32] == bytes(want), "记录字节24..31 == settings[0..7]（%s）"
         % C.hexs(rec[24:32]))
    need(rec[31] == (value & 0xFF), "记录字节31 = settings[7] = 0x%02X" % rec[31])

    clear_settings(m)
    res = load(m)
    need(not res["fault"], "0x0800AC58 重新加载正常返回（reason=%s）" % res["reason"])
    got = read_settings(m)
    log.append("  settings(重新加载后) = %s" % C.hexs(got))
    need(got == bytes(want), "重新加载后 settings[0..7] 与保存值一致")
    need(got[7] == (value & 0xFF), "重新加载后 settings[7] = 0x%02X（期望 0x%02X）"
         % (got[7], value & 0xFF))

    m2, boot, after = power_cycle_flash(m, slot)
    log.append("  掉电重上电: 启动到主循环 insns=%d pc=0x%08X state=%s settings=%s"
               % (boot["insns"], boot["pc"], boot["state"], C.hexs(after)))
    reached = (boot["reason"] in ("breakpoint", "loop_zone")
               and C.LOOP_LO <= boot["pc"] < C.LOOP_HI)
    need(reached, "预置参数页后仍能启动到主循环 [0x%08X,0x%08X)（pc=0x%08X）"
         % (C.LOOP_LO, C.LOOP_HI, boot["pc"]))
    need(after[7] == (value & 0xFF),
         "冷启动后 settings[7] = 0x%02X（期望 0x%02X，掉电保持）" % (after[7], value & 0xFF))
    return ok, rec, m2


def main(argv=None):
    C.ensure_stdout_utf8()
    ap = argparse.ArgumentParser(description="AD200 Pro F8/settings[7] 持久化 roundtrip 验证")
    ap.add_argument("--bin", help="固件路径（默认原厂）")
    ap.add_argument("--slot", type=int, default=19, help="参数记录 slot 0..19（默认 19）")
    ap.add_argument("--json", help="结果写入 JSON")
    args = ap.parse_args(argv)

    path = args.bin or C.resolve_orig()
    print("== persist_test: settings[7] -> flash 0x0801F000 记录 -> 重新加载 ==")
    print("镜像: %s" % os.path.abspath(path))
    if args.bin:
        print("sha256: %s" % C.sha256_hex(C.read_file_bytes(path)))
    if not (0 <= args.slot < 20):
        ap.error("--slot 必须在 0..19")

    m = C.load_machine(path)
    boot0 = C.boot_to_main(m, exact_insns=False)
    print("启动到主循环: insns=%d pc=0x%08X state=%s" % (boot0["insns"], boot0["pc"], boot0["state"]))
    addr, rec0 = show_record(m, args.slot)
    print("slot %d 初始 flash @0x%08X: %s" % (args.slot, addr, C.hexs(rec0)))

    log = []
    ok1, rec, m_after1 = roundtrip(m, args.slot, 1, log)
    print("\n[settings[7] = 1]")
    for line in log:
        print(line)

    log2 = []
    ok2, rec0b, m_after0 = roundtrip(m_after1 if m_after1 else m, args.slot, 0, log2)
    print("\n[settings[7] = 0]")
    for line in log2:
        print(line)

    # 再验证：保存到不同 slot 时地址 = 0x0801F000 + slot*100
    print("\n[slot 寻址校验]")
    m3 = C.load_machine(path)
    C.boot_to_main(m3, exact_insns=False)
    m3.write(C.SETTINGS7, b"\x01")
    ok3 = True
    for slot in (0, 1, 7, 19):
        store(m3, slot)
        a = record_addr(slot)
        rec = C.read_bytes(m3, a, RECORD_SIZE)
        good = rec[0] == 0x5C and rec[31] == 1
        print("  slot %2d 记录 @0x%08X: magic=0x%02X byte31=0x%02X %s"
              % (slot, a, rec[0], rec[31], "PASS" if good else "FAIL"))
        ok3 = ok3 and good

    ok = ok1 and ok2 and ok3
    print("\n%s persist_test" % ("PASS" if ok else "FAIL"))
    if args.json:
        with open(args.json, "w", encoding="utf-8") as f:
            json.dump({"ok": ok, "image": os.path.abspath(path), "slot": args.slot,
                       "settings7_1": ok1, "settings7_0": ok2, "slot_addressing": ok3},
                      f, ensure_ascii=False, indent=1)
        print("（结果已写入 %s）" % os.path.abspath(args.json))
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
