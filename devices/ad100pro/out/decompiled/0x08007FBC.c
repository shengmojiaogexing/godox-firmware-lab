/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007FBC
 * Name      : FUN_08007FBC
 * End       : 0x08007FC0
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08007F68 FUN_08007F68  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x0800ACA8 FUN_0800ACA8
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007FBC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007FBE: 0002     lsls     r0, r0, #8 */
    r0 = r0 << 8;
}
