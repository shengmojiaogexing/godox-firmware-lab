# -*- coding: utf-8 -*-
"""Lead 独立验证 harness：直接执行固件里的"同步字设置函数"，记录它对 CC2500 的写。

不依赖各设备模块的 verify()，用于交叉核对队友的补丁。用法::

    tools\python\python.exe keypatch\dev\lead\call_harness.py

它会分别对原厂镜像和"临时打补丁"镜像执行 ID=0/1/37/99，打印实际写入的 SYNC1/SYNC0。
"""
from __future__ import annotations

import os
import struct
import sys

from unicorn import (Uc, UcError, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS,
                     UC_HOOK_CODE)
from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                               UC_ARM_REG_R3, UC_ARM_REG_SP, UC_ARM_REG_LR,
                               UC_ARM_REG_PC, UC_ARM_REG_CPSR)

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))

SENTINEL = 0xFFFFFFF0  # 函数返回哨兵（奇数 -> Thumb）

# 每个设备的 harness 参数
DEVICES = {
    "ad100": dict(
        image=os.path.join(ROOT, "devices", "ad100pro", "firmware", "AD100pro_V1.2.bin"),
        base=0x08003000, entry=0x0800B758, div=0x080030F0,
        write_reg=0x0800CD32, strobe=0x0800CDF4, delay=0x0800CAB8,
        sp=0x20000C00, ram=(0x20000000, 0x4000), arch="m0",
        patch=lambda d: _patch_ad100(d),
    ),
    "ad200": dict(
        image=os.path.join(ROOT, "devices", "ad200pro", "firmware", "AD200pro_V1.60.bin"),
        base=0x08003000, entry=0x0800B9AC, div=0x080030F0,
        write_reg=0x0800CAA2, strobe=0x0800CB64, delay=0x0800C828,
        sp=0x20000880, ram=(0x20000000, 0x2000), arch="m0",
        patch=lambda d: _patch_ad200(d),
    ),
    "x3c": dict(
        image=os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin"),
        base=0x08004800, entry=0x08014864, div=None,
        write_reg=0x08023540, strobe=0x08023520, delay=0x080232B6,
        sp=0x20047000, ram=(0x20000000, 0x48000), arch="m4",
        patch=lambda d: _patch_x3c(d),
    ),
    # Godox V1C V2.0：ID==0 -> KEY，ID!=0 -> 50024/(ID+1)（语义与 AD100 id0 版一致）
    "v1c": dict(
        image=os.path.join(ROOT, "devices", "v1c", "firmware", "V1C_V2.0.bin"),
        base=0x08003000, entry=0x080121BC, div=0x080030F0,
        write_reg=0x080156CE, strobe=0x08015790, delay=0x08015440,
        sp=0x20001400, ram=(0x20000000, 0x4000), arch="m0",
        patch=lambda d: _patch_v1c(d),
    ),
    # Godox V1S V1.8：C.Fn LIGHT 开关（RAM 0x2000105C==2 -> KEY）
    "v1s": dict(
        image=os.path.join(ROOT, "devices", "v1s", "firmware", "V1S_V1.8.bin"),
        base=0x08003000, entry=0x080112DC, div=0x080030F0,
        write_reg=0x0801365A, strobe=0x0801371C, delay=0x080133CC,
        sp=0x20001400, ram=(0x20000000, 0x4000), arch="m0",
        patch=lambda d: _patch_v1s(d),
    ),
    # Godox V860III-S V1.1：C.Fn LIGHT 开关（RAM 0x20001060==2 -> KEY）
    "v860iiis": dict(
        image=os.path.join(ROOT, "devices", "v860iiis", "firmware", "V860IIIS_V1.1.bin"),
        base=0x08003000, entry=0x080115B8, div=0x080030F0,
        write_reg=0x08013926, strobe=0x080139E8, delay=0x08013698,
        sp=0x20001400, ram=(0x20000000, 0x4000), arch="m0",
        patch=lambda d: _patch_v860iiis(d),
    ),
    # Godox V860III-C V1.3：C.Fn LIGHT 开关（RAM 0x20001092==2 -> KEY）
    "v860iiic": dict(
        image=os.path.join(ROOT, "devices", "v860iiic", "firmware", "V860IIIC_V1.3.bin"),
        base=0x08003000, entry=0x080122E8, div=0x080030F0,
        write_reg=0x080157EE, strobe=0x080158B0, delay=0x08015560,
        sp=0x20001400, ram=(0x20000000, 0x4000), arch="m0",
        patch=lambda d: _patch_v860iiic(d),
    ),
}


def _movw_hi_lo(hw1, hw2):
    return struct.pack("<HH", hw1, hw2)


def _patch_ad100(data: bytearray) -> None:
    # 仅用于 harness 的临时补丁（正式补丁在 devices/ad100.py）
    off = 0x0800B75C - 0x08003000
    assert data[off:off + 2] == b"\x41\x1c", data[off:off + 2].hex()
    data[off:off + 2] = b"\x01\x21"
    off = 0x0800B78C - 0x08003000
    assert data[off:off + 4] == b"\x68\xc3\x00\x00", data[off:off + 4].hex()
    data[off:off + 2] = struct.pack("<H", 0x6B21)


