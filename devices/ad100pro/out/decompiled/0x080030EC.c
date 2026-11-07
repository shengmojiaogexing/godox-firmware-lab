/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080030EC
 * Name      : literal_080030C1
 * End       : 0x080030F0
 * Size      : 4 bytes, 2 instructions decoded in body (functions.json insns=2, blocks=1)
 * Class     : tiny_fragment
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void literal_080030C1(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080030EC: c130     adds     r0, #0xc1 */
    r0 = r0 + 0xc1;
    /* 0x080030EE: 0008     lsrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
}
