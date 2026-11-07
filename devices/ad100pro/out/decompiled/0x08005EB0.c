/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005EB0
 * Name      : FUN_08005EB0
 * End       : 0x08005EB8
 * Size      : 8 bytes, 4 instructions decoded in body (functions.json insns=4, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x0800AD10 FUN_0800AD10
 * Callees   : 0  -> -
 * MMIO      : FLASH_IF.ACR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005EB0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005EB0: 0149     ldr      r1, [pc, #4]  ; =RAM_20000120 */
    r1 = *(uint32_t *)0x08005EB8;   /* = RAM 0x20000120 */
    /* 0x08005EB2: 8860     str      r0, [r1, #8] */
    *(uint32_t *)(r1 + 8) = r0;   /* store */
    /* 0x08005EB4: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);  /* MMIO: FLASH_IF.ACR(r) */
    /* 0x08005EB6: 7047     bx       lr */
    return;
}
