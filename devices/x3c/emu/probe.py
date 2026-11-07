"""Quick entry-point probe: does the firmware execute from the vector-table reset,
from the relocatable stub at 0x080002F8, or from image2's stub?

This is a throwaway recon tool (superseded by machine.py/run.py).
"""
import os, struct, sys
from unicorn import *
from unicorn.arm_const import *
from capstone import *

IMG = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                   "..", "firmware", "X3_C_v1.38_app.bin")
d = open(IMG, "rb").read()

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
md.detail = False


def make(verbose_mmio=True):
    uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
    # flash: 2 MB
    uc.mem_map(0x08000000, 0x200000)
    uc.mem_write(0x08000000, d)
    # RAM
    uc.mem_map(0x20000000, 0x80000)
    # CCM
    uc.mem_map(0x10000000, 0x10000)
    # periph arena + PPB as plain memory filled with 0
    uc.mem_map(0x40000000, 0x400000)
    uc.mem_map(0xA0000000, 0x100000)
    uc.mem_map(0xE0000000, 0x100000)
    # USB
    uc.mem_map(0x50000000, 0x100000)
    # system memory (boot rom)
    uc.mem_map(0x1FFF0000, 0x10000)

    uc.hook_add(UC_HOOK_MEM_READ_UNMAPPED | UC_HOOK_MEM_WRITE_UNMAPPED,
                unmapped_cb, None)
    uc.hook_add(UC_HOOK_MEM_FETCH_UNMAPPED, fetch_cb, None)
    uc.hook_add(UC_HOOK_MEM_INVALID, invalid_cb, None)
    return uc


def unmapped_cb(uc, access, addr, size, value, ud):
    print(f"  [unmapped {'R' if access == UC_MEM_READ_UNMAPPED else 'W'}] pc={uc.reg_read(UC_ARM_REG_PC):#x} addr={addr:#x}")
    try:
        uc.mem_map(addr & ~0xFFF, 0x1000)
        return True
    except UcError:
        return False


def fetch_cb(uc, access, addr, size, value, ud):
    print(f"  [fetch unmapped] pc={uc.reg_read(UC_ARM_REG_PC):#x} addr={addr:#x}")
    return False


def invalid_cb(uc, access, addr, size, value, ud):
    print(f"  [invalid mem] pc={uc.reg_read(UC_ARM_REG_PC):#x} addr={addr:#x} access={access} size={size}")
    return False


def run(name, pc, sp, max_insn=400):
    print(f"\n===== {name}: pc={pc:#x} sp={sp:#x}")
    uc = make()
    uc.reg_write(UC_ARM_REG_SP, sp)
    uc.reg_write(UC_ARM_REG_PC, pc | 1)
    n = 0
    last = []
    for step in range(max_insn // 10):
        try:
            uc.emu_start(pc | 1, 0xFFFFFFFE, 0, 10)
        except UcError as e:
            print(f"  UcError after {n} insns: {e}")
            break
        pc = uc.reg_read(UC_ARM_REG_PC)
        n += 10
        last.append(pc)
        if step < 40:
            try:
                for i in md.disasm(uc.mem_read(pc, 8), pc):
                    print(f"  {n:5d} {i.address:#010x} {i.mnemonic} {i.op_str}")
                    break
            except UcError:
                print(f"  {n:5d} pc={pc:#x} (unreadable)")
    print(f"  final pc={pc:#x} lr={uc.reg_read(UC_ARM_REG_LR):#x} sp={uc.reg_read(UC_ARM_REG_SP):#x}")


if __name__ == "__main__":
    sp0 = struct.unpack_from("<I", d, 0)[0]
    rst = struct.unpack_from("<I", d, 4)[0] & ~1
    run("vector-reset", rst, sp0)
    run("stub@0x2F8", 0x080002F8, 0x20001000)
