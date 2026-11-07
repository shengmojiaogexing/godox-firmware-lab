/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080071F4
 * Name      : FUN_080071F4
 * End       : 0x08007202
 * Size      : 14 bytes, 7 instructions decoded in body (functions.json insns=7, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x08005EF4 FUN_08005EF4
 * Callees   : 0  -> -
 * MMIO      : FLASH_IF.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080071F4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080071F4: 0348     ldr      r0, [pc, #0xc]  ; =FLASH_IF.ACR */
    r0 = *(uint32_t *)0x08007204;   /* = MMIO FLASH+0x00 */
    /* 0x080071F6: 0169     ldr      r1, [r0, #0x10] */
    r1 = *(uint32_t *)(r0 + 16);  /* MMIO: FLASH_IF.CR(r) */
    /* 0x080071F8: 8022     movs     r2, #0x80 */
    r2 = 0x80;
    /* 0x080071FA: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x080071FC: 0161     str      r1, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r1;   /* store */  /* MMIO: FLASH_IF.CR(w) */
    /* 0x080071FE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08007200: 7047     bx       lr */
    return;
}
