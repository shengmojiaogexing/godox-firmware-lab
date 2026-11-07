/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B266
 * Name      : FUN_0800B266
 * End       : 0x0800B26C
 * Size      : 6 bytes, 3 instructions decoded in body (functions.json insns=3, blocks=2)
 * Class     : nested_block
 * Parent    : 0x0800B0BE FUN_0800B0BE  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B266(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B266: 2c48     ldr      r0, [pc, #0xb0]  ; =RAM_200001BD */
    r0 = *(uint32_t *)0x0800B318;   /* = RAM 0x200001BD */
    /* 0x0800B268: 8255     strb     r2, [r0, r6] */
    *(uint8_t *)(r0 + r6) = r2;   /* store */
    /* 0x0800B26A: 36e0     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */
}
