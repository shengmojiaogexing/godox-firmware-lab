/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080069AC
 * Name      : FUN_080069AC
 * End       : 0x08006B18
 * Size      : 364 bytes, 181 instructions decoded in body (functions.json insns=181, blocks=41)
 * Class     : function
 * Callers   : 1  -> 0x08009310 FUN_08009310
 * Callees   : 1  -> 0x08006B20 FUN_08006B20
 * MMIO      : ADC.+0x300
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080069AC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080069AC: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x080069AE: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x080069B0: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x080069B2: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080069B4: 03d0     beq      #0x80069be */
    if (r0 == 0) goto L_080069BE;
    /* 0x080069B6: 606c     ldr      r0, [r4, #0x44] */
    r0 = *(uint32_t *)(r4 + 68);
    /* 0x080069B8: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080069BA: 02d0     beq      #0x80069c2 */
    if (r0 == 0) goto L_080069C2;
    /* 0x080069BC: 08e0     b        #0x80069d0 */
    goto L_080069D0;

    L_080069BE:
    /* 0x080069BE: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x080069C0: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_080069C2:
    /* 0x080069C2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080069C4: a064     str      r0, [r4, #0x48] */
    *(uint32_t *)(r4 + 72) = r0;   /* store */
    /* 0x080069C6: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x080069C8: 0855     strb     r0, [r1, r4] */
    *(uint8_t *)(r1 + r4) = r0;   /* store */  /* MMIO: ADC.+0x300(w) */
    /* 0x080069CA: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080069CC: 00f0a8f8 bl       #0x8006b20 */
    FUN_08006B20();   /* BL -> 0x08006B20 FUN_08006B20 */

    L_080069D0:
    /* 0x080069D0: 606c     ldr      r0, [r4, #0x44] */
    r0 = *(uint32_t *)(r4 + 68);
    /* 0x080069D2: 1022     movs     r2, #0x10 */
    r2 = 0x10;
    /* 0x080069D4: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x080069D6: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080069D8: 70db     blt      #0x8006abc */
    if (r0 < 0) goto L_08006ABC;   /* signed */
    /* 0x080069DA: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x080069DC: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);
    /* 0x080069DE: 4907     lsls     r1, r1, #0x1d */
    r1 = r1 << 0x1d;
    /* 0x080069E0: 6cd4     bmi      #0x8006abc */
    if (r0 < 0) goto L_08006ABC;   /* result negative */
    /* 0x080069E2: 616c     ldr      r1, [r4, #0x44] */
    r1 = *(uint32_t *)(r4 + 68);
    /* 0x080069E4: 1301     lsls     r3, r2, #4 */
    r3 = r2 << 4;
    /* 0x080069E6: 9943     bics     r1, r3 */
    r1 = r1 & ~r3;
    /* 0x080069E8: 0223     movs     r3, #2 */
    r3 = 2;
    /* 0x080069EA: 1943     orrs     r1, r3 */
    r1 = r1 | r3;
    /* 0x080069EC: 6164     str      r1, [r4, #0x44] */
    *(uint32_t *)(r4 + 68) = r1;   /* store */
    /* 0x080069EE: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);
    /* 0x080069F0: 8907     lsls     r1, r1, #0x1e */
    r1 = r1 << 0x1e;
    /* 0x080069F2: 890f     lsrs     r1, r1, #0x1e */
    r1 = r1 >> 0x1e;
    /* 0x080069F4: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x080069F6: 05d1     bne      #0x8006a04 */
    if (r1 != 1) goto L_08006A04;
    /* 0x080069F8: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);
    /* 0x080069FA: c907     lsls     r1, r1, #0x1f */
    r1 = r1 << 0x1f;
    /* 0x080069FC: 0fd1     bne      #0x8006a1e */
    if (r1 != 1) goto L_08006A1E;
    /* 0x080069FE: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);
    /* 0x08006A00: 0904     lsls     r1, r1, #0x10 */
    r1 = r1 << 0x10;
    /* 0x08006A02: 0cd4     bmi      #0x8006a1e */
    if (r1 < 1) goto L_08006A1E;   /* result negative */

    L_08006A04:
    /* 0x08006A04: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);
    /* 0x08006A06: 1823     movs     r3, #0x18 */
    r3 = 0x18;
    /* 0x08006A08: 9943     bics     r1, r3 */
    r1 = r1 & ~r3;
    /* 0x08006A0A: a368     ldr      r3, [r4, #8] */
    r3 = *(uint32_t *)(r4 + 8);
    /* 0x08006A0C: 1943     orrs     r1, r3 */
    r1 = r1 | r3;
    /* 0x08006A0E: c160     str      r1, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r1;   /* store */
    /* 0x08006A10: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x08006A12: 0169     ldr      r1, [r0, #0x10] */
    r1 = *(uint32_t *)(r0 + 16);
    /* 0x08006A14: 6368     ldr      r3, [r4, #4] */
    r3 = *(uint32_t *)(r4 + 4);
    /* 0x08006A16: 8900     lsls     r1, r1, #2 */
    r1 = r1 << 2;
    /* 0x08006A18: 8908     lsrs     r1, r1, #2 */
    r1 = r1 >> 2;
    /* 0x08006A1A: 1943     orrs     r1, r3 */
    r1 = r1 | r3;
    /* 0x08006A1C: 0161     str      r1, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r1;   /* store */

    L_08006A1E:
    /* 0x08006A1E: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x08006A20: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);
    /* 0x08006A22: 3d4b     ldr      r3, [pc, #0xf4]  ; =0xFFFE0219 */
    r3 = *(uint32_t *)0x08006B18;   /* = 0xFFFE0219 */
    /* 0x08006A24: 1940     ands     r1, r3 */
    r1 = r1 & r3;
    /* 0x08006A26: c160     str      r1, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r1;   /* store */
    /* 0x08006A28: a069     ldr      r0, [r4, #0x18] */
    r0 = *(uint32_t *)(r4 + 24);
    /* 0x08006A2A: e169     ldr      r1, [r4, #0x1c] */
    r1 = *(uint32_t *)(r4 + 28);
    /* 0x08006A2C: 8003     lsls     r0, r0, #0xe */
    r0 = r0 << 0xe;
    /* 0x08006A2E: c903     lsls     r1, r1, #0xf */
    r1 = r1 << 0xf;
    /* 0x08006A30: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08006A32: 216a     ldr      r1, [r4, #0x20] */
    r1 = *(uint32_t *)(r4 + 32);
    /* 0x08006A34: 4b03     lsls     r3, r1, #0xd */
    r3 = r1 << 0xd;
    /* 0x08006A36: 1843     orrs     r0, r3 */
    r0 = r0 | r3;
    /* 0x08006A38: 636b     ldr      r3, [r4, #0x34] */
    r3 = *(uint32_t *)(r4 + 52);
    /* 0x08006A3A: 012b     cmp      r3, #1 */
    /* flags = cmp(r3, #1) */
    /* 0x08006A3C: 02d0     beq      #0x8006a44 */
    if (r3 == 1) goto L_08006A44;
    /* 0x08006A3E: 0123     movs     r3, #1 */
    r3 = 1;
    /* 0x08006A40: 1b03     lsls     r3, r3, #0xc */
    r3 = r3 << 0xc;
    /* 0x08006A42: 00e0     b        #0x8006a46 */
    goto L_08006A46;

    L_08006A44:
    /* 0x08006A44: 0023     movs     r3, #0 */
    r3 = 0;

    L_08006A46:
    /* 0x08006A46: 1843     orrs     r0, r3 */
    r0 = r0 | r3;
    /* 0x08006A48: e368     ldr      r3, [r4, #0xc] */
    r3 = *(uint32_t *)(r4 + 12);
    /* 0x08006A4A: 1843     orrs     r0, r3 */
    r0 = r0 | r3;
    /* 0x08006A4C: 2369     ldr      r3, [r4, #0x10] */
    r3 = *(uint32_t *)(r4 + 16);
    /* 0x08006A4E: 022b     cmp      r3, #2 */
    /* flags = cmp(r3, #2) */
    /* 0x08006A50: 09d0     beq      #0x8006a66 */
    if (r3 == 2) goto L_08006A66;
    /* 0x08006A52: 0023     movs     r3, #0 */
    r3 = 0;

    L_08006A54:
    /* 0x08006A54: 1843     orrs     r0, r3 */
    r0 = r0 | r3;
    /* 0x08006A56: 236b     ldr      r3, [r4, #0x30] */
    r3 = *(uint32_t *)(r4 + 48);
    /* 0x08006A58: 666a     ldr      r6, [r4, #0x24] */
    r6 = *(uint32_t *)(r4 + 36);
    /* 0x08006A5A: 5b00     lsls     r3, r3, #1 */
    r3 = r3 << 1;
    /* 0x08006A5C: 1843     orrs     r0, r3 */
    r0 = r0 | r3;
    /* 0x08006A5E: 0123     movs     r3, #1 */
    r3 = 1;
    /* 0x08006A60: 012e     cmp      r6, #1 */
    /* flags = cmp(r6, #1) */
    /* 0x08006A62: 02d0     beq      #0x8006a6a */
    if (r6 == 1) goto L_08006A6A;
    /* 0x08006A64: 0ae0     b        #0x8006a7c */
    goto L_08006A7C;

    L_08006A66:
    /* 0x08006A66: 0423     movs     r3, #4 */
    r3 = 4;
    /* 0x08006A68: f4e7     b        #0x8006a54 */
    goto L_08006A54;   /* loop */

    L_08006A6A:
    /* 0x08006A6A: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x08006A6C: 27d0     beq      #0x8006abe */
    if (r1 == 0) goto L_08006ABE;
    /* 0x08006A6E: 616c     ldr      r1, [r4, #0x44] */
    r1 = *(uint32_t *)(r4 + 68);
    /* 0x08006A70: 2026     movs     r6, #0x20 */
    r6 = 0x20;
    /* 0x08006A72: 3143     orrs     r1, r6 */
    r1 = r1 | r6;
    /* 0x08006A74: 6164     str      r1, [r4, #0x44] */
    *(uint32_t *)(r4 + 68) = r1;   /* store */
    /* 0x08006A76: a16c     ldr      r1, [r4, #0x48] */
    r1 = *(uint32_t *)(r4 + 72);
    /* 0x08006A78: 1943     orrs     r1, r3 */
    r1 = r1 | r3;
    /* 0x08006A7A: a164     str      r1, [r4, #0x48] */
    *(uint32_t *)(r4 + 72) = r1;   /* store */

    L_08006A7C:
    /* 0x08006A7C: a16a     ldr      r1, [r4, #0x28] */
    r1 = *(uint32_t *)(r4 + 40);
    /* 0x08006A7E: 0e46     mov      r6, r1 */
    r6 = r1;
    /* 0x08006A80: ff3e     subs     r6, #0xff */
    r6 = r6 - 0xff;
    /* 0x08006A82: c23e     subs     r6, #0xc2 */
    r6 = r6 - 0xc2;
    /* 0x08006A84: 02d0     beq      #0x8006a8c */
    if (r1 == 0) goto L_08006A8C;
    /* 0x08006A86: e66a     ldr      r6, [r4, #0x2c] */
    r6 = *(uint32_t *)(r4 + 44);
    /* 0x08006A88: 3143     orrs     r1, r6 */
    r1 = r1 | r6;
    /* 0x08006A8A: 0843     orrs     r0, r1 */
    r0 = r0 | r1;

    L_08006A8C:
    /* 0x08006A8C: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);
    /* 0x08006A8E: ce68     ldr      r6, [r1, #0xc] */
    r6 = *(uint32_t *)(r1 + 12);  /* MMIO: ADC.+0x30C(r) */
    /* 0x08006A90: 0643     orrs     r6, r0 */
    r6 = r6 | r0;
    /* 0x08006A92: ce60     str      r6, [r1, #0xc] */
    *(uint32_t *)(r1 + 12) = r6;   /* store */  /* MMIO: ADC.+0x30C(w) */
    /* 0x08006A94: 0126     movs     r6, #1 */
    r6 = 1;
    /* 0x08006A96: a16b     ldr      r1, [r4, #0x38] */
    r1 = *(uint32_t *)(r4 + 56);
    /* 0x08006A98: 3607     lsls     r6, r6, #0x1c */
    r6 = r6 << 0x1c;
    /* 0x08006A9A: b142     cmp      r1, r6 */
    /* flags = cmp(r1, r6) */
    /* 0x08006A9C: 13d0     beq      #0x8006ac6 */
    if (r1 == r6) goto L_08006AC6;
    /* 0x08006A9E: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x08006AA0: 11d0     beq      #0x8006ac6 */
    if (r1 == 1) goto L_08006AC6;
    /* 0x08006AA2: 0229     cmp      r1, #2 */
    /* flags = cmp(r1, #2) */
    /* 0x08006AA4: 0fd0     beq      #0x8006ac6 */
    if (r1 == 2) goto L_08006AC6;
    /* 0x08006AA6: 0329     cmp      r1, #3 */
    /* flags = cmp(r1, #3) */
    /* 0x08006AA8: 0dd0     beq      #0x8006ac6 */
    if (r1 == 3) goto L_08006AC6;
    /* 0x08006AAA: 0429     cmp      r1, #4 */
    /* flags = cmp(r1, #4) */
    /* 0x08006AAC: 0bd0     beq      #0x8006ac6 */
    if (r1 == 4) goto L_08006AC6;
    /* 0x08006AAE: 0529     cmp      r1, #5 */
    /* flags = cmp(r1, #5) */
    /* 0x08006AB0: 09d0     beq      #0x8006ac6 */
    if (r1 == 5) goto L_08006AC6;
    /* 0x08006AB2: 0629     cmp      r1, #6 */
    /* flags = cmp(r1, #6) */
    /* 0x08006AB4: 07d0     beq      #0x8006ac6 */
    if (r1 == 6) goto L_08006AC6;
    /* 0x08006AB6: 0729     cmp      r1, #7 */
    /* flags = cmp(r1, #7) */
    /* 0x08006AB8: 05d0     beq      #0x8006ac6 */
    if (r1 == 7) goto L_08006AC6;
    /* 0x08006ABA: 10e0     b        #0x8006ade */
    goto L_08006ADE;

    L_08006ABC:
    /* 0x08006ABC: 28e0     b        #0x8006b10 */
    goto L_08006B10;

    L_08006ABE:
    /* 0x08006ABE: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08006AC0: 0904     lsls     r1, r1, #0x10 */
    r1 = r1 << 0x10;
    /* 0x08006AC2: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08006AC4: dae7     b        #0x8006a7c */
    goto L_08006A7C;   /* loop */

    L_08006AC6:
    /* 0x08006AC6: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);
    /* 0x08006AC8: 4e69     ldr      r6, [r1, #0x14] */
    r6 = *(uint32_t *)(r1 + 20);  /* MMIO: ADC.+0x314(r) */
    /* 0x08006ACA: f608     lsrs     r6, r6, #3 */
    r6 = r6 >> 3;
    /* 0x08006ACC: f600     lsls     r6, r6, #3 */
    r6 = r6 << 3;
    /* 0x08006ACE: 4e61     str      r6, [r1, #0x14] */
    *(uint32_t *)(r1 + 20) = r6;   /* store */  /* MMIO: ADC.+0x314(w) */
    /* 0x08006AD0: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);
    /* 0x08006AD2: 4e69     ldr      r6, [r1, #0x14] */
    r6 = *(uint32_t *)(r1 + 20);  /* MMIO: ADC.+0x314(r) */
    /* 0x08006AD4: 278f     ldrh     r7, [r4, #0x38] */
    r7 = *(uint16_t *)(r4 + 56);
    /* 0x08006AD6: 7f07     lsls     r7, r7, #0x1d */
    r7 = r7 << 0x1d;
    /* 0x08006AD8: 7f0f     lsrs     r7, r7, #0x1d */
    r7 = r7 >> 0x1d;
    /* 0x08006ADA: 3e43     orrs     r6, r7 */
    r6 = r6 | r7;
    /* 0x08006ADC: 4e61     str      r6, [r1, #0x14] */
    *(uint32_t *)(r1 + 20) = r6;   /* store */  /* MMIO: ADC.+0x314(w) */

    L_08006ADE:
    /* 0x08006ADE: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);
    /* 0x08006AE0: c968     ldr      r1, [r1, #0xc] */
    r1 = *(uint32_t *)(r1 + 12);  /* MMIO: ADC.+0x30C(r) */
    /* 0x08006AE2: 0e4e     ldr      r6, [pc, #0x38]  ; =0x833FFFE7 */
    r6 = *(uint32_t *)0x08006B1C;   /* = 0x833FFFE7 */
    /* 0x08006AE4: 3140     ands     r1, r6 */
    r1 = r1 & r6;
    /* 0x08006AE6: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x08006AE8: 07d1     bne      #0x8006afa */
    if (r1 != r0) goto L_08006AFA;
    /* 0x08006AEA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006AEC: a064     str      r0, [r4, #0x48] */
    *(uint32_t *)(r4 + 72) = r0;   /* store */
    /* 0x08006AEE: 606c     ldr      r0, [r4, #0x44] */
    r0 = *(uint32_t *)(r4 + 68);
    /* 0x08006AF0: 8008     lsrs     r0, r0, #2 */
    r0 = r0 >> 2;
    /* 0x08006AF2: 8000     lsls     r0, r0, #2 */
    r0 = r0 << 2;
    /* 0x08006AF4: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08006AF6: 6064     str      r0, [r4, #0x44] */
    *(uint32_t *)(r4 + 68) = r0;   /* store */
    /* 0x08006AF8: 08e0     b        #0x8006b0c */
    goto L_08006B0C;

    L_08006AFA:
    /* 0x08006AFA: 606c     ldr      r0, [r4, #0x44] */
    r0 = *(uint32_t *)(r4 + 68);
    /* 0x08006AFC: 0221     movs     r1, #2 */
    r1 = 2;
    /* 0x08006AFE: 8843     bics     r0, r1 */
    r0 = r0 & ~r1;
    /* 0x08006B00: 1043     orrs     r0, r2 */
    r0 = r0 | r2;
    /* 0x08006B02: 6064     str      r0, [r4, #0x44] */
    *(uint32_t *)(r4 + 68) = r0;   /* store */
    /* 0x08006B04: a06c     ldr      r0, [r4, #0x48] */
    r0 = *(uint32_t *)(r4 + 72);
    /* 0x08006B06: 1843     orrs     r0, r3 */
    r0 = r0 | r3;
    /* 0x08006B08: a064     str      r0, [r4, #0x48] */
    *(uint32_t *)(r4 + 72) = r0;   /* store */

    L_08006B0A:
    /* 0x08006B0A: 0125     movs     r5, #1 */
    r5 = 1;

    L_08006B0C:
    /* 0x08006B0C: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08006B0E: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_08006B10:
    /* 0x08006B10: 606c     ldr      r0, [r4, #0x44] */
    r0 = *(uint32_t *)(r4 + 68);
    /* 0x08006B12: 1043     orrs     r0, r2 */
    r0 = r0 | r2;
    /* 0x08006B14: 6064     str      r0, [r4, #0x44] */
    *(uint32_t *)(r4 + 68) = r0;   /* store */
    /* 0x08006B16: f8e7     b        #0x8006b0a */
    goto L_08006B0A;   /* loop */
}
