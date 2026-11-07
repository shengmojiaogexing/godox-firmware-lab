/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005E6C
 * Name      : FUN_08005E6C
 * End       : 0x08005E82
 * Size      : 22 bytes, 11 instructions decoded in body (functions.json insns=11, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x08007208 FUN_08007208
 * Callees   : 0  -> -
 * MMIO      : FLASH_IF.ACR, FLASH_IF.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005E6C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005E6C: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08005E6E: 054b     ldr      r3, [pc, #0x14]  ; =RAM_20000240 */
    r3 = *(uint32_t *)0x08005E84;   /* = RAM 0x20000240 */
    /* 0x08005E70: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08005E72: da61     str      r2, [r3, #0x1c] */
    *(uint32_t *)(r3 + 28) = r2;   /* store */
    /* 0x08005E74: 044a     ldr      r2, [pc, #0x10]  ; =FLASH_IF.ACR */
    r2 = *(uint32_t *)0x08005E88;   /* = MMIO FLASH+0x00 */
    /* 0x08005E76: 1369     ldr      r3, [r2, #0x10] */
    r3 = *(uint32_t *)(r2 + 16);  /* MMIO: FLASH_IF.CR(r) */
    /* 0x08005E78: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x08005E7A: 2343     orrs     r3, r4 */
    r3 = r3 | r4;
    /* 0x08005E7C: 1361     str      r3, [r2, #0x10] */
    *(uint32_t *)(r2 + 16) = r3;   /* store */  /* MMIO: FLASH_IF.CR(w) */
    /* 0x08005E7E: 0180     strh     r1, [r0] */
    *(uint16_t *)(r0) = r1;   /* store */  /* MMIO: FLASH_IF.ACR(w) */
    /* 0x08005E80: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
