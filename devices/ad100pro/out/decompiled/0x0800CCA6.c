/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CCA6
 * Name      : FUN_0800CCA6
 * End       : 0x0800CCBA
 * Size      : 20 bytes, 9 instructions decoded in body (functions.json insns=9, blocks=1)
 * Class     : function
 * Callers   : 3  -> 0x08004AF0 FUN_08004AF0, 0x08004B08 FUN_08004B08, 0x08004B0E FUN_08004B0E
 * Callees   : 1  -> 0x0800CDC8 FUN_0800CDC8
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CCA6(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CCA6: 08b5     push     {r3, lr} */
    push({r3, lr});
    /* 0x0800CCA8: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800CCAA: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x0800CCAC: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800CCAE: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x0800CCB0: 00f08af8 bl       #0x800cdc8 */
    FUN_0800CDC8();   /* BL -> 0x0800CDC8 FUN_0800CDC8 */
    /* 0x0800CCB4: 6846     mov      r0, sp */
    r0 = sp;
    /* 0x0800CCB6: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800CCB8: 08bd     pop      {r3, pc} */
    pop({r3, pc});   /* return */
}
