/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007648
 * Name      : FUN_08007648
 * End       : 0x08007656
 * Size      : 14 bytes, 7 instructions decoded in body (functions.json insns=7, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x08007F10 FUN_08007F10
 * Callees   : 0  -> -
 * MMIO      : NVIC.+0x80
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007648(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007648: c106     lsls     r1, r0, #0x1b */
    r1 = r0 << 0x1b;
    /* 0x0800764A: c90e     lsrs     r1, r1, #0x1b */
    r1 = r1 >> 0x1b;
    /* 0x0800764C: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800764E: 8840     lsls     r0, r1 */
    r0 = r0 << r1;
    /* 0x08007650: 0149     ldr      r1, [pc, #4]  ; =NVIC.ICER */
    r1 = *(uint32_t *)0x08007658;   /* = MMIO SCS+0x180 */
    /* 0x08007652: 0860     str      r0, [r1] */
    *(uint32_t *)(r1) = r0;   /* store */  /* MMIO: NVIC.+0x80(w) */
    /* 0x08007654: 7047     bx       lr */
    return;
}
