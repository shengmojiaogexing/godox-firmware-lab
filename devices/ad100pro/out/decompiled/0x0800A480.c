/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A480
 * Name      : FUN_0800A480
 * End       : 0x0800A4E2
 * Size      : 98 bytes, 45 instructions decoded in body (functions.json insns=45, blocks=9)
 * Class     : hub
 * Callers   : 16  -> 0x080047BE FUN_080047BE, 0x0800A30C FUN_0800A30C, 0x0800A312 FUN_0800A312, 0x0800A31A FUN_0800A31A, 0x0800A33E FUN_0800A33E, 0x0800A346 FUN_0800A346, 0x0800A368 FUN_0800A368, 0x0800A370 FUN_0800A370, ... +8
 * Callees   : 1  -> 0x08007426 FUN_08007426
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A480(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A480: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x0800A482: 184c     ldr      r4, [pc, #0x60]  ; =GPIOB.MODER */
    r4 = *(uint32_t *)0x0800A4E4;   /* = MMIO GPIOB+0x00 */
    /* 0x0800A484: 0546     mov      r5, r0 */
    r5 = r0;
    /* 0x0800A486: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x0800A488: 15d0     beq      #0x800a4b6 */
    if (r1 == 1) goto L_0800A4B6;
    /* 0x0800A48A: 0022     movs     r2, #0 */
    r2 = 0;

    L_0800A48C:
    /* 0x0800A48C: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800A48E: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800A490: fcf7c9ff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A494: 0127     movs     r7, #1 */
    r7 = 1;
    /* 0x0800A496: 7f02     lsls     r7, r7, #9 */
    r7 = r7 << 9;
    /* 0x0800A498: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800A49A: 3946     mov      r1, r7 */
    r1 = r7;
    /* 0x0800A49C: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800A49E: fcf7c2ff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A4A2: 7b00     lsls     r3, r7, #1 */
    r3 = r7 << 1;
    /* 0x0800A4A4: 0922     movs     r2, #9 */
    r2 = 9;
    /* 0x0800A4A6: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800A4A8: d206     lsls     r2, r2, #0x1b */
    r2 = r2 << 0x1b;
    /* 0x0800A4AA: 4021     movs     r1, #0x40 */
    r1 = 0x40;

    L_0800A4AC:
    /* 0x0800A4AC: a162     str      r1, [r4, #0x28] */
    *(uint32_t *)(r4 + 40) = r1;   /* store */  /* MMIO: GPIOB.BRR(w) */
    /* 0x0800A4AE: 2e06     lsls     r6, r5, #0x18 */
    r6 = r5 << 0x18;
    /* 0x0800A4B0: 03d5     bpl      #0x800a4ba */
    if (r1 >= 1) goto L_0800A4BA;   /* result non-negative */
    /* 0x0800A4B2: 9361     str      r3, [r2, #0x18] */
    *(uint32_t *)(r2 + 24) = r3;   /* store */
    /* 0x0800A4B4: 02e0     b        #0x800a4bc */
    goto L_0800A4BC;

    L_0800A4B6:
    /* 0x0800A4B6: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800A4B8: e8e7     b        #0x800a48c */
    goto L_0800A48C;   /* loop */

    L_0800A4BA:
    /* 0x0800A4BA: 9362     str      r3, [r2, #0x28] */
    *(uint32_t *)(r2 + 40) = r3;   /* store */

    L_0800A4BC:
    /* 0x0800A4BC: a161     str      r1, [r4, #0x18] */
    *(uint32_t *)(r4 + 24) = r1;   /* store */  /* MMIO: GPIOB.BSRR(w) */
    /* 0x0800A4BE: 6d06     lsls     r5, r5, #0x19 */
    r5 = r5 << 0x19;
    /* 0x0800A4C0: 2d0e     lsrs     r5, r5, #0x18 */
    r5 = r5 >> 0x18;
    /* 0x0800A4C2: 00bf     nop       */
    /* nop */
    /* 0x0800A4C4: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A4C6: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A4C8: 0828     cmp      r0, #8 */
    /* flags = cmp(r0, #8) */
    /* 0x0800A4CA: efd3     blo      #0x800a4ac */
    if (r0 < 8) goto L_0800A4AC;   /* unsigned */
    /* 0x0800A4CC: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800A4CE: 3946     mov      r1, r7 */
    r1 = r7;
    /* 0x0800A4D0: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800A4D2: fcf7a8ff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A4D6: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800A4D8: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800A4DA: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800A4DC: fcf7a3ff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A4E0: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
