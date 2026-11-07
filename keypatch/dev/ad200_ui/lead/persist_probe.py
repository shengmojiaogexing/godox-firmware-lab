# -*- coding: utf-8 -*-
"""Lead: settings[7]（F8 密钥开关）flash 保存/加载 roundtrip 探针。

用法（必须用 devices/ad200pro 的 python）：
    devices\ad200pro\\python\\python.exe keypatch\\dev\\ad200_ui\\lead\\persist_probe.py

流程：
  1) 补丁镜像启动到主循环；
  2) settings[7]=1 → 置保存请求 0x20000216=1 → 直调 save task 0x08009B38；
  3) 读 flash 0x0801F000+slot*100（slot 从 0x20000214 取），断言记录字节 31 == 1；
  4) 清零 RAM settings[] → 直调 settings_load_task 0x0800AC58 → 断言 settings[7] 恢复为 1。
"""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(
    os.path.dirname(os.path.abspath(__file__))))))
ADROOT = os.path.join(ROOT, "devices/ad200pro")
sys.path.insert(0, ADROOT)
from sim.machine import Machine  # noqa: E402

LOOP = 0x0800CE40
SET = 0x20000418
SENT = 0x20000600
PARAM_PAGE = 0x0801F000
SLOT_STATE = 0x20000214       # [0]=slot 计数, [1]=magic 缓存?, [2]=保存请求
SAVE_TASK = 0x08009B38
LOAD_TASK = 0x0800AC58


def boot(path):
    m = Machine(path)
    m.reset()
    m.max_slice_rate = 1
    m.set_breakpoint(LOOP)
    r = m.run(max_insns=5_000_000)
    assert r == "breakpoint", (r, m.state)
    m.clear_breakpoint(LOOP)
    m.max_slice_rate = 4000
    return m


def call(m, addr, insns=60000):
    m.write(SENT, bytes.fromhex("fee7"))
    m.set_reg("lr", SENT | 1)
    m.set_reg("pc", addr | 1)
    m.set_reg("sp", 0x200008A0)
    m.run(max_insns=insns)


def main():
    patched = os.path.join(ROOT, "keypatch", "out", "AD200pro_V1.60_keyD798.bin")
    m = boot(patched)
    ok = True

    # 1) F8=ON + 保存请求 → save task
    m.write(SET + 7, b"\x01")
    m.write(0x20000214, b"\x00")       # slot 计数从 0 开始
    m.write(0x20000216, b"\x01")       # save request
    call(m, SAVE_TASK, 30000)
    slot = m.read(SLOT_STATE, 1)[0]
    print("保存后 slot 状态: %s（slot=%d）" % (m.read(SLOT_STATE, 3).hex(" "), slot))
    used = slot                      # save task 先把计数 +1 再写
    rec = bytes(m.read(PARAM_PAGE + used * 100, 36))
    print("记录 slot%d 前 32B: %s" % (used, rec[:32].hex(" ")))
    print("记录 magic=0x%02X, 字节31=0x%02X（settings[7]）" % (rec[0], rec[31]))
    ok &= (rec[0] == 0x5C and rec[31] == 1)

    # 2) 清 RAM settings → load task 应恢复 settings[7]=1
    m.write(SET, bytes(8))
    print("加载前 settings[0..7] = %s" % m.read(SET, 8).hex(" "))
    call(m, LOAD_TASK, 60000)
    after = bytes(m.read(SET, 8))
    print("加载后 settings[0..7] = %s" % after.hex(" "))
    ok &= (after[7] == 1)
    print("结论:", "PASS" if ok else "FAIL")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
