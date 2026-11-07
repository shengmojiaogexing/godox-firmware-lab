/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08003900
 * Name      : FUN_08003900
 * End       : 0x08003904
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x080038C0 FUN_080038C0  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x0800339C FUN_0800339C, 0x0800741C FUN_0800741C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08003900(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08003902: 2889     ldrh     r0, [r5, #8] */
    r0 = *(uint16_t *)(r5 + 8);
}