def _patch_ad200(data: bytearray) -> None:
    off = 0x0800B9B0 - 0x08003000
    assert data[off:off + 2] == b"\x41\x1c", data[off:off + 2].hex()
    data[off:off + 2] = b"\x01\x21"
    off = 0x0800B9E0 - 0x08003000
    assert data[off:off + 4] == b"\x68\xc3\x00\x00", data[off:off + 4].hex()
    data[off:off + 2] = struct.pack("<H", 0x6B21)


def movw_encoding(imm16: int, rd: int = 1) -> bytes:
    imm4 = (imm16 >> 12) & 0xF
    i = (imm16 >> 11) & 1
    imm3 = (imm16 >> 8) & 0x7
    imm8 = imm16 & 0xFF
    hw1 = 0xF240 | (i << 10) | imm4
    hw2 = (imm3 << 12) | (rd << 8) | imm8
    return struct.pack("<HH", hw1, hw2)


def _patch_x3c(data: bytearray) -> None:
    off = 0x08014866 - 0x08004800
    assert data[off:off + 4] == bytes.fromhex("4cf26831"), data[off:off + 4].hex()
    data[off:off + 4] = movw_encoding(0x6B21, rd=1)
    off = 0x0801486A - 0x08004800
    assert data[off:off + 2] == b"\x40\x1c", data[off:off + 2].hex()
    data[off:off + 2] = b"\x01\x20"


def _patch_v1c(data: bytearray) -> None:
    """V1C：直接用正式模块的 apply()（含 C.Fn hook/读者屏蔽）。"""
    keypatch_dir = os.path.join(ROOT, "keypatch")
    if keypatch_dir not in sys.path:
        sys.path.insert(0, keypatch_dir)
    from devices import v1c as v1c_mod  # noqa: E402
    patched, _log = v1c_mod.apply(bytes(data), 0x6B21)
    assert len(patched) == len(data)
    data[:] = patched


def _patch_v1s(data: bytearray) -> None:
    """V1S：直接用正式模块的 apply()（含 C.Fn 门改 nop / 读者屏蔽）。"""
    keypatch_dir = os.path.join(ROOT, "keypatch")
    if keypatch_dir not in sys.path:
        sys.path.insert(0, keypatch_dir)
    from devices import v1s as v1s_mod  # noqa: E402
    patched, _log = v1s_mod.apply(bytes(data), 0x6B21)
    assert len(patched) == len(data)
    data[:] = patched


def _patch_v860iiis(data: bytearray) -> None:
    """V860III-S：直接用正式模块的 apply()（含 C.Fn hook/读者屏蔽）。"""
    keypatch_dir = os.path.join(ROOT, "keypatch")
    if keypatch_dir not in sys.path:
        sys.path.insert(0, keypatch_dir)
    from devices import v860iiis as v860_mod  # noqa: E402
    patched, _log = v860_mod.apply(bytes(data), 0x6B21)
    assert len(patched) == len(data)
    data[:] = patched


def _patch_v860iiic(data: bytearray) -> None:
    """V860III-C：直接用正式模块的 apply()（含 C.Fn hook/读者屏蔽）。"""
    keypatch_dir = os.path.join(ROOT, "keypatch")
    if keypatch_dir not in sys.path:
        sys.path.insert(0, keypatch_dir)
    from devices import v860iiic as v860c_mod  # noqa: E402
    patched, _log = v860c_mod.apply(bytes(data), 0x6B21)
    assert len(patched) == len(data)
    data[:] = patched


