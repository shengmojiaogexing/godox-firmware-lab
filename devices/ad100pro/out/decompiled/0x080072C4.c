/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080072C4
 * Name      : FUN_080072C4
 * End       : 0x080073FE
 * Size      : 314 bytes, 157 instructions decoded in body (functions.json insns=157, blocks=37)
 * Class     : hub
 * Callers   : 28  -> 0x0800655C FUN_0800655C, 0x08006584 FUN_08006584, 0x080065AC FUN_080065AC, 0x08006B20 FUN_08006B20, 0x08006D18 FUN_08006D18, 0x0800854C FUN_0800854C, 0x08008A2C FUN_08008A2C, 0x08008A3A FUN_08008A3A, ... +20
 * Callees   : 0  -> -
 * MMIO      : DMA1.CH1_CCR, DMA1.CH2_IFCR, EXTI.EMR, EXTI.FTSR, EXTI.IMR, EXTI.RTSR, EXTI.SWIER, FLASH_IF.ACR, FLASH_IF.KEYR, FLASH_IF.OPTKEYR, FLASH_IF.SR, RCC.APB2ENR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080072C4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080072C4: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x080072C6: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x080072C8: 93e0     b        #0x80073f2 */
    goto L_080073F2;

    L_080072CA:
    /* 0x080072CA: 2246     mov      r2, r4 */
    r2 = r4;
    /* 0x080072CC: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x080072CE: 9c40     lsls     r4, r3 */
    r4 = r4 << r3;
    /* 0x080072D0: a446     mov      ip, r4 */
    ip = r4;
    /* 0x080072D2: 2240     ands     r2, r4 */
    r2 = r2 & r4;
    /* 0x080072D4: 7ed0     beq      #0x80073d4 */
    if (/* beq */) goto L_080073D4;
    /* 0x080072D6: 4c68     ldr      r4, [r1, #4] */
    r4 = *(uint32_t *)(r1 + 4);  /* MMIO: EXTI.EMR(r) */
    /* 0x080072D8: 022c     cmp      r4, #2 */
    /* flags = cmp(r4, #2) */
    /* 0x080072DA: 01d0     beq      #0x80072e0 */
    if (r4 == 2) goto L_080072E0;
    /* 0x080072DC: 122c     cmp      r4, #0x12 */
    /* flags = cmp(r4, #0x12) */
    /* 0x080072DE: 0cd1     bne      #0x80072fa */
    if (r4 != 0x12) goto L_080072FA;

    L_080072E0:
    /* 0x080072E0: dc08     lsrs     r4, r3, #3 */
    r4 = r3 >> 3;
    /* 0x080072E2: a400     lsls     r4, r4, #2 */
    r4 = r4 << 2;
    /* 0x080072E4: 2718     adds     r7, r4, r0 */
    r7 = r4 + r0;
    /* 0x080072E6: 3c6a     ldr      r4, [r7, #0x20] */
    r4 = *(uint32_t *)(r7 + 32);  /* MMIO: DMA1.CH2_IFCR(r) */
    /* 0x080072E8: 5d07     lsls     r5, r3, #0x1d */
    r5 = r3 << 0x1d;
    /* 0x080072EA: ed0e     lsrs     r5, r5, #0x1b */
    r5 = r5 >> 0x1b;
    /* 0x080072EC: 0f26     movs     r6, #0xf */
    r6 = 0xf;
    /* 0x080072EE: ae40     lsls     r6, r5 */
    r6 = r6 << r5;
    /* 0x080072F0: b443     bics     r4, r6 */
    r4 = r4 & ~r6;
    /* 0x080072F2: 0e69     ldr      r6, [r1, #0x10] */
    r6 = *(uint32_t *)(r1 + 16);  /* MMIO: EXTI.SWIER(r) */
    /* 0x080072F4: ae40     lsls     r6, r5 */
    r6 = r6 << r5;
    /* 0x080072F6: 2643     orrs     r6, r4 */
    r6 = r6 | r4;
    /* 0x080072F8: 3e62     str      r6, [r7, #0x20] */
    *(uint32_t *)(r7 + 32) = r6;   /* store */  /* MMIO: DMA1.CH2_IFCR(w) */

    L_080072FA:
    /* 0x080072FA: 0568     ldr      r5, [r0] */
    r5 = *(uint32_t *)(r0);  /* MMIO: FLASH_IF.ACR(r) */
    /* 0x080072FC: 5e00     lsls     r6, r3, #1 */
    r6 = r3 << 1;
    /* 0x080072FE: 0324     movs     r4, #3 */
    r4 = 3;
    /* 0x08007300: b440     lsls     r4, r6 */
    r4 = r4 << r6;
    /* 0x08007302: 0f79     ldrb     r7, [r1, #4] */
    r7 = *(uint8_t *)(r1 + 4);  /* MMIO: EXTI.EMR(r) */
    /* 0x08007304: a543     bics     r5, r4 */
    r5 = r5 & ~r4;
    /* 0x08007306: bf07     lsls     r7, r7, #0x1e */
    r7 = r7 << 0x1e;
    /* 0x08007308: bf0f     lsrs     r7, r7, #0x1e */
    r7 = r7 >> 0x1e;
    /* 0x0800730A: b740     lsls     r7, r6 */
    r7 = r7 << r6;
    /* 0x0800730C: 2f43     orrs     r7, r5 */
    r7 = r7 | r5;
    /* 0x0800730E: 0760     str      r7, [r0] */
    *(uint32_t *)(r0) = r7;   /* store */  /* MMIO: FLASH_IF.ACR(w) */
    /* 0x08007310: 4d68     ldr      r5, [r1, #4] */
    r5 = *(uint32_t *)(r1 + 4);  /* MMIO: EXTI.EMR(r) */
    /* 0x08007312: 012d     cmp      r5, #1 */
    /* flags = cmp(r5, #1) */
    /* 0x08007314: 05d0     beq      #0x8007322 */
    if (r5 == 1) goto L_08007322;
    /* 0x08007316: 022d     cmp      r5, #2 */
    /* flags = cmp(r5, #2) */
    /* 0x08007318: 03d0     beq      #0x8007322 */
    if (r5 == 2) goto L_08007322;
    /* 0x0800731A: 112d     cmp      r5, #0x11 */
    /* flags = cmp(r5, #0x11) */
    /* 0x0800731C: 01d0     beq      #0x8007322 */
    if (r5 == 0x11) goto L_08007322;
    /* 0x0800731E: 122d     cmp      r5, #0x12 */
    /* flags = cmp(r5, #0x12) */
    /* 0x08007320: 0ed1     bne      #0x8007340 */
    if (r5 != 0x12) goto L_08007340;

    L_08007322:
    /* 0x08007322: 8568     ldr      r5, [r0, #8] */
    r5 = *(uint32_t *)(r0 + 8);  /* MMIO: FLASH_IF.OPTKEYR(r) */
    /* 0x08007324: cf68     ldr      r7, [r1, #0xc] */
    r7 = *(uint32_t *)(r1 + 12);  /* MMIO: EXTI.FTSR(r) */
    /* 0x08007326: a543     bics     r5, r4 */
    r5 = r5 & ~r4;
    /* 0x08007328: b740     lsls     r7, r6 */
    r7 = r7 << r6;
    /* 0x0800732A: 2f43     orrs     r7, r5 */
    r7 = r7 | r5;
    /* 0x0800732C: 8760     str      r7, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r7;   /* store */  /* MMIO: FLASH_IF.OPTKEYR(w) */
    /* 0x0800732E: 4768     ldr      r7, [r0, #4] */
    r7 = *(uint32_t *)(r0 + 4);  /* MMIO: FLASH_IF.KEYR(r) */
    /* 0x08007330: 6546     mov      r5, ip */
    r5 = ip;
    /* 0x08007332: af43     bics     r7, r5 */
    r7 = r7 & ~r5;
    /* 0x08007334: 0d79     ldrb     r5, [r1, #4] */
    r5 = *(uint8_t *)(r1 + 4);  /* MMIO: EXTI.EMR(r) */
    /* 0x08007336: ed06     lsls     r5, r5, #0x1b */
    r5 = r5 << 0x1b;
    /* 0x08007338: ed0f     lsrs     r5, r5, #0x1f */
    r5 = r5 >> 0x1f;
    /* 0x0800733A: 9d40     lsls     r5, r3 */
    r5 = r5 << r3;
    /* 0x0800733C: 3d43     orrs     r5, r7 */
    r5 = r5 | r7;
    /* 0x0800733E: 4560     str      r5, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r5;   /* store */  /* MMIO: FLASH_IF.KEYR(w) */

    L_08007340:
    /* 0x08007340: c568     ldr      r5, [r0, #0xc] */
    r5 = *(uint32_t *)(r0 + 12);  /* MMIO: FLASH_IF.SR(r) */
    /* 0x08007342: a543     bics     r5, r4 */
    r5 = r5 & ~r4;
    /* 0x08007344: 8c68     ldr      r4, [r1, #8] */
    r4 = *(uint32_t *)(r1 + 8);  /* MMIO: EXTI.RTSR(r) */
    /* 0x08007346: b440     lsls     r4, r6 */
    r4 = r4 << r6;
    /* 0x08007348: 2c43     orrs     r4, r5 */
    r4 = r4 | r5;
    /* 0x0800734A: c460     str      r4, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r4;   /* store */  /* MMIO: FLASH_IF.SR(w) */
    /* 0x0800734C: 4c68     ldr      r4, [r1, #4] */
    r4 = *(uint32_t *)(r1 + 4);  /* MMIO: EXTI.EMR(r) */
    /* 0x0800734E: e400     lsls     r4, r4, #3 */
    r4 = r4 << 3;
    /* 0x08007350: 4ed5     bpl      #0x80073f0 */
    if (r5 >= 0x12) goto L_080073F0;   /* result non-negative */
    /* 0x08007352: 2b4c     ldr      r4, [pc, #0xac]  ; =RCC.CR */
    r4 = *(uint32_t *)0x08007400;   /* = MMIO RCC+0x00 */
    /* 0x08007354: a569     ldr      r5, [r4, #0x18] */
    r5 = *(uint32_t *)(r4 + 24);  /* MMIO: RCC.APB2ENR(r) */
    /* 0x08007356: 0126     movs     r6, #1 */
    r6 = 1;
    /* 0x08007358: 3543     orrs     r5, r6 */
    r5 = r5 | r6;
    /* 0x0800735A: a561     str      r5, [r4, #0x18] */
    *(uint32_t *)(r4 + 24) = r5;   /* store */  /* MMIO: RCC.APB2ENR(w) */
    /* 0x0800735C: a469     ldr      r4, [r4, #0x18] */
    r4 = *(uint32_t *)(r4 + 24);  /* MMIO: RCC.APB2ENR(r) */
    /* 0x0800735E: 9d08     lsrs     r5, r3, #2 */
    r5 = r3 >> 2;
    /* 0x08007360: 3440     ands     r4, r6 */
    r4 = r4 & r6;
    /* 0x08007362: 0094     str      r4, [sp] */
    *(uint32_t *)(sp) = r4;   /* store */
    /* 0x08007364: 274c     ldr      r4, [pc, #0x9c]  ; =SYSCFG.CFGR1 */
    r4 = *(uint32_t *)0x08007404;   /* = MMIO SYSCFG+0x00 */
    /* 0x08007366: ad00     lsls     r5, r5, #2 */
    r5 = r5 << 2;
    /* 0x08007368: 2f19     adds     r7, r5, r4 */
    r7 = r5 + r4;
    /* 0x0800736A: bc68     ldr      r4, [r7, #8] */
    r4 = *(uint32_t *)(r7 + 8);  /* MMIO: DMA1.CH1_CCR(r) */
    /* 0x0800736C: 9d07     lsls     r5, r3, #0x1e */
    r5 = r3 << 0x1e;
    /* 0x0800736E: 2e0f     lsrs     r6, r5, #0x1c */
    r6 = r5 >> 0x1c;
    /* 0x08007370: 0f25     movs     r5, #0xf */
    r5 = 0xf;
    /* 0x08007372: b540     lsls     r5, r6 */
    r5 = r5 << r6;
    /* 0x08007374: ac43     bics     r4, r5 */
    r4 = r4 & ~r5;
    /* 0x08007376: 0925     movs     r5, #9 */
    r5 = 9;
    /* 0x08007378: ed06     lsls     r5, r5, #0x1b */
    r5 = r5 << 0x1b;
    /* 0x0800737A: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x0800737C: 01d1     bne      #0x8007382 */
    if (r0 != r5) goto L_08007382;
    /* 0x0800737E: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x08007380: 14e0     b        #0x80073ac */
    goto L_080073AC;

    L_08007382:
    /* 0x08007382: 214d     ldr      r5, [pc, #0x84]  ; =GPIOB.MODER */
    r5 = *(uint32_t *)0x08007408;   /* = MMIO GPIOB+0x00 */
    /* 0x08007384: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x08007386: 01d1     bne      #0x800738c */
    if (r0 != r5) goto L_0800738C;
    /* 0x08007388: 0125     movs     r5, #1 */
    r5 = 1;
    /* 0x0800738A: 0fe0     b        #0x80073ac */
    goto L_080073AC;

    L_0800738C:
    /* 0x0800738C: 1f4d     ldr      r5, [pc, #0x7c]  ; =GPIOC.MODER */
    r5 = *(uint32_t *)0x0800740C;   /* = MMIO GPIOC+0x00 */
    /* 0x0800738E: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x08007390: 01d1     bne      #0x8007396 */
    if (r0 != r5) goto L_08007396;
    /* 0x08007392: 0225     movs     r5, #2 */
    r5 = 2;
    /* 0x08007394: 0ae0     b        #0x80073ac */
    goto L_080073AC;

    L_08007396:
    /* 0x08007396: 1e4d     ldr      r5, [pc, #0x78]  ; =GPIOD.MODER */
    r5 = *(uint32_t *)0x08007410;   /* = MMIO GPIOD+0x00 */
    /* 0x08007398: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x0800739A: 01d1     bne      #0x80073a0 */
    if (r0 != r5) goto L_080073A0;
    /* 0x0800739C: 0325     movs     r5, #3 */
    r5 = 3;
    /* 0x0800739E: 05e0     b        #0x80073ac */
    goto L_080073AC;

    L_080073A0:
    /* 0x080073A0: 1c4d     ldr      r5, [pc, #0x70]  ; =GPIOE.MODER */
    r5 = *(uint32_t *)0x08007414;   /* = MMIO GPIOE+0x00 */
    /* 0x080073A2: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x080073A4: 01d1     bne      #0x80073aa */
    if (r0 != r5) goto L_080073AA;
    /* 0x080073A6: 0425     movs     r5, #4 */
    r5 = 4;
    /* 0x080073A8: 00e0     b        #0x80073ac */
    goto L_080073AC;

    L_080073AA:
    /* 0x080073AA: 0525     movs     r5, #5 */
    r5 = 5;

    L_080073AC:
    /* 0x080073AC: b540     lsls     r5, r6 */
    r5 = r5 << r6;
    /* 0x080073AE: 2543     orrs     r5, r4 */
    r5 = r5 | r4;
    /* 0x080073B0: bd60     str      r5, [r7, #8] */
    *(uint32_t *)(r7 + 8) = r5;   /* store */  /* MMIO: DMA1.CH1_CCR(w) */
    /* 0x080073B2: 194c     ldr      r4, [pc, #0x64]  ; =EXTI.IMR */
    r4 = *(uint32_t *)0x08007418;   /* = MMIO EXTI+0x00 */
    /* 0x080073B4: 2568     ldr      r5, [r4] */
    r5 = *(uint32_t *)(r4);  /* MMIO: EXTI.IMR(r) */
    /* 0x080073B6: 4e68     ldr      r6, [r1, #4] */
    r6 = *(uint32_t *)(r1 + 4);  /* MMIO: EXTI.EMR(r) */
    /* 0x080073B8: 9543     bics     r5, r2 */
    r5 = r5 & ~r2;
    /* 0x080073BA: f603     lsls     r6, r6, #0xf */
    r6 = r6 << 0xf;
    /* 0x080073BC: 00d5     bpl      #0x80073c0 */
    if (r0 >= r5) goto L_080073C0;   /* result non-negative */
    /* 0x080073BE: 1543     orrs     r5, r2 */
    r5 = r5 | r2;

    L_080073C0:
    /* 0x080073C0: 2560     str      r5, [r4] */
    *(uint32_t *)(r4) = r5;   /* store */  /* MMIO: EXTI.IMR(w) */
    /* 0x080073C2: 6568     ldr      r5, [r4, #4] */
    r5 = *(uint32_t *)(r4 + 4);  /* MMIO: EXTI.EMR(r) */
    /* 0x080073C4: 4e68     ldr      r6, [r1, #4] */
    r6 = *(uint32_t *)(r1 + 4);  /* MMIO: EXTI.EMR(r) */
    /* 0x080073C6: 9543     bics     r5, r2 */
    r5 = r5 & ~r2;
    /* 0x080073C8: b603     lsls     r6, r6, #0xe */
    r6 = r6 << 0xe;
    /* 0x080073CA: 00d5     bpl      #0x80073ce */
    if (r0 >= r5) goto L_080073CE;   /* result non-negative */
    /* 0x080073CC: 1543     orrs     r5, r2 */
    r5 = r5 | r2;

    L_080073CE:
    /* 0x080073CE: 6560     str      r5, [r4, #4] */
    *(uint32_t *)(r4 + 4) = r5;   /* store */  /* MMIO: EXTI.EMR(w) */
    /* 0x080073D0: a568     ldr      r5, [r4, #8] */
    r5 = *(uint32_t *)(r4 + 8);  /* MMIO: EXTI.RTSR(r) */
    /* 0x080073D2: 00e0     b        #0x80073d6 */
    goto L_080073D6;

    L_080073D4:
    /* 0x080073D4: 0ce0     b        #0x80073f0 */
    goto L_080073F0;

    L_080073D6:
    /* 0x080073D6: 4e68     ldr      r6, [r1, #4] */
    r6 = *(uint32_t *)(r1 + 4);  /* MMIO: EXTI.EMR(r) */
    /* 0x080073D8: 9543     bics     r5, r2 */
    r5 = r5 & ~r2;
    /* 0x080073DA: f602     lsls     r6, r6, #0xb */
    r6 = r6 << 0xb;
    /* 0x080073DC: 00d5     bpl      #0x80073e0 */
    if (r0 >= r5) goto L_080073E0;   /* result non-negative */
    /* 0x080073DE: 1543     orrs     r5, r2 */
    r5 = r5 | r2;

    L_080073E0:
    /* 0x080073E0: a560     str      r5, [r4, #8] */
    *(uint32_t *)(r4 + 8) = r5;   /* store */  /* MMIO: EXTI.RTSR(w) */
    /* 0x080073E2: e568     ldr      r5, [r4, #0xc] */
    r5 = *(uint32_t *)(r4 + 12);  /* MMIO: EXTI.FTSR(r) */
    /* 0x080073E4: 4e68     ldr      r6, [r1, #4] */
    r6 = *(uint32_t *)(r1 + 4);  /* MMIO: EXTI.EMR(r) */
    /* 0x080073E6: 9543     bics     r5, r2 */
    r5 = r5 & ~r2;
    /* 0x080073E8: b602     lsls     r6, r6, #0xa */
    r6 = r6 << 0xa;
    /* 0x080073EA: 00d5     bpl      #0x80073ee */
    if (r0 >= r5) goto L_080073EE;   /* result non-negative */
    /* 0x080073EC: 1543     orrs     r5, r2 */
    r5 = r5 | r2;

    L_080073EE:
    /* 0x080073EE: e560     str      r5, [r4, #0xc] */
    *(uint32_t *)(r4 + 12) = r5;   /* store */  /* MMIO: EXTI.FTSR(w) */

    L_080073F0:
    /* 0x080073F0: 5b1c     adds     r3, r3, #1 */
    r3 = r3 + 1;

    L_080073F2:
    /* 0x080073F2: 0a68     ldr      r2, [r1] */
    r2 = *(uint32_t *)(r1);  /* MMIO: EXTI.IMR(r) */
    /* 0x080073F4: 1446     mov      r4, r2 */
    r4 = r2;
    /* 0x080073F6: da40     lsrs     r2, r3 */
    r2 = r2 >> r3;
    /* 0x080073F8: 00d0     beq      #0x80073fc */
    if (r0 == r5) goto L_080073FC;
    /* 0x080073FA: 66e7     b        #0x80072ca */
    goto L_080072CA;   /* loop */

    L_080073FC:
    /* 0x080073FC: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
