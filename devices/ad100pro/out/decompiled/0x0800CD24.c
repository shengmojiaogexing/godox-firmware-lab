/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CD24
 * Name      : FUN_0800CD24
 * End       : 0x0800CD32
 * Size      : 14 bytes, 6 instructions decoded in body (functions.json insns=6, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x0800C204 FUN_0800C204
 * Callees   : 1  -> 0x0800CE0C FUN_0800CE0C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CD24(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CD24: 0a46     mov      r2, r1 */
    r2 = r1;
    /* 0x0800CD26: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800CD28: 0146     mov      r1, r0 */
    r1 = r0;
    /* 0x0800CD2A: 7f20     movs     r0, #0x7f */
    r0 = 0x7f;
    /* 0x0800CD2C: 00f06ef8 bl       #0x800ce0c */
    FUN_0800CE0C();   /* BL -> 0x0800CE0C FUN_0800CE0C */
    /* 0x0800CD30: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
