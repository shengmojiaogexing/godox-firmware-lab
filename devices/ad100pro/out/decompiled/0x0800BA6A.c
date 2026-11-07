/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800BA6A
 * Name      : FUN_0800BA6A
 * End       : 0x0800BA7C
 * Size      : 18 bytes, 8 instructions decoded in body (functions.json insns=8, blocks=1)
 * Class     : nested_block
 * Parent    : 0x0800BA44 IRQ19_Handler  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800BA6A(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800BA6C: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
    /* 0x0800BA6E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800BA70: d401     lsls     r4, r2, #7 */
    r4 = r2 << 7;
    /* 0x0800BA72: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800BA74: ca01     lsls     r2, r1, #7 */
    r2 = r1 << 7;
    /* 0x0800BA76: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800BA78: 4803     lsls     r0, r1, #0xd */
    r0 = r1 << 0xd;
    /* 0x0800BA7A: 0020     movs     r0, #0 */
    r0 = 0;
}
