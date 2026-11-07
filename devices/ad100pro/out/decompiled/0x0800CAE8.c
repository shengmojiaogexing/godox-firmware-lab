/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CAE8
 * Name      : FUN_0800CAE8
 * End       : 0x0800CC92
 * Size      : 426 bytes, 161 instructions decoded in body (functions.json insns=161, blocks=1)
 * Class     : function
 * Callers   : 2  -> 0x08004AF0 FUN_08004AF0, 0x08004B04 FUN_08004B04
 * Callees   : 4  -> 0x0800CAB8 FUN_0800CAB8, 0x0800CD32 FUN_0800CD32, 0x0800CDF4 FUN_0800CDF4, 0x0800CE0C FUN_0800CE0C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CAE8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CAE8: 38b5     push     {r3, r4, r5, lr} */
    push({r3, r4, r5, lr});
    /* 0x0800CAEA: 6aa0     adr      r0, #0x1a8 */
    r0 = 0x00000000;   /* address */
    /* 0x0800CAEC: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);
    /* 0x0800CAEE: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x0800CAF0: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x0800CAF2: 2921     movs     r1, #0x29 */
    r1 = 0x29;
    /* 0x0800CAF4: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800CAF6: 00f01cf9 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CAFA: 2e21     movs     r1, #0x2e */
    r1 = 0x2e;
    /* 0x0800CAFC: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800CAFE: 00f018f9 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CB02: 0621     movs     r1, #6 */
    r1 = 6;
    /* 0x0800CB04: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x0800CB06: 00f014f9 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CB0A: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800CB0C: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x0800CB0E: 00f010f9 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CB12: c321     movs     r1, #0xc3 */
    r1 = 0xc3;
    /* 0x0800CB14: 0420     movs     r0, #4 */
    r0 = 4;
    /* 0x0800CB16: 00f00cf9 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CB1A: 6821     movs     r1, #0x68 */
    r1 = 0x68;
    /* 0x0800CB1C: 0520     movs     r0, #5 */
    r0 = 5;
    /* 0x0800CB1E: 00f008f9 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CB22: 0421     movs     r1, #4 */
    r1 = 4;
    /* 0x0800CB24: 0620     movs     r0, #6 */
    r0 = 6;
    /* 0x0800CB26: 00f004f9 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CB2A: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800CB2C: 0720     movs     r0, #7 */
    r0 = 7;
    /* 0x0800CB2E: 00f000f9 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CB32: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800CB34: 0820     movs     r0, #8 */
    r0 = 8;
    /* 0x0800CB36: 00f0fcf8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CB3A: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800CB3C: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x0800CB3E: 00f0f8f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CB42: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800CB44: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x0800CB46: 00f0f4f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CB4A: 0721     movs     r1, #7 */
    r1 = 7;
    /* 0x0800CB4C: 0b20     movs     r0, #0xb */
    r0 = 0xb;
    /* 0x0800CB4E: 00f0f0f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CB52: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800CB54: 0c20     movs     r0, #0xc */
    r0 = 0xc;
    /* 0x0800CB56: 00f0ecf8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CB5A: 5c21     movs     r1, #0x5c */
    r1 = 0x5c;
    /* 0x0800CB5C: 0d20     movs     r0, #0xd */
    r0 = 0xd;
    /* 0x0800CB5E: 00f0e8f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CB62: ce21     movs     r1, #0xce */
    r1 = 0xce;
    /* 0x0800CB64: 0e20     movs     r0, #0xe */
    r0 = 0xe;
    /* 0x0800CB66: 00f0e4f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CB6A: c421     movs     r1, #0xc4 */
    r1 = 0xc4;
    /* 0x0800CB6C: 0f20     movs     r0, #0xf */
    r0 = 0xf;
    /* 0x0800CB6E: 00f0e0f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CB72: 1d21     movs     r1, #0x1d */
    r1 = 0x1d;
    /* 0x0800CB74: 1020     movs     r0, #0x10 */
    r0 = 0x10;
    /* 0x0800CB76: 00f0dcf8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CB7A: 3b21     movs     r1, #0x3b */
    r1 = 0x3b;
    /* 0x0800CB7C: 1120     movs     r0, #0x11 */
    r0 = 0x11;
    /* 0x0800CB7E: 00f0d8f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CB82: 7321     movs     r1, #0x73 */
    r1 = 0x73;
    /* 0x0800CB84: 1220     movs     r0, #0x12 */
    r0 = 0x12;
    /* 0x0800CB86: 00f0d4f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CB8A: 2221     movs     r1, #0x22 */
    r1 = 0x22;
    /* 0x0800CB8C: 1320     movs     r0, #0x13 */
    r0 = 0x13;
    /* 0x0800CB8E: 00f0d0f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CB92: f821     movs     r1, #0xf8 */
    r1 = 0xf8;
    /* 0x0800CB94: 1420     movs     r0, #0x14 */
    r0 = 0x14;
    /* 0x0800CB96: 00f0ccf8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CB9A: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800CB9C: 1520     movs     r0, #0x15 */
    r0 = 0x15;
    /* 0x0800CB9E: 00f0c8f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CBA2: 0721     movs     r1, #7 */
    r1 = 7;
    /* 0x0800CBA4: 1620     movs     r0, #0x16 */
    r0 = 0x16;
    /* 0x0800CBA6: 00f0c4f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CBAA: 3021     movs     r1, #0x30 */
    r1 = 0x30;
    /* 0x0800CBAC: 1720     movs     r0, #0x17 */
    r0 = 0x17;
    /* 0x0800CBAE: 00f0c0f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CBB2: 0821     movs     r1, #8 */
    r1 = 8;
    /* 0x0800CBB4: 1820     movs     r0, #0x18 */
    r0 = 0x18;
    /* 0x0800CBB6: 00f0bcf8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CBBA: 1d21     movs     r1, #0x1d */
    r1 = 0x1d;
    /* 0x0800CBBC: 1920     movs     r0, #0x19 */
    r0 = 0x19;
    /* 0x0800CBBE: 00f0b8f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CBC2: 1c21     movs     r1, #0x1c */
    r1 = 0x1c;
    /* 0x0800CBC4: 1a20     movs     r0, #0x1a */
    r0 = 0x1a;
    /* 0x0800CBC6: 00f0b4f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CBCA: c721     movs     r1, #0xc7 */
    r1 = 0xc7;
    /* 0x0800CBCC: 1b20     movs     r0, #0x1b */
    r0 = 0x1b;
    /* 0x0800CBCE: 00f0b0f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CBD2: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800CBD4: 1c20     movs     r0, #0x1c */
    r0 = 0x1c;
    /* 0x0800CBD6: 00f0acf8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CBDA: b221     movs     r1, #0xb2 */
    r1 = 0xb2;
    /* 0x0800CBDC: 1d20     movs     r0, #0x1d */
    r0 = 0x1d;
    /* 0x0800CBDE: 00f0a8f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CBE2: 8721     movs     r1, #0x87 */
    r1 = 0x87;
    /* 0x0800CBE4: 1f20     movs     r0, #0x1f */
    r0 = 0x1f;
    /* 0x0800CBE6: 00f0a4f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CBEA: 6b21     movs     r1, #0x6b */
    r1 = 0x6b;
    /* 0x0800CBEC: 1e20     movs     r0, #0x1e */
    r0 = 0x1e;
    /* 0x0800CBEE: 00f0a0f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CBF2: f821     movs     r1, #0xf8 */
    r1 = 0xf8;
    /* 0x0800CBF4: 2020     movs     r0, #0x20 */
    r0 = 0x20;
    /* 0x0800CBF6: 00f09cf8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CBFA: b621     movs     r1, #0xb6 */
    r1 = 0xb6;
    /* 0x0800CBFC: 2120     movs     r0, #0x21 */
    r0 = 0x21;
    /* 0x0800CBFE: 00f098f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CC02: 1021     movs     r1, #0x10 */
    r1 = 0x10;
    /* 0x0800CC04: 2220     movs     r0, #0x22 */
    r0 = 0x22;
    /* 0x0800CC06: 00f094f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CC0A: ea21     movs     r1, #0xea */
    r1 = 0xea;
    /* 0x0800CC0C: 2320     movs     r0, #0x23 */
    r0 = 0x23;
    /* 0x0800CC0E: 00f090f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CC12: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x0800CC14: 2420     movs     r0, #0x24 */
    r0 = 0x24;
    /* 0x0800CC16: 00f08cf8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CC1A: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800CC1C: 2520     movs     r0, #0x25 */
    r0 = 0x25;
    /* 0x0800CC1E: 00f088f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CC22: 1121     movs     r1, #0x11 */
    r1 = 0x11;
    /* 0x0800CC24: 2620     movs     r0, #0x26 */
    r0 = 0x26;
    /* 0x0800CC26: 00f084f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CC2A: 4121     movs     r1, #0x41 */
    r1 = 0x41;
    /* 0x0800CC2C: 2720     movs     r0, #0x27 */
    r0 = 0x27;
    /* 0x0800CC2E: 00f080f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CC32: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800CC34: 2820     movs     r0, #0x28 */
    r0 = 0x28;
    /* 0x0800CC36: 00f07cf8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CC3A: 5921     movs     r1, #0x59 */
    r1 = 0x59;
    /* 0x0800CC3C: 2920     movs     r0, #0x29 */
    r0 = 0x29;
    /* 0x0800CC3E: 00f078f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CC42: 7f21     movs     r1, #0x7f */
    r1 = 0x7f;
    /* 0x0800CC44: 2a20     movs     r0, #0x2a */
    r0 = 0x2a;
    /* 0x0800CC46: 00f074f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CC4A: 3f21     movs     r1, #0x3f */
    r1 = 0x3f;
    /* 0x0800CC4C: 2b20     movs     r0, #0x2b */
    r0 = 0x2b;
    /* 0x0800CC4E: 00f070f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CC52: 8821     movs     r1, #0x88 */
    r1 = 0x88;
    /* 0x0800CC54: 2c20     movs     r0, #0x2c */
    r0 = 0x2c;
    /* 0x0800CC56: 00f06cf8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CC5A: 3121     movs     r1, #0x31 */
    r1 = 0x31;
    /* 0x0800CC5C: 2d20     movs     r0, #0x2d */
    r0 = 0x2d;
    /* 0x0800CC5E: 00f068f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CC62: 0b21     movs     r1, #0xb */
    r1 = 0xb;
    /* 0x0800CC64: 2e20     movs     r0, #0x2e */
    r0 = 0x2e;
    /* 0x0800CC66: 00f064f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800CC6A: 2246     mov      r2, r4 */
    r2 = r4;
    /* 0x0800CC6C: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x0800CC6E: 7e20     movs     r0, #0x7e */
    r0 = 0x7e;
    /* 0x0800CC70: 00f0ccf8 bl       #0x800ce0c */
    FUN_0800CE0C();   /* BL -> 0x0800CE0C FUN_0800CE0C */
    /* 0x0800CC74: 3320     movs     r0, #0x33 */
    r0 = 0x33;
    /* 0x0800CC76: 00f0bdf8 bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800CC7A: 7d24     movs     r4, #0x7d */
    r4 = 0x7d;
    /* 0x0800CC7C: e400     lsls     r4, r4, #3 */
    r4 = r4 << 3;
    /* 0x0800CC7E: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800CC80: fff71aff bl       #0x800cab8 */
    FUN_0800CAB8();   /* BL -> 0x0800CAB8 FUN_0800CAB8 */
    /* 0x0800CC84: 3920     movs     r0, #0x39 */
    r0 = 0x39;
    /* 0x0800CC86: 00f0b5f8 bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800CC8A: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800CC8C: fff714ff bl       #0x800cab8 */
    FUN_0800CAB8();   /* BL -> 0x0800CAB8 FUN_0800CAB8 */
    /* 0x0800CC90: 38bd     pop      {r3, r4, r5, pc} */
    pop({r3, r4, r5, pc});   /* return */
}
