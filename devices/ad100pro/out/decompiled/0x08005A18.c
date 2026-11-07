/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005A18
 * Name      : FUN_08005A18
 * End       : 0x08005A24
 * Size      : 12 bytes, 5 instructions decoded in body (functions.json insns=5, blocks=1)
 * Class     : function
 * Callers   : 2  -> 0x0800C2B8 FUN_0800C2B8, 0x0800C2D8 FUN_0800C2D8
 * Callees   : 1  -> 0x08005354 FUN_08005354
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005A18(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005A18: 00b5     push     {lr} */
    push({lr});
    /* 0x08005A1A: 2821     movs     r1, #0x28 */
    r1 = 0x28;
    /* 0x08005A1C: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08005A1E: fff799fc bl       #0x8005354 */
    FUN_08005354();   /* BL -> 0x08005354 FUN_08005354 */
    /* 0x08005A22: 00bd     pop      {pc} */
    pop({pc});   /* return */
}