def run(dev_name: str, data: bytes, ids=(0, 1, 37, 99), key=0x6B21,
        pre_ram: dict | None = None):
    cfg = DEVICES[dev_name]
    base = cfg["base"]
    uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
    flash_size = ((len(data) + 0xFFF) // 0x1000) * 0x1000
    uc.mem_map(base, flash_size)
    uc.mem_write(base, bytes(data))
    rstart, rsize = cfg["ram"]
    uc.mem_map(rstart, rsize)

    log = []       # (what, r0, r1)
    stop = [False]

    def on_code(uc, address, size, user):
        if address == SENTINEL:
            stop[0] = True
            uc.emu_stop()
            return
        if address == cfg["div"]:
            r0 = uc.reg_read(UC_ARM_REG_R0)
            r1 = uc.reg_read(UC_ARM_REG_R1)
            q = 0 if r1 == 0 else r0 // r1
            uc.reg_write(UC_ARM_REG_R0, q & 0xFFFFFFFF)
            uc.reg_write(UC_ARM_REG_PC, uc.reg_read(UC_ARM_REG_LR))
            return
        if address in (cfg["write_reg"], cfg["strobe"], cfg["delay"]):
            r0 = uc.reg_read(UC_ARM_REG_R0)
            r1 = uc.reg_read(UC_ARM_REG_R1)
            kind = {cfg["write_reg"]: "reg", cfg["strobe"]: "strobe",
                    cfg["delay"]: "delay"}[address]
            log.append((kind, r0, r1))
            uc.reg_write(UC_ARM_REG_PC, uc.reg_read(UC_ARM_REG_LR))
            return

    uc.hook_add(UC_HOOK_CODE, on_code)

    results = {}
    for ident in ids:
        log.clear()
        stop[0] = False
        if pre_ram:
            for addr, val in pre_ram.items():
                uc.mem_write(addr, val)
        uc.reg_write(UC_ARM_REG_SP, cfg["sp"])
        uc.reg_write(UC_ARM_REG_LR, SENTINEL | 1)
        uc.reg_write(UC_ARM_REG_PC, cfg["entry"] | 1)
        uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)
        uc.reg_write(UC_ARM_REG_R0, ident)
        finished = False
        try:
            uc.emu_start(cfg["entry"] | 1, 0, count=20000)
        except UcError as e:
            # 函数用 pop {r4,pc} 返回，哨兵地址未映射 -> Unicorn 报异常；
            # 只要 PC 落在哨兵上就说明函数已正常执行完。
            if (uc.reg_read(UC_ARM_REG_PC) & ~1) == SENTINEL:
                finished = True
            else:
                results[ident] = ("UC_ERR", str(e), list(log))
                continue
        if not finished:
            results[ident] = ("NO_RETURN", "函数没有返回到哨兵", list(log))
            continue
        regs = {r: v for (k, r, v) in log if k == "reg"}
        sync = (regs.get(4, -1) << 8) | regs.get(5, -1) if 4 in regs and 5 in regs else None
        strobes = [r for (k, r, v) in log if k == "strobe"]
        results[ident] = (sync, regs, strobes)
    return results


def main() -> int:
    key = 0x6B21
    ok_all = True
    # 用「C.Fn 开关」语义的设备：模块里声明 KEY_FLAG_ADDR / KEY_FLAG_ON_VALUE
    cfn_devices = {"v1c", "v1s", "v860iiis", "v860iiic"}
    for dev in ("ad100", "ad200", "v1c", "v1s", "v860iiis", "v860iiic", "x3c"):
        cfg = DEVICES[dev]
        orig = bytearray(open(cfg["image"], "rb").read())
        patched = bytearray(orig)
        cfg["patch"](patched)
        print("=" * 78)
        print("设备 %-9s 镜像 %s  (%d B)" % (dev, os.path.basename(cfg["image"]), len(orig)))
        print("-" * 78)
        r_orig = run(dev, orig)
        if dev in cfn_devices:
            # C.Fn 开关语义：RAM[KEY_FLAG_ADDR]=ON -> KEY（任意 ID）；=0 -> 原厂公式。
            keypatch_dir = os.path.join(ROOT, "keypatch")
            if keypatch_dir not in sys.path:
                sys.path.insert(0, keypatch_dir)
            mod = __import__("devices." + dev, fromlist=["x"])
            flag_addr = int(getattr(mod, "KEY_FLAG_ADDR"))
            on_value = int(getattr(mod, "KEY_FLAG_ON_VALUE", 1)) & 0xFF
            r_on = run(dev, patched, pre_ram={flag_addr: bytes([on_value])})
            r_off = run(dev, patched, pre_ram={flag_addr: b"\x00"})
            for ident in (0, 1, 37, 99):
                o = r_orig[ident]
                exp_orig = 50024 // (ident + 1)
                on = r_on[ident][0]
                off = r_off[ident][0]
                p = (o[0] == exp_orig and on == key and off == exp_orig)
                ok_all = ok_all and p
                print("  ID=%-2d 原厂 sync=0x%04X (期望 0x%04X) | 开关ON sync=0x%04X (期望 0x%04X) | "
                      "开关OFF sync=0x%04X (期望 0x%04X) %s"
                      % (ident, o[0] if o[0] is not None else -1, exp_orig,
                         on if on is not None else -1, key,
                         off if off is not None else -1, exp_orig,
                         "PASS" if p else "FAIL"))
            continue
        r_pat = run(dev, patched)
        for ident in (0, 1, 37, 99):
            o = r_orig[ident]
            p = r_pat[ident]
            exp_orig = 50024 // (ident + 1)
            exp_pat = key
            print("  ID=%-2d 原厂 sync=0x%04X (期望 0x%04X) | 补丁后 sync=0x%04X (期望 0x%04X=%s)"
                  % (ident, o[0] if o[0] is not None else -1, exp_orig,
                     p[0] if p[0] is not None else -1, exp_pat,
                     "PASS" if p[0] == exp_pat else "FAIL"))
            if o[0] != exp_orig or p[0] != exp_pat:
                ok_all = False
                print("      原厂记录=%r\n      补丁记录=%r" % (o, p))
        if r_pat[0][2]:
            print("  补丁后 strobe 序列: %s" % ", ".join("0x%02X" % s for s in r_pat[0][2]))
    print("=" * 78)
    print("结论:", "ALL PASS" if ok_all else "FAIL")
    return 0 if ok_all else 1


if __name__ == "__main__":
    sys.exit(main())
