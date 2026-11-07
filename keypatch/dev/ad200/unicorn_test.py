# -*- coding: utf-8 -*-
"""Unicorn 调用级测试（v2 可开关密钥版，详版）。

用法（tools/python 或 devices/ad200pro/python 均可）：
    tools\\python\\python.exe keypatch\\dev\\ad200\\unicorn_test.py

覆盖：
 1) apply() 输出与 keypatch/out 文件一致；差异只在被改写的 56 字节函数内；
 2) 与 Lead 已验证原型 toggle_proto.build_light_patch() 的输出**逐字节一致**（若原型存在）；
 3) Unicorn 跑**原厂**镜像：ID=0/1/37/99 -> 50024//(ID+1)，且保留 SCAL(0x33)+1ms 延时；
 4) Unicorn 跑**补丁**镜像：ID=0 -> KEY，ID=1/37/99 -> 原厂公式；无 SCAL，只有 delay_us(20)；
 5) 模块 verify() 汇总。
"""
import hashlib
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
sys.path.insert(0, os.path.join(ROOT, "keypatch"))
from devices import ad200  # noqa: E402

KEY = 0x6B21
IDS = (0, 1, 37, 99)
LEAD_PROTO = os.path.join(ROOT, "keypatch", "dev", "lead", "toggle_proto.py")


def load(path):
    with open(path, "rb") as f:
        return f.read()


def reg_writes(records):
    return [x for x in records if isinstance(x[0], int)]


def kinds(records, kind):
    return [x[1] for x in records if x[0] == kind]


def expected_sync(ident):
    return KEY if ident == 0 else 50024 // (ident + 1)


