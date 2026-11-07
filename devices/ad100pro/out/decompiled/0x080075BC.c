/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080075BC
 * Name      : FUN_080075BC
 * End       : 0x080075C6
 * Size      : 10 bytes, 5 instructions decoded in body (functions.json insns=5, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x0800B8E4 SysTick_Handler
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080075BC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080075BC: 0248     ldr      r0, [pc, #8]  ; =RAM_20000104 */
    r0 = *(uint32_t *)0x080075C8;   /* = RAM 0x20000104 */
    /* 0x080075BE: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);
    /* 0x080075C0: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x080075C2: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */
    /* 0x080075C4: 7047     bx       lr */
    return;
}
