/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006722
 * Name      : FUN_08006722
 * End       : 0x08006730
 * Size      : 14 bytes, 6 instructions decoded in body (functions.json insns=6, blocks=1)
 * Class     : nested_block
 * Parent    : 0x080066C0 FUN_080066C0  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006722(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006724: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
    /* 0x08006726: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x08006728: 8501     lsls     r5, r0, #6 */
    r5 = r0 << 6;
    /* 0x0800672A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800672C: 7c01     lsls     r4, r7, #5 */
    r4 = r7 << 5;
    /* 0x0800672E: 0020     movs     r0, #0 */
    r0 = 0;
}
