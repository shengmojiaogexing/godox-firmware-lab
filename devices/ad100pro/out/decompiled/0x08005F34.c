/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005F34
 * Name      : FUN_08005F34
 * End       : 0x08005F80
 * Size      : 76 bytes, 35 instructions decoded in body (functions.json insns=35, blocks=12)
 * Class     : function
 * Callers   : 5  -> 0x08007154 FUN_08007154, 0x0800718C FUN_0800718C, 0x080071B0 FUN_080071B0, 0x08007208 FUN_08007208, 0x08007250 FUN_08007250
 * Callees   : 2  -> 0x08005EBC FUN_08005EBC, 0x08007434 delay_or_timing
 * MMIO      : FLASH_IF.SR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005F34(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005F34: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08005F36: 0546     mov      r5, r0 */
    r5 = r0;
    /* 0x08005F38: 01f07cfa bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08005F3C: 0646     mov      r6, r0 */
    r6 = r0;
    /* 0x08005F3E: 104c     ldr      r4, [pc, #0x40]  ; =FLASH_IF.ACR */
    r4 = *(uint32_t *)0x08005F80;   /* = MMIO FLASH+0x00 */
    /* 0x08005F40: 0ae0     b        #0x8005f58 */
    goto L_08005F58;

    L_08005F42:
    /* 0x08005F42: 681c     adds     r0, r5, #1 */
    r0 = r5 + 1;
    /* 0x08005F44: 08d0     beq      #0x8005f58 */
    if (/* beq */) goto L_08005F58;
    /* 0x08005F46: 002d     cmp      r5, #0 */
    /* flags = cmp(r5, #0) */
    /* 0x08005F48: 04d0     beq      #0x8005f54 */
    if (r5 == 0) goto L_08005F54;
    /* 0x08005F4A: 01f073fa bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08005F4E: 801b     subs     r0, r0, r6 */
    r0 = r0 - r6;
    /* 0x08005F50: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x08005F52: 01d9     bls      #0x8005f58 */
    if (r0 <= r5) goto L_08005F58;   /* unsigned */

    L_08005F54:
    /* 0x08005F54: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x08005F56: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08005F58:
    /* 0x08005F58: e068     ldr      r0, [r4, #0xc] */
    r0 = *(uint32_t *)(r4 + 12);  /* MMIO: FLASH_IF.SR(r) */
    /* 0x08005F5A: c007     lsls     r0, r0, #0x1f */
    r0 = r0 << 0x1f;
    /* 0x08005F5C: f1d1     bne      #0x8005f42 */
    if (r0 != r5) goto L_08005F42;
    /* 0x08005F5E: e068     ldr      r0, [r4, #0xc] */
    r0 = *(uint32_t *)(r4 + 12);  /* MMIO: FLASH_IF.SR(r) */
    /* 0x08005F60: 8006     lsls     r0, r0, #0x1a */
    r0 = r0 << 0x1a;
    /* 0x08005F62: 01d5     bpl      #0x8005f68 */
    if (r0 >= r5) goto L_08005F68;   /* result non-negative */
    /* 0x08005F64: 2020     movs     r0, #0x20 */
    r0 = 0x20;
    /* 0x08005F66: e060     str      r0, [r4, #0xc] */
    *(uint32_t *)(r4 + 12) = r0;   /* store */  /* MMIO: FLASH_IF.SR(w) */

    L_08005F68:
    /* 0x08005F68: e068     ldr      r0, [r4, #0xc] */
    r0 = *(uint32_t *)(r4 + 12);  /* MMIO: FLASH_IF.SR(r) */
    /* 0x08005F6A: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x08005F6C: 02d4     bmi      #0x8005f74 */
    if (r0 < r5) goto L_08005F74;   /* result negative */
    /* 0x08005F6E: e068     ldr      r0, [r4, #0xc] */
    r0 = *(uint32_t *)(r4 + 12);  /* MMIO: FLASH_IF.SR(r) */
    /* 0x08005F70: 4007     lsls     r0, r0, #0x1d */
    r0 = r0 << 0x1d;
    /* 0x08005F72: 03d5     bpl      #0x8005f7c */
    if (r0 >= r5) goto L_08005F7C;   /* result non-negative */

    L_08005F74:
    /* 0x08005F74: fff7a2ff bl       #0x8005ebc */
    FUN_08005EBC();   /* BL -> 0x08005EBC FUN_08005EBC */
    /* 0x08005F78: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08005F7A: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08005F7C:
    /* 0x08005F7C: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005F7E: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
