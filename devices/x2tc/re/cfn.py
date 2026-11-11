# -*- coding: utf-8 -*-
"""X2TC_Z02_V1.01.bin（Godox X2T-C 引闪器）C.Fn / 设置区 / 补丁点侦察。

只读分析：输出到 stdout，并把完整日志写到本目录 log_cfn.txt。

用法（仓库根）::

    tools\\python\\python.exe devices\\x2tc\\re\\cfn.py
"""
from __future__ import annotations

import hashlib
import os
import re
import struct
import sys

sys.dont_write_bytecode = True
_HERE = os.path.dirname(os.path.abspath(__file__))
if _HERE not in sys.path:
    sys.path.insert(0, _HERE)

import fwlib as st  # noqa: E402

BASE = st.BASE
sys.stdout.reconfigure(encoding="utf-8", errors="replace")


def header(t: str) -> None:
    print()
    print("=" * 78)
    print(t)
    print("=" * 78)


def fmt(ins) -> str:
    return "0x%08X  %-12s %-7s %s" % (ins.address, ins.bytes.hex(" "),
                                      ins.mnemonic, ins.op_str)


def ann_disasm(code, lo: int, hi: int) -> None:
    for ins in code.disasm(lo, hi):
        line = "0x%08X: %-7s %-30s" % (ins.address, ins.mnemonic, ins.op_str)
        lv = code.literal_value(ins)
        if lv is not None:
            line += "; lit@0x%08X = 0x%08X" % lv
        print(line)


def refs_to(image: bytes, val: int) -> list[int]:
    n = struct.pack("<I", val)
    out = []
    i = image.find(n)
    while i >= 0:
        out.append(i)
        i = image.find(n, i + 1)
    return out


def all_zero_runs(image: bytes, min_len: int):
    runs = []
    start = None
    for i, b in enumerate(image):
        if b == 0:
            if start is None:
                start = i
        else:
            if start is not None and i - start >= min_len:
                runs.append((start, i - start))
            start = None
    if start is not None and len(image) - start >= min_len:
        runs.append((start, len(image) - start))
    return runs


def all_ff_runs(image: bytes, min_len: int):
    runs = []
    start = None
    for i, b in enumerate(image):
        if b == 0xFF:
            if start is None:
                start = i
        else:
            if start is not None and i - start >= min_len:
                runs.append((start, i - start))
            start = None
    if start is not None and len(image) - start >= min_len:
        runs.append((start, len(image) - start))
    return runs


def abs_refs(image: bytes, lo: int, hi: int) -> list[int]:
    out = []
    for off in range(0, len(image) - 3):
        v = struct.unpack_from("<I", image, off)[0]
        if lo <= v < hi:
            out.append(off)
    return out


