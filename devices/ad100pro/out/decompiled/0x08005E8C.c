/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005E8C
 * Name      : FUN_08005E8C
 * End       : 0x08005EA6
 * Size      : 26 bytes, 13 instructions decoded in body (functions.json insns=13, blocks=3)
 * Class     : function
 * Callers   : 1  -> 0x0800AD44 FUN_0800AD44
 * Callees   : 0  -> -
 * MMIO      : FLASH_IF.ACR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005E8C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005E8C: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08005E8E: 0649     ldr      r1, [pc, #0x18]  ; =RAM_20000120 */
    r1 = *(uint32_t *)0x08005EA8;   /* = RAM 0x20000120 */
    /* 0x08005E90: 064b     ldr      r3, [pc, #0x18]  ; =RAM_20000434 */
    r3 = *(uint32_t *)0x08005EAC;   /* = RAM 0x20000434 */
    /* 0x08005E92: 8860     str      r0, [r1, #8] */
    *(uint32_t *)(r1 + 8) = r0;   /* store */
    /* 0x08005E94: 0021     movs     r1, #0 */
    r1 = 0;

    L_08005E96:
    /* 0x08005E96: 8a00     lsls     r2, r1, #2 */
    r2 = r1 << 2;
    /* 0x08005E98: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x08005E9A: 8458     ldr      r4, [r0, r2] */
    r4 = *(uint32_t *)(r0 + r2);  /* MMIO: FLASH_IF.ACR(r) */
    /* 0x08005E9C: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x08005E9E: 9c50     str      r4, [r3, r2] */
    *(uint32_t *)(r3 + r2) = r4;   /* store */
    /* 0x08005EA0: 0829     cmp      r1, #8 */
    /* flags = cmp(r1, #8) */
    /* 0x08005EA2: f8d3     blo      #0x8005e96 */
    if (r1 < 8) goto L_08005E96;   /* unsigned */
    /* 0x08005EA4: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
