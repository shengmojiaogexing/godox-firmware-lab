/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008734
 * Name      : HardFault_Handler
 * End       : 0x08008736
 * Size      : 2 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : vector_handler
 * Callers   : 1  -> 0x08008734 HardFault_Handler
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void HardFault_Handler(void)   /* args in r0-r3, return in r0 unless noted */
{

    L_08008734:
    /* 0x08008734: fee7     b        #0x8008734 */
    goto L_08008734;   /* loop */
}
