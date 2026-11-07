/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009198
 * Name      : FUN_08009198
 * End       : 0x080091C0
 * Size      : 40 bytes, 20 instructions decoded in body (functions.json insns=20, blocks=5)
 * Class     : function
 * Callers   : 2  -> 0x08008D00 dispatcher_large, 0x08008F3A FUN_08008F3A
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009198(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009198: 0948     ldr      r0, [pc, #0x24]  ; =RAM_20000164 */
    r0 = *(uint32_t *)0x080091C0;   /* = RAM 0x20000164 */
    /* 0x0800919A: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800919C: 427d     ldrb     r2, [r0, #0x15] */
    r2 = *(uint8_t *)(r0 + 21);
    /* 0x0800919E: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x080091A0: 09d0     beq      #0x80091b6 */
    if (r2 == 1) goto L_080091B6;
    /* 0x080091A2: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x080091A4: 2032     adds     r2, #0x20 */
    r2 = r2 + 0x20;
    /* 0x080091A6: 9378     ldrb     r3, [r2, #2] */
    r3 = *(uint8_t *)(r2 + 2);
    /* 0x080091A8: 012b     cmp      r3, #1 */
    /* flags = cmp(r3, #1) */
    /* 0x080091AA: 06d0     beq      #0x80091ba */
    if (r3 == 1) goto L_080091BA;
    /* 0x080091AC: 8179     ldrb     r1, [r0, #6] */
    r1 = *(uint8_t *)(r0 + 6);
    /* 0x080091AE: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x080091B0: 5140     eors     r1, r2 */
    r1 = r1 ^ r2;
    /* 0x080091B2: 8171     strb     r1, [r0, #6] */
    *(uint8_t *)(r0 + 6) = r1;   /* store */
    /* 0x080091B4: 7047     bx       lr */
    return;

    L_080091B6:
    /* 0x080091B6: 4175     strb     r1, [r0, #0x15] */
    *(uint8_t *)(r0 + 21) = r1;   /* store */
    /* 0x080091B8: 7047     bx       lr */
    return;

    L_080091BA:
    /* 0x080091BA: 9170     strb     r1, [r2, #2] */
    *(uint8_t *)(r2 + 2) = r1;   /* store */
    /* 0x080091BC: c174     strb     r1, [r0, #0x13] */
    *(uint8_t *)(r0 + 19) = r1;   /* store */
    /* 0x080091BE: 7047     bx       lr */
    return;
}