def main():
    original = load(os.path.join(ROOT, "devices", "x3c", "firmware", "AD200pro_V1.60.bin"))
    patched_path = os.path.join(ROOT, "keypatch", "out", "AD200pro_V1.60_key6B21.bin")
    patched = load(patched_path)
    print("原厂镜像 :", len(original), "字节 sha256", hashlib.sha256(original).hexdigest())
    print("补丁镜像 :", len(patched), "字节 sha256", hashlib.sha256(patched).hexdigest())
    print("补丁区域 : 0x%08X..0x%08X（%d 字节，下一函数 0x%08X）"
          % (ad200.SYNC_FUNC, ad200.SYNC_FUNC_END, ad200.SYNC_FUNC_SIZE,
             ad200.SYNC_FUNC_END))

    # ---- 1) apply() 与 out 文件一致性 ----
    applied, log = ad200.apply(original, KEY)
    print("\n== apply() 补丁清单 ==")
    for p in log:
        print("  0x%08X off+0x%04X  %-14s -> %-14s %s"
              % (p["addr"], p["offset"], p["before"], p["after"], p["desc"]))
    assert applied == patched, "apply() 输出与 keypatch/out 文件不一致"
    diffs = [i for i in range(len(original)) if original[i] != applied[i]]
    print("apply() 输出 == out 文件：是；差异 %d 字节，偏移 %s"
          % (len(diffs), [hex(x) for x in diffs]))
    off = ad200.SYNC_FUNC - ad200.BASE
    assert all(off <= x < off + ad200.SYNC_FUNC_SIZE for x in diffs), "差异超出补丁区域"
    assert len(applied) == len(original), "长度变化"
    assert applied[off + 0x34:off + 0x38] == original[off + 0x34:off + 0x38], \
        "50024 字面量应保持与原厂同址同值"
    print("[PASS] 长度不变、差异仅在 56 字节函数区域、50024 字面量未变")

    # ---- 2) 与 Lead 原型逐字节交叉比对 ----
    print("\n== 与 Lead 原型 toggle_proto.build_light_patch() 交叉比对 ==")
    if os.path.isfile(LEAD_PROTO):
        sys.path.insert(0, os.path.dirname(LEAD_PROTO))
        import toggle_proto as T  # noqa: E402
        lead_blob = T.build_light_patch(ad200.SYNC_FUNC, ad200.DIV_HELPER,
                                        ad200.HELPER_REG_WRITE, ad200.HELPER_DELAY, KEY)
        mine = ad200.build_light_patch(key=KEY)
        print("  本地 blob : %s" % mine.hex(" "))
        print("  Lead blob : %s" % lead_blob.hex(" "))
        assert mine == lead_blob, "与 Lead 原型输出不一致"
        print("[PASS] 56 字节 blob 与 Lead 已验证原型逐字节一致")
    else:
        print("  [skip] 未找到 %s" % LEAD_PROTO)

    # ---- 反例：错误输入必须被拒绝 ----
    print("\n== apply() 反例 ==")
    bad_region = bytearray(original)
    bad_region[off + 8] ^= 0x01     # 篡改函数区域一个字节
    for bad, why in ((original[:-1], "长度不对"),
                     (bytes(bad_region), "函数区域字节不对"),
                     (applied, "已是补丁镜像（sha256 不对）")):
        try:
            ad200.apply(bad, KEY)
        except ValueError as e:
            print("[PASS] 拒绝异常输入（%s）：%s" % (why, str(e).splitlines()[0]))
        else:
            raise AssertionError("apply() 未拒绝错误输入: " + why)

    # ---- 3) harness 跑原厂镜像 ----
    print("\n== Unicorn: 原厂镜像 0x0800B9AC(ID) ==")
    rec_o = ad200._run_sync_harness(original, IDS)
    for idv in IDS:
        q = 50024 // (idv + 1)
        w = reg_writes(rec_o[idv])
        print("  ID=%-3d 记录=%s -> SYNC=0x%02X%02X (公式 50024/%d=0x%04X) strobe=%s delay=%s"
              % (idv, rec_o[idv], w[0][1], w[1][1], idv + 1, q,
                 ["0x%02X" % s for s in kinds(rec_o[idv], "strobe")],
                 kinds(rec_o[idv], "delay")))
        assert w == [(4, (q >> 8) & 0xFF), (5, q & 0xFF)], "原厂公式结果不对"
        assert kinds(rec_o[idv], "strobe") == [0x33]
        assert kinds(rec_o[idv], "delay") == [0x14, 0x3E8]
    print("[PASS] 原厂镜像 = 50024//(ID+1) + SCAL + 1ms 延时（harness 自证有效）")

    # ---- 4) harness 跑补丁镜像 ----
    print("\n== Unicorn: 补丁镜像 0x0800B9AC(ID) ==")
    rec_p = ad200._run_sync_harness(patched, IDS)
    for idv in IDS:
        sync = expected_sync(idv)
        want = [(4, (sync >> 8) & 0xFF), (5, sync & 0xFF)]
        w = reg_writes(rec_p[idv])
        mode = "密钥模式" if idv == 0 else "普通模式"
        print("  ID=%-3d %s 记录=%s -> SYNC=0x%02X%02X (期望 0x%04X, 原公式 0x%04X) "
              "strobe=%s delay=%s"
              % (idv, mode, rec_p[idv], w[0][1], w[1][1], sync,
                 50024 // (idv + 1),
                 ["0x%02X" % s for s in kinds(rec_p[idv], "strobe")],
                 kinds(rec_p[idv], "delay")))
        assert w == want, "补丁镜像写寄存器不对（ID=%d）" % idv
        assert kinds(rec_p[idv], "strobe") == [], "v2 不应再有 SCAL strobe"
        assert kinds(rec_p[idv], "delay") == [0x14], "v2 应只保留 delay_us(20)"
    print("[PASS] ID=0 -> KEY；ID=1/37/99 -> 原厂公式；无 SCAL，仅 delay_us(20)")

    # ---- 5) 模块 verify() 汇总 ----
    print("\n== 模块 verify() ==")
    results = ad200.verify(patched, KEY)
    ok_all = True
    for ok, msg in results:
        ok_all = ok_all and ok
        print("  [%s] %s" % ("PASS" if ok else "FAIL", msg))
    assert ok_all, "verify() 有失败项"
    print("\nPASS unicorn_test (v2)")
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except AssertionError as e:
        print("FAIL unicorn_test:", e)
        sys.exit(1)
