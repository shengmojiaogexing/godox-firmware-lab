/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007D58
 * Name      : FUN_08007D58
 * End       : 0x08007E2C
 * Size      : 212 bytes, 102 instructions decoded in body (functions.json insns=102, blocks=11)
 * Class     : function
 * Callers   : 2  -> 0x080095F8 FUN_080095F8, 0x0800963C FUN_0800963C
 * Callees   : 1  -> 0x08007434 delay_or_timing
 * MMIO      : RCC.+0x1D, RCC.APB1ENR, RCC.CR, RCC.CSR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007D58(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007D58: f7b5     push     {r0, r1, r2, r4, r5, r6, r7, lr} */
    push({r0, r1, r2, r4, r5, r6, r7, lr});
    /* 0x08007D5A: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x08007D5C: 007f     ldrb     r0, [r0, #0x1c] */
    r0 = *(uint8_t *)(r0 + 28);
    /* 0x08007D5E: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08007D60: 14d0     beq      #0x8007d8c */
    if (r0 == 1) goto L_08007D8C;
    /* 0x08007D62: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08007D64: 2077     strb     r0, [r4, #0x1c] */
    *(uint8_t *)(r4 + 28) = r0;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x08007D66: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08007D68: 6077     strb     r0, [r4, #0x1d] */
    *(uint8_t *)(r4 + 29) = r0;   /* store */  /* MMIO: RCC.+0x1D(w) */
    /* 0x08007D6A: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007D6C: ca20     movs     r0, #0xca */
    r0 = 0xca;
    /* 0x08007D6E: 4862     str      r0, [r1, #0x24] */
    *(uint32_t *)(r1 + 36) = r0;   /* store */  /* MMIO: RCC.CSR(w) */
    /* 0x08007D70: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007D72: 5320     movs     r0, #0x53 */
    r0 = 0x53;
    /* 0x08007D74: 4862     str      r0, [r1, #0x24] */
    *(uint32_t *)(r1 + 36) = r0;   /* store */  /* MMIO: RCC.CSR(w) */
    /* 0x08007D76: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007D78: 8068     ldr      r0, [r0, #8] */
    r0 = *(uint32_t *)(r0 + 8);
    /* 0x08007D7A: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x08007D7C: 4005     lsls     r0, r0, #0x15 */
    r0 = r0 << 0x15;
    /* 0x08007D7E: ff26     movs     r6, #0xff */
    r6 = 0xff;
    /* 0x08007D80: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08007D82: 10da     bge      #0x8007da6 */
    if (r0 >= 0) goto L_08007DA6;   /* signed */
    /* 0x08007D84: fff756fb bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007D88: 0746     mov      r7, r0 */
    r7 = r0;
    /* 0x08007D8A: 08e0     b        #0x8007d9e */
    goto L_08007D9E;

    L_08007D8C:
    /* 0x08007D8C: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08007D8E: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */

    L_08007D90:
    /* 0x08007D90: fff750fb bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007D94: c11b     subs     r1, r0, r7 */
    r1 = r0 - r7;
    /* 0x08007D96: 7d20     movs     r0, #0x7d */
    r0 = 0x7d;
    /* 0x08007D98: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x08007D9A: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x08007D9C: 14d8     bhi      #0x8007dc8 */
    if (r1 > r0) goto L_08007DC8;   /* unsigned */

    L_08007D9E:
    /* 0x08007D9E: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007DA0: c068     ldr      r0, [r0, #0xc] */
    r0 = *(uint32_t *)(r0 + 12);
    /* 0x08007DA2: 4007     lsls     r0, r0, #0x1d */
    r0 = r0 << 0x1d;
    /* 0x08007DA4: f4d4     bmi      #0x8007d90 */
    if (r1 < r0) goto L_08007D90;   /* result negative */

    L_08007DA6:
    /* 0x08007DA6: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007DA8: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);
    /* 0x08007DAA: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08007DAC: 9202     lsls     r2, r2, #0xa */
    r2 = r2 << 0xa;
    /* 0x08007DAE: 9143     bics     r1, r2 */
    r1 = r1 & ~r2;
    /* 0x08007DB0: 8160     str      r1, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r1;   /* store */
    /* 0x08007DB2: fff73ffb bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007DB6: 0746     mov      r7, r0 */
    r7 = r0;
    /* 0x08007DB8: 0ce0     b        #0x8007dd4 */
    goto L_08007DD4;

    L_08007DBA:
    /* 0x08007DBA: fff73bfb bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007DBE: c11b     subs     r1, r0, r7 */
    r1 = r0 - r7;
    /* 0x08007DC0: 7d20     movs     r0, #0x7d */
    r0 = 0x7d;
    /* 0x08007DC2: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x08007DC4: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x08007DC6: 05d9     bls      #0x8007dd4 */
    if (r1 <= r0) goto L_08007DD4;   /* unsigned */

    L_08007DC8:
    /* 0x08007DC8: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007DCA: 4662     str      r6, [r0, #0x24] */
    *(uint32_t *)(r0 + 36) = r6;   /* store */
    /* 0x08007DCC: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x08007DCE: 6077     strb     r0, [r4, #0x1d] */
    *(uint8_t *)(r4 + 29) = r0;   /* store */  /* MMIO: RCC.+0x1D(w) */
    /* 0x08007DD0: 2577     strb     r5, [r4, #0x1c] */
    *(uint8_t *)(r4 + 28) = r5;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x08007DD2: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */

    L_08007DD4:
    /* 0x08007DD4: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007DD6: c068     ldr      r0, [r0, #0xc] */
    r0 = *(uint32_t *)(r0 + 12);
    /* 0x08007DD8: 4007     lsls     r0, r0, #0x1d */
    r0 = r0 << 0x1d;
    /* 0x08007DDA: eed5     bpl      #0x8007dba */
    if (r1 >= r0) goto L_08007DBA;   /* result non-negative */
    /* 0x08007DDC: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007DDE: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x08007DE0: 4861     str      r0, [r1, #0x14] */
    *(uint32_t *)(r1 + 20) = r0;   /* store */  /* MMIO: RCC.AHBENR(w) */
    /* 0x08007DE2: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007DE4: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);
    /* 0x08007DE6: c908     lsrs     r1, r1, #3 */
    r1 = r1 >> 3;
    /* 0x08007DE8: c900     lsls     r1, r1, #3 */
    r1 = r1 << 3;
    /* 0x08007DEA: 8160     str      r1, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r1;   /* store */
    /* 0x08007DEC: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007DEE: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);
    /* 0x08007DF0: 029a     ldr      r2, [sp, #8] */
    r2 = *(uint32_t *)(sp + 8);
    /* 0x08007DF2: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x08007DF4: 8160     str      r1, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r1;   /* store */
    /* 0x08007DF6: 0d48     ldr      r0, [pc, #0x34]  ; =EXTI.IMR */
    r0 = *(uint32_t *)0x08007E2C;   /* = MMIO EXTI+0x00 */
    /* 0x08007DF8: 0268     ldr      r2, [r0] */
    r2 = *(uint32_t *)(r0);  /* MMIO: EXTI.IMR(r) */
    /* 0x08007DFA: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08007DFC: 0905     lsls     r1, r1, #0x14 */
    r1 = r1 << 0x14;
    /* 0x08007DFE: 0a43     orrs     r2, r1 */
    r2 = r2 | r1;
    /* 0x08007E00: 0260     str      r2, [r0] */
    *(uint32_t *)(r0) = r2;   /* store */  /* MMIO: EXTI.IMR(w) */
    /* 0x08007E02: 8268     ldr      r2, [r0, #8] */
    r2 = *(uint32_t *)(r0 + 8);  /* MMIO: EXTI.RTSR(r) */
    /* 0x08007E04: 0a43     orrs     r2, r1 */
    r2 = r2 | r1;
    /* 0x08007E06: 8260     str      r2, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r2;   /* store */  /* MMIO: EXTI.RTSR(w) */
    /* 0x08007E08: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007E0A: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);  /* MMIO: EXTI.RTSR(r) */
    /* 0x08007E0C: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08007E0E: 9203     lsls     r2, r2, #0xe */
    r2 = r2 << 0xe;
    /* 0x08007E10: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x08007E12: 8160     str      r1, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r1;   /* store */  /* MMIO: EXTI.RTSR(w) */
    /* 0x08007E14: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007E16: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);  /* MMIO: EXTI.RTSR(r) */
    /* 0x08007E18: 1211     asrs     r2, r2, #4 */
    r2 = r2 >> 4;
    /* 0x08007E1A: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x08007E1C: 8160     str      r1, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r1;   /* store */  /* MMIO: EXTI.RTSR(w) */
    /* 0x08007E1E: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007E20: 4662     str      r6, [r0, #0x24] */
    *(uint32_t *)(r0 + 36) = r6;   /* store */  /* MMIO: EXTI.+0x24(w) */
    /* 0x08007E22: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08007E24: 6077     strb     r0, [r4, #0x1d] */
    *(uint8_t *)(r4 + 29) = r0;   /* store */  /* MMIO: RCC.+0x1D(w) */
    /* 0x08007E26: 2577     strb     r5, [r4, #0x1c] */
    *(uint8_t *)(r4 + 28) = r5;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x08007E28: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08007E2A: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */
}
