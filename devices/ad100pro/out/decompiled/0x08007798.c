/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007798
 * Name      : FUN_08007798
 * End       : 0x080078D8
 * Size      : 320 bytes, 153 instructions decoded in body (functions.json insns=153, blocks=35)
 * Class     : function
 * Callers   : 1  -> 0x0800B8F0 FUN_0800B8F0
 * Callees   : 3  -> 0x08007434 delay_or_timing, 0x080075EC FUN_080075EC, 0x080078F8 FUN_080078F8
 * MMIO      : FLASH_IF.ACR, RCC.CFGR, RCC.CIR, RCC.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007798(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007798: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x0800779A: 0546     mov      r5, r0 */
    r5 = r0;
    /* 0x0800779C: 4e48     ldr      r0, [pc, #0x138]  ; =FLASH_IF.ACR */
    r0 = *(uint32_t *)0x080078D8;   /* = MMIO FLASH+0x00 */
    /* 0x0800779E: 0e46     mov      r6, r1 */
    r6 = r1;
    /* 0x080077A0: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);  /* MMIO: FLASH_IF.ACR(r) */
    /* 0x080077A2: c007     lsls     r0, r0, #0x1f */
    r0 = r0 << 0x1f;
    /* 0x080077A4: c00f     lsrs     r0, r0, #0x1f */
    r0 = r0 >> 0x1f;
    /* 0x080077A6: b042     cmp      r0, r6 */
    /* flags = cmp(r0, r6) */
    /* 0x080077A8: 0ad2     bhs      #0x80077c0 */
    if (r0 >= r6) goto L_080077C0;   /* unsigned */
    /* 0x080077AA: 4b48     ldr      r0, [pc, #0x12c]  ; =FLASH_IF.ACR */
    r0 = *(uint32_t *)0x080078D8;   /* = MMIO FLASH+0x00 */
    /* 0x080077AC: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: FLASH_IF.ACR(r) */
    /* 0x080077AE: 4908     lsrs     r1, r1, #1 */
    r1 = r1 >> 1;
    /* 0x080077B0: 4900     lsls     r1, r1, #1 */
    r1 = r1 << 1;
    /* 0x080077B2: 3143     orrs     r1, r6 */
    r1 = r1 | r6;
    /* 0x080077B4: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: FLASH_IF.ACR(w) */
    /* 0x080077B6: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);  /* MMIO: FLASH_IF.ACR(r) */
    /* 0x080077B8: c007     lsls     r0, r0, #0x1f */
    r0 = r0 << 0x1f;
    /* 0x080077BA: c00f     lsrs     r0, r0, #0x1f */
    r0 = r0 >> 0x1f;
    /* 0x080077BC: b042     cmp      r0, r6 */
    /* flags = cmp(r0, r6) */
    /* 0x080077BE: 70d1     bne      #0x80078a2 */
    if (r0 != r6) goto L_080078A2;

    L_080077C0:
    /* 0x080077C0: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x080077C2: 464c     ldr      r4, [pc, #0x118]  ; =RCC.CR */
    r4 = *(uint32_t *)0x080078DC;   /* = MMIO RCC+0x00 */
    /* 0x080077C4: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x080077C6: 05d5     bpl      #0x80077d4 */
    if (r0 >= r6) goto L_080077D4;   /* result non-negative */
    /* 0x080077C8: 6068     ldr      r0, [r4, #4] */
    r0 = *(uint32_t *)(r4 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x080077CA: f021     movs     r1, #0xf0 */
    r1 = 0xf0;
    /* 0x080077CC: 8843     bics     r0, r1 */
    r0 = r0 & ~r1;
    /* 0x080077CE: a968     ldr      r1, [r5, #8] */
    r1 = *(uint32_t *)(r5 + 8);  /* MMIO: RCC.CIR(r) */
    /* 0x080077D0: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x080077D2: 6060     str      r0, [r4, #4] */
    *(uint32_t *)(r4 + 4) = r0;   /* store */  /* MMIO: RCC.CFGR(w) */

    L_080077D4:
    /* 0x080077D4: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x080077D6: c007     lsls     r0, r0, #0x1f */
    r0 = r0 << 0x1f;
    /* 0x080077D8: 53d0     beq      #0x8007882 */
    if (r0 == r6) goto L_08007882;
    /* 0x080077DA: 6868     ldr      r0, [r5, #4] */
    r0 = *(uint32_t *)(r5 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x080077DC: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x080077DE: 17d0     beq      #0x8007810 */
    if (r0 == 1) goto L_08007810;
    /* 0x080077E0: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x080077E2: 18d0     beq      #0x8007816 */
    if (r0 == 2) goto L_08007816;
    /* 0x080077E4: 0328     cmp      r0, #3 */
    /* flags = cmp(r0, #3) */
    /* 0x080077E6: 19d0     beq      #0x800781c */
    if (r0 == 3) goto L_0800781C;
    /* 0x080077E8: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x080077EA: 8907     lsls     r1, r1, #0x1e */
    r1 = r1 << 0x1e;

    L_080077EC:
    /* 0x080077EC: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x080077EE: 58da     bge      #0x80078a2 */
    if (r1 >= 0) goto L_080078A2;   /* signed */
    /* 0x080077F0: 6168     ldr      r1, [r4, #4] */
    r1 = *(uint32_t *)(r4 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x080077F2: 8908     lsrs     r1, r1, #2 */
    r1 = r1 >> 2;
    /* 0x080077F4: 8900     lsls     r1, r1, #2 */
    r1 = r1 << 2;
    /* 0x080077F6: 0143     orrs     r1, r0 */
    r1 = r1 | r0;
    /* 0x080077F8: 6160     str      r1, [r4, #4] */
    *(uint32_t *)(r4 + 4) = r1;   /* store */  /* MMIO: RCC.CFGR(w) */
    /* 0x080077FA: fff71bfe bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x080077FE: 6968     ldr      r1, [r5, #4] */
    r1 = *(uint32_t *)(r5 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x08007800: 0746     mov      r7, r0 */
    r7 = r0;
    /* 0x08007802: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x08007804: 13d0     beq      #0x800782e */
    if (r1 == 1) goto L_0800782E;
    /* 0x08007806: 0229     cmp      r1, #2 */
    /* flags = cmp(r1, #2) */
    /* 0x08007808: 1dd0     beq      #0x8007846 */
    if (r1 == 2) goto L_08007846;
    /* 0x0800780A: 0329     cmp      r1, #3 */
    /* flags = cmp(r1, #3) */
    /* 0x0800780C: 27d0     beq      #0x800785e */
    if (r1 == 3) goto L_0800785E;
    /* 0x0800780E: 34e0     b        #0x800787a */
    goto L_0800787A;

    L_08007810:
    /* 0x08007810: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007812: 8903     lsls     r1, r1, #0xe */
    r1 = r1 << 0xe;
    /* 0x08007814: eae7     b        #0x80077ec */
    goto L_080077EC;   /* loop */

    L_08007816:
    /* 0x08007816: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007818: 8901     lsls     r1, r1, #6 */
    r1 = r1 << 6;
    /* 0x0800781A: e7e7     b        #0x80077ec */
    goto L_080077EC;   /* loop */

    L_0800781C:
    /* 0x0800781C: 616b     ldr      r1, [r4, #0x34] */
    r1 = *(uint32_t *)(r4 + 52);  /* MMIO: RCC.CR2(r) */
    /* 0x0800781E: c903     lsls     r1, r1, #0xf */
    r1 = r1 << 0xf;
    /* 0x08007820: e4e7     b        #0x80077ec */
    goto L_080077EC;   /* loop */

    L_08007822:
    /* 0x08007822: fff707fe bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007826: c11b     subs     r1, r0, r7 */
    r1 = r0 - r7;
    /* 0x08007828: 2d48     ldr      r0, [pc, #0xb4]  ; =0x00001388 */
    r0 = *(uint32_t *)0x080078E0;   /* = 0x00001388 */
    /* 0x0800782A: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x0800782C: 23d8     bhi      #0x8007876 */
    if (r1 > r0) goto L_08007876;   /* unsigned */

    L_0800782E:
    /* 0x0800782E: 6068     ldr      r0, [r4, #4] */
    r0 = *(uint32_t *)(r4 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x08007830: 0007     lsls     r0, r0, #0x1c */
    r0 = r0 << 0x1c;
    /* 0x08007832: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x08007834: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08007836: f4d1     bne      #0x8007822 */
    if (r0 != 1) goto L_08007822;
    /* 0x08007838: 23e0     b        #0x8007882 */
    goto L_08007882;

    L_0800783A:
    /* 0x0800783A: fff7fbfd bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x0800783E: c11b     subs     r1, r0, r7 */
    r1 = r0 - r7;
    /* 0x08007840: 2748     ldr      r0, [pc, #0x9c]  ; =0x00001388 */
    r0 = *(uint32_t *)0x080078E0;   /* = 0x00001388 */
    /* 0x08007842: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x08007844: 17d8     bhi      #0x8007876 */
    if (r1 > r0) goto L_08007876;   /* unsigned */

    L_08007846:
    /* 0x08007846: 6068     ldr      r0, [r4, #4] */
    r0 = *(uint32_t *)(r4 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x08007848: 0007     lsls     r0, r0, #0x1c */
    r0 = r0 << 0x1c;
    /* 0x0800784A: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x0800784C: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x0800784E: f4d1     bne      #0x800783a */
    if (r0 != 2) goto L_0800783A;
    /* 0x08007850: 17e0     b        #0x8007882 */
    goto L_08007882;

    L_08007852:
    /* 0x08007852: fff7effd bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007856: c11b     subs     r1, r0, r7 */
    r1 = r0 - r7;
    /* 0x08007858: 2148     ldr      r0, [pc, #0x84]  ; =0x00001388 */
    r0 = *(uint32_t *)0x080078E0;   /* = 0x00001388 */
    /* 0x0800785A: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x0800785C: 0bd8     bhi      #0x8007876 */
    if (r1 > r0) goto L_08007876;   /* unsigned */

    L_0800785E:
    /* 0x0800785E: 6068     ldr      r0, [r4, #4] */
    r0 = *(uint32_t *)(r4 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x08007860: 0007     lsls     r0, r0, #0x1c */
    r0 = r0 << 0x1c;
    /* 0x08007862: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x08007864: 0328     cmp      r0, #3 */
    /* flags = cmp(r0, #3) */
    /* 0x08007866: f4d1     bne      #0x8007852 */
    if (r0 != 3) goto L_08007852;
    /* 0x08007868: 0be0     b        #0x8007882 */
    goto L_08007882;

    L_0800786A:
    /* 0x0800786A: fff7e3fd bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x0800786E: c11b     subs     r1, r0, r7 */
    r1 = r0 - r7;
    /* 0x08007870: 1b48     ldr      r0, [pc, #0x6c]  ; =0x00001388 */
    r0 = *(uint32_t *)0x080078E0;   /* = 0x00001388 */
    /* 0x08007872: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x08007874: 01d9     bls      #0x800787a */
    if (r1 <= r0) goto L_0800787A;   /* unsigned */

    L_08007876:
    /* 0x08007876: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x08007878: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_0800787A:
    /* 0x0800787A: 6068     ldr      r0, [r4, #4] */
    r0 = *(uint32_t *)(r4 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x0800787C: 0007     lsls     r0, r0, #0x1c */
    r0 = r0 << 0x1c;
    /* 0x0800787E: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x08007880: f3d1     bne      #0x800786a */
    if (r1 != r0) goto L_0800786A;

    L_08007882:
    /* 0x08007882: 1548     ldr      r0, [pc, #0x54]  ; =FLASH_IF.ACR */
    r0 = *(uint32_t *)0x080078D8;   /* = MMIO FLASH+0x00 */
    /* 0x08007884: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: FLASH_IF.ACR(r) */
    /* 0x08007886: c907     lsls     r1, r1, #0x1f */
    r1 = r1 << 0x1f;
    /* 0x08007888: c90f     lsrs     r1, r1, #0x1f */
    r1 = r1 >> 0x1f;
    /* 0x0800788A: b142     cmp      r1, r6 */
    /* flags = cmp(r1, r6) */
    /* 0x0800788C: 0bd9     bls      #0x80078a6 */
    if (r1 <= r6) goto L_080078A6;   /* unsigned */
    /* 0x0800788E: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: FLASH_IF.ACR(r) */
    /* 0x08007890: 4908     lsrs     r1, r1, #1 */
    r1 = r1 >> 1;
    /* 0x08007892: 4900     lsls     r1, r1, #1 */
    r1 = r1 << 1;
    /* 0x08007894: 3143     orrs     r1, r6 */
    r1 = r1 | r6;
    /* 0x08007896: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: FLASH_IF.ACR(w) */
    /* 0x08007898: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);  /* MMIO: FLASH_IF.ACR(r) */
    /* 0x0800789A: c007     lsls     r0, r0, #0x1f */
    r0 = r0 << 0x1f;
    /* 0x0800789C: c00f     lsrs     r0, r0, #0x1f */
    r0 = r0 >> 0x1f;
    /* 0x0800789E: b042     cmp      r0, r6 */
    /* flags = cmp(r0, r6) */
    /* 0x080078A0: 01d0     beq      #0x80078a6 */
    if (r0 == r6) goto L_080078A6;

    L_080078A2:
    /* 0x080078A2: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x080078A4: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_080078A6:
    /* 0x080078A6: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x080078A8: 4007     lsls     r0, r0, #0x1d */
    r0 = r0 << 0x1d;
    /* 0x080078AA: 06d5     bpl      #0x80078ba */
    if (r0 >= r6) goto L_080078BA;   /* result non-negative */
    /* 0x080078AC: 6068     ldr      r0, [r4, #4] */
    r0 = *(uint32_t *)(r4 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x080078AE: 0721     movs     r1, #7 */
    r1 = 7;
    /* 0x080078B0: 0902     lsls     r1, r1, #8 */
    r1 = r1 << 8;
    /* 0x080078B2: 8843     bics     r0, r1 */
    r0 = r0 & ~r1;
    /* 0x080078B4: e968     ldr      r1, [r5, #0xc] */
    r1 = *(uint32_t *)(r5 + 12);  /* MMIO: RCC.APB2RSTR(r) */
    /* 0x080078B6: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x080078B8: 6060     str      r0, [r4, #4] */
    *(uint32_t *)(r4 + 4) = r0;   /* store */  /* MMIO: RCC.CFGR(w) */

    L_080078BA:
    /* 0x080078BA: 00f01df8 bl       #0x80078f8 */
    FUN_080078F8();   /* BL -> 0x080078F8 FUN_080078F8 */
    /* 0x080078BE: 6168     ldr      r1, [r4, #4] */
    r1 = *(uint32_t *)(r4 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x080078C0: 084a     ldr      r2, [pc, #0x20]  ; =flash_0800D164 */
    r2 = *(uint32_t *)0x080078E4;   /* = flash data/const 0x0800D164 */
    /* 0x080078C2: 0906     lsls     r1, r1, #0x18 */
    r1 = r1 << 0x18;
    /* 0x080078C4: 090f     lsrs     r1, r1, #0x1c */
    r1 = r1 >> 0x1c;
    /* 0x080078C6: 515c     ldrb     r1, [r2, r1] */
    r1 = *(uint8_t *)(r2 + r1);
    /* 0x080078C8: c840     lsrs     r0, r1 */
    r0 = r0 >> r1;
    /* 0x080078CA: 0749     ldr      r1, [pc, #0x1c]  ; =RAM_20000100 */
    r1 = *(uint32_t *)0x080078E8;   /* = RAM 0x20000100 */
    /* 0x080078CC: 0860     str      r0, [r1] */
    *(uint32_t *)(r1) = r0;   /* store */
    /* 0x080078CE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080078D0: fff78cfe bl       #0x80075ec */
    FUN_080075EC();   /* BL -> 0x080075EC FUN_080075EC */
    /* 0x080078D4: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080078D6: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
