/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CC98
 * Name      : FUN_0800CC98
 * End       : 0x0800CCA6
 * Size      : 14 bytes, 6 instructions decoded in body (functions.json insns=6, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x0800AA74 FUN_0800AA74
 * Callees   : 1  -> 0x0800CDC8 FUN_0800CDC8
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CC98(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CC98: 0a46     mov      r2, r1 */
    r2 = r1;
    /* 0x0800CC9A: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800CC9C: 0146     mov      r1, r0 */
    r1 = r0;
    /* 0x0800CC9E: ff20     movs     r0, #0xff */
    r0 = 0xff;
    /* 0x0800CCA0: 00f092f8 bl       #0x800cdc8 */
    FUN_0800CDC8();   /* BL -> 0x0800CDC8 FUN_0800CDC8 */
    /* 0x0800CCA4: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
