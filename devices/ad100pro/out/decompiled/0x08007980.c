/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007980
 * Name      : FUN_08007980
 * End       : 0x08007D46
 * Size      : 966 bytes, 451 instructions decoded in body (functions.json insns=451, blocks=115)
 * Class     : function
 * Callers   : 1  -> 0x0800B8F0 FUN_0800B8F0
 * Callees   : 1  -> 0x08007434 delay_or_timing
 * MMIO      : RCC.CFGR, RCC.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007980(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007980: feb5     push     {r1, r2, r3, r4, r5, r6, r7, lr} */
    push({r1, r2, r3, r4, r5, r6, r7, lr});
    /* 0x08007982: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x08007984: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08007986: f04e     ldr      r6, [pc, #0x3c0]  ; =0x00010000 */
    r6 = *(uint32_t *)0x08007D48;   /* = 0x00010000 */
    /* 0x08007988: f04d     ldr      r5, [pc, #0x3c0]  ; =RCC.CR */
    r5 = *(uint32_t *)0x08007D4C;   /* = MMIO RCC+0x00 */
    /* 0x0800798A: c007     lsls     r0, r0, #0x1f */
    r0 = r0 << 0x1f;
    /* 0x0800798C: 48d0     beq      #0x8007a20 */
    if (/* beq */) goto L_08007A20;
    /* 0x0800798E: 6868     ldr      r0, [r5, #4] */
    r0 = *(uint32_t *)(r5 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x08007990: 0007     lsls     r0, r0, #0x1c */
    r0 = r0 << 0x1c;
    /* 0x08007992: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x08007994: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08007996: 1fd0     beq      #0x80079d8 */
    if (r0 == 1) goto L_080079D8;
    /* 0x08007998: 6868     ldr      r0, [r5, #4] */
    r0 = *(uint32_t *)(r5 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x0800799A: 0007     lsls     r0, r0, #0x1c */
    r0 = r0 << 0x1c;
    /* 0x0800799C: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x0800799E: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x080079A0: 04d1     bne      #0x80079ac */
    if (r0 != 2) goto L_080079AC;
    /* 0x080079A2: 6868     ldr      r0, [r5, #4] */
    r0 = *(uint32_t *)(r5 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x080079A4: c003     lsls     r0, r0, #0xf */
    r0 = r0 << 0xf;
    /* 0x080079A6: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x080079A8: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x080079AA: 15d0     beq      #0x80079d8 */
    if (r0 == 2) goto L_080079D8;

    L_080079AC:
    /* 0x080079AC: 6168     ldr      r1, [r4, #4] */
    r1 = *(uint32_t *)(r4 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x080079AE: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x080079B0: 19d0     beq      #0x80079e6 */
    if (r1 == 1) goto L_080079E6;
    /* 0x080079B2: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x080079B4: 8004     lsls     r0, r0, #0x12 */
    r0 = r0 << 0x12;
    /* 0x080079B6: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x080079B8: 01d0     beq      #0x80079be */
    if (r1 == 0) goto L_080079BE;
    /* 0x080079BA: 0529     cmp      r1, #5 */
    /* flags = cmp(r1, #5) */
    /* 0x080079BC: 17d0     beq      #0x80079ee */
    if (r1 == 5) goto L_080079EE;

    L_080079BE:
    /* 0x080079BE: 2968     ldr      r1, [r5] */
    r1 = *(uint32_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x080079C0: b143     bics     r1, r6 */
    r1 = r1 & ~r6;
    /* 0x080079C2: 2960     str      r1, [r5] */
    *(uint32_t *)(r5) = r1;   /* store */  /* MMIO: RCC.CR(w) */
    /* 0x080079C4: 2968     ldr      r1, [r5] */
    r1 = *(uint32_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x080079C6: 8143     bics     r1, r0 */
    r1 = r1 & ~r0;
    /* 0x080079C8: 2960     str      r1, [r5] */
    *(uint32_t *)(r5) = r1;   /* store */  /* MMIO: RCC.CR(w) */

    L_080079CA:
    /* 0x080079CA: 6068     ldr      r0, [r4, #4] */
    r0 = *(uint32_t *)(r4 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x080079CC: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080079CE: 1bd0     beq      #0x8007a08 */
    if (r0 == 0) goto L_08007A08;
    /* 0x080079D0: fff730fd bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x080079D4: 0746     mov      r7, r0 */
    r7 = r0;
    /* 0x080079D6: 13e0     b        #0x8007a00 */
    goto L_08007A00;

    L_080079D8:
    /* 0x080079D8: 2868     ldr      r0, [r5] */
    r0 = *(uint32_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x080079DA: 8003     lsls     r0, r0, #0xe */
    r0 = r0 << 0xe;
    /* 0x080079DC: 20d5     bpl      #0x8007a20 */
    if (r0 >= 0) goto L_08007A20;   /* result non-negative */
    /* 0x080079DE: 6068     ldr      r0, [r4, #4] */
    r0 = *(uint32_t *)(r4 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x080079E0: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080079E2: 76d0     beq      #0x8007ad2 */
    if (r0 == 0) goto L_08007AD2;
    /* 0x080079E4: 1ce0     b        #0x8007a20 */
    goto L_08007A20;

    L_080079E6:
    /* 0x080079E6: 2868     ldr      r0, [r5] */
    r0 = *(uint32_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x080079E8: 3043     orrs     r0, r6 */
    r0 = r0 | r6;
    /* 0x080079EA: 2860     str      r0, [r5] */
    *(uint32_t *)(r5) = r0;   /* store */  /* MMIO: RCC.CR(w) */
    /* 0x080079EC: ede7     b        #0x80079ca */
    goto L_080079CA;   /* loop */

    L_080079EE:
    /* 0x080079EE: 2968     ldr      r1, [r5] */
    r1 = *(uint32_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x080079F0: 0143     orrs     r1, r0 */
    r1 = r1 | r0;
    /* 0x080079F2: 2960     str      r1, [r5] */
    *(uint32_t *)(r5) = r1;   /* store */  /* MMIO: RCC.CR(w) */
    /* 0x080079F4: f7e7     b        #0x80079e6 */
    goto L_080079E6;   /* loop */

    L_080079F6:
    /* 0x080079F6: fff71dfd bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x080079FA: c01b     subs     r0, r0, r7 */
    r0 = r0 - r7;
    /* 0x080079FC: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */

    L_080079FE:
    /* 0x080079FE: 69d8     bhi      #0x8007ad4 */
    if (r0 > 0x64) goto L_08007AD4;   /* unsigned */

    L_08007A00:
    /* 0x08007A00: 2868     ldr      r0, [r5] */
    r0 = *(uint32_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x08007A02: 8003     lsls     r0, r0, #0xe */
    r0 = r0 << 0xe;
    /* 0x08007A04: f7d5     bpl      #0x80079f6 */
    if (r0 >= 0x64) goto L_080079F6;   /* result non-negative */
    /* 0x08007A06: 0be0     b        #0x8007a20 */
    goto L_08007A20;

    L_08007A08:
    /* 0x08007A08: fff714fd bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007A0C: 0746     mov      r7, r0 */
    r7 = r0;
    /* 0x08007A0E: 04e0     b        #0x8007a1a */
    goto L_08007A1A;

    L_08007A10:
    /* 0x08007A10: fff710fd bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007A14: c01b     subs     r0, r0, r7 */
    r0 = r0 - r7;
    /* 0x08007A16: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x08007A18: f1d8     bhi      #0x80079fe */
    if (r0 > 0x64) goto L_080079FE;   /* unsigned */

    L_08007A1A:
    /* 0x08007A1A: 2868     ldr      r0, [r5] */
    r0 = *(uint32_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x08007A1C: 8003     lsls     r0, r0, #0xe */
    r0 = r0 << 0xe;
    /* 0x08007A1E: f7d4     bmi      #0x8007a10 */
    if (r0 < 0x64) goto L_08007A10;   /* result negative */

    L_08007A20:
    /* 0x08007A20: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007A22: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08007A24: 3ed5     bpl      #0x8007aa4 */
    if (r0 >= 0x64) goto L_08007AA4;   /* result non-negative */
    /* 0x08007A26: 6868     ldr      r0, [r5, #4] */
    r0 = *(uint32_t *)(r5 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x08007A28: 0007     lsls     r0, r0, #0x1c */
    r0 = r0 << 0x1c;
    /* 0x08007A2A: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x08007A2C: 14d0     beq      #0x8007a58 */
    if (r0 == 0x64) goto L_08007A58;
    /* 0x08007A2E: 6868     ldr      r0, [r5, #4] */
    r0 = *(uint32_t *)(r5 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x08007A30: 0007     lsls     r0, r0, #0x1c */
    r0 = r0 << 0x1c;
    /* 0x08007A32: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x08007A34: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08007A36: 04d1     bne      #0x8007a42 */
    if (r0 != 2) goto L_08007A42;
    /* 0x08007A38: 6868     ldr      r0, [r5, #4] */
    r0 = *(uint32_t *)(r5 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x08007A3A: c003     lsls     r0, r0, #0xf */
    r0 = r0 << 0xf;
    /* 0x08007A3C: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x08007A3E: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08007A40: 0ad0     beq      #0x8007a58 */
    if (r0 == 1) goto L_08007A58;

    L_08007A42:
    /* 0x08007A42: e068     ldr      r0, [r4, #0xc] */
    r0 = *(uint32_t *)(r4 + 12);  /* MMIO: RCC.APB2RSTR(r) */
    /* 0x08007A44: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08007A46: 2868     ldr      r0, [r5] */
    r0 = *(uint32_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x08007A48: 1dd0     beq      #0x8007a86 */
    if (r0 == 0) goto L_08007A86;
    /* 0x08007A4A: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08007A4C: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08007A4E: 2860     str      r0, [r5] */
    *(uint32_t *)(r5) = r0;   /* store */  /* MMIO: RCC.CR(w) */
    /* 0x08007A50: fff7f0fc bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007A54: 0746     mov      r7, r0 */
    r7 = r0;
    /* 0x08007A56: 0be0     b        #0x8007a70 */
    goto L_08007A70;

    L_08007A58:
    /* 0x08007A58: 2868     ldr      r0, [r5] */
    r0 = *(uint32_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x08007A5A: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08007A5C: 0bd5     bpl      #0x8007a76 */
    if (r0 >= 0) goto L_08007A76;   /* result non-negative */
    /* 0x08007A5E: e068     ldr      r0, [r4, #0xc] */
    r0 = *(uint32_t *)(r4 + 12);  /* MMIO: RCC.APB2RSTR(r) */
    /* 0x08007A60: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08007A62: 36d1     bne      #0x8007ad2 */
    if (r0 != 1) goto L_08007AD2;
    /* 0x08007A64: 07e0     b        #0x8007a76 */
    goto L_08007A76;

    L_08007A66:
    /* 0x08007A66: fff7e5fc bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007A6A: c01b     subs     r0, r0, r7 */
    r0 = r0 - r7;
    /* 0x08007A6C: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */

    L_08007A6E:
    /* 0x08007A6E: c6d8     bhi      #0x80079fe */
    if (r0 > 2) goto L_080079FE;   /* unsigned */

    L_08007A70:
    /* 0x08007A70: 2868     ldr      r0, [r5] */
    r0 = *(uint32_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x08007A72: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08007A74: f7d5     bpl      #0x8007a66 */
    if (r0 >= 2) goto L_08007A66;   /* result non-negative */

    L_08007A76:
    /* 0x08007A76: 2868     ldr      r0, [r5] */
    r0 = *(uint32_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x08007A78: f821     movs     r1, #0xf8 */
    r1 = 0xf8;
    /* 0x08007A7A: 8843     bics     r0, r1 */
    r0 = r0 & ~r1;
    /* 0x08007A7C: 2169     ldr      r1, [r4, #0x10] */
    r1 = *(uint32_t *)(r4 + 16);  /* MMIO: RCC.APB1RSTR(r) */
    /* 0x08007A7E: c900     lsls     r1, r1, #3 */
    r1 = r1 << 3;
    /* 0x08007A80: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08007A82: 2860     str      r0, [r5] */
    *(uint32_t *)(r5) = r0;   /* store */  /* MMIO: RCC.CR(w) */
    /* 0x08007A84: 0ee0     b        #0x8007aa4 */
    goto L_08007AA4;

    L_08007A86:
    /* 0x08007A86: 4008     lsrs     r0, r0, #1 */
    r0 = r0 >> 1;
    /* 0x08007A88: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x08007A8A: 2860     str      r0, [r5] */
    *(uint32_t *)(r5) = r0;   /* store */  /* MMIO: RCC.CR(w) */
    /* 0x08007A8C: fff7d2fc bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007A90: 0746     mov      r7, r0 */
    r7 = r0;
    /* 0x08007A92: 04e0     b        #0x8007a9e */
    goto L_08007A9E;

    L_08007A94:
    /* 0x08007A94: fff7cefc bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007A98: c01b     subs     r0, r0, r7 */
    r0 = r0 - r7;
    /* 0x08007A9A: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08007A9C: afd8     bhi      #0x80079fe */
    if (r0 > 2) goto L_080079FE;   /* unsigned */

    L_08007A9E:
    /* 0x08007A9E: 2868     ldr      r0, [r5] */
    r0 = *(uint32_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x08007AA0: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08007AA2: f7d4     bmi      #0x8007a94 */
    if (r0 < 2) goto L_08007A94;   /* result negative */

    L_08007AA4:
    /* 0x08007AA4: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007AA6: 0007     lsls     r0, r0, #0x1c */
    r0 = r0 << 0x1c;
    /* 0x08007AA8: 24d5     bpl      #0x8007af4 */
    if (r0 >= 2) goto L_08007AF4;   /* result non-negative */
    /* 0x08007AAA: e069     ldr      r0, [r4, #0x1c] */
    r0 = *(uint32_t *)(r4 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x08007AAC: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08007AAE: 686a     ldr      r0, [r5, #0x24] */
    r0 = *(uint32_t *)(r5 + 36);  /* MMIO: RCC.CSR(r) */
    /* 0x08007AB0: 11d0     beq      #0x8007ad6 */
    if (r0 == 0) goto L_08007AD6;
    /* 0x08007AB2: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08007AB4: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08007AB6: 6862     str      r0, [r5, #0x24] */
    *(uint32_t *)(r5 + 36) = r0;   /* store */  /* MMIO: RCC.CSR(w) */
    /* 0x08007AB8: fff7bcfc bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007ABC: 0746     mov      r7, r0 */
    r7 = r0;
    /* 0x08007ABE: 04e0     b        #0x8007aca */
    goto L_08007ACA;

    L_08007AC0:
    /* 0x08007AC0: fff7b8fc bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007AC4: c01b     subs     r0, r0, r7 */
    r0 = r0 - r7;
    /* 0x08007AC6: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08007AC8: 99d8     bhi      #0x80079fe */
    if (r0 > 2) goto L_080079FE;   /* unsigned */

    L_08007ACA:
    /* 0x08007ACA: 686a     ldr      r0, [r5, #0x24] */
    r0 = *(uint32_t *)(r5 + 36);  /* MMIO: RCC.CSR(r) */
    /* 0x08007ACC: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08007ACE: f7d5     bpl      #0x8007ac0 */
    if (r0 >= 2) goto L_08007AC0;   /* result non-negative */
    /* 0x08007AD0: 10e0     b        #0x8007af4 */
    goto L_08007AF4;

    L_08007AD2:
    /* 0x08007AD2: 36e1     b        #0x8007d42 */
    goto L_08007D42;

    L_08007AD4:
    /* 0x08007AD4: d1e0     b        #0x8007c7a */
    goto L_08007C7A;

    L_08007AD6:
    /* 0x08007AD6: 4008     lsrs     r0, r0, #1 */
    r0 = r0 >> 1;
    /* 0x08007AD8: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x08007ADA: 6862     str      r0, [r5, #0x24] */
    *(uint32_t *)(r5 + 36) = r0;   /* store */  /* MMIO: RCC.CSR(w) */
    /* 0x08007ADC: fff7aafc bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007AE0: 0746     mov      r7, r0 */
    r7 = r0;
    /* 0x08007AE2: 04e0     b        #0x8007aee */
    goto L_08007AEE;

    L_08007AE4:
    /* 0x08007AE4: fff7a6fc bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007AE8: c01b     subs     r0, r0, r7 */
    r0 = r0 - r7;
    /* 0x08007AEA: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08007AEC: 87d8     bhi      #0x80079fe */
    if (r0 > 2) goto L_080079FE;   /* unsigned */

    L_08007AEE:
    /* 0x08007AEE: 686a     ldr      r0, [r5, #0x24] */
    r0 = *(uint32_t *)(r5 + 36);  /* MMIO: RCC.CSR(r) */
    /* 0x08007AF0: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08007AF2: f7d4     bmi      #0x8007ae4 */
    if (r0 < 2) goto L_08007AE4;   /* result negative */

    L_08007AF4:
    /* 0x08007AF4: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007AF6: 0427     movs     r7, #4 */
    r7 = 4;
    /* 0x08007AF8: 4007     lsls     r0, r0, #0x1d */
    r0 = r0 << 0x1d;
    /* 0x08007AFA: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08007AFC: 61da     bge      #0x8007bc2 */
    if (r0 >= 0) goto L_08007BC2;   /* signed */
    /* 0x08007AFE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08007B00: 0190     str      r0, [sp, #4] */
    *(uint32_t *)(sp + 4) = r0;   /* store */
    /* 0x08007B02: e869     ldr      r0, [r5, #0x1c] */
    r0 = *(uint32_t *)(r5 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x08007B04: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x08007B06: 08d4     bmi      #0x8007b1a */
    if (r0 < 0) goto L_08007B1A;   /* result negative */
    /* 0x08007B08: e969     ldr      r1, [r5, #0x1c] */
    r1 = *(uint32_t *)(r5 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x08007B0A: b806     lsls     r0, r7, #0x1a */
    r0 = r7 << 0x1a;
    /* 0x08007B0C: 0143     orrs     r1, r0 */
    r1 = r1 | r0;
    /* 0x08007B0E: e961     str      r1, [r5, #0x1c] */
    *(uint32_t *)(r5 + 28) = r1;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x08007B10: e969     ldr      r1, [r5, #0x1c] */
    r1 = *(uint32_t *)(r5 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x08007B12: 0140     ands     r1, r0 */
    r1 = r1 & r0;
    /* 0x08007B14: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08007B16: 0091     str      r1, [sp] */
    *(uint32_t *)(sp) = r1;   /* store */
    /* 0x08007B18: 0190     str      r0, [sp, #4] */
    *(uint32_t *)(sp + 4) = r0;   /* store */

    L_08007B1A:
    /* 0x08007B1A: 8d48     ldr      r0, [pc, #0x234]  ; =PWR.CR */
    r0 = *(uint32_t *)0x08007D50;   /* = MMIO PWR+0x00 */
    /* 0x08007B1C: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: PWR.CR(r) */
    /* 0x08007B1E: c905     lsls     r1, r1, #0x17 */
    r1 = r1 << 0x17;
    /* 0x08007B20: 11d4     bmi      #0x8007b46 */
    if (r0 < 0) goto L_08007B46;   /* result negative */
    /* 0x08007B22: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: PWR.CR(r) */
    /* 0x08007B24: 8215     asrs     r2, r0, #0x16 */
    r2 = r0 >> 0x16;
    /* 0x08007B26: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x08007B28: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: PWR.CR(w) */
    /* 0x08007B2A: fff783fc bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007B2E: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08007B30: 05e0     b        #0x8007b3e */
    goto L_08007B3E;

    L_08007B32:
    /* 0x08007B32: fff77ffc bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007B36: 0099     ldr      r1, [sp] */
    r1 = *(uint32_t *)(sp);
    /* 0x08007B38: 401a     subs     r0, r0, r1 */
    r0 = r0 - r1;
    /* 0x08007B3A: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x08007B3C: 97d8     bhi      #0x8007a6e */
    if (r0 > 0x64) goto L_08007A6E;   /* unsigned */

    L_08007B3E:
    /* 0x08007B3E: 8448     ldr      r0, [pc, #0x210]  ; =PWR.CR */
    r0 = *(uint32_t *)0x08007D50;   /* = MMIO PWR+0x00 */
    /* 0x08007B40: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);  /* MMIO: PWR.CR(r) */
    /* 0x08007B42: c005     lsls     r0, r0, #0x17 */
    r0 = r0 << 0x17;
    /* 0x08007B44: f5d5     bpl      #0x8007b32 */
    if (r0 >= 0x64) goto L_08007B32;   /* result non-negative */

    L_08007B46:
    /* 0x08007B46: a068     ldr      r0, [r4, #8] */
    r0 = *(uint32_t *)(r4 + 8);  /* MMIO: RCC.CIR(r) */
    /* 0x08007B48: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08007B4A: 0ad0     beq      #0x8007b62 */
    if (r0 == 1) goto L_08007B62;
    /* 0x08007B4C: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08007B4E: 01d0     beq      #0x8007b54 */
    if (r0 == 0) goto L_08007B54;
    /* 0x08007B50: 0528     cmp      r0, #5 */
    /* flags = cmp(r0, #5) */
    /* 0x08007B52: 11d0     beq      #0x8007b78 */
    if (r0 == 5) goto L_08007B78;

    L_08007B54:
    /* 0x08007B54: 286a     ldr      r0, [r5, #0x20] */
    r0 = *(uint32_t *)(r5 + 32);  /* MMIO: RCC.BDCR(r) */
    /* 0x08007B56: 4008     lsrs     r0, r0, #1 */
    r0 = r0 >> 1;
    /* 0x08007B58: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x08007B5A: 2862     str      r0, [r5, #0x20] */
    *(uint32_t *)(r5 + 32) = r0;   /* store */  /* MMIO: RCC.BDCR(w) */
    /* 0x08007B5C: 286a     ldr      r0, [r5, #0x20] */
    r0 = *(uint32_t *)(r5 + 32);  /* MMIO: RCC.BDCR(r) */
    /* 0x08007B5E: b843     bics     r0, r7 */
    r0 = r0 & ~r7;
    /* 0x08007B60: 02e0     b        #0x8007b68 */
    goto L_08007B68;

    L_08007B62:
    /* 0x08007B62: 286a     ldr      r0, [r5, #0x20] */
    r0 = *(uint32_t *)(r5 + 32);  /* MMIO: RCC.BDCR(r) */
    /* 0x08007B64: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08007B66: 0843     orrs     r0, r1 */
    r0 = r0 | r1;

    L_08007B68:
    /* 0x08007B68: 2862     str      r0, [r5, #0x20] */
    *(uint32_t *)(r5 + 32) = r0;   /* store */  /* MMIO: RCC.BDCR(w) */
    /* 0x08007B6A: a068     ldr      r0, [r4, #8] */
    r0 = *(uint32_t *)(r4 + 8);  /* MMIO: RCC.CIR(r) */
    /* 0x08007B6C: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08007B6E: 12d0     beq      #0x8007b96 */
    if (r0 == 0) goto L_08007B96;
    /* 0x08007B70: fff760fc bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007B74: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08007B76: 0ae0     b        #0x8007b8e */
    goto L_08007B8E;

    L_08007B78:
    /* 0x08007B78: 286a     ldr      r0, [r5, #0x20] */
    r0 = *(uint32_t *)(r5 + 32);  /* MMIO: RCC.BDCR(r) */
    /* 0x08007B7A: 3843     orrs     r0, r7 */
    r0 = r0 | r7;
    /* 0x08007B7C: 2862     str      r0, [r5, #0x20] */
    *(uint32_t *)(r5 + 32) = r0;   /* store */  /* MMIO: RCC.BDCR(w) */
    /* 0x08007B7E: f0e7     b        #0x8007b62 */
    goto L_08007B62;   /* loop */

    L_08007B80:
    /* 0x08007B80: fff758fc bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007B84: 0099     ldr      r1, [sp] */
    r1 = *(uint32_t *)(sp);
    /* 0x08007B86: 411a     subs     r1, r0, r1 */
    r1 = r0 - r1;
    /* 0x08007B88: 7248     ldr      r0, [pc, #0x1c8]  ; =0x00001388 */
    r0 = *(uint32_t *)0x08007D54;   /* = 0x00001388 */
    /* 0x08007B8A: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x08007B8C: 75d8     bhi      #0x8007c7a */
    if (r1 > r0) goto L_08007C7A;   /* unsigned */

    L_08007B8E:
    /* 0x08007B8E: 286a     ldr      r0, [r5, #0x20] */
    r0 = *(uint32_t *)(r5 + 32);  /* MMIO: RCC.BDCR(r) */
    /* 0x08007B90: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08007B92: f5d5     bpl      #0x8007b80 */
    if (r1 >= r0) goto L_08007B80;   /* result non-negative */
    /* 0x08007B94: 0de0     b        #0x8007bb2 */
    goto L_08007BB2;

    L_08007B96:
    /* 0x08007B96: fff74dfc bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007B9A: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08007B9C: 06e0     b        #0x8007bac */
    goto L_08007BAC;

    L_08007B9E:
    /* 0x08007B9E: fff749fc bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007BA2: 0099     ldr      r1, [sp] */
    r1 = *(uint32_t *)(sp);
    /* 0x08007BA4: 411a     subs     r1, r0, r1 */
    r1 = r0 - r1;
    /* 0x08007BA6: 6b48     ldr      r0, [pc, #0x1ac]  ; =0x00001388 */
    r0 = *(uint32_t *)0x08007D54;   /* = 0x00001388 */
    /* 0x08007BA8: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x08007BAA: 66d8     bhi      #0x8007c7a */
    if (r1 > r0) goto L_08007C7A;   /* unsigned */

    L_08007BAC:
    /* 0x08007BAC: 286a     ldr      r0, [r5, #0x20] */
    r0 = *(uint32_t *)(r5 + 32);  /* MMIO: RCC.BDCR(r) */
    /* 0x08007BAE: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08007BB0: f5d4     bmi      #0x8007b9e */
    if (r1 < r0) goto L_08007B9E;   /* result negative */

    L_08007BB2:
    /* 0x08007BB2: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x08007BB4: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08007BB6: 04d0     beq      #0x8007bc2 */
    if (r0 == 0) goto L_08007BC2;
    /* 0x08007BB8: e969     ldr      r1, [r5, #0x1c] */
    r1 = *(uint32_t *)(r5 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x08007BBA: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08007BBC: 0007     lsls     r0, r0, #0x1c */
    r0 = r0 << 0x1c;
    /* 0x08007BBE: 8143     bics     r1, r0 */
    r1 = r1 & ~r0;
    /* 0x08007BC0: e961     str      r1, [r5, #0x1c] */
    *(uint32_t *)(r5 + 28) = r1;   /* store */  /* MMIO: RCC.APB1ENR(w) */

    L_08007BC2:
    /* 0x08007BC2: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007BC4: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x08007BC6: 35d5     bpl      #0x8007c34 */
    if (r0 >= 0) goto L_08007C34;   /* result non-negative */
    /* 0x08007BC8: 6069     ldr      r0, [r4, #0x14] */
    r0 = *(uint32_t *)(r4 + 20);  /* MMIO: RCC.AHBENR(r) */
    /* 0x08007BCA: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08007BCC: 0cd0     beq      #0x8007be8 */
    if (r0 == 1) goto L_08007BE8;
    /* 0x08007BCE: 401d     adds     r0, r0, #5 */
    r0 = r0 + 5;
    /* 0x08007BD0: 686b     ldr      r0, [r5, #0x34] */
    r0 = *(uint32_t *)(r5 + 52);  /* MMIO: RCC.CR2(r) */
    /* 0x08007BD2: 24d0     beq      #0x8007c1e */
    if (r0 == 1) goto L_08007C1E;
    /* 0x08007BD4: 3843     orrs     r0, r7 */
    r0 = r0 | r7;
    /* 0x08007BD6: 6863     str      r0, [r5, #0x34] */
    *(uint32_t *)(r5 + 52) = r0;   /* store */  /* MMIO: RCC.CR2(w) */
    /* 0x08007BD8: 686b     ldr      r0, [r5, #0x34] */
    r0 = *(uint32_t *)(r5 + 52);  /* MMIO: RCC.CR2(r) */
    /* 0x08007BDA: 4008     lsrs     r0, r0, #1 */
    r0 = r0 >> 1;
    /* 0x08007BDC: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x08007BDE: 6863     str      r0, [r5, #0x34] */
    *(uint32_t *)(r5 + 52) = r0;   /* store */  /* MMIO: RCC.CR2(w) */
    /* 0x08007BE0: fff728fc bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007BE4: 0746     mov      r7, r0 */
    r7 = r0;
    /* 0x08007BE6: 22e0     b        #0x8007c2e */
    goto L_08007C2E;

    L_08007BE8:
    /* 0x08007BE8: 686b     ldr      r0, [r5, #0x34] */
    r0 = *(uint32_t *)(r5 + 52);  /* MMIO: RCC.CR2(r) */
    /* 0x08007BEA: 3843     orrs     r0, r7 */
    r0 = r0 | r7;
    /* 0x08007BEC: 6863     str      r0, [r5, #0x34] */
    *(uint32_t *)(r5 + 52) = r0;   /* store */  /* MMIO: RCC.CR2(w) */
    /* 0x08007BEE: 686b     ldr      r0, [r5, #0x34] */
    r0 = *(uint32_t *)(r5 + 52);  /* MMIO: RCC.CR2(r) */
    /* 0x08007BF0: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08007BF2: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08007BF4: 6863     str      r0, [r5, #0x34] */
    *(uint32_t *)(r5 + 52) = r0;   /* store */  /* MMIO: RCC.CR2(w) */
    /* 0x08007BF6: fff71dfc bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007BFA: 0746     mov      r7, r0 */
    r7 = r0;
    /* 0x08007BFC: 04e0     b        #0x8007c08 */
    goto L_08007C08;

    L_08007BFE:
    /* 0x08007BFE: fff719fc bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007C02: c01b     subs     r0, r0, r7 */
    r0 = r0 - r7;
    /* 0x08007C04: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08007C06: 38d8     bhi      #0x8007c7a */
    if (r0 > 2) goto L_08007C7A;   /* unsigned */

    L_08007C08:
    /* 0x08007C08: 686b     ldr      r0, [r5, #0x34] */
    r0 = *(uint32_t *)(r5 + 52);  /* MMIO: RCC.CR2(r) */
    /* 0x08007C0A: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08007C0C: f7d5     bpl      #0x8007bfe */
    if (r0 >= 2) goto L_08007BFE;   /* result non-negative */

    L_08007C0E:
    /* 0x08007C0E: 686b     ldr      r0, [r5, #0x34] */
    r0 = *(uint32_t *)(r5 + 52);  /* MMIO: RCC.CR2(r) */
    /* 0x08007C10: f821     movs     r1, #0xf8 */
    r1 = 0xf8;
    /* 0x08007C12: 8843     bics     r0, r1 */
    r0 = r0 & ~r1;
    /* 0x08007C14: a169     ldr      r1, [r4, #0x18] */
    r1 = *(uint32_t *)(r4 + 24);  /* MMIO: RCC.APB2ENR(r) */
    /* 0x08007C16: c900     lsls     r1, r1, #3 */
    r1 = r1 << 3;
    /* 0x08007C18: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08007C1A: 6863     str      r0, [r5, #0x34] */
    *(uint32_t *)(r5 + 52) = r0;   /* store */  /* MMIO: RCC.CR2(w) */
    /* 0x08007C1C: 0ae0     b        #0x8007c34 */
    goto L_08007C34;

    L_08007C1E:
    /* 0x08007C1E: b843     bics     r0, r7 */
    r0 = r0 & ~r7;
    /* 0x08007C20: 6863     str      r0, [r5, #0x34] */
    *(uint32_t *)(r5 + 52) = r0;   /* store */  /* MMIO: RCC.CR2(w) */
    /* 0x08007C22: f4e7     b        #0x8007c0e */
    goto L_08007C0E;   /* loop */

    L_08007C24:
    /* 0x08007C24: fff706fc bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007C28: c01b     subs     r0, r0, r7 */
    r0 = r0 - r7;
    /* 0x08007C2A: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08007C2C: 25d8     bhi      #0x8007c7a */
    if (r0 > 2) goto L_08007C7A;   /* unsigned */

    L_08007C2E:
    /* 0x08007C2E: 686b     ldr      r0, [r5, #0x34] */
    r0 = *(uint32_t *)(r5 + 52);  /* MMIO: RCC.CR2(r) */
    /* 0x08007C30: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08007C32: f7d4     bmi      #0x8007c24 */
    if (r0 < 2) goto L_08007C24;   /* result negative */

    L_08007C34:
    /* 0x08007C34: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007C36: 8006     lsls     r0, r0, #0x1a */
    r0 = r0 << 0x1a;
    /* 0x08007C38: 37d5     bpl      #0x8007caa */
    if (r0 >= 2) goto L_08007CAA;   /* result non-negative */
    /* 0x08007C3A: 6868     ldr      r0, [r5, #4] */
    r0 = *(uint32_t *)(r5 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x08007C3C: 0007     lsls     r0, r0, #0x1c */
    r0 = r0 << 0x1c;
    /* 0x08007C3E: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x08007C40: 0328     cmp      r0, #3 */
    /* flags = cmp(r0, #3) */
    /* 0x08007C42: 13d0     beq      #0x8007c6c */
    if (r0 == 3) goto L_08007C6C;
    /* 0x08007C44: 6868     ldr      r0, [r5, #4] */
    r0 = *(uint32_t *)(r5 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x08007C46: 0007     lsls     r0, r0, #0x1c */
    r0 = r0 << 0x1c;
    /* 0x08007C48: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x08007C4A: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08007C4C: 04d1     bne      #0x8007c58 */
    if (r0 != 2) goto L_08007C58;
    /* 0x08007C4E: 6868     ldr      r0, [r5, #4] */
    r0 = *(uint32_t *)(r5 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x08007C50: c003     lsls     r0, r0, #0xf */
    r0 = r0 << 0xf;
    /* 0x08007C52: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x08007C54: 0328     cmp      r0, #3 */
    /* flags = cmp(r0, #3) */
    /* 0x08007C56: 09d0     beq      #0x8007c6c */
    if (r0 == 3) goto L_08007C6C;

    L_08007C58:
    /* 0x08007C58: 206a     ldr      r0, [r4, #0x20] */
    r0 = *(uint32_t *)(r4 + 32);  /* MMIO: RCC.BDCR(r) */
    /* 0x08007C5A: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08007C5C: 686b     ldr      r0, [r5, #0x34] */
    r0 = *(uint32_t *)(r5 + 52);  /* MMIO: RCC.CR2(r) */
    /* 0x08007C5E: 16d0     beq      #0x8007c8e */
    if (r0 == 0) goto L_08007C8E;
    /* 0x08007C60: 3043     orrs     r0, r6 */
    r0 = r0 | r6;
    /* 0x08007C62: 6863     str      r0, [r5, #0x34] */
    *(uint32_t *)(r5 + 52) = r0;   /* store */  /* MMIO: RCC.CR2(w) */
    /* 0x08007C64: fff7e6fb bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007C68: 0646     mov      r6, r0 */
    r6 = r0;
    /* 0x08007C6A: 0ce0     b        #0x8007c86 */
    goto L_08007C86;

    L_08007C6C:
    /* 0x08007C6C: 686b     ldr      r0, [r5, #0x34] */
    r0 = *(uint32_t *)(r5 + 52);  /* MMIO: RCC.CR2(r) */
    /* 0x08007C6E: c003     lsls     r0, r0, #0xf */
    r0 = r0 << 0xf;
    /* 0x08007C70: 1bd5     bpl      #0x8007caa */
    if (r0 >= 0) goto L_08007CAA;   /* result non-negative */
    /* 0x08007C72: 206a     ldr      r0, [r4, #0x20] */
    r0 = *(uint32_t *)(r4 + 32);  /* MMIO: RCC.BDCR(r) */
    /* 0x08007C74: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08007C76: 64d1     bne      #0x8007d42 */
    if (r0 != 1) goto L_08007D42;
    /* 0x08007C78: 17e0     b        #0x8007caa */
    goto L_08007CAA;

    L_08007C7A:
    /* 0x08007C7A: 5be0     b        #0x8007d34 */
    goto L_08007D34;

    L_08007C7C:
    /* 0x08007C7C: fff7dafb bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007C80: 801b     subs     r0, r0, r6 */
    r0 = r0 - r6;
    /* 0x08007C82: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08007C84: 56d8     bhi      #0x8007d34 */
    if (r0 > 2) goto L_08007D34;   /* unsigned */

    L_08007C86:
    /* 0x08007C86: 686b     ldr      r0, [r5, #0x34] */
    r0 = *(uint32_t *)(r5 + 52);  /* MMIO: RCC.CR2(r) */
    /* 0x08007C88: c003     lsls     r0, r0, #0xf */
    r0 = r0 << 0xf;
    /* 0x08007C8A: f7d5     bpl      #0x8007c7c */
    if (r0 >= 2) goto L_08007C7C;   /* result non-negative */
    /* 0x08007C8C: 0de0     b        #0x8007caa */
    goto L_08007CAA;

    L_08007C8E:
    /* 0x08007C8E: b043     bics     r0, r6 */
    r0 = r0 & ~r6;
    /* 0x08007C90: 6863     str      r0, [r5, #0x34] */
    *(uint32_t *)(r5 + 52) = r0;   /* store */  /* MMIO: RCC.CR2(w) */
    /* 0x08007C92: fff7cffb bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007C96: 0646     mov      r6, r0 */
    r6 = r0;
    /* 0x08007C98: 04e0     b        #0x8007ca4 */
    goto L_08007CA4;

    L_08007C9A:
    /* 0x08007C9A: fff7cbfb bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007C9E: 801b     subs     r0, r0, r6 */
    r0 = r0 - r6;
    /* 0x08007CA0: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08007CA2: 47d8     bhi      #0x8007d34 */
    if (r0 > 2) goto L_08007D34;   /* unsigned */

    L_08007CA4:
    /* 0x08007CA4: 686b     ldr      r0, [r5, #0x34] */
    r0 = *(uint32_t *)(r5 + 52);  /* MMIO: RCC.CR2(r) */
    /* 0x08007CA6: c003     lsls     r0, r0, #0xf */
    r0 = r0 << 0xf;
    /* 0x08007CA8: f7d4     bmi      #0x8007c9a */
    if (r0 < 2) goto L_08007C9A;   /* result negative */

    L_08007CAA:
    /* 0x08007CAA: 606a     ldr      r0, [r4, #0x24] */
    r0 = *(uint32_t *)(r4 + 36);  /* MMIO: RCC.CSR(r) */
    /* 0x08007CAC: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08007CAE: 46d0     beq      #0x8007d3e */
    if (r0 == 0) goto L_08007D3E;
    /* 0x08007CB0: 6968     ldr      r1, [r5, #4] */
    r1 = *(uint32_t *)(r5 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x08007CB2: 0907     lsls     r1, r1, #0x1c */
    r1 = r1 << 0x1c;
    /* 0x08007CB4: 890f     lsrs     r1, r1, #0x1e */
    r1 = r1 >> 0x1e;
    /* 0x08007CB6: 0229     cmp      r1, #2 */
    /* flags = cmp(r1, #2) */
    /* 0x08007CB8: 43d0     beq      #0x8007d42 */
    if (r1 == 2) goto L_08007D42;
    /* 0x08007CBA: 0126     movs     r6, #1 */
    r6 = 1;
    /* 0x08007CBC: 3606     lsls     r6, r6, #0x18 */
    r6 = r6 << 0x18;
    /* 0x08007CBE: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08007CC0: 2868     ldr      r0, [r5] */
    r0 = *(uint32_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x08007CC2: 05d0     beq      #0x8007cd0 */
    if (r0 == 2) goto L_08007CD0;
    /* 0x08007CC4: b043     bics     r0, r6 */
    r0 = r0 & ~r6;
    /* 0x08007CC6: 2860     str      r0, [r5] */
    *(uint32_t *)(r5) = r0;   /* store */  /* MMIO: RCC.CR(w) */
    /* 0x08007CC8: fff7b4fb bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007CCC: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x08007CCE: 33e0     b        #0x8007d38 */
    goto L_08007D38;

    L_08007CD0:
    /* 0x08007CD0: b043     bics     r0, r6 */
    r0 = r0 & ~r6;
    /* 0x08007CD2: 2860     str      r0, [r5] */
    *(uint32_t *)(r5) = r0;   /* store */  /* MMIO: RCC.CR(w) */
    /* 0x08007CD4: fff7aefb bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007CD8: 0746     mov      r7, r0 */
    r7 = r0;
    /* 0x08007CDA: 04e0     b        #0x8007ce6 */
    goto L_08007CE6;

    L_08007CDC:
    /* 0x08007CDC: fff7aafb bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007CE0: c01b     subs     r0, r0, r7 */
    r0 = r0 - r7;
    /* 0x08007CE2: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08007CE4: 26d8     bhi      #0x8007d34 */
    if (r0 > 2) goto L_08007D34;   /* unsigned */

    L_08007CE6:
    /* 0x08007CE6: 2868     ldr      r0, [r5] */
    r0 = *(uint32_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x08007CE8: 8001     lsls     r0, r0, #6 */
    r0 = r0 << 6;
    /* 0x08007CEA: f7d4     bmi      #0x8007cdc */
    if (r0 < 2) goto L_08007CDC;   /* result negative */
    /* 0x08007CEC: e86a     ldr      r0, [r5, #0x2c] */
    r0 = *(uint32_t *)(r5 + 44);  /* MMIO: RCC.CFGR2(r) */
    /* 0x08007CEE: 216b     ldr      r1, [r4, #0x30] */
    r1 = *(uint32_t *)(r4 + 48);  /* MMIO: RCC.CFGR3(r) */
    /* 0x08007CF0: 0009     lsrs     r0, r0, #4 */
    r0 = r0 >> 4;
    /* 0x08007CF2: 0001     lsls     r0, r0, #4 */
    r0 = r0 << 4;
    /* 0x08007CF4: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08007CF6: e862     str      r0, [r5, #0x2c] */
    *(uint32_t *)(r5 + 44) = r0;   /* store */  /* MMIO: RCC.CFGR2(w) */
    /* 0x08007CF8: a16a     ldr      r1, [r4, #0x28] */
    r1 = *(uint32_t *)(r4 + 40);  /* MMIO: RCC.AHBRSTR(r) */
    /* 0x08007CFA: e06a     ldr      r0, [r4, #0x2c] */
    r0 = *(uint32_t *)(r4 + 44);  /* MMIO: RCC.CFGR2(r) */
    /* 0x08007CFC: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08007CFE: 6968     ldr      r1, [r5, #4] */
    r1 = *(uint32_t *)(r5 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x08007D00: 7b22     movs     r2, #0x7b */
    r2 = 0x7b;
    /* 0x08007D02: d203     lsls     r2, r2, #0xf */
    r2 = r2 << 0xf;
    /* 0x08007D04: 9143     bics     r1, r2 */
    r1 = r1 & ~r2;
    /* 0x08007D06: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08007D08: 6860     str      r0, [r5, #4] */
    *(uint32_t *)(r5 + 4) = r0;   /* store */  /* MMIO: RCC.CFGR(w) */
    /* 0x08007D0A: 2868     ldr      r0, [r5] */
    r0 = *(uint32_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x08007D0C: 3043     orrs     r0, r6 */
    r0 = r0 | r6;
    /* 0x08007D0E: 2860     str      r0, [r5] */
    *(uint32_t *)(r5) = r0;   /* store */  /* MMIO: RCC.CR(w) */
    /* 0x08007D10: fff790fb bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007D14: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x08007D16: 04e0     b        #0x8007d22 */
    goto L_08007D22;

    L_08007D18:
    /* 0x08007D18: fff78cfb bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007D1C: 001b     subs     r0, r0, r4 */
    r0 = r0 - r4;
    /* 0x08007D1E: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08007D20: 08d8     bhi      #0x8007d34 */
    if (r0 > 2) goto L_08007D34;   /* unsigned */

    L_08007D22:
    /* 0x08007D22: 2868     ldr      r0, [r5] */
    r0 = *(uint32_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x08007D24: 8001     lsls     r0, r0, #6 */
    r0 = r0 << 6;
    /* 0x08007D26: f7d5     bpl      #0x8007d18 */
    if (r0 >= 2) goto L_08007D18;   /* result non-negative */
    /* 0x08007D28: 09e0     b        #0x8007d3e */
    goto L_08007D3E;

    L_08007D2A:
    /* 0x08007D2A: fff783fb bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007D2E: 001b     subs     r0, r0, r4 */
    r0 = r0 - r4;
    /* 0x08007D30: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08007D32: 01d9     bls      #0x8007d38 */
    if (r0 <= 2) goto L_08007D38;   /* unsigned */

    L_08007D34:
    /* 0x08007D34: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x08007D36: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */

    L_08007D38:
    /* 0x08007D38: 2868     ldr      r0, [r5] */
    r0 = *(uint32_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x08007D3A: 8001     lsls     r0, r0, #6 */
    r0 = r0 << 6;
    /* 0x08007D3C: f5d4     bmi      #0x8007d2a */
    if (r0 < 2) goto L_08007D2A;   /* result negative */

    L_08007D3E:
    /* 0x08007D3E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08007D40: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */

    L_08007D42:
    /* 0x08007D42: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08007D44: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */
}
