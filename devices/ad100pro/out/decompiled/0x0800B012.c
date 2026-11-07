/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B012
 * Name      : FUN_0800B012
 * End       : 0x0800B03C
 * Size      : 42 bytes, 21 instructions decoded in body (functions.json insns=21, blocks=9)
 * Class     : nested_block
 * Parent    : 0x0800AFC4 FUN_0800AFC4  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B012(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B012: e91e     subs     r1, r5, #3 */
    r1 = r5 - 3;
    /* 0x0800B014: b848     ldr      r0, [pc, #0x2e0]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800B2F8;   /* = RAM 0x20000175 */
    /* 0x0800B016: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800B018: 8008     lsrs     r0, r0, #2 */
    r0 = r0 >> 2;
    /* 0x0800B01A: 8000     lsls     r0, r0, #2 */
    r0 = r0 << 2;
    /* 0x0800B01C: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x0800B01E: 04d0     beq      #0x800b02a */
    if (r2 == 0) goto L_0800B02A;
    /* 0x0800B020: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800B022: 04d0     beq      #0x800b02e */
    if (r2 == 1) goto L_0800B02E;
    /* 0x0800B024: 022a     cmp      r2, #2 */
    /* flags = cmp(r2, #2) */
    /* 0x0800B026: 05d0     beq      #0x800b034 */
    if (r2 == 2) goto L_0800B034;
    /* 0x0800B028: 57e1     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */

    L_0800B02A:
    /* 0x0800B02A: b349     ldr      r1, [pc, #0x2cc]  ; =RAM_20000175 */
    r1 = *(uint32_t *)0x0800B2F8;   /* = RAM 0x20000175 */
    /* 0x0800B02C: 04e0     b        #0x800b038 */
    goto L_0800B038;

    L_0800B02E:
    /* 0x0800B02E: b249     ldr      r1, [pc, #0x2c8]  ; =RAM_20000175 */
    r1 = *(uint32_t *)0x0800B2F8;   /* = RAM 0x20000175 */
    /* 0x0800B030: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800B032: 01e0     b        #0x800b038 */
    goto L_0800B038;

    L_0800B034:
    /* 0x0800B034: b049     ldr      r1, [pc, #0x2c0]  ; =RAM_20000175 */
    r1 = *(uint32_t *)0x0800B2F8;   /* = RAM 0x20000175 */
    /* 0x0800B036: 801c     adds     r0, r0, #2 */
    r0 = r0 + 2;

    L_0800B038:
    /* 0x0800B038: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x0800B03A: 4ee1     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */
}
