/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004C5C
 * Name      : FUN_08004C5C
 * End       : 0x08004C6C
 * Size      : 16 bytes, 7 instructions decoded in body (functions.json insns=7, blocks=3)
 * Class     : function
 * Callers   : 2  -> 0x0800C2B8 FUN_0800C2B8, 0x0800C398 FUN_0800C398
 * Callees   : 1  -> 0x08005058 FUN_08005058
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004C5C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004C5C: 00b5     push     {lr} */
    push({lr});
    /* 0x08004C5E: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08004C60: 03d0     beq      #0x8004c6a */
    if (r0 == 0) goto L_08004C6A;
    /* 0x08004C62: 5f21     movs     r1, #0x5f */
    r1 = 0x5f;
    /* 0x08004C64: 0620     movs     r0, #6 */
    r0 = 6;
    /* 0x08004C66: 00f0f7f9 bl       #0x8005058 */
    FUN_08005058();   /* BL -> 0x08005058 FUN_08005058 */

    L_08004C6A:
    /* 0x08004C6A: 00bd     pop      {pc} */
    pop({pc});   /* return */
}
