/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080095E8
 * Name      : FUN_080095E8
 * End       : 0x080095F8
 * Size      : 16 bytes, 7 instructions decoded in body (functions.json insns=7, blocks=1)
 * Class     : nested_block
 * Parent    : 0x080095CC FUN_080095CC  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080095E8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080095EA: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
    /* 0x080095EC: 0030     adds     r0, #0 */
    r0 = r0 + 0;
    /* 0x080095EE: 0040     ands     r0, r0 */
    r0 = r0 & r0;
    /* 0x080095F0: 8403     lsls     r4, r0, #0xe */
    r4 = r0 << 0xe;
    /* 0x080095F2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080095F4: ff0f     lsrs     r7, r7, #0x1f */
    r7 = r7 >> 0x1f;
    /* 0x080095F6: 0000     movs     r0, r0 */
    r0 = r0;
}
