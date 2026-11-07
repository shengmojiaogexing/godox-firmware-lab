/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006D18
 * Name      : FUN_08006D18
 * End       : 0x08006D4E
 * Size      : 54 bytes, 26 instructions decoded in body (functions.json insns=26, blocks=3)
 * Class     : function
 * Callers   : 1  -> 0x08006CEC FUN_08006CEC
 * Callees   : 1  -> 0x080072C4 FUN_080072C4
 * MMIO      : RCC.APB1ENR, RCC.APB1RSTR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006D18(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006D18: 00b5     push     {lr} */
    push({lr});
    /* 0x08006D1A: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);
    /* 0x08006D1C: 0c49     ldr      r1, [pc, #0x30]  ; =0x40007400 */
    r1 = *(uint32_t *)0x08006D50;   /* = MMIO APB+0x7400 */
    /* 0x08006D1E: 87b0     sub      sp, #0x1c */
    sp = sp - 0x1c;
    /* 0x08006D20: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08006D22: 12d1     bne      #0x8006d4a */
    if (r0 != r1) goto L_08006D4A;
    /* 0x08006D24: 0b48     ldr      r0, [pc, #0x2c]  ; =RCC.CR */
    r0 = *(uint32_t *)0x08006D54;   /* = MMIO RCC+0x00 */
    /* 0x08006D26: c269     ldr      r2, [r0, #0x1c] */
    r2 = *(uint32_t *)(r0 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x08006D28: 4104     lsls     r1, r0, #0x11 */
    r1 = r0 << 0x11;
    /* 0x08006D2A: 0a43     orrs     r2, r1 */
    r2 = r2 | r1;
    /* 0x08006D2C: c261     str      r2, [r0, #0x1c] */
    *(uint32_t *)(r0 + 28) = r2;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x08006D2E: c069     ldr      r0, [r0, #0x1c] */
    r0 = *(uint32_t *)(r0 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x08006D30: 0840     ands     r0, r1 */
    r0 = r0 & r1;
    /* 0x08006D32: 0590     str      r0, [sp, #0x14] */
    *(uint32_t *)(sp + 20) = r0;   /* store */
    /* 0x08006D34: 3020     movs     r0, #0x30 */
    r0 = 0x30;
    /* 0x08006D36: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08006D38: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x08006D3A: 0190     str      r0, [sp, #4] */
    *(uint32_t *)(sp + 4) = r0;   /* store */
    /* 0x08006D3C: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006D3E: 0290     str      r0, [sp, #8] */
    *(uint32_t *)(sp + 8) = r0;   /* store */
    /* 0x08006D40: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x08006D42: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08006D44: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x08006D46: 00f0bdfa bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */

    L_08006D4A:
    /* 0x08006D4A: 07b0     add      sp, #0x1c */
    sp = sp + 0x1c;
    /* 0x08006D4C: 00bd     pop      {pc} */
    pop({pc});   /* return */
}
