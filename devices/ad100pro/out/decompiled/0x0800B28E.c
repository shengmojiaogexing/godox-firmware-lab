/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B28E
 * Name      : FUN_0800B28E
 * End       : 0x0800B298
 * Size      : 10 bytes, 5 instructions decoded in body (functions.json insns=5, blocks=3)
 * Class     : nested_block
 * Parent    : 0x0800B282 FUN_0800B282  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B28E(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B28E: 2748     ldr      r0, [pc, #0x9c]  ; =RAM_20000166 */
    r0 = *(uint32_t *)0x0800B32C;   /* = RAM 0x20000166 */
    /* 0x0800B290: 632a     cmp      r2, #0x63 */
    /* flags = cmp(r2, #0x63) */
    /* 0x0800B292: 0270     strb     r2, [r0] */
    *(uint8_t *)(r0) = r2;   /* store */
    /* 0x0800B294: 02d9     bls      #0x800b29c */
    if (r2 <= 0x63) goto L_0800B29C;   /* unsigned */
    /* 0x0800B296: 6321     movs     r1, #0x63 */
    r1 = 0x63;
}
