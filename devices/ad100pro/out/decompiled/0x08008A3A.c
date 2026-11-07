/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008A3A
 * Name      : FUN_08008A3A
 * End       : 0x08008A3E
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08008A2C FUN_08008A2C  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x080072C4 FUN_080072C4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08008A3A(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08008A3C: 0649     ldr      r1, [pc, #0x18]  ; =RAM_20000398 */
    r1 = *(uint32_t *)0x08008A58;   /* = RAM 0x20000398 */
}
