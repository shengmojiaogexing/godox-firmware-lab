/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005B00
 * Name      : IRQ6_Handler
 * End       : 0x08005B0A
 * Size      : 10 bytes, 4 instructions decoded in body (functions.json insns=4, blocks=1)
 * Class     : vector_handler
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x080072AC FUN_080072AC
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void IRQ6_Handler(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005B00: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08005B02: 0820     movs     r0, #8 */
    r0 = 8;
    /* 0x08005B04: 01f0d2fb bl       #0x80072ac */
    FUN_080072AC();   /* BL -> 0x080072AC FUN_080072AC */
    /* 0x08005B08: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
