# -*- coding: utf-8 -*-
"""X2T-S 侦察 4：外设/RAM 字面量地图、尾部数据表、flash 保存链路线索。"""
import struct
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

IMG = r"D:\神牛\devices\x2ts\firmware\X2T-S_V1.4.bin"
DATA = open(IMG, "rb").read()
BASE = 0x08000000
SIZE = len(DATA)


def main():
    print("=== tail 0xE380..0xE504 hex ===")
    for a in range(0xE380, SIZE, 16):
        print("  0x%05X  %s" % (a, DATA[a:a + 16].hex(" ")))
    print()

    # 所有 32 位字面量（对齐）+ 引用它的 LDR 字面量指令
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
    md.detail = True
    ldr_refs = {}   # literal_addr -> [(insn_addr, rt)]
    for off in range(0, SIZE - 4, 2):
        for ins in md.disasm(DATA[off:off + 4], BASE + off):
            if ins.mnemonic in ("ldr", "ldr.w") and ins.operands:
                op = ins.operands[0]
                if len(ins.operands) >= 2 and ins.operands[1].type == 2:  # MEM
                    mem = ins.operands[1].mem
                    if mem.base == 0 and mem.index == 0:
                        lit = mem.disp
                        if BASE <= lit < BASE + SIZE:
                            ldr_refs.setdefault(lit, []).append(ins.address)
            break

    print("=== literals in 0x40000000..0x50000000 (peripheral) ===")
    seen = {}
    for off in range(0, SIZE - 3, 4):
        v = struct.unpack_from("<I", DATA, off)[0]
        if 0x40000000 <= v < 0x50000000:
            seen.setdefault(v, []).append(BASE + off)
    for v in sorted(seen):
        print("  0x%08X x%d at %s" % (v, len(seen[v]), [hex(x) for x in seen[v][:6]]))
    print()

    print("=== literals in 0x20000000..0x20030000 (RAM) ===")
    seen = {}
    for off in range(0, SIZE - 3, 4):
        v = struct.unpack_from("<I", DATA, off)[0]
        if 0x20000000 <= v < 0x20030000:
            seen.setdefault(v, []).append(BASE + off)
    for v in sorted(seen):
        print("  0x%08X x%d at %s" % (v, len(seen[v]), [hex(x) for x in seen[v][:6]]))
    print()

    print("=== data-ish literals (flash pointers to 0x0800A000..0x0800F000) ===")
    cnt = 0
    for off in range(0, SIZE - 3, 4):
        v = struct.unpack_from("<I", DATA, off)[0]
        if 0x0800A000 <= v < BASE + SIZE:
            refs = ldr_refs.get(BASE + off, [])
            print("  0x%08X (at 0x%05X) refs=%s" % (v, off, [hex(x) for x in refs[:6]]))
            cnt += 1
            if cnt > 80:
                print("  ...")
                break
    print()

    print("=== search byte seq '1f 63' ===")
    idx = 0
    while True:
        i = DATA.find(b"\x1f\x63", idx)
        if i < 0:
            break
        print("  off 0x%05X addr 0x%08X ctx %s" % (i, BASE + i, DATA[max(0, i - 8):i + 12].hex(" ")))
        idx = i + 1
    print()

    print("=== search byte seq '01 01 01 01' near end ===")
    idx = 0
    while True:
        i = DATA.find(b"\x01\x01\x01\x01", idx)
        if i < 0:
            break
        if i > 0xC000:
            print("  off 0x%05X addr 0x%08X ctx %s" % (i, BASE + i, DATA[max(0, i - 8):i + 20].hex(" ")))
        idx = i + 1


if __name__ == "__main__":
    main()
