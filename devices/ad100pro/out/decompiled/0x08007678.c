/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007678
 * Name      : FUN_08007678
 * End       : 0x0800778A
 * Size      : 274 bytes, 133 instructions decoded in body (functions.json insns=133, blocks=27)
 * Class     : function
 * Callers   : 1  -> 0x0800B8F0 FUN_0800B8F0
 * Callees   : 1  -> 0x08007434 delay_or_timing
 * MMIO      : PWR.CR, RCC.APB1ENR, RCC.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007678(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007678: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x0800767A: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x0800767C: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x0800767E: 434d     ldr      r5, [pc, #0x10c]  ; =RCC.CR */
    r5 = *(uint32_t *)0x0800778C;   /* = MMIO RCC+0x00 */
    /* 0x08007680: c003     lsls     r0, r0, #0xf */
    r0 = r0 << 0xf;
    /* 0x08007682: 52d5     bpl      #0x800772a */
    if (/* bpl */) goto L_0800772A;   /* result non-negative */
    /* 0x08007684: e869     ldr      r0, [r5, #0x1c] */
    r0 = *(uint32_t *)(r5 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x08007686: 0026     movs     r6, #0 */
    r6 = 0;
    /* 0x08007688: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x0800768A: 07d4     bmi      #0x800769c */
    if (/* bmi */) goto L_0800769C;   /* result negative */
    /* 0x0800768C: e969     ldr      r1, [r5, #0x1c] */
    r1 = *(uint32_t *)(r5 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x0800768E: 2804     lsls     r0, r5, #0x10 */
    r0 = r5 << 0x10;
    /* 0x08007690: 0143     orrs     r1, r0 */
    r1 = r1 | r0;
    /* 0x08007692: e961     str      r1, [r5, #0x1c] */
    *(uint32_t *)(r5 + 28) = r1;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x08007694: e969     ldr      r1, [r5, #0x1c] */
    r1 = *(uint32_t *)(r5 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x08007696: 0140     ands     r1, r0 */
    r1 = r1 & r0;
    /* 0x08007698: 0126     movs     r6, #1 */
    r6 = 1;
    /* 0x0800769A: 0091     str      r1, [sp] */
    *(uint32_t *)(sp) = r1;   /* store */

    L_0800769C:
    /* 0x0800769C: 3c48     ldr      r0, [pc, #0xf0]  ; =PWR.CR */
    r0 = *(uint32_t *)0x08007790;   /* = MMIO PWR+0x00 */
    /* 0x0800769E: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: PWR.CR(r) */
    /* 0x080076A0: c905     lsls     r1, r1, #0x17 */
    r1 = r1 << 0x17;
    /* 0x080076A2: 10d4     bmi      #0x80076c6 */
    if (/* bmi */) goto L_080076C6;   /* result negative */
    /* 0x080076A4: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: PWR.CR(r) */
    /* 0x080076A6: 8215     asrs     r2, r0, #0x16 */
    r2 = r0 >> 0x16;
    /* 0x080076A8: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x080076AA: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: PWR.CR(w) */
    /* 0x080076AC: fff7c2fe bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x080076B0: 0746     mov      r7, r0 */
    r7 = r0;
    /* 0x080076B2: 04e0     b        #0x80076be */
    goto L_080076BE;

    L_080076B4:
    /* 0x080076B4: fff7befe bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x080076B8: c01b     subs     r0, r0, r7 */
    r0 = r0 - r7;
    /* 0x080076BA: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x080076BC: 24d8     bhi      #0x8007708 */
    if (r0 > 0x64) goto L_08007708;   /* unsigned */

    L_080076BE:
    /* 0x080076BE: 3448     ldr      r0, [pc, #0xd0]  ; =PWR.CR */
    r0 = *(uint32_t *)0x08007790;   /* = MMIO PWR+0x00 */
    /* 0x080076C0: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);  /* MMIO: PWR.CR(r) */
    /* 0x080076C2: c005     lsls     r0, r0, #0x17 */
    r0 = r0 << 0x17;
    /* 0x080076C4: f6d5     bpl      #0x80076b4 */
    if (r0 >= 0x64) goto L_080076B4;   /* result non-negative */

    L_080076C6:
    /* 0x080076C6: 286a     ldr      r0, [r5, #0x20] */
    r0 = *(uint32_t *)(r5 + 32);  /* MMIO: RCC.BDCR(r) */
    /* 0x080076C8: 0327     movs     r7, #3 */
    r7 = 3;
    /* 0x080076CA: 3f02     lsls     r7, r7, #8 */
    r7 = r7 << 8;
    /* 0x080076CC: 3840     ands     r0, r7 */
    r0 = r0 & r7;
    /* 0x080076CE: 20d0     beq      #0x8007712 */
    if (r0 == 0x64) goto L_08007712;
    /* 0x080076D0: 6168     ldr      r1, [r4, #4] */
    r1 = *(uint32_t *)(r4 + 4);  /* MMIO: EXTI.EMR(r) */
    /* 0x080076D2: 3940     ands     r1, r7 */
    r1 = r1 & r7;
    /* 0x080076D4: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x080076D6: 1cd0     beq      #0x8007712 */
    if (r1 == r0) goto L_08007712;
    /* 0x080076D8: 286a     ldr      r0, [r5, #0x20] */
    r0 = *(uint32_t *)(r5 + 32);  /* MMIO: RCC.BDCR(r) */
    /* 0x080076DA: 2a6a     ldr      r2, [r5, #0x20] */
    r2 = *(uint32_t *)(r5 + 32);  /* MMIO: RCC.BDCR(r) */
    /* 0x080076DC: b843     bics     r0, r7 */
    r0 = r0 & ~r7;
    /* 0x080076DE: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x080076E0: 0904     lsls     r1, r1, #0x10 */
    r1 = r1 << 0x10;
    /* 0x080076E2: 0a43     orrs     r2, r1 */
    r2 = r2 | r1;
    /* 0x080076E4: 2a62     str      r2, [r5, #0x20] */
    *(uint32_t *)(r5 + 32) = r2;   /* store */  /* MMIO: RCC.BDCR(w) */
    /* 0x080076E6: 2a6a     ldr      r2, [r5, #0x20] */
    r2 = *(uint32_t *)(r5 + 32);  /* MMIO: RCC.BDCR(r) */
    /* 0x080076E8: 8a43     bics     r2, r1 */
    r2 = r2 & ~r1;
    /* 0x080076EA: 2a62     str      r2, [r5, #0x20] */
    *(uint32_t *)(r5 + 32) = r2;   /* store */  /* MMIO: RCC.BDCR(w) */
    /* 0x080076EC: 2862     str      r0, [r5, #0x20] */
    *(uint32_t *)(r5 + 32) = r0;   /* store */  /* MMIO: RCC.BDCR(w) */
    /* 0x080076EE: c007     lsls     r0, r0, #0x1f */
    r0 = r0 << 0x1f;
    /* 0x080076F0: 0fd0     beq      #0x8007712 */
    if (r1 == r0) goto L_08007712;
    /* 0x080076F2: fff79ffe bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x080076F6: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x080076F8: 08e0     b        #0x800770c */
    goto L_0800770C;

    L_080076FA:
    /* 0x080076FA: fff79bfe bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x080076FE: 0099     ldr      r1, [sp] */
    r1 = *(uint32_t *)(sp);
    /* 0x08007700: 411a     subs     r1, r0, r1 */
    r1 = r0 - r1;
    /* 0x08007702: 2448     ldr      r0, [pc, #0x90]  ; =0x00001388 */
    r0 = *(uint32_t *)0x08007794;   /* = 0x00001388 */
    /* 0x08007704: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x08007706: 01d9     bls      #0x800770c */
    if (r1 <= r0) goto L_0800770C;   /* unsigned */

    L_08007708:
    /* 0x08007708: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x0800770A: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_0800770C:
    /* 0x0800770C: 286a     ldr      r0, [r5, #0x20] */
    r0 = *(uint32_t *)(r5 + 32);  /* MMIO: RCC.BDCR(r) */
    /* 0x0800770E: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08007710: f3d5     bpl      #0x80076fa */
    if (r1 >= r0) goto L_080076FA;   /* result non-negative */

    L_08007712:
    /* 0x08007712: 286a     ldr      r0, [r5, #0x20] */
    r0 = *(uint32_t *)(r5 + 32);  /* MMIO: RCC.BDCR(r) */
    /* 0x08007714: 6168     ldr      r1, [r4, #4] */
    r1 = *(uint32_t *)(r4 + 4);  /* MMIO: EXTI.EMR(r) */
    /* 0x08007716: b843     bics     r0, r7 */
    r0 = r0 & ~r7;
    /* 0x08007718: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x0800771A: 2862     str      r0, [r5, #0x20] */
    *(uint32_t *)(r5 + 32) = r0;   /* store */  /* MMIO: RCC.BDCR(w) */
    /* 0x0800771C: 002e     cmp      r6, #0 */
    /* flags = cmp(r6, #0) */
    /* 0x0800771E: 04d0     beq      #0x800772a */
    if (r6 == 0) goto L_0800772A;
    /* 0x08007720: e869     ldr      r0, [r5, #0x1c] */
    r0 = *(uint32_t *)(r5 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x08007722: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08007724: 0907     lsls     r1, r1, #0x1c */
    r1 = r1 << 0x1c;
    /* 0x08007726: 8843     bics     r0, r1 */
    r0 = r0 & ~r1;
    /* 0x08007728: e861     str      r0, [r5, #0x1c] */
    *(uint32_t *)(r5 + 28) = r0;   /* store */  /* MMIO: RCC.APB1ENR(w) */

    L_0800772A:
    /* 0x0800772A: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);  /* MMIO: EXTI.IMR(r) */
    /* 0x0800772C: c007     lsls     r0, r0, #0x1f */
    r0 = r0 << 0x1f;
    /* 0x0800772E: 05d0     beq      #0x800773c */
    if (r6 == 0) goto L_0800773C;
    /* 0x08007730: 286b     ldr      r0, [r5, #0x30] */
    r0 = *(uint32_t *)(r5 + 48);  /* MMIO: RCC.CFGR3(r) */
    /* 0x08007732: a168     ldr      r1, [r4, #8] */
    r1 = *(uint32_t *)(r4 + 8);  /* MMIO: EXTI.RTSR(r) */
    /* 0x08007734: 8008     lsrs     r0, r0, #2 */
    r0 = r0 >> 2;
    /* 0x08007736: 8000     lsls     r0, r0, #2 */
    r0 = r0 << 2;
    /* 0x08007738: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x0800773A: 2863     str      r0, [r5, #0x30] */
    *(uint32_t *)(r5 + 48) = r0;   /* store */  /* MMIO: RCC.CFGR3(w) */

    L_0800773C:
    /* 0x0800773C: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);  /* MMIO: EXTI.IMR(r) */
    /* 0x0800773E: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08007740: 06d5     bpl      #0x8007750 */
    if (r6 >= 0) goto L_08007750;   /* result non-negative */
    /* 0x08007742: 286b     ldr      r0, [r5, #0x30] */
    r0 = *(uint32_t *)(r5 + 48);  /* MMIO: RCC.CFGR3(r) */
    /* 0x08007744: 0321     movs     r1, #3 */
    r1 = 3;
    /* 0x08007746: 0904     lsls     r1, r1, #0x10 */
    r1 = r1 << 0x10;
    /* 0x08007748: 8843     bics     r0, r1 */
    r0 = r0 & ~r1;
    /* 0x0800774A: e168     ldr      r1, [r4, #0xc] */
    r1 = *(uint32_t *)(r4 + 12);  /* MMIO: EXTI.FTSR(r) */
    /* 0x0800774C: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x0800774E: 2863     str      r0, [r5, #0x30] */
    *(uint32_t *)(r5 + 48) = r0;   /* store */  /* MMIO: RCC.CFGR3(w) */

    L_08007750:
    /* 0x08007750: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);  /* MMIO: EXTI.IMR(r) */
    /* 0x08007752: 8006     lsls     r0, r0, #0x1a */
    r0 = r0 << 0x1a;
    /* 0x08007754: 05d5     bpl      #0x8007762 */
    if (r6 >= 0) goto L_08007762;   /* result non-negative */
    /* 0x08007756: 286b     ldr      r0, [r5, #0x30] */
    r0 = *(uint32_t *)(r5 + 48);  /* MMIO: RCC.CFGR3(r) */
    /* 0x08007758: 1021     movs     r1, #0x10 */
    r1 = 0x10;
    /* 0x0800775A: 8843     bics     r0, r1 */
    r0 = r0 & ~r1;
    /* 0x0800775C: 2169     ldr      r1, [r4, #0x10] */
    r1 = *(uint32_t *)(r4 + 16);  /* MMIO: EXTI.SWIER(r) */
    /* 0x0800775E: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08007760: 2863     str      r0, [r5, #0x30] */
    *(uint32_t *)(r5 + 48) = r0;   /* store */  /* MMIO: RCC.CFGR3(w) */

    L_08007762:
    /* 0x08007762: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: EXTI.IMR(r) */
    /* 0x08007764: 8003     lsls     r0, r0, #0xe */
    r0 = r0 << 0xe;
    /* 0x08007766: 05d5     bpl      #0x8007774 */
    if (r6 >= 0) goto L_08007774;   /* result non-negative */
    /* 0x08007768: 286b     ldr      r0, [r5, #0x30] */
    r0 = *(uint32_t *)(r5 + 48);  /* MMIO: RCC.CFGR3(r) */
    /* 0x0800776A: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800776C: 8843     bics     r0, r1 */
    r0 = r0 & ~r1;
    /* 0x0800776E: a169     ldr      r1, [r4, #0x18] */
    r1 = *(uint32_t *)(r4 + 24);  /* MMIO: EXTI.+0x18(r) */
    /* 0x08007770: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08007772: 2863     str      r0, [r5, #0x30] */
    *(uint32_t *)(r5 + 48) = r0;   /* store */  /* MMIO: RCC.CFGR3(w) */

    L_08007774:
    /* 0x08007774: 2088     ldrh     r0, [r4] */
    r0 = *(uint16_t *)(r4);  /* MMIO: EXTI.IMR(r) */
    /* 0x08007776: 4005     lsls     r0, r0, #0x15 */
    r0 = r0 << 0x15;
    /* 0x08007778: 05d5     bpl      #0x8007786 */
    if (r6 >= 0) goto L_08007786;   /* result non-negative */
    /* 0x0800777A: 286b     ldr      r0, [r5, #0x30] */
    r0 = *(uint32_t *)(r5 + 48);  /* MMIO: RCC.CFGR3(r) */
    /* 0x0800777C: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x0800777E: 8843     bics     r0, r1 */
    r0 = r0 & ~r1;
    /* 0x08007780: 6169     ldr      r1, [r4, #0x14] */
    r1 = *(uint32_t *)(r4 + 20);  /* MMIO: EXTI.PR(r) */
    /* 0x08007782: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08007784: 2863     str      r0, [r5, #0x30] */
    *(uint32_t *)(r5 + 48) = r0;   /* store */  /* MMIO: RCC.CFGR3(w) */

    L_08007786:
    /* 0x08007786: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08007788: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
