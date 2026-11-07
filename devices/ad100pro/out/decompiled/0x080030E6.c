/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080030E6
 * Name      : padding_b_self
 * End       : 0x080030E8
 * Size      : 2 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : vector_handler
 * Callers   : 1  -> 0x080030E6 padding_b_self
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void padding_b_self(void)   /* args in r0-r3, return in r0 unless noted */
{

    L_080030E6:
    /* 0x080030E6: fee7     b        #0x80030e6 */
    goto L_080030E6;   /* loop */
}
