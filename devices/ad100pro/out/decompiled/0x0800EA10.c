/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800EA10
 * Name      : FUN_0800EA10
 * End       : 0x0800EA3C
 * Size      : 44 bytes, 18 instructions decoded in body (functions.json insns=18, blocks=2)
 * Class     : nested_block
 * Parent    : 0x0800E800 FUN_0800E800  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800EA10(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800EA10: 0e07     lsls     r6, r1, #0x1c */
    r6 = r1 << 0x1c;
    /* 0x0800EA12: f80e     lsrs     r0, r7, #0x1b */
    r0 = r7 >> 0x1b;
    /* 0x0800EA14: 03e0     b        #0x800ea1e */
    goto L_0800EA1E;

    L_0800EA1E:
    /* 0x0800EA1E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EA20: 0d03     lsls     r5, r1, #0xc */
    r5 = r1 << 0xc;
    /* 0x0800EA22: 0060     str      r0, [r0] */
    *(uint32_t *)(r0) = r0;   /* store */
    /* 0x0800EA24: 0700     movs     r7, r0 */
    r7 = r0;
    /* 0x0800EA26: 780f     lsrs     r0, r7, #0x1d */
    r0 = r7 >> 0x1d;
    /* 0x0800EA28: 007c     ldrb     r0, [r0, #0x10] */
    r0 = *(uint8_t *)(r0 + 16);
    /* 0x0800EA2A: 1f00     movs     r7, r3 */
    r7 = r3;
    /* 0x0800EA2C: 7c1e     subs     r4, r7, #1 */
    r4 = r7 - 1;
    /* 0x0800EA2E: 001e     subs     r0, r0, #0 */
    r0 = r0 - 0;
    /* 0x0800EA30: 1c38     subs     r0, #0x1c */
    r0 = r0 - 0x1c;
    /* 0x0800EA32: 0e1c     adds     r6, r1, #0 */
    r6 = r1 + 0;
    /* 0x0800EA34: 380e     lsrs     r0, r7, #0x18 */
    r0 = r7 >> 0x18;
    /* 0x0800EA36: 1e78     ldrb     r6, [r3] */
    r6 = *(uint8_t *)(r3);
    /* 0x0800EA38: 0e1f     subs     r6, r1, #4 */
    r6 = r1 - 4;
    /* 0x0800EA3A: fc1e     subs     r4, r7, #3 */
    r4 = r7 - 3;
}
