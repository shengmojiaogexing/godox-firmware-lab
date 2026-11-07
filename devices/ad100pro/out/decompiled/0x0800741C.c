/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800741C
 * Name      : FUN_0800741C
 * End       : 0x08007426
 * Size      : 10 bytes, 5 instructions decoded in body (functions.json insns=5, blocks=3)
 * Class     : hub
 * Callers   : 27  -> 0x080038C0 FUN_080038C0, 0x08003900 FUN_08003900, 0x08004568 FUN_08004568, 0x080047CC FUN_080047CC, 0x080047DA FUN_080047DA, 0x08005B0C IRQ7_Handler, 0x08005B42 FUN_08005B42, 0x08008D00 dispatcher_large, ... +19
 * Callees   : 0  -> -
 * MMIO      : EXTI.SWIER
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800741C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800741C: 0069     ldr      r0, [r0, #0x10] */
    r0 = *(uint32_t *)(r0 + 16);  /* MMIO: EXTI.SWIER(r) */
    /* 0x0800741E: 0840     ands     r0, r1 */
    r0 = r0 & r1;
    /* 0x08007420: 00d0     beq      #0x8007424 */
    if (/* beq */) goto L_08007424;
    /* 0x08007422: 0120     movs     r0, #1 */
    r0 = 1;

    L_08007424:
    /* 0x08007424: 7047     bx       lr */
    return;
}
