/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800AD04
 * Name      : FUN_0800AD04
 * End       : 0x0800AD10
 * Size      : 12 bytes, 5 instructions decoded in body (functions.json insns=5, blocks=1)
 * Class     : nested_block
 * Parent    : 0x0800ACF8 IRQ2_Handler  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800AD04(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800AD06: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
    /* 0x0800AD08: 1a02     lsls     r2, r3, #8 */
    r2 = r3 << 8;
    /* 0x0800AD0A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800AD0C: 7402     lsls     r4, r6, #9 */
    r4 = r6 << 9;
    /* 0x0800AD0E: 0020     movs     r0, #0 */
    r0 = 0;
}
