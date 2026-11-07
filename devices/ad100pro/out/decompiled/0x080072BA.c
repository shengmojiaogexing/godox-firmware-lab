/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080072BA
 * Name      : FUN_080072BA
 * End       : 0x080072C4
 * Size      : 10 bytes, 4 instructions decoded in body (functions.json insns=4, blocks=1)
 * Class     : nested_block
 * Parent    : 0x080072AC FUN_080072AC  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080072BA(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080072BC: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
    /* 0x080072BE: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x080072C0: 0004     lsls     r0, r0, #0x10 */
    r0 = r0 << 0x10;
    /* 0x080072C2: 0140     ands     r1, r0 */
    r1 = r1 & r0;
}
