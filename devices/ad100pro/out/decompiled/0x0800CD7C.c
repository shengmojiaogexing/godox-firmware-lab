/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CD7C
 * Name      : FUN_0800CD7C
 * End       : 0x0800CDC4
 * Size      : 72 bytes, 34 instructions decoded in body (functions.json insns=34, blocks=8)
 * Class     : function
 * Callers   : 3  -> 0x0800CDC8 FUN_0800CDC8, 0x0800CDF4 FUN_0800CDF4, 0x0800CE0C FUN_0800CE0C
 * Callees   : 1  -> 0x0800B874 FUN_0800B874
 * MMIO      : GPIOB.BRR, GPIOB.BSRR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CD7C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CD7C: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x0800CD7E: 0822     movs     r2, #8 */
    r2 = 8;
    /* 0x0800CD80: 0346     mov      r3, r0 */
    r3 = r0;
    /* 0x0800CD82: 5502     lsls     r5, r2, #9 */
    r5 = r2 << 9;
    /* 0x0800CD84: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800CD86: 9402     lsls     r4, r2, #0xa */
    r4 = r2 << 0xa;
    /* 0x0800CD88: 0e49     ldr      r1, [pc, #0x38]  ; =GPIOB.MODER */
    r1 = *(uint32_t *)0x0800CDC4;   /* = MMIO GPIOB+0x00 */
    /* 0x0800CD8A: 0127     movs     r7, #1 */
    r7 = 1;

    L_0800CD8C:
    /* 0x0800CD8C: 8c62     str      r4, [r1, #0x28] */
    *(uint32_t *)(r1 + 40) = r4;   /* store */  /* MMIO: GPIOB.BRR(w) */
    /* 0x0800CD8E: 1e06     lsls     r6, r3, #0x18 */
    r6 = r3 << 0x18;
    /* 0x0800CD90: 01d5     bpl      #0x800cd96 */
    if (/* bpl */) goto L_0800CD96;   /* result non-negative */
    /* 0x0800CD92: 8d61     str      r5, [r1, #0x18] */
    *(uint32_t *)(r1 + 24) = r5;   /* store */  /* MMIO: GPIOB.BSRR(w) */
    /* 0x0800CD94: 00e0     b        #0x800cd98 */
    goto L_0800CD98;

    L_0800CD96:
    /* 0x0800CD96: 8d62     str      r5, [r1, #0x28] */
    *(uint32_t *)(r1 + 40) = r5;   /* store */  /* MMIO: GPIOB.BRR(w) */

    L_0800CD98:
    /* 0x0800CD98: 5b06     lsls     r3, r3, #0x19 */
    r3 = r3 << 0x19;
    /* 0x0800CD9A: 1b0e     lsrs     r3, r3, #0x18 */
    r3 = r3 >> 0x18;
    /* 0x0800CD9C: 00bf     nop       */
    /* nop */
    /* 0x0800CD9E: 00bf     nop       */
    /* nop */
    /* 0x0800CDA0: 00bf     nop       */
    /* nop */
    /* 0x0800CDA2: fef767fd bl       #0x800b874 */
    FUN_0800B874();   /* BL -> 0x0800B874 FUN_0800B874 */
    /* 0x0800CDA6: 8c61     str      r4, [r1, #0x18] */
    *(uint32_t *)(r1 + 24) = r4;   /* store */  /* MMIO: GPIOB.BSRR(w) */
    /* 0x0800CDA8: fef764fd bl       #0x800b874 */
    FUN_0800B874();   /* BL -> 0x0800B874 FUN_0800B874 */
    /* 0x0800CDAC: 4006     lsls     r0, r0, #0x19 */
    r0 = r0 << 0x19;
    /* 0x0800CDAE: 0e69     ldr      r6, [r1, #0x10] */
    r6 = *(uint32_t *)(r1 + 16);  /* MMIO: GPIOB.IDR(r) */
    /* 0x0800CDB0: 000e     lsrs     r0, r0, #0x18 */
    r0 = r0 >> 0x18;
    /* 0x0800CDB2: 7604     lsls     r6, r6, #0x11 */
    r6 = r6 << 0x11;
    /* 0x0800CDB4: 00d5     bpl      #0x800cdb8 */
    if (/* bpl */) goto L_0800CDB8;   /* result non-negative */
    /* 0x0800CDB6: 3843     orrs     r0, r7 */
    r0 = r0 | r7;

    L_0800CDB8:
    /* 0x0800CDB8: 521e     subs     r2, r2, #1 */
    r2 = r2 - 1;
    /* 0x0800CDBA: 1206     lsls     r2, r2, #0x18 */
    r2 = r2 << 0x18;
    /* 0x0800CDBC: 120e     lsrs     r2, r2, #0x18 */
    r2 = r2 >> 0x18;
    /* 0x0800CDBE: e5d1     bne      #0x800cd8c */
    if (/* bne */) goto L_0800CD8C;
    /* 0x0800CDC0: 8c62     str      r4, [r1, #0x28] */
    *(uint32_t *)(r1 + 40) = r4;   /* store */  /* MMIO: GPIOB.BRR(w) */
    /* 0x0800CDC2: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
