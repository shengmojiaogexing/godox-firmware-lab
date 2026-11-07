/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006DDE
 * Name      : FUN_08006DDE
 * End       : 0x08006E2A
 * Size      : 76 bytes, 36 instructions decoded in body (functions.json insns=36, blocks=5)
 * Class     : function
 * Callers   : 1  -> 0x08006C44 FUN_08006C44
 * Callees   : 1  -> 0x08007434 delay_or_timing
 * MMIO      : RCC.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006DDE(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006DDE: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x08006DE0: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x08006DE2: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x08006DE4: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x08006DE6: 4908     lsrs     r1, r1, #1 */
    r1 = r1 >> 1;
    /* 0x08006DE8: 4900     lsls     r1, r1, #1 */
    r1 = r1 << 1;
    /* 0x08006DEA: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: RCC.CR(w) */
    /* 0x08006DEC: 00f022fb bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08006DF0: 0027     movs     r7, #0 */
    r7 = 0;
    /* 0x08006DF2: 2546     mov      r5, r4 */
    r5 = r4;
    /* 0x08006DF4: 0646     mov      r6, r0 */
    r6 = r0;
    /* 0x08006DF6: 2035     adds     r5, #0x20 */
    r5 = r5 + 0x20;
    /* 0x08006DF8: 0ee0     b        #0x8006e18 */
    goto L_08006E18;

    L_08006DFA:
    /* 0x08006DFA: 00f01bfb bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08006DFE: 811b     subs     r1, r0, r6 */
    r1 = r0 - r6;
    /* 0x08006E00: 7d20     movs     r0, #0x7d */
    r0 = 0x7d;
    /* 0x08006E02: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x08006E04: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x08006E06: 07d9     bls      #0x8006e18 */
    if (r1 <= r0) goto L_08006E18;   /* unsigned */
    /* 0x08006E08: a06b     ldr      r0, [r4, #0x38] */
    r0 = *(uint32_t *)(r4 + 56);
    /* 0x08006E0A: 2021     movs     r1, #0x20 */
    r1 = 0x20;
    /* 0x08006E0C: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08006E0E: a063     str      r0, [r4, #0x38] */
    *(uint32_t *)(r4 + 56) = r0;   /* store */
    /* 0x08006E10: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x08006E12: 6870     strb     r0, [r5, #1] */
    *(uint8_t *)(r5 + 1) = r0;   /* store */
    /* 0x08006E14: 2f70     strb     r7, [r5] */
    *(uint8_t *)(r5) = r7;   /* store */
    /* 0x08006E16: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_08006E18:
    /* 0x08006E18: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x08006E1A: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x08006E1C: c007     lsls     r0, r0, #0x1f */
    r0 = r0 << 0x1f;
    /* 0x08006E1E: ecd1     bne      #0x8006dfa */
    if (r1 != r0) goto L_08006DFA;
    /* 0x08006E20: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08006E22: 6870     strb     r0, [r5, #1] */
    *(uint8_t *)(r5 + 1) = r0;   /* store */
    /* 0x08006E24: 2f70     strb     r7, [r5] */
    *(uint8_t *)(r5) = r7;   /* store */
    /* 0x08006E26: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006E28: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
