/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005464
 * Name      : FUN_08005464
 * End       : 0x08005470
 * Size      : 12 bytes, 5 instructions decoded in body (functions.json insns=5, blocks=1)
 * Class     : function
 * Callers   : 2  -> 0x0800C2B8 FUN_0800C2B8, 0x0800C358 FUN_0800C358
 * Callees   : 1  -> 0x080058D0 FUN_080058D0
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005464(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005464: 00b5     push     {lr} */
    push({lr});
    /* 0x08005466: 0248     ldr      r0, [pc, #8]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x08005470;   /* = RAM 0x20000175 */
    /* 0x08005468: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800546A: 00f031fa bl       #0x80058d0 */
    FUN_080058D0();   /* BL -> 0x080058D0 FUN_080058D0 */
    /* 0x0800546E: 00bd     pop      {pc} */
    pop({pc});   /* return */
}
