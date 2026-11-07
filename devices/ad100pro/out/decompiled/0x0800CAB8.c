/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CAB8
 * Name      : FUN_0800CAB8
 * End       : 0x0800CAE8
 * Size      : 48 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=4)
 * Class     : hub
 * Callers   : 23  -> 0x08004AF0 FUN_08004AF0, 0x08004B16 FUN_08004B16, 0x08004B32 FUN_08004B32, 0x0800AB7C FUN_0800AB7C, 0x0800AB92 FUN_0800AB92, 0x0800ABB8 FUN_0800ABB8, 0x0800ABE4 FUN_0800ABE4, 0x0800AC36 FUN_0800AC36, ... +15
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CAB8(void)   /* args in r0-r3, return in r0 unless noted */
{

    L_0800CAB8:
    /* 0x0800CAB8: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x0800CABA: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x0800CABC: 0021     movs     r1, #0 */
    r1 = 0;

    L_0800CABE:
    /* 0x0800CABE: 00bf     nop       */
    /* nop */
    /* 0x0800CAC0: 00bf     nop       */
    /* nop */
    /* 0x0800CAC2: 00bf     nop       */
    /* nop */
    /* 0x0800CAC4: 00bf     nop       */
    /* nop */
    /* 0x0800CAC6: 00bf     nop       */
    /* nop */
    /* 0x0800CAC8: 00bf     nop       */
    /* nop */
    /* 0x0800CACA: 00bf     nop       */
    /* nop */
    /* 0x0800CACC: 00bf     nop       */
    /* nop */
    /* 0x0800CACE: 00bf     nop       */
    /* nop */
    /* 0x0800CAD0: 00bf     nop       */
    /* nop */
    /* 0x0800CAD2: 00bf     nop       */
    /* nop */
    /* 0x0800CAD4: 00bf     nop       */
    /* nop */
    /* 0x0800CAD6: 00bf     nop       */
    /* nop */
    /* 0x0800CAD8: 00bf     nop       */
    /* nop */
    /* 0x0800CADA: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x0800CADC: 89b2     uxth     r1, r1 */
    r1 = uxth(r1);
    /* 0x0800CADE: 0229     cmp      r1, #2 */
    /* flags = cmp(r1, #2) */
    /* 0x0800CAE0: edd3     blo      #0x800cabe */
    if (r1 < 2) goto L_0800CABE;   /* unsigned */
    /* 0x0800CAE2: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800CAE4: e8d1     bne      #0x800cab8 */
    if (r0 != 0) goto L_0800CAB8;
    /* 0x0800CAE6: 7047     bx       lr */
    return;
}
