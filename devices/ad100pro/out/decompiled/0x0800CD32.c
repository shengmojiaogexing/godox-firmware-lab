/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CD32
 * Name      : FUN_0800CD32
 * End       : 0x0800CD3E
 * Size      : 12 bytes, 5 instructions decoded in body (functions.json insns=5, blocks=1)
 * Class     : hub
 * Callers   : 43  -> 0x08004AF0 FUN_08004AF0, 0x08004B32 FUN_08004B32, 0x0800AA74 FUN_0800AA74, 0x0800AAB2 FUN_0800AAB2, 0x0800AFC4 FUN_0800AFC4, 0x0800B03C FUN_0800B03C, 0x0800B0BE FUN_0800B0BE, 0x0800B176 FUN_0800B176, ... +35
 * Callees   : 1  -> 0x0800CE0C FUN_0800CE0C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CD32(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CD32: 13b5     push     {r0, r1, r4, lr} */
    push({r0, r1, r4, lr});
    /* 0x0800CD34: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800CD36: 01a9     add      r1, sp, #4 */
    r1 = sp + 4;
    /* 0x0800CD38: 00f068f8 bl       #0x800ce0c */
    FUN_0800CE0C();   /* BL -> 0x0800CE0C FUN_0800CE0C */
    /* 0x0800CD3C: 1cbd     pop      {r2, r3, r4, pc} */
    pop({r2, r3, r4, pc});   /* return */
}
