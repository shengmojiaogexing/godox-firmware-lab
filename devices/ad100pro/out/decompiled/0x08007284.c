/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007284
 * Name      : FUN_08007284
 * End       : 0x0800729C
 * Size      : 24 bytes, 12 instructions decoded in body (functions.json insns=12, blocks=3)
 * Class     : function
 * Callers   : 2  -> 0x08005DFC FUN_08005DFC, 0x08005EF4 FUN_08005EF4
 * Callees   : 0  -> -
 * MMIO      : FLASH_IF.CR, FLASH_IF.KEYR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007284(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007284: 0548     ldr      r0, [pc, #0x14]  ; =FLASH_IF.ACR */
    r0 = *(uint32_t *)0x0800729C;   /* = MMIO FLASH+0x00 */
    /* 0x08007286: 0169     ldr      r1, [r0, #0x10] */
    r1 = *(uint32_t *)(r0 + 16);  /* MMIO: FLASH_IF.CR(r) */
    /* 0x08007288: 0906     lsls     r1, r1, #0x18 */
    r1 = r1 << 0x18;
    /* 0x0800728A: 05d5     bpl      #0x8007298 */
    if (/* bpl */) goto L_08007298;   /* result non-negative */
    /* 0x0800728C: 0449     ldr      r1, [pc, #0x10]  ; =0x45670123 */
    r1 = *(uint32_t *)0x080072A0;   /* = 0x45670123 */
    /* 0x0800728E: 4160     str      r1, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r1;   /* store */  /* MMIO: FLASH_IF.KEYR(w) */
    /* 0x08007290: 0449     ldr      r1, [pc, #0x10]  ; =0xCDEF89AB */
    r1 = *(uint32_t *)0x080072A4;   /* = 0xCDEF89AB */
    /* 0x08007292: 4160     str      r1, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r1;   /* store */  /* MMIO: FLASH_IF.KEYR(w) */
    /* 0x08007294: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08007296: 7047     bx       lr */
    return;

    L_08007298:
    /* 0x08007298: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800729A: 7047     bx       lr */
    return;
}
