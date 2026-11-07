/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800E0E0
 * Name      : FUN_0800E0E0
 * End       : 0x0800E0F0
 * Size      : 16 bytes, 7 instructions decoded in body (functions.json insns=7, blocks=1)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800E0E0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800E0E0: 03f80000 strb.w   r0, [r3, r0] */
    *(uint8_t *)(r3 + r0) = r0;   /* store */
    /* 0x0800E0E4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E0E6: 0803     lsls     r0, r1, #0xc */
    r0 = r1 << 0xc;
    /* 0x0800E0E8: f003     lsls     r0, r6, #0xf */
    r0 = r6 << 0xf;
    /* 0x0800E0EA: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800E0EC: 0e00     movs     r6, r1 */
    r6 = r1;
    /* 0x0800E0EE: 7000     lsls     r0, r6, #1 */
    r0 = r6 << 1;
}
