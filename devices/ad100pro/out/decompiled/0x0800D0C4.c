/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800D0C4
 * Name      : FUN_0800D0C4
 * End       : 0x0800D0E4
 * Size      : 32 bytes, 11 instructions decoded in body (functions.json insns=11, blocks=1)
 * Class     : function
 * Callers   : 3  -> 0x0800C230 FUN_0800C230, 0x0800C24A FUN_0800C24A, 0x0800C268 FUN_0800C268
 * Callees   : 2  -> 0x0800C2B8 FUN_0800C2B8, 0x0800C82C FUN_0800C82C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800D0C4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800D0C4: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800D0C6: fff7f7f8 bl       #0x800c2b8 */
    FUN_0800C2B8();   /* BL -> 0x0800C2B8 FUN_0800C2B8 */
    /* 0x0800D0CA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800D0CC: fff7aefb bl       #0x800c82c */
    FUN_0800C82C();   /* BL -> 0x0800C82C FUN_0800C82C */
    /* 0x0800D0D0: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800D0D2: fff7abfb bl       #0x800c82c */
    FUN_0800C82C();   /* BL -> 0x0800C82C FUN_0800C82C */
    /* 0x0800D0D6: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x0800D0D8: fff7a8fb bl       #0x800c82c */
    FUN_0800C82C();   /* BL -> 0x0800C82C FUN_0800C82C */
    /* 0x0800D0DC: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x0800D0DE: fff7a5fb bl       #0x800c82c */
    FUN_0800C82C();   /* BL -> 0x0800C82C FUN_0800C82C */
    /* 0x0800D0E2: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
