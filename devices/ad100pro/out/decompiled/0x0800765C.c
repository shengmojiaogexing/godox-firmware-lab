/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800765C
 * Name      : FUN_0800765C
 * End       : 0x0800766A
 * Size      : 14 bytes, 7 instructions decoded in body (functions.json insns=7, blocks=1)
 * Class     : function
 * Callers   : 7  -> 0x08007F38 FUN_08007F38, 0x08007F56 FUN_08007F56, 0x0800821C FUN_0800821C, 0x080093F4 FUN_080093F4, 0x0800940E FUN_0800940E, 0x0800941C FUN_0800941C, 0x08009576 FUN_08009576
 * Callees   : 0  -> -
 * MMIO      : NVIC.+0x0
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800765C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800765C: c106     lsls     r1, r0, #0x1b */
    r1 = r0 << 0x1b;
    /* 0x0800765E: c90e     lsrs     r1, r1, #0x1b */
    r1 = r1 >> 0x1b;
    /* 0x08007660: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08007662: 8840     lsls     r0, r1 */
    r0 = r0 << r1;
    /* 0x08007664: 0149     ldr      r1, [pc, #4]  ; =NVIC.ISER */
    r1 = *(uint32_t *)0x0800766C;   /* = MMIO SCS+0x100 */
    /* 0x08007666: 0860     str      r0, [r1] */
    *(uint32_t *)(r1) = r0;   /* store */  /* MMIO: NVIC.+0x0(w) */
    /* 0x08007668: 7047     bx       lr */
    return;
}
