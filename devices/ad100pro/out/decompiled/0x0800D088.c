/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800D088
 * Name      : FUN_0800D088
 * End       : 0x0800D098
 * Size      : 16 bytes, 5 instructions decoded in body (functions.json insns=5, blocks=1)
 * Class     : function
 * Callers   : 3  -> 0x0800B9C0 FUN_0800B9C0, 0x0800B9F8 FUN_0800B9F8, 0x0800C230 FUN_0800C230
 * Callees   : 3  -> 0x0800B5C0 FUN_0800B5C0, 0x0800C954 FUN_0800C954, 0x0800D134 FUN_0800D134
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800D088(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800D088: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800D08A: 00f053f8 bl       #0x800d134 */
    FUN_0800D134();   /* BL -> 0x0800D134 FUN_0800D134 */
    /* 0x0800D08E: fff761fc bl       #0x800c954 */
    FUN_0800C954();   /* BL -> 0x0800C954 FUN_0800C954 */
    /* 0x0800D092: fef795fa bl       #0x800b5c0 */
    FUN_0800B5C0();   /* BL -> 0x0800B5C0 FUN_0800B5C0 */
    /* 0x0800D096: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
