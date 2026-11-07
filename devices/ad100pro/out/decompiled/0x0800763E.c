/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800763E
 * Name      : FUN_0800763E
 * End       : 0x08007648
 * Size      : 10 bytes, 4 instructions decoded in body (functions.json insns=4, blocks=1)
 * Class     : nested_block
 * Parent    : 0x08007610 FUN_08007610  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800763E(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007640: 08bd     pop      {r3, pc} */
    pop({r3, pc});   /* return */
    /* 0x08007642: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x08007644: 0010     asrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x08007646: 0240     ands     r2, r0 */
    r2 = r2 & r0;
}
