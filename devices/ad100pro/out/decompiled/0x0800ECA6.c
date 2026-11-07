/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800ECA6
 * Name      : FUN_0800ECA6
 * End       : 0x0800ECC6
 * Size      : 32 bytes, 7 instructions decoded in body (functions.json insns=7, blocks=2)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800ECA6(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800ECA6: 1e00     movs     r6, r3 */
    r6 = r3;
    /* 0x0800ECA8: 07e0     b        #0x800ecba */
    goto L_0800ECBA;

    L_0800ECBA:
    /* 0x0800ECBA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800ECBC: 03f803fc strb     pc, [r3, #-0x3] */
    *(uint8_t *)(r3 - 3) = pc;   /* store */
    /* 0x0800ECC0: 000c     lsrs     r0, r0, #0x10 */
    r0 = r0 >> 0x10;
    /* 0x0800ECC2: 000c     lsrs     r0, r0, #0x10 */
    r0 = r0 >> 0x10;
    /* 0x0800ECC4: 0018     adds     r0, r0, r0 */
    r0 = r0 + r0;
}
