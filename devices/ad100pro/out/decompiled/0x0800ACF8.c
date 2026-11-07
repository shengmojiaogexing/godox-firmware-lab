/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800ACF8
 * Name      : IRQ2_Handler
 * End       : 0x0800AD08
 * Size      : 16 bytes, 7 instructions decoded in body (functions.json insns=7, blocks=1)
 * Class     : vector_handler
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x08007E34 FUN_08007E34
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void IRQ2_Handler(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800ACF8: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800ACFA: 0349     ldr      r1, [pc, #0xc]  ; =RAM_2000021A */
    r1 = *(uint32_t *)0x0800AD08;   /* = RAM 0x2000021A */
    /* 0x0800ACFC: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800ACFE: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x0800AD00: 0248     ldr      r0, [pc, #8]  ; =RAM_20000274 */
    r0 = *(uint32_t *)0x0800AD0C;   /* = RAM 0x20000274 */
    /* 0x0800AD02: fdf797f8 bl       #0x8007e34 */
    FUN_08007E34();   /* BL -> 0x08007E34 FUN_08007E34 */
    /* 0x0800AD06: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
