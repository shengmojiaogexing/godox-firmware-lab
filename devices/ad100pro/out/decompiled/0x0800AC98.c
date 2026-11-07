/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800AC98
 * Name      : FUN_0800AC98
 * End       : 0x0800ACA8
 * Size      : 16 bytes, 8 instructions decoded in body (functions.json insns=8, blocks=1)
 * Class     : function
 * Callers   : 3  -> 0x08007F68 FUN_08007F68, 0x08007F84 FUN_08007F84, 0x08008030 FUN_08008030
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800AC98(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800AC98: 0109     lsrs     r1, r0, #4 */
    r1 = r0 >> 4;
    /* 0x0800AC9A: 0a22     movs     r2, #0xa */
    r2 = 0xa;
    /* 0x0800AC9C: 0007     lsls     r0, r0, #0x1c */
    r0 = r0 << 0x1c;
    /* 0x0800AC9E: 5143     muls     r1, r2, r1 */
    r1 = r2 * r1;
    /* 0x0800ACA0: 000f     lsrs     r0, r0, #0x1c */
    r0 = r0 >> 0x1c;
    /* 0x0800ACA2: 4018     adds     r0, r0, r1 */
    r0 = r0 + r1;
    /* 0x0800ACA4: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800ACA6: 7047     bx       lr */
    return;
}
