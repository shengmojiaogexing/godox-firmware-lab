/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008194
 * Name      : FUN_08008194
 * End       : 0x0800819C
 * Size      : 8 bytes, 3 instructions decoded in body (functions.json insns=3, blocks=1)
 * Class     : function
 * Callers   : 2  -> 0x0800B8E4 SysTick_Handler, 0x0800B8E8 FUN_0800B8E8
 * Callees   : 1  -> 0x08008160 FUN_08008160
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08008194(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08008194: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08008196: fff7e3ff bl       #0x8008160 */
    FUN_08008160();   /* BL -> 0x08008160 FUN_08008160 */
    /* 0x0800819A: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
