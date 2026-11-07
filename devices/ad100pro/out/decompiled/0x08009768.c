/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009768
 * Name      : FUN_08009768
 * End       : 0x080097EA
 * Size      : 130 bytes, 54 instructions decoded in body (functions.json insns=54, blocks=11)
 * Class     : function
 * Callers   : 1  -> 0x0800CF74 main
 * Callees   : 7  -> 0x08005C40 FUN_08005C40, 0x080081A0 FUN_080081A0, 0x080081E8 FUN_080081E8, 0x080082F8 FUN_080082F8, 0x0800854C FUN_0800854C, 0x08008582 FUN_08008582, 0x08008652 FUN_08008652
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009768(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009768: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800976A: 2148     ldr      r0, [pc, #0x84]  ; =RAM_2000030C */
    r0 = *(uint32_t *)0x080097F0;   /* = RAM 0x2000030C */
    /* 0x0800976C: 8eb0     sub      sp, #0x38 */
    sp = sp - 0x38;
    /* 0x0800976E: 1f49     ldr      r1, [pc, #0x7c]  ; =TIM3.CR1 */
    r1 = *(uint32_t *)0x080097EC;   /* = MMIO TIM3+0x00 */
    /* 0x08009770: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */
    /* 0x08009772: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x08009774: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08009776: 8460     str      r4, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r4;   /* store */
    /* 0x08009778: 4160     str      r1, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r1;   /* store */
    /* 0x0800977A: 1e49     ldr      r1, [pc, #0x78]  ; =0x0000027F */
    r1 = *(uint32_t *)0x080097F4;   /* = 0x0000027F */
    /* 0x0800977C: 0461     str      r4, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r4;   /* store */
    /* 0x0800977E: c160     str      r1, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r1;   /* store */
    /* 0x08009780: fef732fd bl       #0x80081e8 */
    FUN_080081E8();   /* BL -> 0x080081E8 FUN_080081E8 */
    /* 0x08009784: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009786: 01d0     beq      #0x800978c */
    if (r0 == 0) goto L_0800978C;
    /* 0x08009788: fcf75afa bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_0800978C:
    /* 0x0800978C: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800978E: 0003     lsls     r0, r0, #0xc */
    r0 = r0 << 0xc;
    /* 0x08009790: 0790     str      r0, [sp, #0x1c] */
    *(uint32_t *)(sp + 28) = r0;   /* store */
    /* 0x08009792: 07a9     add      r1, sp, #0x1c */
    r1 = sp + 0x1c;
    /* 0x08009794: 1648     ldr      r0, [pc, #0x58]  ; =RAM_2000030C */
    r0 = *(uint32_t *)0x080097F0;   /* = RAM 0x2000030C */
    /* 0x08009796: fef7affd bl       #0x80082f8 */
    FUN_080082F8();   /* BL -> 0x080082F8 FUN_080082F8 */
    /* 0x0800979A: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800979C: 01d0     beq      #0x80097a2 */
    if (r0 == 0) goto L_080097A2;
    /* 0x0800979E: fcf74ffa bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_080097A2:
    /* 0x080097A2: 1348     ldr      r0, [pc, #0x4c]  ; =RAM_2000030C */
    r0 = *(uint32_t *)0x080097F0;   /* = RAM 0x2000030C */
    /* 0x080097A4: fef755ff bl       #0x8008652 */
    FUN_08008652();   /* BL -> 0x08008652 FUN_08008652 */
    /* 0x080097A8: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080097AA: 01d0     beq      #0x80097b0 */
    if (r0 == 0) goto L_080097B0;
    /* 0x080097AC: fcf748fa bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_080097B0:
    /* 0x080097B0: 0b94     str      r4, [sp, #0x2c] */
    *(uint32_t *)(sp + 44) = r4;   /* store */
    /* 0x080097B2: 0ba9     add      r1, sp, #0x2c */
    r1 = sp + 0x2c;
    /* 0x080097B4: 0e48     ldr      r0, [pc, #0x38]  ; =RAM_2000030C */
    r0 = *(uint32_t *)0x080097F0;   /* = RAM 0x2000030C */
    /* 0x080097B6: 0c94     str      r4, [sp, #0x30] */
    *(uint32_t *)(sp + 48) = r4;   /* store */
    /* 0x080097B8: fef7f2fc bl       #0x80081a0 */
    FUN_080081A0();   /* BL -> 0x080081A0 FUN_080081A0 */
    /* 0x080097BC: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080097BE: 01d0     beq      #0x80097c4 */
    if (r0 == 0) goto L_080097C4;
    /* 0x080097C0: fcf73efa bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_080097C4:
    /* 0x080097C4: 6020     movs     r0, #0x60 */
    r0 = 0x60;
    /* 0x080097C6: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x080097C8: 0194     str      r4, [sp, #4] */
    *(uint32_t *)(sp + 4) = r4;   /* store */
    /* 0x080097CA: 0294     str      r4, [sp, #8] */
    *(uint32_t *)(sp + 8) = r4;   /* store */
    /* 0x080097CC: 0422     movs     r2, #4 */
    r2 = 4;
    /* 0x080097CE: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x080097D0: 0494     str      r4, [sp, #0x10] */
    *(uint32_t *)(sp + 16) = r4;   /* store */
    /* 0x080097D2: 0748     ldr      r0, [pc, #0x1c]  ; =RAM_2000030C */
    r0 = *(uint32_t *)0x080097F0;   /* = RAM 0x2000030C */
    /* 0x080097D4: fef7d5fe bl       #0x8008582 */
    FUN_08008582();   /* BL -> 0x08008582 FUN_08008582 */
    /* 0x080097D8: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080097DA: 01d0     beq      #0x80097e0 */
    if (r0 == 0) goto L_080097E0;
    /* 0x080097DC: fcf730fa bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_080097E0:
    /* 0x080097E0: 0348     ldr      r0, [pc, #0xc]  ; =RAM_2000030C */
    r0 = *(uint32_t *)0x080097F0;   /* = RAM 0x2000030C */
    /* 0x080097E2: fef7b3fe bl       #0x800854c */
    FUN_0800854C();   /* BL -> 0x0800854C FUN_0800854C */
    /* 0x080097E6: 0eb0     add      sp, #0x38 */
    sp = sp + 0x38;
    /* 0x080097E8: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
