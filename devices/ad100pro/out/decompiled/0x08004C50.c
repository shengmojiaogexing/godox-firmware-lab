/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004C50
 * Name      : FUN_08004C50
 * End       : 0x08004C5C
 * Size      : 12 bytes, 5 instructions decoded in body (functions.json insns=5, blocks=1)
 * Class     : function
 * Callers   : 2  -> 0x0800C2B8 FUN_0800C2B8, 0x0800C2EA FUN_0800C2EA
 * Callees   : 1  -> 0x08005008 FUN_08005008
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004C50(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004C50: 00b5     push     {lr} */
    push({lr});
    /* 0x08004C52: 2021     movs     r1, #0x20 */
    r1 = 0x20;
    /* 0x08004C54: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08004C56: 00f0d7f9 bl       #0x8005008 */
    FUN_08005008();   /* BL -> 0x08005008 FUN_08005008 */
    /* 0x08004C5A: 00bd     pop      {pc} */
    pop({pc});   /* return */
}
