/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080030D4
 * Name      : Reset_Handler
 * End       : 0x080030D8
 * Size      : 4 bytes, 2 instructions decoded in body (functions.json insns=2, blocks=1)
 * Class     : vector_handler
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void Reset_Handler(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080030D4: 0448     ldr      r0, [pc, #0x10]  ; =func_0800B964 */
    r0 = *(uint32_t *)0x080030E8;   /* = flash data/const 0x0800B965 */
    /* 0x080030D6: 8047     blx      r0 */
    ((void(*)())r0)();   /* indirect call */
}