def main() -> int:
    blob = st.load_container()
    image = st.load_image()
    print("X2TC_Z02_V1.01.bin 侦察（payload=%d B，容器=%d B）" % (len(image), len(blob)))
    print("容器 sha256=%s" % hashlib.sha256(blob).hexdigest())
    print("payload sha256=%s" % hashlib.sha256(image).hexdigest())
    info = st.split_container(blob)
    print("容器解析:", {k: (v if k != "payload" else len(v)) for k, v in info.items()})

    code = st.Code(image)
    entries = code.function_entries()
    code.detect_switch_helpers(entries)
    print("函数入口: %d 个" % len(entries))

    header("1. 基址 / 向量表 / 同步字函数")
    for i in range(0, 0x40, 4):
        v = struct.unpack_from("<I", image, i)[0]
        mark = "IN" if (BASE <= (v & ~1) < BASE + len(image)) else ""
        print("  vec[%02X] = 0x%08X %s" % (i // 4, v, mark))
    off = st._off(st.SYNC_FN)
    print("SYNC_FN 0x%08X 原厂 56 B = %s" % (st.SYNC_FN, image[off:off + 56].hex(" ")))
    print("与 ORIG_FUNC 一致:", image[off:off + 56] == st.ORIG_FUNC)
    ann_disasm(code, st.SYNC_FN, st.SYNC_FN_END)
    print("50024 字面量全镜像出现位置:", [hex(x) for x in refs_to(image, st.ID_NUMERATOR)])
    print("辅助函数: uidiv=0x%08X write_reg=0x%08X delay=0x%08X strobe=0x%08X"
          % (st.UIDIV, st.WRITE_REG, st.DELAY, st.STROBE))
    callers = [ins.address for ins in code.insns
               if ins.mnemonic == "bl" and code.bl_target(ins) == st.SYNC_FN]
    print("BL -> SYNC_FN 调用点:", ["0x%08X" % a for a in callers])
    for c in callers:
        fn = code.containing_function(c, entries)
        end = code.next_entry(fn, entries)
        print("  --- 调用点 0x%08X 所在函数 0x%08X..0x%08X ---" % (c, fn, end - 1))
        ann_disasm(code, fn, min(end, fn + 0x120))

    header("2. settings / max 表 / UI 状态引用")
    for name, val in (("SETTINGS_BASE", st.SETTINGS_BASE),
                      ("MAX_TABLE_BASE", st.MAX_TABLE_BASE),
                      ("UI_BASE", st.UI_BASE),
                      ("UI2_BASE", st.UI2_BASE),
                      ("FB_BASE", st.FB_BASE)):
        rs = refs_to(image, val)
        print("%-16s 0x%08X: %d 处 %s" % (name, val, len(rs),
                                          ["0x%08X" % (BASE + r) for r in rs[:20]]))
    print()
    print("字面量引用（capstone LDR literal）指向 0x20000900..0x20000A00：")
    for ins in code.insns:
        lv = code.literal_value(ins)
        if lv is None:
            continue
        lit_addr, val = lv
        if 0x20000900 <= val < 0x20000A00:
            print("  0x%08X -> 0x%08X (lit@0x%08X)" % (ins.address, val, lit_addr))

    header("3. 最大值表来源：RAM 0x20000978 的写者 + flash 源表候选")
    for r in refs_to(image, st.MAX_TABLE_BASE):
        addr = BASE + r
        fn = code.containing_function(addr, entries)
        print("  引用处 0x%08X（函数 0x%08X..0x%08X）"
              % (addr, fn, code.next_entry(fn, entries) - 1))
        ann_disasm(code, fn, min(code.next_entry(fn, entries), fn + 0x100))

    # flash 里候选 max 表：item5=31, item6=99 的 12/16/32/64 字节表
    print()
    print("flash 中候选 max 表（[5]==31 且 [6]==99，长度 12/16/24/32/64）:")
    for length in (12, 16, 24, 32, 64):
        for off2 in range(0, len(image) - length - 1):
            seg = image[off2:off2 + length]
            if seg[5] == 31 and seg[6] == 99 and all(1 <= b <= 99 for b in seg):
                print("  文件 0x%05X 地址 0x%08X len=%d: %s"
                      % (off2, BASE + off2, length, seg.hex(" ")))

    header("4. 以 item 索引访问 settings 的动态访问（扫 ldrb/strb [reg, reg]）")
    for ins in code.insns:
        if ins.mnemonic in ("ldrb", "strb", "ldr", "str") and re.search(r"\[r\d+, r\d+\]", ins.op_str):
            fn = code.containing_function(ins.address, entries)
            print("  0x%08X fn=0x%08X: %s" % (ins.address, fn, fmt(ins)))

    header("5. 菜单改值 / 即时生效函数（0x080116DC 附近）")
    ann_disasm(code, 0x08011680, 0x08011884)

    header("6. 参数页 0x0801F000 / 0x0801F800 引用（持久化）")
    for val in (0x0801F000, 0x0801F800):
        for r in refs_to(image, val):
            addr = BASE + r
            fn = code.containing_function(addr, entries)
            print("  0x%08X -> 函数 0x%08X..0x%08X" % (addr, fn, code.next_entry(fn, entries) - 1))
            ann_disasm(code, fn, min(code.next_entry(fn, entries), fn + 0x100))

    header("7. 代码洞（全 0x00 / 全 0xFF，>=32 B，无引用）")
    for label, runs in (("0x00", all_zero_runs(image, 32)),
                        ("0xFF", all_ff_runs(image, 32))):
        print("--- 全 %s 段 ---" % label)
        for off2, ln in sorted(runs, key=lambda x: -x[1])[:15]:
            lo, hi = BASE + off2, BASE + off2 + ln
            refs = abs_refs(image, lo, hi)
            bl = [ins.address for ins in code.insns
                  if (code.bl_target(ins) or 0) and lo <= code.bl_target(ins) < hi]
            br = [ins.address for ins in code.insns
                  if code.branch_target(ins) is not None
                  and lo <= (code.branch_target(ins) & ~1) < hi]
            lits = [ins.address for ins in code.insns
                    if code.literal_value(ins) is not None
                    and lo <= code.literal_value(ins)[1] < hi]
            print("  0x%08X %4d B  原始引用=%d BL=%d B=%d LDRlit=%d"
                  % (lo, ln, len(refs), len(bl), len(br), len(lits)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
