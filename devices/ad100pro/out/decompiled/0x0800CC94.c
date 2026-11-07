/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CC94
 * Name      : FUN_0800CC94
 * End       : 0x0800CC98
 * Size      : 4 bytes, 2 instructions decoded in body (functions.json insns=2, blocks=1)
 * Class     : tiny_fragment
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CC94(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CC94: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800CC96: 0000     movs     r0, r0 */
    r0 = r0;
}
