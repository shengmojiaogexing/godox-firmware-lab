/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005E24
 * Name      : FUN_08005E24
 * End       : 0x08005E3E
 * Size      : 26 bytes, 13 instructions decoded in body (functions.json insns=13, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x08007154 FUN_08007154
 * Callees   : 0  -> -
 * MMIO      : FLASH_IF.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005E24(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005E24: 0649     ldr      r1, [pc, #0x18]  ; =RAM_20000240 */
    r1 = *(uint32_t *)0x08005E40;   /* = RAM 0x20000240 */
    /* 0x08005E26: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005E28: c861     str      r0, [r1, #0x1c] */
    *(uint32_t *)(r1 + 28) = r0;   /* store */
    /* 0x08005E2A: 0648     ldr      r0, [pc, #0x18]  ; =FLASH_IF.ACR */
    r0 = *(uint32_t *)0x08005E44;   /* = MMIO FLASH+0x00 */
    /* 0x08005E2C: 0169     ldr      r1, [r0, #0x10] */
    r1 = *(uint32_t *)(r0 + 16);  /* MMIO: FLASH_IF.CR(r) */
    /* 0x08005E2E: 0422     movs     r2, #4 */
    r2 = 4;
    /* 0x08005E30: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x08005E32: 0161     str      r1, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r1;   /* store */  /* MMIO: FLASH_IF.CR(w) */
    /* 0x08005E34: 0169     ldr      r1, [r0, #0x10] */
    r1 = *(uint32_t *)(r0 + 16);  /* MMIO: FLASH_IF.CR(r) */
    /* 0x08005E36: 4022     movs     r2, #0x40 */
    r2 = 0x40;
    /* 0x08005E38: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x08005E3A: 0161     str      r1, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r1;   /* store */  /* MMIO: FLASH_IF.CR(w) */
    /* 0x08005E3C: 7047     bx       lr */
    return;
}
