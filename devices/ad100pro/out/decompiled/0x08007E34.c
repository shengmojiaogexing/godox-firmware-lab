/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007E34
 * Name      : FUN_08007E34
 * End       : 0x08007E68
 * Size      : 52 bytes, 25 instructions decoded in body (functions.json insns=25, blocks=4)
 * Class     : function
 * Callers   : 1  -> 0x0800ACF8 IRQ2_Handler
 * Callees   : 1  -> 0x08007E30 FUN_08007E30
 * MMIO      : EXTI.FTSR, EXTI.IMR, EXTI.RTSR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007E34(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007E34: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08007E36: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x08007E38: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);  /* MMIO: EXTI.IMR(r) */
    /* 0x08007E3A: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);  /* MMIO: EXTI.RTSR(r) */
    /* 0x08007E3C: 4904     lsls     r1, r1, #0x11 */
    r1 = r1 << 0x11;
    /* 0x08007E3E: 0cd5     bpl      #0x8007e5a */
    if (/* bpl */) goto L_08007E5A;   /* result non-negative */
    /* 0x08007E40: c068     ldr      r0, [r0, #0xc] */
    r0 = *(uint32_t *)(r0 + 12);  /* MMIO: EXTI.FTSR(r) */
    /* 0x08007E42: 4005     lsls     r0, r0, #0x15 */
    r0 = r0 << 0x15;
    /* 0x08007E44: 09d5     bpl      #0x8007e5a */
    if (/* bpl */) goto L_08007E5A;   /* result non-negative */
    /* 0x08007E46: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08007E48: fff7f2ff bl       #0x8007e30 */
    FUN_08007E30();   /* BL -> 0x08007E30 FUN_08007E30 */
    /* 0x08007E4C: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007E4E: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);  /* MMIO: EXTI.FTSR(r) */
    /* 0x08007E50: 8022     movs     r2, #0x80 */
    r2 = 0x80;
    /* 0x08007E52: 1140     ands     r1, r2 */
    r1 = r1 & r2;
    /* 0x08007E54: 044a     ldr      r2, [pc, #0x10]  ; =0xFFFFFB7F */
    r2 = *(uint32_t *)0x08007E68;   /* = 0xFFFFFB7F */
    /* 0x08007E56: 8918     adds     r1, r1, r2 */
    r1 = r1 + r2;
    /* 0x08007E58: c160     str      r1, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r1;   /* store */  /* MMIO: EXTI.FTSR(w) */

    L_08007E5A:
    /* 0x08007E5A: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08007E5C: 0349     ldr      r1, [pc, #0xc]  ; =EXTI.IMR */
    r1 = *(uint32_t *)0x08007E6C;   /* = MMIO EXTI+0x00 */
    /* 0x08007E5E: 0005     lsls     r0, r0, #0x14 */
    r0 = r0 << 0x14;
    /* 0x08007E60: 4861     str      r0, [r1, #0x14] */
    *(uint32_t *)(r1 + 20) = r0;   /* store */  /* MMIO: EXTI.PR(w) */
    /* 0x08007E62: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08007E64: 6077     strb     r0, [r4, #0x1d] */
    *(uint8_t *)(r4 + 29) = r0;   /* store */  /* MMIO: RCC.+0x1D(w) */
    /* 0x08007E66: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
