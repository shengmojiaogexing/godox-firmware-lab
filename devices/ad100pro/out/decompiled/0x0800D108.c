/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800D108
 * Name      : FUN_0800D108
 * End       : 0x0800D12C
 * Size      : 36 bytes, 17 instructions decoded in body (functions.json insns=17, blocks=1)
 * Class     : function
 * Callers   : 2  -> 0x0800B7B0 FUN_0800B7B0, 0x0800B7D0 FUN_0800B7D0
 * Callees   : 1  -> 0x080072C4 FUN_080072C4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800D108(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800D108: 00b5     push     {lr} */
    push({lr});
    /* 0x0800D10A: 85b0     sub      sp, #0x14 */
    sp = sp - 0x14;
    /* 0x0800D10C: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800D10E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800D110: 0091     str      r1, [sp] */
    *(uint32_t *)(sp) = r1;   /* store */
    /* 0x0800D112: 0291     str      r1, [sp, #8] */
    *(uint32_t *)(sp + 8) = r1;   /* store */
    /* 0x0800D114: 0190     str      r0, [sp, #4] */
    *(uint32_t *)(sp + 4) = r0;   /* store */
    /* 0x0800D116: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x0800D118: 0448     ldr      r0, [pc, #0x10]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x0800D12C;   /* = MMIO GPIOB+0x00 */
    /* 0x0800D11A: faf7d3f8 bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x0800D11E: 0448     ldr      r0, [pc, #0x10]  ; =EXTI.IMR */
    r0 = *(uint32_t *)0x0800D130;   /* = MMIO EXTI+0x00 */
    /* 0x0800D120: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: EXTI.IMR(r) */
    /* 0x0800D122: 4908     lsrs     r1, r1, #1 */
    r1 = r1 >> 1;
    /* 0x0800D124: 4900     lsls     r1, r1, #1 */
    r1 = r1 << 1;
    /* 0x0800D126: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: EXTI.IMR(w) */
    /* 0x0800D128: 05b0     add      sp, #0x14 */
    sp = sp + 0x14;
    /* 0x0800D12A: 00bd     pop      {pc} */
    pop({pc});   /* return */
}
