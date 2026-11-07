/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800BAAA
 * Name      : FUN_0800BAAA
 * End       : 0x0800BABC
 * Size      : 18 bytes, 8 instructions decoded in body (functions.json insns=8, blocks=1)
 * Class     : nested_block
 * Parent    : 0x0800BA8C IRQ15_Handler  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800BAAA(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800BAAC: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
    /* 0x0800BAAE: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800BAB0: d401     lsls     r4, r2, #7 */
    r4 = r2 << 7;
    /* 0x0800BAB2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800BAB4: 0901     lsls     r1, r1, #4 */
    r1 = r1 << 4;
    /* 0x0800BAB6: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800BAB8: d002     lsls     r0, r2, #0xb */
    r0 = r2 << 0xb;
    /* 0x0800BABA: 0020     movs     r0, #0 */
    r0 = 0;
}
