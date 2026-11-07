/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800BA7C
 * Name      : IRQ13_Handler
 * End       : 0x0800BA86
 * Size      : 10 bytes, 4 instructions decoded in body (functions.json insns=4, blocks=1)
 * Class     : vector_handler
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x080083E8 FUN_080083E8
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void IRQ13_Handler(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800BA7C: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800BA7E: 0248     ldr      r0, [pc, #8]  ; =RAM_20000294 */
    r0 = *(uint32_t *)0x0800BA88;   /* = RAM 0x20000294 */
    /* 0x0800BA80: fcf7b2fc bl       #0x80083e8 */
    FUN_080083E8();   /* BL -> 0x080083E8 FUN_080083E8 */
    /* 0x0800BA84: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
