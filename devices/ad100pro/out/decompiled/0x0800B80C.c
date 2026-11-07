/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B80C
 * Name      : FUN_0800B80C
 * End       : 0x0800B814
 * Size      : 8 bytes, 3 instructions decoded in body (functions.json insns=3, blocks=1)
 * Class     : function
 * Callers   : 2  -> 0x0800CF74 main, 0x0800D014 FUN_0800D014
 * Callees   : 1  -> 0x0800B814 FUN_0800B814
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B80C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B80C: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800B80E: 00f001f8 bl       #0x800b814 */
    FUN_0800B814();   /* BL -> 0x0800B814 FUN_0800B814 */
    /* 0x0800B812: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
