/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CA68
 * Name      : FUN_0800CA68
 * End       : 0x0800CA98
 * Size      : 48 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=4)
 * Class     : function
 * Callers   : 2  -> 0x08005B0C IRQ7_Handler, 0x08005B28 FUN_08005B28
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CA68(void)   /* args in r0-r3, return in r0 unless noted */
{

    L_0800CA68:
    /* 0x0800CA68: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x0800CA6A: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x0800CA6C: 0021     movs     r1, #0 */
    r1 = 0;

    L_0800CA6E:
    /* 0x0800CA6E: 00bf     nop       */
    /* nop */
    /* 0x0800CA70: 00bf     nop       */
    /* nop */
    /* 0x0800CA72: 00bf     nop       */
    /* nop */
    /* 0x0800CA74: 00bf     nop       */
    /* nop */
    /* 0x0800CA76: 00bf     nop       */
    /* nop */
    /* 0x0800CA78: 00bf     nop       */
    /* nop */
    /* 0x0800CA7A: 00bf     nop       */
    /* nop */
    /* 0x0800CA7C: 00bf     nop       */
    /* nop */
    /* 0x0800CA7E: 00bf     nop       */
    /* nop */
    /* 0x0800CA80: 00bf     nop       */
    /* nop */
    /* 0x0800CA82: 00bf     nop       */
    /* nop */
    /* 0x0800CA84: 00bf     nop       */
    /* nop */
    /* 0x0800CA86: 00bf     nop       */
    /* nop */
    /* 0x0800CA88: 00bf     nop       */
    /* nop */
    /* 0x0800CA8A: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x0800CA8C: 89b2     uxth     r1, r1 */
    r1 = uxth(r1);
    /* 0x0800CA8E: 0229     cmp      r1, #2 */
    /* flags = cmp(r1, #2) */
    /* 0x0800CA90: edd3     blo      #0x800ca6e */
    if (r1 < 2) goto L_0800CA6E;   /* unsigned */
    /* 0x0800CA92: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800CA94: e8d1     bne      #0x800ca68 */
    if (r0 != 0) goto L_0800CA68;
    /* 0x0800CA96: 7047     bx       lr */
    return;
}
