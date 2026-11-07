/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004D18
 * Name      : FUN_08004D18
 * End       : 0x08004D28
 * Size      : 16 bytes, 7 instructions decoded in body (functions.json insns=7, blocks=1)
 * Class     : function
 * Callers   : 2  -> 0x0800C2B8 FUN_0800C2B8, 0x0800C33E FUN_0800C33E
 * Callees   : 1  -> 0x080050C8 FUN_080050C8
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004D18(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004D18: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x08004D1A: 00b5     push     {lr} */
    push({lr});
    /* 0x08004D1C: c2b2     uxtb     r2, r0 */
    r2 = uxtb(r0);
    /* 0x08004D1E: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x08004D20: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08004D22: 00f0d1f9 bl       #0x80050c8 */
    FUN_080050C8();   /* BL -> 0x080050C8 FUN_080050C8 */
    /* 0x08004D26: 00bd     pop      {pc} */
    pop({pc});   /* return */
}
