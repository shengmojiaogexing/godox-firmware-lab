# -*- coding: utf-8 -*-
"""对比 0x0800B758 补丁前后的反汇编（capstone），写入 UTF-8 日志。

运行：python disasm_region.py [out.log] [key]
"""
import os
import struct
import sys

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

ROOT = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "keypatch"))
from devices import ad100  # noqa: E402

ORIG = os.path.join(ROOT, "devices", "ad100pro", "firmware", "AD100pro_V1.2.bin")
KEY = 0xD798


def dump(title, blob):
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
    print("---- %s ----" % title)
    for ins in md.disasm(blob[:ad100.SYNC_FN_SIZE], ad100.SYNC_FN):
        print("  0x%08X  %-11s %-8s %s" % (ins.address, ins.bytes.hex(" "), ins.mnemonic, ins.op_str))


def main():
    a = open(ORIG, "rb").read()
    out = os.path.join(ROOT, "keypatch", "out", ad100.DEVICE["out_name"].format(key=KEY))
    b = open(out, "rb").read()
    off = ad100.SYNC_FN - ad100.BASE
    print("补丁区: 0x%08X..0x%08X（%d 字节，下一函数 0x%08X）"
          % (ad100.SYNC_FN, ad100.SYNC_FN_END - 1, ad100.SYNC_FN_SIZE, ad100.SYNC_FN_END))
    print("KEY=0x%04X, 标志字节地址=0x%08X" % (KEY, ad100.KEY_FLAG_ADDR))
    dump("补丁前（原厂）", a[off:off + ad100.SYNC_FN_SIZE])
    print()
    dump("补丁后（v4, KEY=0x%04X）" % KEY, b[off:off + ad100.SYNC_FN_SIZE])
    print()
    print("原始字节: %s" % a[off:off + ad100.SYNC_FN_SIZE].hex(" "))
    print("补丁字节: %s" % b[off:off + ad100.SYNC_FN_SIZE].hex(" "))
    print("差异偏移: %s" % ", ".join("0x%05X" % i for i in range(off, off + ad100.SYNC_FN_SIZE)
                                    if a[i] != b[i]))
    lits = struct.unpack("<III", b[off + 44:off + 56])
    print("字面量[+0x2C] KEY   : 0x%08X @0x%08X" % (lits[0], ad100.SYNC_FN + 44))
    print("字面量[+0x30] FLAG  : 0x%08X @0x%08X" % (lits[1], ad100.SYNC_FN + 48))
    print("字面量[+0x34] 50024 : 0x%08X @0x%08X（与原厂字面量地址相同）"
          % (lits[2], ad100.SYNC_FN + 52))
    return 0


if __name__ == "__main__":
    if len(sys.argv) > 1:
        import contextlib
        with open(sys.argv[1], "w", encoding="utf-8", newline="\n") as f, \
                contextlib.redirect_stdout(f):
            rc = main()
        sys.exit(rc)
    sys.exit(main())
