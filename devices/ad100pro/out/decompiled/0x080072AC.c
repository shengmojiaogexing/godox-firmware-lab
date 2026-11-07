/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080072AC
 * Name      : FUN_080072AC
 * End       : 0x080072BE
 * Size      : 18 bytes, 8 instructions decoded in body (functions.json insns=8, blocks=3)
 * Class     : hub
 * Callers   : 9  -> 0x08005A78 IRQ5_Handler, 0x08005AD0 FUN_08005AD0, 0x08005B00 IRQ6_Handler, 0x08005B0C IRQ7_Handler, 0x08005B28 FUN_08005B28, 0x08005B78 FUN_08005B78, 0x08005F84 FUN_08005F84, 0x08005FD4 FUN_08005FD4, ... +1
 * Callees   : 1  -> 0x080072A8 FUN_080072A8
 * MMIO      : EXTI.PR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080072AC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080072AC: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x080072AE: 0449     ldr      r1, [pc, #0x10]  ; =EXTI.IMR */
    r1 = *(uint32_t *)0x080072C0;   /* = MMIO EXTI+0x00 */
    /* 0x080072B0: 4a69     ldr      r2, [r1, #0x14] */
    r2 = *(uint32_t *)(r1 + 20);  /* MMIO: EXTI.PR(r) */
    /* 0x080072B2: 0242     tst      r2, r0 */
    /* flags = tst(r2, r0) */
    /* 0x080072B4: 02d0     beq      #0x80072bc */
    if ((r2 & r0)) goto L_080072BC;   /* test bits */
    /* 0x080072B6: 4861     str      r0, [r1, #0x14] */
    *(uint32_t *)(r1 + 20) = r0;   /* store */  /* MMIO: EXTI.PR(w) */
    /* 0x080072B8: fff7f6ff bl       #0x80072a8 */
    FUN_080072A8();   /* BL -> 0x080072A8 FUN_080072A8 */

    L_080072BC:
    /* 0x080072BC: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
