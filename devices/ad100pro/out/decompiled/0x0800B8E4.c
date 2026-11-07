/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B8E4
 * Name      : SysTick_Handler
 * End       : 0x0800B8F0
 * Size      : 12 bytes, 4 instructions decoded in body (functions.json insns=4, blocks=1)
 * Class     : vector_handler
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x080075BC FUN_080075BC, 0x08008194 FUN_08008194
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void SysTick_Handler(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B8E4: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800B8E6: fbf769fe bl       #0x80075bc */
    FUN_080075BC();   /* BL -> 0x080075BC FUN_080075BC */
    /* 0x0800B8EA: fcf753fc bl       #0x8008194 */
    FUN_08008194();   /* BL -> 0x08008194 FUN_08008194 */
    /* 0x0800B8EE: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
