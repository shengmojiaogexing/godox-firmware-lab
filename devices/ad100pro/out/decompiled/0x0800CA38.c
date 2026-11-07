/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CA38
 * Name      : FUN_0800CA38
 * End       : 0x0800CA68
 * Size      : 48 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=4)
 * Class     : hub
 * Callers   : 85  -> 0x080035A8 FUN_080035A8, 0x08003AC8 FUN_08003AC8, 0x08003AD4 FUN_08003AD4, 0x08003E2C FUN_08003E2C, 0x08003E56 FUN_08003E56, 0x08003E66 FUN_08003E66, 0x08003E78 FUN_08003E78, 0x08003E88 FUN_08003E88, ... +77
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CA38(void)   /* args in r0-r3, return in r0 unless noted */
{

    L_0800CA38:
    /* 0x0800CA38: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x0800CA3A: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x0800CA3C: 0021     movs     r1, #0 */
    r1 = 0;

    L_0800CA3E:
    /* 0x0800CA3E: 00bf     nop       */
    /* nop */
    /* 0x0800CA40: 00bf     nop       */
    /* nop */
    /* 0x0800CA42: 00bf     nop       */
    /* nop */
    /* 0x0800CA44: 00bf     nop       */
    /* nop */
    /* 0x0800CA46: 00bf     nop       */
    /* nop */
    /* 0x0800CA48: 00bf     nop       */
    /* nop */
    /* 0x0800CA4A: 00bf     nop       */
    /* nop */
    /* 0x0800CA4C: 00bf     nop       */
    /* nop */
    /* 0x0800CA4E: 00bf     nop       */
    /* nop */
    /* 0x0800CA50: 00bf     nop       */
    /* nop */
    /* 0x0800CA52: 00bf     nop       */
    /* nop */
    /* 0x0800CA54: 00bf     nop       */
    /* nop */
    /* 0x0800CA56: 00bf     nop       */
    /* nop */
    /* 0x0800CA58: 00bf     nop       */
    /* nop */
    /* 0x0800CA5A: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x0800CA5C: 89b2     uxth     r1, r1 */
    r1 = uxth(r1);
    /* 0x0800CA5E: 0229     cmp      r1, #2 */
    /* flags = cmp(r1, #2) */
    /* 0x0800CA60: edd3     blo      #0x800ca3e */
    if (r1 < 2) goto L_0800CA3E;   /* unsigned */
    /* 0x0800CA62: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800CA64: e8d1     bne      #0x800ca38 */
    if (r0 != 0) goto L_0800CA38;
    /* 0x0800CA66: 7047     bx       lr */
    return;
}
