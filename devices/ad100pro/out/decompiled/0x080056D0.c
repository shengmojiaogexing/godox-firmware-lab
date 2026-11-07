/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080056D0
 * Name      : FUN_080056D0
 * End       : 0x080056E4
 * Size      : 20 bytes, 8 instructions decoded in body (functions.json insns=8, blocks=1)
 * Class     : function
 * Callers   : 2  -> 0x0800C2B8 FUN_0800C2B8, 0x0800C380 FUN_0800C380
 * Callees   : 2  -> 0x080056E4 FUN_080056E4, 0x080057E4 FUN_080057E4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080056D0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080056D0: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x080056D2: 3221     movs     r1, #0x32 */
    r1 = 0x32;
    /* 0x080056D4: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x080056D6: 00f085f8 bl       #0x80057e4 */
    FUN_080057E4();   /* BL -> 0x080057E4 FUN_080057E4 */
    /* 0x080056DA: 3221     movs     r1, #0x32 */
    r1 = 0x32;
    /* 0x080056DC: 0520     movs     r0, #5 */
    r0 = 5;
    /* 0x080056DE: 00f001f8 bl       #0x80056e4 */
    FUN_080056E4();   /* BL -> 0x080056E4 FUN_080056E4 */
    /* 0x080056E2: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
