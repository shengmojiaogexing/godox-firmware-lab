/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006600
 * Name      : FUN_08006600
 * End       : 0x08006604
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x080065CC FUN_080065CC  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x08005C44 FUN_08005C44, 0x0800A4E8 FUN_0800A4E8
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006600(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006602: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
