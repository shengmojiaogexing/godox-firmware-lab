# -*- coding: utf-8 -*-
"""X2T-S V1.4 侦察 3：定位同步字函数（movw 0xC368 附近）。"""
import struct
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

IMG = r"D:\神牛\devices\x2ts\firmware\X2T-S_V1.4.bin"
DATA = open(IMG, "rb").read()
BASE = 0x08000000


def dis(addr, nbytes):
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
    md.detail = True
    off = addr - BASE
    count = 0
    for ins in md.disasm(DATA[off:off + nbytes], addr):
        mark = ""
        print("  0x%08X  %-20s %s %s" % (ins.address, ins.bytes.hex(" "), ins.mnemonic, ins.op_str))
        count += 1


def main():
    print("=== 0x080097C0 .. 0x080099A0 ===")
    dis(0x080097C0, 0x1E0)
    print()
    # 找 0x080098FA 所在函数的 BL 调用者
    print("=== find BL/B.W targets into 0x080098xx..0x080099xx ===")
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
    md.detail = True
    seen = 0
    for off in range(0, len(DATA) - 4, 2):
        for ins in md.disasm(DATA[off:off + 4], BASE + off):
            if ins.mnemonic in ("bl", "bl.w", "b.w", "b") and ins.operands:
                tgt = ins.operands[0].imm
                if 0x08009800 <= tgt < 0x08009A00:
                    print("  caller 0x%08X -> 0x%08X (%s)" % (ins.address, tgt, ins.mnemonic))
                    seen += 1
            break
    print("  total:", seen)


if __name__ == "__main__":
    main()
