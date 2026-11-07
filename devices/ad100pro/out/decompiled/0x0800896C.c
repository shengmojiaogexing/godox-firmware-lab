/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800896C
 * Name      : FUN_0800896C
 * End       : 0x08008A1E
 * Size      : 178 bytes, 86 instructions decoded in body (functions.json insns=86, blocks=26)
 * Class     : function
 * Callers   : 2  -> 0x0800CF74 main, 0x0800D014 FUN_0800D014
 * Callees   : 2  -> 0x080030F0 FUN_080030F0, 0x08008948 FUN_08008948
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800896C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800896C: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800896E: 2c4c     ldr      r4, [pc, #0xb0]  ; =RAM_20000134 */
    r4 = *(uint32_t *)0x08008A20;   /* = RAM 0x20000134 */
    /* 0x08008970: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x08008972: 207a     ldrb     r0, [r4, #8] */
    r0 = *(uint8_t *)(r4 + 8);
    /* 0x08008974: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08008976: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08008978: 2072     strb     r0, [r4, #8] */
    *(uint8_t *)(r4 + 8) = r0;   /* store */
    /* 0x0800897A: 1428     cmp      r0, #0x14 */
    /* flags = cmp(r0, #0x14) */
    /* 0x0800897C: 46d3     blo      #0x8008a0c */
    if (r0 < 0x14) goto L_08008A0C;   /* unsigned */
    /* 0x0800897E: 2572     strb     r5, [r4, #8] */
    *(uint8_t *)(r4 + 8) = r5;   /* store */
    /* 0x08008980: 6089     ldrh     r0, [r4, #0xa] */
    r0 = *(uint16_t *)(r4 + 10);
    /* 0x08008982: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008984: 10d0     beq      #0x80089a8 */
    if (r0 == 0) goto L_080089A8;
    /* 0x08008986: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x08008988: 0004     lsls     r0, r0, #0x10 */
    r0 = r0 << 0x10;
    /* 0x0800898A: 000c     lsrs     r0, r0, #0x10 */
    r0 = r0 >> 0x10;
    /* 0x0800898C: 6081     strh     r0, [r4, #0xa] */
    *(uint16_t *)(r4 + 10) = r0;   /* store */
    /* 0x0800898E: 0ad0     beq      #0x80089a6 */
    if (r0 == 0) goto L_080089A6;
    /* 0x08008990: 3221     movs     r1, #0x32 */
    r1 = 0x32;
    /* 0x08008992: faf7adfb bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x08008996: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x08008998: 06d1     bne      #0x80089a8 */
    if (r1 != 0) goto L_080089A8;
    /* 0x0800899A: a089     ldrh     r0, [r4, #0xc] */
    r0 = *(uint16_t *)(r4 + 12);
    /* 0x0800899C: 1e28     cmp      r0, #0x1e */
    /* flags = cmp(r0, #0x1e) */
    /* 0x0800899E: 02d3     blo      #0x80089a6 */
    if (r0 < 0x1e) goto L_080089A6;   /* unsigned */
    /* 0x080089A0: 1e38     subs     r0, #0x1e */
    r0 = r0 - 0x1e;
    /* 0x080089A2: a081     strh     r0, [r4, #0xc] */
    *(uint16_t *)(r4 + 12) = r0;   /* store */
    /* 0x080089A4: 00e0     b        #0x80089a8 */
    goto L_080089A8;

    L_080089A6:
    /* 0x080089A6: a581     strh     r5, [r4, #0xc] */
    *(uint16_t *)(r4 + 12) = r5;   /* store */

    L_080089A8:
    /* 0x080089A8: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x080089AA: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080089AC: 06d0     beq      #0x80089bc */
    if (r0 == 0) goto L_080089BC;
    /* 0x080089AE: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x080089B0: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x080089B2: 000e     lsrs     r0, r0, #0x18 */
    r0 = r0 >> 0x18;
    /* 0x080089B4: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x080089B6: 01d1     bne      #0x80089bc */
    if (r0 != 0) goto L_080089BC;
    /* 0x080089B8: 1a48     ldr      r0, [pc, #0x68]  ; =RAM_200001B9 */
    r0 = *(uint32_t *)0x08008A24;   /* = RAM 0x200001B9 */
    /* 0x080089BA: 0570     strb     r5, [r0] */
    *(uint8_t *)(r0) = r5;   /* store */

    L_080089BC:
    /* 0x080089BC: e089     ldrh     r0, [r4, #0xe] */
    r0 = *(uint16_t *)(r4 + 14);
    /* 0x080089BE: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080089C0: 06d0     beq      #0x80089d0 */
    if (r0 == 0) goto L_080089D0;
    /* 0x080089C2: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x080089C4: 0004     lsls     r0, r0, #0x10 */
    r0 = r0 << 0x10;
    /* 0x080089C6: 000c     lsrs     r0, r0, #0x10 */
    r0 = r0 >> 0x10;
    /* 0x080089C8: e081     strh     r0, [r4, #0xe] */
    *(uint16_t *)(r4 + 14) = r0;   /* store */
    /* 0x080089CA: 01d1     bne      #0x80089d0 */
    if (r0 != 0) goto L_080089D0;
    /* 0x080089CC: fff7bcff bl       #0x8008948 */
    FUN_08008948();   /* BL -> 0x08008948 FUN_08008948 */

    L_080089D0:
    /* 0x080089D0: 608a     ldrh     r0, [r4, #0x12] */
    r0 = *(uint16_t *)(r4 + 18);
    /* 0x080089D2: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080089D4: 15d0     beq      #0x8008a02 */
    if (r0 == 0) goto L_08008A02;
    /* 0x080089D6: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x080089D8: 0004     lsls     r0, r0, #0x10 */
    r0 = r0 << 0x10;
    /* 0x080089DA: 000c     lsrs     r0, r0, #0x10 */
    r0 = r0 >> 0x10;
    /* 0x080089DC: 6082     strh     r0, [r4, #0x12] */
    *(uint16_t *)(r4 + 18) = r0;   /* store */
    /* 0x080089DE: 05d0     beq      #0x80089ec */
    if (r0 == 0) goto L_080089EC;
    /* 0x080089E0: 3221     movs     r1, #0x32 */
    r1 = 0x32;
    /* 0x080089E2: faf785fb bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x080089E6: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x080089E8: 04d0     beq      #0x80089f4 */
    if (r1 == 0) goto L_080089F4;
    /* 0x080089EA: 0ae0     b        #0x8008a02 */
    goto L_08008A02;

    L_080089EC:
    /* 0x080089EC: a571     strb     r5, [r4, #6] */
    *(uint8_t *)(r4 + 6) = r5;   /* store */
    /* 0x080089EE: a582     strh     r5, [r4, #0x14] */
    *(uint16_t *)(r4 + 20) = r5;   /* store */
    /* 0x080089F0: 2571     strb     r5, [r4, #4] */
    *(uint8_t *)(r4 + 4) = r5;   /* store */
    /* 0x080089F2: 0be0     b        #0x8008a0c */
    goto L_08008A0C;

    L_080089F4:
    /* 0x080089F4: a08a     ldrh     r0, [r4, #0x14] */
    r0 = *(uint16_t *)(r4 + 20);
    /* 0x080089F6: 1e28     cmp      r0, #0x1e */
    /* flags = cmp(r0, #0x1e) */
    /* 0x080089F8: 02d3     blo      #0x8008a00 */
    if (r0 < 0x1e) goto L_08008A00;   /* unsigned */
    /* 0x080089FA: 1e38     subs     r0, #0x1e */
    r0 = r0 - 0x1e;
    /* 0x080089FC: a082     strh     r0, [r4, #0x14] */
    *(uint16_t *)(r4 + 20) = r0;   /* store */
    /* 0x080089FE: 00e0     b        #0x8008a02 */
    goto L_08008A02;

    L_08008A00:
    /* 0x08008A00: a582     strh     r5, [r4, #0x14] */
    *(uint16_t *)(r4 + 20) = r5;   /* store */

    L_08008A02:
    /* 0x08008A02: 2079     ldrb     r0, [r4, #4] */
    r0 = *(uint8_t *)(r4 + 4);
    /* 0x08008A04: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008A06: 01d0     beq      #0x8008a0c */
    if (r0 == 0) goto L_08008A0C;
    /* 0x08008A08: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x08008A0A: 2071     strb     r0, [r4, #4] */
    *(uint8_t *)(r4 + 4) = r0;   /* store */

    L_08008A0C:
    /* 0x08008A0C: 208a     ldrh     r0, [r4, #0x10] */
    r0 = *(uint16_t *)(r4 + 16);
    /* 0x08008A0E: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008A10: 02d0     beq      #0x8008a18 */
    if (r0 == 0) goto L_08008A18;
    /* 0x08008A12: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x08008A14: 2082     strh     r0, [r4, #0x10] */
    *(uint16_t *)(r4 + 16) = r0;   /* store */
    /* 0x08008A16: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08008A18:
    /* 0x08008A18: 0348     ldr      r0, [pc, #0xc]  ; =RAM_200001BB */
    r0 = *(uint32_t *)0x08008A28;   /* = RAM 0x200001BB */
    /* 0x08008A1A: 0570     strb     r5, [r0] */
    *(uint8_t *)(r0) = r5;   /* store */
    /* 0x08008A1C: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
