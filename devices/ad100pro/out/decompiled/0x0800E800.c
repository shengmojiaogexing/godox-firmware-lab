/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800E800
 * Name      : FUN_0800E800
 * End       : 0x0800EAD6
 * Size      : 726 bytes, 178 instructions decoded in body (functions.json insns=53, blocks=2)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800E800(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800E800: 0009     lsrs     r0, r0, #4 */
    r0 = r0 >> 4;
    /* 0x0800E802: 07f81ffe strbt    pc, [r7, #0x1f] */
    /* TODO: strbt pc, [r7, #0x1f] */
    /* 0x0800E806: 3807     lsls     r0, r7, #0x1c */
    r0 = r7 << 0x1c;
    /* 0x0800E808: 2001     lsls     r0, r4, #4 */
    r0 = r4 << 4;
    /* 0x0800E80A: 2001     lsls     r0, r4, #4 */
    r0 = r4 << 4;
    /* 0x0800E80C: 2001     lsls     r0, r4, #4 */
    r0 = r4 << 4;
    /* 0x0800E80E: 3806     lsls     r0, r7, #0x18 */
    r0 = r7 << 0x18;
    /* 0x0800E810: 1ffe07f8 cdp2     p8, #1, c15, c15, c7, #0 */
    /* TODO: cdp2 p8, #1, c15, c15, c7, #0 */
    /* 0x0800E814: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E816: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E818: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E81A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E81C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E81E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E820: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E822: 0900     movs     r1, r1 */
    r1 = r1;
    /* 0x0800E824: 0004     lsls     r0, r0, #0x10 */
    r0 = r0 << 0x10;
    /* 0x0800E826: 0004     lsls     r0, r0, #0x10 */
    r0 = r0 << 0x10;
    /* 0x0800E828: 000c     lsrs     r0, r0, #0x10 */
    r0 = r0 >> 0x10;
    /* 0x0800E82A: 003f     subs     r7, #0 */
    r7 = r7 - 0;
    /* 0x0800E82C: ff3f     subs     r7, #0xff */
    r7 = r7 - 0xff;
    /* 0x0800E82E: ff00     lsls     r7, r7, #3 */
    r7 = r7 << 3;
    /* 0x0800E830: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E832: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E834: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E836: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E838: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E83A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E83C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E83E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E840: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E842: 0009     lsrs     r0, r0, #4 */
    r0 = r0 >> 4;
    /* 0x0800E844: 0e03     lsls     r6, r1, #0xc */
    r6 = r1 << 0xc;
    /* 0x0800E846: 1e0f     lsrs     r6, r3, #0x1c */
    r6 = r3 >> 0x1c;
    /* 0x0800E848: 301d     adds     r0, r6, #4 */
    r0 = r6 + 4;
    /* 0x0800E84A: 2031     adds     r1, #0x20 */
    r1 = r1 + 0x20;
    /* 0x0800E84C: 2031     adds     r1, #0x20 */
    r1 = r1 + 0x20;
    /* 0x0800E84E: 2061     str      r0, [r4, #0x10] */
    *(uint32_t *)(r4 + 16) = r0;   /* store */
    /* 0x0800E850: 30e1     b        #0x800eab4 */
    goto L_0800EAB4;
    /* 0x0800E964: 03f00000 and      r0, r3, #0 */
    r0 = r3 & 0;
    /* 0x0800E968: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E96A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E96C: 0900     movs     r1, r1 */
    r1 = r1;
    /* 0x0800E96E: 003f     subs     r7, #0 */
    r7 = r7 - 0;
    /* 0x0800E970: ff3f     subs     r7, #0xff */
    r7 = r7 - 0xff;
    /* 0x0800E972: ff30     adds     r0, #0xff */
    r0 = r0 + 0xff;
    /* 0x0800E974: c030     adds     r0, #0xc0 */
    r0 = r0 + 0xc0;
    /* 0x0800E976: c030     adds     r0, #0xc0 */
    r0 = r0 + 0xc0;
    /* 0x0800E978: c030     adds     r0, #0xc0 */
    r0 = r0 + 0xc0;
    /* 0x0800E97A: c030     adds     r0, #0xc0 */
    r0 = r0 + 0xc0;
    /* 0x0800E97C: c030     adds     r0, #0xc0 */
    r0 = r0 + 0xc0;
    /* 0x0800E97E: c030     adds     r0, #0xc0 */
    r0 = r0 + 0xc0;
    /* 0x0800E980: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E982: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E984: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E986: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E988: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E98A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E98C: 000d     lsrs     r0, r0, #0x14 */
    r0 = r0 >> 0x14;
    /* 0x0800E98E: 00ffc003 vcgt.u8  q0, q8, q0 */
    /* TODO: vcgt.u8 q0, q8, q0 */
    /* 0x0800E992: fff00fff bl       #0x810e7b4 */
    FUN_0810E7B4();   /* BL -> 0x0810E7B4 FUN_0810E7B4 */
    /* 0x0800E996: fc0f     lsrs     r4, r7, #0x1f */
    r4 = r7 >> 0x1f;
    /* 0x0800E998: fffc1f00 ldc2l    p0, c0, [pc], #0x7c */
    /* TODO: ldc2l p0, c0, [pc], #0x7c */
    /* 0x0800E99C: 3e1c     adds     r6, r7, #0 */
    r6 = r7 + 0;
    /* 0x0800E99E: 000e     lsrs     r0, r0, #0x18 */
    r0 = r0 >> 0x18;
    /* 0x0800E9A0: 1c00     movs     r4, r3 */
    r4 = r3;
    /* 0x0800E9A2: 0e1c     adds     r6, r1, #0 */
    r6 = r1 + 0;
    /* 0x0800E9A4: 000e     lsrs     r0, r0, #0x18 */
    r0 = r0 >> 0x18;
    /* 0x0800E9A6: 1f00     movs     r7, r3 */
    r7 = r3;
    /* 0x0800E9A8: 3e0f     lsrs     r6, r7, #0x1c */
    r6 = r7 >> 0x1c;
    /* 0x0800E9AA: fffc0fff ldc2l    p15, c15, [pc], #0x3c */
    /* TODO: ldc2l p15, c15, [pc], #0x3c */
    /* 0x0800E9AE: fc03     lsls     r4, r7, #0xf */
    r4 = r7 << 0xf;
    /* 0x0800E9B0: fff000ff bl       #0x810e7b4 */
    FUN_0810E7B4();   /* BL -> 0x0810E7B4 FUN_0810E7B4 */
    /* 0x0800E9B4: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x0800E9B6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9B8: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9BA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9BC: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9BE: 0d00     movs     r5, r1 */
    r5 = r1;
    /* 0x0800E9C0: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9C2: 00f00001 and      r1, r0, #0 */
    r1 = r0 & 0;
    /* 0x0800E9C6: e000     lsls     r0, r4, #3 */
    r0 = r4 << 3;
    /* 0x0800E9C8: 01e0     b        #0x800e9ce */
    goto L_0800E9CE;

    L_0800E9CE:
    /* 0x0800E9CE: 0780     strh     r7, [r0] */
    *(uint16_t *)(r0) = r7;   /* store */
    /* 0x0800E9D0: 000f     lsrs     r0, r0, #0x1c */
    r0 = r0 >> 0x1c;
    /* 0x0800E9D2: fffe1fff mrc2     p15, #7, apsr_nzcv, c15, c15, #0 */
    /* TODO: mrc2 p15, #7, apsr_nzcv, c15, c15, #0 */
    /* 0x0800E9D6: fe1f     subs     r6, r7, #7 */
    r6 = r7 - 7;
    /* 0x0800E9D8: fffe1fff mrc2     p15, #7, apsr_nzcv, c15, c15, #0 */
    /* TODO: mrc2 p15, #7, apsr_nzcv, c15, c15, #0 */
    /* 0x0800E9DC: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800E9DE: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9E0: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9E2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9E4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9E6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9E8: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9EA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9EC: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9EE: 000d     lsrs     r0, r0, #0x14 */
    r0 = r0 >> 0x14;
    /* 0x0800E9F0: 0180     strh     r1, [r0] */
    *(uint16_t *)(r0) = r1;   /* store */
    /* 0x0800E9F2: 0607     lsls     r6, r0, #0x1c */
    r6 = r0 << 0x1c;
    /* 0x0800E9F4: 801e     subs     r0, r0, #2 */
    r0 = r0 - 2;
    /* 0x0800E9F6: 0f80     strh     r7, [r1] */
    *(uint16_t *)(r1) = r7;   /* store */
    /* 0x0800E9F8: 7e0f     lsrs     r6, r7, #0x1d */
    r6 = r7 >> 0x1d;
    /* 0x0800E9FA: 80fe1e01 mcr2     p1, #4, r0, c0, c14, #0 */
    /* TODO: mcr2 p1, #4, r0, c0, c14, #0 */
    /* 0x0800E9FE: fe1c     adds     r6, r7, #3 */
    r6 = r7 + 3;
    /* 0x0800EA00: 03ee1c07 mcr      p7, #0, r0, c3, c12, #0 */
    /* TODO: mcr p7, #0, r0, c3, c12, #0 */
    /* 0x0800EA04: ce1c     adds     r6, r1, #3 */
    r6 = r1 + 3;
    /* 0x0800EA06: 0f8e     ldrh     r7, [r1, #0x30] */
    r7 = *(uint16_t *)(r1 + 48);
    /* 0x0800EA08: 1e1f     subs     r6, r3, #4 */
    r6 = r3 - 4;
    /* 0x0800EA0A: 0e1f     subs     r6, r1, #4 */
    r6 = r1 - 4;
    /* 0x0800EA0C: fe0e     lsrs     r6, r7, #0x1b */
    r6 = r7 >> 0x1b;
    /* 0x0800EA10: 0e07     lsls     r6, r1, #0x1c */
    r6 = r1 << 0x1c;
    /* 0x0800EA12: f80e     lsrs     r0, r7, #0x1b */
    r0 = r7 >> 0x1b;
    /* 0x0800EA14: 03e0     b        #0x800ea1e */
    goto L_0800EA1E;

    L_0800EA1E:
    /* 0x0800EA1E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EA20: 0d03     lsls     r5, r1, #0xc */
    r5 = r1 << 0xc;
    /* 0x0800EA22: 0060     str      r0, [r0] */
    *(uint32_t *)(r0) = r0;   /* store */
    /* 0x0800EA24: 0700     movs     r7, r0 */
    r7 = r0;
    /* 0x0800EA26: 780f     lsrs     r0, r7, #0x1d */
    r0 = r7 >> 0x1d;
    /* 0x0800EA28: 007c     ldrb     r0, [r0, #0x10] */
    r0 = *(uint8_t *)(r0 + 16);
    /* 0x0800EA2A: 1f00     movs     r7, r3 */
    r7 = r3;
    /* 0x0800EA2C: 7c1e     subs     r4, r7, #1 */
    r4 = r7 - 1;
    /* 0x0800EA2E: 001e     subs     r0, r0, #0 */
    r0 = r0 - 0;
    /* 0x0800EA30: 1c38     subs     r0, #0x1c */
    r0 = r0 - 0x1c;
    /* 0x0800EA32: 0e1c     adds     r6, r1, #0 */
    r6 = r1 + 0;
    /* 0x0800EA34: 380e     lsrs     r0, r7, #0x18 */
    r0 = r7 >> 0x18;
    /* 0x0800EA36: 1e78     ldrb     r6, [r3] */
    r6 = *(uint8_t *)(r3);
    /* 0x0800EA38: 0e1f     subs     r6, r1, #4 */
    r6 = r1 - 4;
    /* 0x0800EA3A: fc1e     subs     r4, r7, #3 */
    r4 = r7 - 3;
    /* 0x0800EA3E: fc07     lsls     r4, r7, #0x1f */
    r4 = r7 << 0x1f;
    /* 0x0800EA40: effc0387 stc2l    p7, c8, [pc], #0xc */
    /* TODO: stc2l p7, c8, [pc], #0xc */
    /* 0x0800EA44: f800     lsls     r0, r7, #3 */
    r0 = r7 << 3;
    /* 0x0800EA46: 03e0     b        #0x800ea50 */
    goto L_0800EA50;

    L_0800EA50:
    /* 0x0800EA50: 000d     lsrs     r0, r0, #0x14 */
    r0 = r0 >> 0x14;
    /* 0x0800EA52: 0003     lsls     r0, r0, #0xc */
    r0 = r0 << 0xc;
    /* 0x0800EA54: e000     lsls     r0, r4, #3 */
    r0 = r4 << 3;
    /* 0x0800EA56: 0fe0     b        #0x800ea78 */
    goto L_0800EA78;
    /* 0x0800EA58: 001e     subs     r0, r0, #0 */
    r0 = r0 - 0;
    /* 0x0800EA5A: e000     lsls     r0, r4, #3 */
    r0 = r4 << 3;
    /* 0x0800EA5C: 7ce0     b        #0x800eb58 */
    FUN_0800EB58();   /* tail call 0x0800EB58 */
    /* 0x0800EA5E: 00f0e003 and      r3, r0, #0xe0 */
    r3 = r0 & 0xe0;
    /* 0x0800EA62: e0e0     b        #0x800ec26 */
    FUN_0800EC26();   /* tail call 0x0800EC26 */

    L_0800EA78:
    /* 0x0800EA78: e000     lsls     r0, r4, #3 */
    r0 = r4 << 3;
    /* 0x0800EA7A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EA7C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EA7E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EA80: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EA82: 0d00     movs     r5, r1 */
    r5 = r1;
    /* 0x0800EA84: 3830     adds     r0, #0x38 */
    r0 = r0 + 0x38;
    /* 0x0800EA86: 07f8381f strb     r1, [r7, #0x38]! */
    *(uint8_t *)(r7 + 56) = r1;   /* store */
    /* 0x0800EA8A: f83c     subs     r4, #0xf8 */
    r4 = r4 - 0xf8;
    /* 0x0800EA8C: 1ff83e1e ldrb.w   r1, [pc, #-0xe3e] */
    r1 = *(uint32_t *)0x0800DC52;   /* = 0x0E709070 */
    /* 0x0800EA90: 301e     subs     r0, r6, #0 */
    r0 = r6 - 0;
    /* 0x0800EA92: 1c70     strb     r4, [r3] */
    *(uint8_t *)(r3) = r4;   /* store */
    /* 0x0800EA94: 0e1c     adds     r6, r1, #0 */
    r6 = r1 + 0;
    /* 0x0800EA96: 700e     lsrs     r0, r6, #0x19 */
    r0 = r6 >> 0x19;
    /* 0x0800EA98: 1c70     strb     r4, [r3] */
    *(uint8_t *)(r3) = r4;   /* store */
    /* 0x0800EA9A: 0e1c     adds     r6, r1, #0 */
    r6 = r1 + 0;
    /* 0x0800EA9C: 781e     subs     r0, r7, #1 */
    r0 = r7 - 1;
    /* 0x0800EA9E: 1c7f     ldrb     r4, [r3, #0x1c] */
    r4 = *(uint8_t *)(r3 + 28);
    /* 0x0800EAA0: fc1c     adds     r4, r7, #3 */
    r4 = r7 + 3;
    /* 0x0800EAA2: 3ffc1c1f ldc2     p15, c1, [pc], #-0x70 */
    /* TODO: ldc2 p15, c1, [pc], #-0x70 */
    /* 0x0800EAA6: f000     lsls     r0, r6, #3 */
    r0 = r6 << 3;
    /* 0x0800EAA8: 07e0     b        #0x800eaba */
    goto L_0800EABA;

    L_0800EAB4:
    /* 0x0800EAB4: 007f     ldrb     r0, [r0, #0x1c] */
    r0 = *(uint8_t *)(r0 + 28);
    /* 0x0800EAB6: c003     lsls     r0, r0, #0xf */
    r0 = r0 << 0xf;
    /* 0x0800EAB8: fff007ff bl       #0x810e8ca */
    FUN_0810E8CA();   /* BL -> 0x0810E8CA FUN_0810E8CA */
    /* 0x0800EABC: f80f     lsrs     r0, r7, #0x1f */
    r0 = r7 >> 0x1f;
    /* 0x0800EABE: fffc0f1c ldc2l    p12, c1, [pc], #0x3c */
    /* TODO: ldc2l p12, c1, [pc], #0x3c */
    /* 0x0800EAC2: 1e1c     adds     r6, r3, #0 */
    r6 = r3 + 0;
    /* 0x0800EAC4: 380e     lsrs     r0, r7, #0x18 */
    r0 = r7 >> 0x18;
    /* 0x0800EAC6: 1c38     subs     r0, #0x1c */
    r0 = r0 - 0x1c;
    /* 0x0800EAC8: 0e1c     adds     r6, r1, #0 */
    r6 = r1 + 0;
    /* 0x0800EACA: 380e     lsrs     r0, r7, #0x18 */
    r0 = r7 >> 0x18;
    /* 0x0800EACC: 1e3c     subs     r4, #0x1e */
    r4 = r4 - 0x1e;
    /* 0x0800EACE: 1e1f     subs     r6, r3, #4 */
    r6 = r3 - 4;
    /* 0x0800EAD0: 3ffc0f1f ldc2     p15, c1, [pc], #-0x3c */
    /* TODO: ldc2 p15, c1, [pc], #-0x3c */
    /* 0x0800EAD4: fc07     lsls     r4, r7, #0x1f */
    r4 = r7 << 0x1f;
}
