/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B282
 * Name      : FUN_0800B282
 * End       : 0x0800B2A4
 * Size      : 34 bytes, 17 instructions decoded in body (functions.json insns=10, blocks=8)
 * Class     : nested_block
 * Parent    : 0x0800B0BE FUN_0800B0BE  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B282(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B282: 2948     ldr      r0, [pc, #0xa4]  ; =RAM_20000165 */
    r0 = *(uint32_t *)0x0800B328;   /* = RAM 0x20000165 */
    /* 0x0800B284: 5a2a     cmp      r2, #0x5a */
    /* flags = cmp(r2, #0x5a) */
    /* 0x0800B286: 0270     strb     r2, [r0] */
    *(uint8_t *)(r0) = r2;   /* store */
    /* 0x0800B288: 08d9     bls      #0x800b29c */
    if (r2 <= 0x5a) goto L_0800B29C;   /* unsigned */
    /* 0x0800B28A: 5a21     movs     r1, #0x5a */
    r1 = 0x5a;
    /* 0x0800B28C: 04e0     b        #0x800b298 */
    goto L_0800B298;
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

    L_0800B298:
    /* 0x0800B298: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x0800B29A: 1ee0     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */

    L_0800B29C:
    /* 0x0800B29C: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x0800B29E: 00d1     bne      #0x800b2a2 */
    if (r2 != 0) goto L_0800B2A2;
    /* 0x0800B2A0: dbe6     b        #0x800b05a */
    goto L_0800B05A;   /* loop */

    L_0800B2A2:
    /* 0x0800B2A2: 1ae0     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */
}
