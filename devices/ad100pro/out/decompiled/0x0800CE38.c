/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CE38
 * Name      : FUN_0800CE38
 * End       : 0x0800CE4E
 * Size      : 22 bytes, 10 instructions decoded in body (functions.json insns=10, blocks=3)
 * Class     : function
 * Callers   : 3  -> 0x0800CDC8 FUN_0800CDC8, 0x0800CDF4 FUN_0800CDF4, 0x0800CE0C FUN_0800CE0C
 * Callees   : 1  -> 0x08007426 FUN_08007426
 * MMIO      : GPIOB.IDR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CE38(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CE38: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800CE3A: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800CE3C: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x0800CE3E: 0448     ldr      r0, [pc, #0x10]  ; =GPIOC.MODER */
    r0 = *(uint32_t *)0x0800CE50;   /* = MMIO GPIOC+0x00 */
    /* 0x0800CE40: faf7f1fa bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800CE44: 0348     ldr      r0, [pc, #0xc]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x0800CE54;   /* = MMIO GPIOB+0x00 */

    L_0800CE46:
    /* 0x0800CE46: 0169     ldr      r1, [r0, #0x10] */
    r1 = *(uint32_t *)(r0 + 16);  /* MMIO: GPIOB.IDR(r) */
    /* 0x0800CE48: 4904     lsls     r1, r1, #0x11 */
    r1 = r1 << 0x11;
    /* 0x0800CE4A: fcd4     bmi      #0x800ce46 */
    if (/* bmi */) goto L_0800CE46;   /* result negative */
    /* 0x0800CE4C: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
