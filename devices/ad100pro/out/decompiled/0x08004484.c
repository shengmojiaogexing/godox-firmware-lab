/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004484
 * Name      : FUN_08004484
 * End       : 0x080044D0
 * Size      : 76 bytes, 38 instructions decoded in body (functions.json insns=38, blocks=14)
 * Class     : function
 * Callers   : 2  -> 0x08008D00 dispatcher_large, 0x08008F9E FUN_08008F9E
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004484(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004484: 1248     ldr      r0, [pc, #0x48]  ; =RAM_20000164 */
    r0 = *(uint32_t *)0x080044D0;   /* = RAM 0x20000164 */
    /* 0x08004486: 017a     ldrb     r1, [r0, #8] */
    r1 = *(uint8_t *)(r0 + 8);
    /* 0x08004488: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x0800448A: 05d0     beq      #0x8004498 */
    if (r1 == 0) goto L_08004498;
    /* 0x0800448C: 418d     ldrh     r1, [r0, #0x2a] */
    r1 = *(uint16_t *)(r0 + 42);
    /* 0x0800448E: c829     cmp      r1, #0xc8 */
    /* flags = cmp(r1, #0xc8) */
    /* 0x08004490: 01d9     bls      #0x8004496 */
    if (r1 <= 0xc8) goto L_08004496;   /* unsigned */
    /* 0x08004492: c839     subs     r1, #0xc8 */
    r1 = r1 - 0xc8;
    /* 0x08004494: 4185     strh     r1, [r0, #0x2a] */
    *(uint16_t *)(r0 + 42) = r1;   /* store */

    L_08004496:
    /* 0x08004496: 7047     bx       lr */
    return;

    L_08004498:
    /* 0x08004498: 0d4a     ldr      r2, [pc, #0x34]  ; =RAM_20000164 */
    r2 = *(uint32_t *)0x080044D0;   /* = RAM 0x20000164 */
    /* 0x0800449A: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800449C: 2032     adds     r2, #0x20 */
    r2 = r2 + 0x20;
    /* 0x0800449E: 9378     ldrb     r3, [r2, #2] */
    r3 = *(uint8_t *)(r2 + 2);
    /* 0x080044A0: 012b     cmp      r3, #1 */
    /* flags = cmp(r3, #1) */
    /* 0x080044A2: 09d0     beq      #0x80044b8 */
    if (r3 == 1) goto L_080044B8;
    /* 0x080044A4: 0278     ldrb     r2, [r0] */
    r2 = *(uint8_t *)(r0);
    /* 0x080044A6: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x080044A8: 02d0     beq      #0x80044b0 */
    if (r2 == 1) goto L_080044B0;
    /* 0x080044AA: 827c     ldrb     r2, [r0, #0x12] */
    r2 = *(uint8_t *)(r0 + 18);
    /* 0x080044AC: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x080044AE: 05d0     beq      #0x80044bc */
    if (r2 == 0) goto L_080044BC;

    L_080044B0:
    /* 0x080044B0: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x080044B2: 8174     strb     r1, [r0, #0x12] */
    *(uint8_t *)(r0 + 18) = r1;   /* store */
    /* 0x080044B4: 4163     str      r1, [r0, #0x34] */
    *(uint32_t *)(r0 + 52) = r1;   /* store */
    /* 0x080044B6: 7047     bx       lr */
    return;

    L_080044B8:
    /* 0x080044B8: 9170     strb     r1, [r2, #2] */
    *(uint8_t *)(r2 + 2) = r1;   /* store */
    /* 0x080044BA: 05e0     b        #0x80044c8 */
    goto L_080044C8;

    L_080044BC:
    /* 0x080044BC: 427d     ldrb     r2, [r0, #0x15] */
    r2 = *(uint8_t *)(r0 + 21);
    /* 0x080044BE: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x080044C0: 04d0     beq      #0x80044cc */
    if (r2 == 0) goto L_080044CC;
    /* 0x080044C2: 0022     movs     r2, #0 */
    r2 = 0;

    L_080044C4:
    /* 0x080044C4: 4275     strb     r2, [r0, #0x15] */
    *(uint8_t *)(r0 + 21) = r2;   /* store */
    /* 0x080044C6: 8175     strb     r1, [r0, #0x16] */
    *(uint8_t *)(r0 + 22) = r1;   /* store */

    L_080044C8:
    /* 0x080044C8: c174     strb     r1, [r0, #0x13] */
    *(uint8_t *)(r0 + 19) = r1;   /* store */
    /* 0x080044CA: 7047     bx       lr */
    return;

    L_080044CC:
    /* 0x080044CC: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x080044CE: f9e7     b        #0x80044c4 */
    goto L_080044C4;   /* loop */
}
