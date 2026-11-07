/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C878
 * Name      : FUN_0800C878
 * End       : 0x0800C8BC
 * Size      : 68 bytes, 34 instructions decoded in body (functions.json insns=34, blocks=10)
 * Class     : function
 * Callers   : 2  -> 0x08008D00 dispatcher_large, 0x08008F12 FUN_08008F12
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C878(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C878: 1048     ldr      r0, [pc, #0x40]  ; =RAM_20000164 */
    r0 = *(uint32_t *)0x0800C8BC;   /* = RAM 0x20000164 */
    /* 0x0800C87A: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800C87C: 427d     ldrb     r2, [r0, #0x15] */
    r2 = *(uint8_t *)(r0 + 21);
    /* 0x0800C87E: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800C880: 0ed0     beq      #0x800c8a0 */
    if (r2 == 1) goto L_0800C8A0;
    /* 0x0800C882: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x0800C884: 2032     adds     r2, #0x20 */
    r2 = r2 + 0x20;
    /* 0x0800C886: 9378     ldrb     r3, [r2, #2] */
    r3 = *(uint8_t *)(r2 + 2);
    /* 0x0800C888: 012b     cmp      r3, #1 */
    /* flags = cmp(r3, #1) */
    /* 0x0800C88A: 0bd0     beq      #0x800c8a4 */
    if (r3 == 1) goto L_0800C8A4;
    /* 0x0800C88C: 0278     ldrb     r2, [r0] */
    r2 = *(uint8_t *)(r0);
    /* 0x0800C88E: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800C890: 02d0     beq      #0x800c898 */
    if (r2 == 1) goto L_0800C898;
    /* 0x0800C892: 827c     ldrb     r2, [r0, #0x12] */
    r2 = *(uint8_t *)(r0 + 18);
    /* 0x0800C894: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x0800C896: 08d0     beq      #0x800c8aa */
    if (r2 == 0) goto L_0800C8AA;

    L_0800C898:
    /* 0x0800C898: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x0800C89A: 8174     strb     r1, [r0, #0x12] */
    *(uint8_t *)(r0 + 18) = r1;   /* store */
    /* 0x0800C89C: 4163     str      r1, [r0, #0x34] */
    *(uint32_t *)(r0 + 52) = r1;   /* store */
    /* 0x0800C89E: 7047     bx       lr */
    return;

    L_0800C8A0:
    /* 0x0800C8A0: 4175     strb     r1, [r0, #0x15] */
    *(uint8_t *)(r0 + 21) = r1;   /* store */
    /* 0x0800C8A2: 7047     bx       lr */
    return;

    L_0800C8A4:
    /* 0x0800C8A4: 9170     strb     r1, [r2, #2] */
    *(uint8_t *)(r2 + 2) = r1;   /* store */
    /* 0x0800C8A6: c174     strb     r1, [r0, #0x13] */
    *(uint8_t *)(r0 + 19) = r1;   /* store */

    L_0800C8A8:
    /* 0x0800C8A8: 7047     bx       lr */
    return;

    L_0800C8AA:
    /* 0x0800C8AA: 0548     ldr      r0, [pc, #0x14]  ; =RAM_20000231 */
    r0 = *(uint32_t *)0x0800C8C0;   /* = RAM 0x20000231 */
    /* 0x0800C8AC: 0278     ldrb     r2, [r0] */
    r2 = *(uint8_t *)(r0);
    /* 0x0800C8AE: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x0800C8B0: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);
    /* 0x0800C8B2: 0270     strb     r2, [r0] */
    *(uint8_t *)(r0) = r2;   /* store */
    /* 0x0800C8B4: 042a     cmp      r2, #4 */
    /* flags = cmp(r2, #4) */
    /* 0x0800C8B6: f7d9     bls      #0x800c8a8 */
    if (r2 <= 4) goto L_0800C8A8;   /* unsigned */
    /* 0x0800C8B8: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x0800C8BA: 7047     bx       lr */
    return;
}
