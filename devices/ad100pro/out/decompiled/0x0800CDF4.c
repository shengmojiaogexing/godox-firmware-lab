/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CDF4
 * Name      : FUN_0800CDF4
 * End       : 0x0800CE0C
 * Size      : 24 bytes, 9 instructions decoded in body (functions.json insns=9, blocks=1)
 * Class     : hub
 * Callers   : 30  -> 0x08004AF0 FUN_08004AF0, 0x08004B16 FUN_08004B16, 0x08004B32 FUN_08004B32, 0x0800AA74 FUN_0800AA74, 0x0800AB40 FUN_0800AB40, 0x0800AB7C FUN_0800AB7C, 0x0800AB8C FUN_0800AB8C, 0x0800AB92 FUN_0800AB92, ... +22
 * Callees   : 3  -> 0x0800CD7C FUN_0800CD7C, 0x0800CE38 FUN_0800CE38, 0x0800CE58 FUN_0800CE58
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CDF4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CDF4: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800CDF6: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x0800CDF8: 00f01ef8 bl       #0x800ce38 */
    FUN_0800CE38();   /* BL -> 0x0800CE38 FUN_0800CE38 */
    /* 0x0800CDFC: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800CDFE: fff7bdff bl       #0x800cd7c */
    FUN_0800CD7C();   /* BL -> 0x0800CD7C FUN_0800CD7C */
    /* 0x0800CE02: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x0800CE04: 00f028f8 bl       #0x800ce58 */
    FUN_0800CE58();   /* BL -> 0x0800CE58 FUN_0800CE58 */
    /* 0x0800CE08: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800CE0A: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
