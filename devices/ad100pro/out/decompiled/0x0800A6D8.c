/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A6D8
 * Name      : FUN_0800A6D8
 * End       : 0x0800A6E0
 * Size      : 8 bytes, 3 instructions decoded in body (functions.json insns=3, blocks=1)
 * Class     : nested_block
 * Parent    : 0x0800A6CC FUN_0800A6CC  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A6D8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A6DA: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
    /* 0x0800A6DC: 0008     lsrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x0800A6DE: 0048     ldr      r0, [pc, #0]  ; =0x4819B510 */
    r0 = *(uint32_t *)0x0800A6E0;   /* = 0x4819B510 */
}
