/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800BB74
 * Name      : FUN_0800BB74
 * End       : 0x0800BB84
 * Size      : 16 bytes, 8 instructions decoded in body (functions.json insns=8, blocks=1)
 * Class     : function
 * Callers   : 2  -> 0x080082F8 FUN_080082F8, 0x080083AC FUN_080083AC
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800BB74(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800BB74: 8268     ldr      r2, [r0, #8] */
    r2 = *(uint32_t *)(r0 + 8);
    /* 0x0800BB76: 7023     movs     r3, #0x70 */
    r3 = 0x70;
    /* 0x0800BB78: 9a43     bics     r2, r3 */
    r2 = r2 & ~r3;
    /* 0x0800BB7A: 0a43     orrs     r2, r1 */
    r2 = r2 | r1;
    /* 0x0800BB7C: 0721     movs     r1, #7 */
    r1 = 7;
    /* 0x0800BB7E: 0a43     orrs     r2, r1 */
    r2 = r2 | r1;
    /* 0x0800BB80: 8260     str      r2, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r2;   /* store */
    /* 0x0800BB82: 7047     bx       lr */
    return;
}
