/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080039B8
 * Name      : FUN_080039B8
 * End       : 0x080039BC
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x080038C0 FUN_080038C0  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x0800341C FUN_0800341C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080039B8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080039BA: 3c4a     ldr      r2, [pc, #0xf0]  ; =0x66666666 */
    r2 = *(uint32_t *)0x08003AAC;   /* = 0x66666666 */
}
