/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005EBC
 * Name      : FUN_08005EBC
 * End       : 0x08005EEA
 * Size      : 46 bytes, 23 instructions decoded in body (functions.json insns=23, blocks=5)
 * Class     : function
 * Callers   : 1  -> 0x08005F34 FUN_08005F34
 * Callees   : 0  -> -
 * MMIO      : FLASH_IF.SR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005EBC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005EBC: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08005EBE: 0b4a     ldr      r2, [pc, #0x2c]  ; =FLASH_IF.ACR */
    r2 = *(uint32_t *)0x08005EEC;   /* = MMIO FLASH+0x00 */
    /* 0x08005EC0: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005EC2: d168     ldr      r1, [r2, #0xc] */
    r1 = *(uint32_t *)(r2 + 12);  /* MMIO: FLASH_IF.SR(r) */
    /* 0x08005EC4: cb06     lsls     r3, r1, #0x1b */
    r3 = r1 << 0x1b;
    /* 0x08005EC6: 0a49     ldr      r1, [pc, #0x28]  ; =RAM_20000240 */
    r1 = *(uint32_t *)0x08005EF0;   /* = RAM 0x20000240 */
    /* 0x08005EC8: 04d5     bpl      #0x8005ed4 */
    if (/* bpl */) goto L_08005ED4;   /* result non-negative */
    /* 0x08005ECA: c869     ldr      r0, [r1, #0x1c] */
    r0 = *(uint32_t *)(r1 + 28);
    /* 0x08005ECC: 0223     movs     r3, #2 */
    r3 = 2;
    /* 0x08005ECE: 1843     orrs     r0, r3 */
    r0 = r0 | r3;
    /* 0x08005ED0: c861     str      r0, [r1, #0x1c] */
    *(uint32_t *)(r1 + 28) = r0;   /* store */
    /* 0x08005ED2: 1020     movs     r0, #0x10 */
    r0 = 0x10;

    L_08005ED4:
    /* 0x08005ED4: d368     ldr      r3, [r2, #0xc] */
    r3 = *(uint32_t *)(r2 + 12);  /* MMIO: FLASH_IF.SR(r) */
    /* 0x08005ED6: 5b07     lsls     r3, r3, #0x1d */
    r3 = r3 << 0x1d;
    /* 0x08005ED8: 05d5     bpl      #0x8005ee6 */
    if (/* bpl */) goto L_08005EE6;   /* result non-negative */
    /* 0x08005EDA: cb69     ldr      r3, [r1, #0x1c] */
    r3 = *(uint32_t *)(r1 + 28);
    /* 0x08005EDC: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x08005EDE: 2343     orrs     r3, r4 */
    r3 = r3 | r4;
    /* 0x08005EE0: cb61     str      r3, [r1, #0x1c] */
    *(uint32_t *)(r1 + 28) = r3;   /* store */
    /* 0x08005EE2: 0421     movs     r1, #4 */
    r1 = 4;
    /* 0x08005EE4: 0843     orrs     r0, r1 */
    r0 = r0 | r1;

    L_08005EE6:
    /* 0x08005EE6: d060     str      r0, [r2, #0xc] */
    *(uint32_t *)(r2 + 12) = r0;   /* store */  /* MMIO: FLASH_IF.SR(w) */
    /* 0x08005EE8: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
