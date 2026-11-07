/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800AA54
 * Name      : FUN_0800AA54
 * End       : 0x0800AA6E
 * Size      : 26 bytes, 12 instructions decoded in body (functions.json insns=12, blocks=3)
 * Class     : function
 * Callers   : 2  -> 0x0800ABE4 FUN_0800ABE4, 0x0800AC58 FUN_0800AC58
 * Callees   : 1  -> 0x0800C204 FUN_0800C204
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800AA54(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800AA54: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800AA56: a924     movs     r4, #0xa9 */
    r4 = 0xa9;
    /* 0x0800AA58: 054b     ldr      r3, [pc, #0x14]  ; =RAM_20000214 */
    r3 = *(uint32_t *)0x0800AA70;   /* = RAM 0x20000214 */
    /* 0x0800AA5A: 5028     cmp      r0, #0x50 */
    /* flags = cmp(r0, #0x50) */
    /* 0x0800AA5C: 1c70     strb     r4, [r3] */
    *(uint8_t *)(r3) = r4;   /* store */
    /* 0x0800AA5E: 00d0     beq      #0x800aa62 */
    if (r0 == 0x50) goto L_0800AA62;
    /* 0x0800AA60: 0930     adds     r0, #9 */
    r0 = r0 + 9;

    L_0800AA62:
    /* 0x0800AA62: 5870     strb     r0, [r3, #1] */
    *(uint8_t *)(r3 + 1) = r0;   /* store */
    /* 0x0800AA64: 9970     strb     r1, [r3, #2] */
    *(uint8_t *)(r3 + 2) = r1;   /* store */
    /* 0x0800AA66: da70     strb     r2, [r3, #3] */
    *(uint8_t *)(r3 + 3) = r2;   /* store */
    /* 0x0800AA68: 01f0ccfb bl       #0x800c204 */
    FUN_0800C204();   /* BL -> 0x0800C204 FUN_0800C204 */
    /* 0x0800AA6C: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
