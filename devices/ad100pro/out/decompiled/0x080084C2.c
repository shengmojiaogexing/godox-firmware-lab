/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080084C2
 * Name      : FUN_080084C2
 * End       : 0x080084C6
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x080083E8 FUN_080083E8  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x08008580 FUN_08008580, 0x08008688 FUN_08008688
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080084C2(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080084C4: 04e0     b        #0x80084d0 */
    FUN_080084D0();   /* tail call 0x080084D0 */
}
