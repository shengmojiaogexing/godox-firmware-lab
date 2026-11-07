/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080030D8
 * Name      : Reset_Handler_call_start
 * End       : 0x080030DC
 * Size      : 4 bytes, 2 instructions decoded in body (functions.json insns=2, blocks=1)
 * Class     : tiny_fragment
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void Reset_Handler_call_start(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080030D8: 0448     ldr      r0, [pc, #0x10]  ; =func_080030C0 */
    r0 = *(uint32_t *)0x080030EC;   /* = flash data/const 0x080030C1 */
    /* 0x080030DA: 0047     bx       r0 */
    goto *r0;   /* tail branch  */
}
