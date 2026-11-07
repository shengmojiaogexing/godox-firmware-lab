/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009E04
 * Name      : FUN_08009E04
 * End       : 0x08009E0E
 * Size      : 10 bytes, 4 instructions decoded in body (functions.json insns=4, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x08009BCC FUN_08009BCC
 * Callees   : 1  -> 0x080082CC FUN_080082CC
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009E04(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009E04: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08009E06: 0248     ldr      r0, [pc, #8]  ; =RAM_200002D0 */
    r0 = *(uint32_t *)0x08009E10;   /* = RAM 0x200002D0 */
    /* 0x08009E08: fef760fa bl       #0x80082cc */
    FUN_080082CC();   /* BL -> 0x080082CC FUN_080082CC */
    /* 0x08009E0C: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
