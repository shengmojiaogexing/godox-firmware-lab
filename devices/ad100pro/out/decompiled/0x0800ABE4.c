/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800ABE4
 * Name      : FUN_0800ABE4
 * End       : 0x0800AC88
 * Size      : 164 bytes, 66 instructions decoded in body (functions.json insns=66, blocks=13)
 * Class     : hub
 * Callers   : 4  -> 0x0800B9C0 FUN_0800B9C0, 0x0800B9F8 FUN_0800B9F8, 0x0800CF74 main, 0x0800CFF8 FUN_0800CFF8
 * Callees   : 9  -> 0x0800741C FUN_0800741C, 0x080074F0 FUN_080074F0, 0x0800AA54 FUN_0800AA54, 0x0800AB7C FUN_0800AB7C, 0x0800B718 FUN_0800B718, 0x0800BDC4 FUN_0800BDC4, 0x0800CA38 FUN_0800CA38, 0x0800CAB8 FUN_0800CAB8, ... +1
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800ABE4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800ABE4: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x0800ABE6: 284c     ldr      r4, [pc, #0xa0]  ; =RAM_2000020E */
    r4 = *(uint32_t *)0x0800AC88;   /* = RAM 0x2000020E */
    /* 0x0800ABE8: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800ABEA: 6071     strb     r0, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r0;   /* store */
    /* 0x0800ABEC: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x0800ABEE: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x0800ABF0: 01f022ff bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800ABF4: 0926     movs     r6, #9 */
    r6 = 9;
    /* 0x0800ABF6: f606     lsls     r6, r6, #0x1b */
    r6 = r6 << 0x1b;
    /* 0x0800ABF8: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x0800ABFA: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x0800ABFC: fcf70efc bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x0800AC00: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800AC02: 00d1     bne      #0x800ac06 */
    if (r0 != 0) goto L_0800AC06;
    /* 0x0800AC04: 0125     movs     r5, #1 */
    r5 = 1;

    L_0800AC06:
    /* 0x0800AC06: 2148     ldr      r0, [pc, #0x84]  ; =0x00002710 */
    r0 = *(uint32_t *)0x0800AC8C;   /* = 0x00002710 */
    /* 0x0800AC08: 01f016ff bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800AC0C: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x0800AC0E: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x0800AC10: fcf704fc bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x0800AC14: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800AC16: 00d1     bne      #0x800ac1a */
    if (r0 != 0) goto L_0800AC1A;
    /* 0x0800AC18: 6d1c     adds     r5, r5, #1 */
    r5 = r5 + 1;

    L_0800AC1A:
    /* 0x0800AC1A: 0027     movs     r7, #0 */
    r7 = 0;
    /* 0x0800AC1C: 022d     cmp      r5, #2 */
    /* flags = cmp(r5, #2) */
    /* 0x0800AC1E: 31d1     bne      #0x800ac84 */
    if (r5 != 2) goto L_0800AC84;
    /* 0x0800AC20: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800AC22: 01f0cff8 bl       #0x800bdc4 */
    FUN_0800BDC4();   /* BL -> 0x0800BDC4 FUN_0800BDC4 */
    /* 0x0800AC26: 6079     ldrb     r0, [r4, #5] */
    r0 = *(uint8_t *)(r4 + 5);
    /* 0x0800AC28: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800AC2A: 2bd1     bne      #0x800ac84 */
    if (r0 != 1) goto L_0800AC84;
    /* 0x0800AC2C: 3620     movs     r0, #0x36 */
    r0 = 0x36;
    /* 0x0800AC2E: 02f0e1f8 bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800AC32: 3320     movs     r0, #0x33 */
    r0 = 0x33;
    /* 0x0800AC34: 02f0def8 bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800AC38: 7d20     movs     r0, #0x7d */
    r0 = 0x7d;
    /* 0x0800AC3A: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x0800AC3C: 01f03cff bl       #0x800cab8 */
    FUN_0800CAB8();   /* BL -> 0x0800CAB8 FUN_0800CAB8 */
    /* 0x0800AC40: 1348     ldr      r0, [pc, #0x4c]  ; =RAM_20000183 */
    r0 = *(uint32_t *)0x0800AC90;   /* = RAM 0x20000183 */
    /* 0x0800AC42: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800AC44: 00f068fd bl       #0x800b718 */
    FUN_0800B718();   /* BL -> 0x0800B718 FUN_0800B718 */
    /* 0x0800AC48: 3620     movs     r0, #0x36 */
    r0 = 0x36;
    /* 0x0800AC4A: 02f0d3f8 bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800AC4E: 3320     movs     r0, #0x33 */
    r0 = 0x33;
    /* 0x0800AC50: 02f0d0f8 bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800AC54: 3b20     movs     r0, #0x3b */
    r0 = 0x3b;
    /* 0x0800AC56: 02f0cdf8 bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800AC5A: 0ee0     b        #0x800ac7a */
    goto L_0800AC7A;

    L_0800AC5C:
    /* 0x0800AC5C: 0222     movs     r2, #2 */
    r2 = 2;
    /* 0x0800AC5E: b421     movs     r1, #0xb4 */
    r1 = 0xb4;
    /* 0x0800AC60: 5020     movs     r0, #0x50 */
    r0 = 0x50;
    /* 0x0800AC62: fff7f7fe bl       #0x800aa54 */
    FUN_0800AA54();   /* BL -> 0x0800AA54 FUN_0800AA54 */
    /* 0x0800AC66: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x0800AC68: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x0800AC6A: fcf7d7fb bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x0800AC6E: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800AC70: 00d0     beq      #0x800ac74 */
    if (r0 == 0) goto L_0800AC74;
    /* 0x0800AC72: 6771     strb     r7, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r7;   /* store */

    L_0800AC74:
    /* 0x0800AC74: 0748     ldr      r0, [pc, #0x1c]  ; =RAM_20000384 */
    r0 = *(uint32_t *)0x0800AC94;   /* = RAM 0x20000384 */
    /* 0x0800AC76: fcf73bfc bl       #0x80074f0 */
    FUN_080074F0();   /* BL -> 0x080074F0 FUN_080074F0 */

    L_0800AC7A:
    /* 0x0800AC7A: 6079     ldrb     r0, [r4, #5] */
    r0 = *(uint8_t *)(r4 + 5);
    /* 0x0800AC7C: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800AC7E: edd1     bne      #0x800ac5c */
    if (r0 != 0) goto L_0800AC5C;
    /* 0x0800AC80: fff77cff bl       #0x800ab7c */
    FUN_0800AB7C();   /* BL -> 0x0800AB7C FUN_0800AB7C */

    L_0800AC84:
    /* 0x0800AC84: 6771     strb     r7, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r7;   /* store */
    /* 0x0800AC86: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
