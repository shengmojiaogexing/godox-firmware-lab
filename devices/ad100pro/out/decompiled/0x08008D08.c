/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008D08
 * Name      : FUN_08008D08
 * End       : 0x08008D0C
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08008D00 dispatcher_large  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x080072C4 FUN_080072C4, 0x0800CA38 FUN_0800CA38
 * MMIO      : GPIOB.BRR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08008D08(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08008D0A: c54d     ldr      r5, [pc, #0x314]  ; =RAM_20000164 */
    r5 = *(uint32_t *)0x08009020;   /* = RAM 0x20000164 */
}
