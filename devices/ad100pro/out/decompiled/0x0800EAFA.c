/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800EAFA
 * Name      : FUN_0800EAFA
 * End       : 0x0800EB4A
 * Size      : 80 bytes, 7 instructions decoded in body (functions.json insns=7, blocks=2)
 * Class     : nested_block
 * Parent    : 0x0800EA58 FUN_0800EA58  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800EAFA(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800EAFA: 1dfe001f cdp2     p15, #1, c1, c13, c0, #0 */
    /* TODO: cdp2 p15, #1, c1, c13, c0, #0 */
    /* 0x0800EAFE: f800     lsls     r0, r7, #3 */
    r0 = r7 << 3;
    /* 0x0800EB00: 1fe0     b        #0x800eb42 */
    goto L_0800EB42;

    L_0800EB42:
    /* 0x0800EB42: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EB44: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EB46: 0d01     lsls     r5, r1, #4 */
    r5 = r1 << 4;
    /* 0x0800EB48: f030     adds     r0, #0xf0 */
    r0 = r0 + 0xf0;
}
