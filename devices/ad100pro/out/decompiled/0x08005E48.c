/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005E48
 * Name      : FUN_08005E48
 * End       : 0x08005E64
 * Size      : 28 bytes, 14 instructions decoded in body (functions.json insns=14, blocks=1)
 * Class     : function
 * Callers   : 2  -> 0x08007154 FUN_08007154, 0x08007192 FUN_08007192
 * Callees   : 0  -> -
 * MMIO      : FLASH_IF.AR, FLASH_IF.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005E48(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005E48: 064a     ldr      r2, [pc, #0x18]  ; =RAM_20000240 */
    r2 = *(uint32_t *)0x08005E64;   /* = RAM 0x20000240 */
    /* 0x08005E4A: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x08005E4C: d161     str      r1, [r2, #0x1c] */
    *(uint32_t *)(r2 + 28) = r1;   /* store */
    /* 0x08005E4E: 0649     ldr      r1, [pc, #0x18]  ; =FLASH_IF.ACR */
    r1 = *(uint32_t *)0x08005E68;   /* = MMIO FLASH+0x00 */
    /* 0x08005E50: 0a69     ldr      r2, [r1, #0x10] */
    r2 = *(uint32_t *)(r1 + 16);  /* MMIO: FLASH_IF.CR(r) */
    /* 0x08005E52: 0223     movs     r3, #2 */
    r3 = 2;
    /* 0x08005E54: 1a43     orrs     r2, r3 */
    r2 = r2 | r3;
    /* 0x08005E56: 0a61     str      r2, [r1, #0x10] */
    *(uint32_t *)(r1 + 16) = r2;   /* store */  /* MMIO: FLASH_IF.CR(w) */
    /* 0x08005E58: 4861     str      r0, [r1, #0x14] */
    *(uint32_t *)(r1 + 20) = r0;   /* store */  /* MMIO: FLASH_IF.AR(w) */
    /* 0x08005E5A: 0869     ldr      r0, [r1, #0x10] */
    r0 = *(uint32_t *)(r1 + 16);  /* MMIO: FLASH_IF.CR(r) */
    /* 0x08005E5C: 4022     movs     r2, #0x40 */
    r2 = 0x40;
    /* 0x08005E5E: 1043     orrs     r0, r2 */
    r0 = r0 | r2;
    /* 0x08005E60: 0861     str      r0, [r1, #0x10] */
    *(uint32_t *)(r1 + 16) = r0;   /* store */  /* MMIO: FLASH_IF.CR(w) */
    /* 0x08005E62: 7047     bx       lr */
    return;
}
