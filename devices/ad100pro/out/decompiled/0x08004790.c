/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004790
 * Name      : IRQ9_Handler
 * End       : 0x0800479A
 * Size      : 10 bytes, 4 instructions decoded in body (functions.json insns=4, blocks=1)
 * Class     : vector_handler
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x08006E2C FUN_08006E2C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void IRQ9_Handler(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004790: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08004792: 0248     ldr      r0, [pc, #8]  ; =RAM_200003F8 */
    r0 = *(uint32_t *)0x0800479C;   /* = RAM 0x200003F8 */
    /* 0x08004794: 02f04afb bl       #0x8006e2c */
    FUN_08006E2C();   /* BL -> 0x08006E2C FUN_08006E2C */
    /* 0x08004798: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
