# -*- coding: utf-8 -*-
"""V1C C.Fn 开关补丁原型（cfn_proto）：验证 56B 同步字函数 + 向量表注入 + 两处 gate hook 的编码与行为。

不写固件；只在内存镜像上构造并用 Unicorn 执行。
"""
import struct
import sys

FW = r'devices/v1c/firmware/V1C_V2.0.bin'
BASE = 0x08003000
SYNC_FN = 0x080121BC
SYNC_FN_END = 0x080121F4
DIV_FUNC = 0x080030F0
WRITE_REG = 0x080156CE
DELAY_US = 0x08015440
SETTINGS = 0x20001086
INJ = 0x08003010          # 向量表保留项 4..10（Cortex-M0 不用），28 字节
CALL_DEC = 0x0800625C     # cmp r1,#6; beq sync
CALL_INC = 0x0800D46E     # cmp r1,#6; beq sync

ORIG_FUNC = bytes.fromhex(
    "0c4a10b5411c1046f0f794ff0446010a042003f07efae1b2052003f07afa"
    "142003f030f9332003f0d5fa7d20c00003f029f910bd68c30000")


def _bl(addr, target):
    off = target - (addr + 4)
    assert -0x400000 <= off <= 0x3FFFFE and off % 2 == 0, (hex(addr), hex(target), off)
    s = 1 if off < 0 else 0
    i1 = (off >> 23) & 1
    i2 = (off >> 22) & 1
    j1 = (i1 if s else ((~i1) & 1))
    j2 = (i2 if s else ((~i2) & 1))
    imm10 = (off >> 12) & 0x3FF
    imm11 = (off >> 1) & 0x7FF
    return struct.pack('<HH', 0xF000 | (s << 10) | imm10,
                       0xD000 | (j1 << 13) | (j2 << 11) | imm11)


def _ldr_lit(rt, instr_addr, target):
    pc = (instr_addr + 4) & ~3
    imm8 = (target - pc) // 4
    assert (target - pc) % 4 == 0 and 0 <= imm8 <= 0xFF, (hex(target), hex(pc), imm8)
    return struct.pack('<H', 0x4800 | ((rt & 7) << 8) | imm8)


def build_sync_fn_flag(key, flag_addr):
    """AD100 v4 布局：44B 代码 + KEY@+0x2C + FLAG@+0x30 + 50024@+0x34。"""
    S = SYNC_FN
    key_lit = S + 0x2C
    flag_lit = S + 0x30
    orig_lit = S + 0x34
    code = bytearray()
    code += b'\x10\xb5'                       # push {r4,lr}
    code += _ldr_lit(3, S + len(code), flag_lit)   # +02 ldr r3,=FLAG
    code += _ldr_lit(2, S + len(code), orig_lit)   # +04 ldr r2,=50024
    code += b'\x1b\x78'                       # +06 ldrb r3,[r3]
    code += b'\x41\x1c'                       # +08 adds r1,r0,#1
    code += b'\x00\x2b'                       # +0A cmp r3,#0
    code += b'\x01\xd0'                       # +0C beq go
    code += _ldr_lit(2, S + len(code), key_lit)    # +0E ldr r2,=KEY
    code += b'\x01\x21'                       # +10 movs r1,#1
    code += b'\x10\x46'                       # +12 go: mov r0,r2
    code += _bl(S + len(code), DIV_FUNC)      # +14 bl uidiv
    code += b'\x04\x46'                       # +18 mov r4,r0
    code += b'\x01\x0a'                       # +1A lsrs r1,r0,#8
    code += b'\x04\x20'                       # +1C movs r0,#4
    code += _bl(S + len(code), WRITE_REG)     # +1E
    code += b'\xe1\xb2'                       # +22 uxtb r1,r4
    code += b'\x05\x20'                       # +24 movs r0,#5
    code += _bl(S + len(code), WRITE_REG)     # +26
    code += b'\x10\xbd'                       # +2A pop {r4,pc}
    assert len(code) == 44, len(code)
    blob = bytes(code) + struct.pack('<III', key, flag_addr, 50024)
    assert len(blob) == 56
    return blob


def build_injection(flag_idx):
    """18 字节：push{r0,r1,lr}; if item==6||item==flag: sync(settings[6]); pop{r0,r1,pc}"""
    assert 0 < flag_idx < 256 and flag_idx != 6
    code = bytearray()
    code += b'\x03\xb5'                          # +00 push {r0,r1,lr}   (0xB503)
    code += b'\x06\x29'                          # +02 cmp r1,#6
    code += b'\x01\xd0'                          # +04 beq do (+0x0A)
    code += struct.pack('<H', 0x2900 | flag_idx) # +06 cmp r1,#flag
    code += b'\x02\xd1'                          # +08 bne out (+0x10)
    code += b'\x80\x79'                          # +0A ldrb r0,[r0,#6]
    code += _bl(INJ + len(code), SYNC_FN)        # +0C bl sync
    code += b'\x03\xbd'                          # +10 pop {r0,r1,pc}
    assert len(code) == 18, len(code)
    return bytes(code)


def apply(data, key, flag_idx):
    assert len(data) == 88032
    buf = bytearray(data)
    # 1) sync fn
    off = SYNC_FN - BASE
    assert bytes(buf[off:off + 56]) == ORIG_FUNC
    buf[off:off + 56] = build_sync_fn_flag(key, SETTINGS + flag_idx)
    # 2) injection
    inj = build_injection(flag_idx)
    off = INJ - BASE
    assert bytes(buf[off:off + 28]) == b'\x00' * 28, bytes(buf[off:off + 28]).hex()
    buf[off:off + len(inj)] = inj
    # 3) hook sites
    for site in (CALL_DEC, CALL_INC):
        o = site - BASE
        assert bytes(buf[o:o + 4]) in (bytes.fromhex('062908d0'), bytes.fromhex('062907d0')), bytes(buf[o:o + 4]).hex()
        buf[o:o + 4] = _bl(site, INJ)
    return bytes(buf)


def main():
    d = open(FW, 'rb').read()
    for flag_idx in (7, 8, 10, 11):
        img = apply(d, 0xD798, flag_idx)
        print('flag_idx=%d image len=%d diff_bytes=%d' % (
            flag_idx, len(img), sum(1 for i in range(len(d)) if d[i] != img[i])))
    print('injection[7]:', build_injection(7).hex(' '))
    print('sync fn[7]:', build_sync_fn_flag(0xD798, SETTINGS + 7).hex(' '))


if __name__ == '__main__':
    main()
