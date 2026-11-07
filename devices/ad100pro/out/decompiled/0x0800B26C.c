/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B26C
 * Name      : FUN_0800B26C
 * End       : 0x0800B282
 * Size      : 22 bytes, 11 instructions decoded in body (functions.json insns=10, blocks=5)
 * Class     : nested_block
 * Parent    : 0x0800B0BE FUN_0800B0BE  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B26C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B26C: 2c48     ldr      r0, [pc, #0xb0]  ; =RAM_2000019C */
    r0 = *(uint32_t *)0x0800B320;   /* = RAM 0x2000019C */
    /* 0x0800B26E: 00e0     b        #0x800b272 */
    goto L_0800B272;
    /* 0x0800B270: 2c48     ldr      r0, [pc, #0xb0]  ; =RAM_200001A2 */
    r0 = *(uint32_t *)0x0800B324;   /* = RAM 0x200001A2 */

    L_0800B272:
    /* 0x0800B272: 502a     cmp      r2, #0x50 */
    /* flags = cmp(r2, #0x50) */
    /* 0x0800B274: 8255     strb     r2, [r0, r6] */
    *(uint8_t *)(r0 + r6) = r2;   /* store */
    /* 0x0800B276: 30d9     bls      #0x800b2da */
    if (r2 <= 0x50) goto L_0800B2DA;   /* unsigned */
    /* 0x0800B278: ff2a     cmp      r2, #0xff */
    /* flags = cmp(r2, #0xff) */
    /* 0x0800B27A: 2ed0     beq      #0x800b2da */
    if (r2 == 0xff) goto L_0800B2DA;
    /* 0x0800B27C: 5021     movs     r1, #0x50 */
    r1 = 0x50;
    /* 0x0800B27E: 8155     strb     r1, [r0, r6] */
    *(uint8_t *)(r0 + r6) = r1;   /* store */
    /* 0x0800B280: 2be0     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */
}
