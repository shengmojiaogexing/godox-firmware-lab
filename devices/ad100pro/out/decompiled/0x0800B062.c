/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B062
 * Name      : FUN_0800B062
 * End       : 0x0800B0BC
 * Size      : 90 bytes, 45 instructions decoded in body (functions.json insns=45, blocks=16)
 * Class     : nested_block
 * Parent    : 0x0800B03C FUN_0800B03C  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B062(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B062: a549     ldr      r1, [pc, #0x294]  ; =RAM_20000175 */
    r1 = *(uint32_t *)0x0800B2F8;   /* = RAM 0x20000175 */
    /* 0x0800B064: e723     movs     r3, #0xe7 */
    r3 = 0xe7;
    /* 0x0800B066: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800B068: 1940     ands     r1, r3 */
    r1 = r1 & r3;
    /* 0x0800B06A: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x0800B06C: 02d0     beq      #0x800b074 */
    if (r2 == 0) goto L_0800B074;
    /* 0x0800B06E: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800B070: 12d0     beq      #0x800b098 */
    if (r2 == 1) goto L_0800B098;
    /* 0x0800B072: 32e1     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */

    L_0800B074:
    /* 0x0800B074: 0428     cmp      r0, #4 */
    /* flags = cmp(r0, #4) */
    /* 0x0800B076: 05d0     beq      #0x800b084 */
    if (r0 == 4) goto L_0800B084;
    /* 0x0800B078: 0528     cmp      r0, #5 */
    /* flags = cmp(r0, #5) */
    /* 0x0800B07A: 03d0     beq      #0x800b084 */
    if (r0 == 5) goto L_0800B084;
    /* 0x0800B07C: 0328     cmp      r0, #3 */
    /* flags = cmp(r0, #3) */
    /* 0x0800B07E: 01d0     beq      #0x800b084 */
    if (r0 == 3) goto L_0800B084;
    /* 0x0800B080: 0728     cmp      r0, #7 */
    /* flags = cmp(r0, #7) */
    /* 0x0800B082: 04d1     bne      #0x800b08e */
    if (r0 != 7) goto L_0800B08E;

    L_0800B084:
    /* 0x0800B084: e806     lsls     r0, r5, #0x1b */
    r0 = r5 << 0x1b;
    /* 0x0800B086: 02d5     bpl      #0x800b08e */
    if (r0 >= 7) goto L_0800B08E;   /* result non-negative */
    /* 0x0800B088: 9f48     ldr      r0, [pc, #0x27c]  ; =RAM_2000020E */
    r0 = *(uint32_t *)0x0800B308;   /* = RAM 0x2000020E */
    /* 0x0800B08A: 1e22     movs     r2, #0x1e */
    r2 = 0x1e;
    /* 0x0800B08C: 0271     strb     r2, [r0, #4] */
    *(uint8_t *)(r0 + 4) = r2;   /* store */

    L_0800B08E:
    /* 0x0800B08E: 9a48     ldr      r0, [pc, #0x268]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800B2F8;   /* = RAM 0x20000175 */
    /* 0x0800B090: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x0800B092: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800B094: 9c49     ldr      r1, [pc, #0x270]  ; =RAM_2000020E */
    r1 = *(uint32_t *)0x0800B308;   /* = RAM 0x2000020E */
    /* 0x0800B096: 0be0     b        #0x800b0b0 */
    goto L_0800B0B0;

    L_0800B098:
    /* 0x0800B098: 9748     ldr      r0, [pc, #0x25c]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800B2F8;   /* = RAM 0x20000175 */
    /* 0x0800B09A: 012f     cmp      r7, #1 */
    /* flags = cmp(r7, #1) */
    /* 0x0800B09C: 03d1     bne      #0x800b0a6 */
    if (r7 != 1) goto L_0800B0A6;
    /* 0x0800B09E: 9b4a     ldr      r2, [pc, #0x26c]  ; =RAM_20000480 */
    r2 = *(uint32_t *)0x0800B30C;   /* = RAM 0x20000480 */
    /* 0x0800B0A0: 9279     ldrb     r2, [r2, #6] */
    r2 = *(uint8_t *)(r2 + 6);
    /* 0x0800B0A2: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800B0A4: 06d0     beq      #0x800b0b4 */
    if (r2 == 1) goto L_0800B0B4;

    L_0800B0A6:
    /* 0x0800B0A6: 1031     adds     r1, #0x10 */
    r1 = r1 + 0x10;
    /* 0x0800B0A8: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x0800B0AA: 9749     ldr      r1, [pc, #0x25c]  ; =RAM_2000020E */
    r1 = *(uint32_t *)0x0800B308;   /* = RAM 0x2000020E */
    /* 0x0800B0AC: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800B0AE: 4870     strb     r0, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r0;   /* store */

    L_0800B0B0:
    /* 0x0800B0B0: 8870     strb     r0, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r0;   /* store */
    /* 0x0800B0B2: 12e1     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */

    L_0800B0B4:
    /* 0x0800B0B4: 9449     ldr      r1, [pc, #0x250]  ; =RAM_2000020E */
    r1 = *(uint32_t *)0x0800B308;   /* = RAM 0x2000020E */
    /* 0x0800B0B6: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800B0B8: 4870     strb     r0, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r0;   /* store */
    /* 0x0800B0BA: 0ee1     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */
}
