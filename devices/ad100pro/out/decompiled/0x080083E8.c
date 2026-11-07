/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080083E8
 * Name      : FUN_080083E8
 * End       : 0x0800854A
 * Size      : 354 bytes, 161 instructions decoded in body (functions.json insns=161, blocks=37)
 * Class     : function
 * Callers   : 5  -> 0x0800BA44 IRQ19_Handler, 0x0800BA64 FUN_0800BA64, 0x0800BA7C IRQ13_Handler, 0x0800BA8C IRQ15_Handler, 0x0800BAA0 FUN_0800BAA0
 * Callees   : 7  -> 0x0800819C FUN_0800819C, 0x0800819E FUN_0800819E, 0x080083E6 FUN_080083E6, 0x08008580 FUN_08008580, 0x08008688 FUN_08008688, 0x080086DC FUN_080086DC, 0x080086DE FUN_080086DE
 * MMIO      : RCC.APB1RSTR, RCC.APB2ENR, RCC.APB2RSTR, RCC.CR, SYS.+0x0, SYSTICK.VAL
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080083E8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080083E8: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x080083EA: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x080083EC: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x080083EE: 0169     ldr      r1, [r0, #0x10] */
    r1 = *(uint32_t *)(r0 + 16);  /* MMIO: RCC.APB1RSTR(r) */
    /* 0x080083F0: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x080083F2: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x080083F4: 8907     lsls     r1, r1, #0x1e */
    r1 = r1 << 0x1e;
    /* 0x080083F6: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x080083F8: 15db     blt      #0x8008426 */
    if (r1 < 0) goto L_08008426;   /* signed */
    /* 0x080083FA: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);  /* MMIO: RCC.APB2RSTR(r) */
    /* 0x080083FC: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x080083FE: 8907     lsls     r1, r1, #0x1e */
    r1 = r1 << 0x1e;
    /* 0x08008400: 11d4     bmi      #0x8008426 */
    if (r1 < 0) goto L_08008426;   /* result negative */
    /* 0x08008402: e91e     subs     r1, r5, #3 */
    r1 = r5 - 3;
    /* 0x08008404: 0161     str      r1, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r1;   /* store */  /* MMIO: RCC.APB1RSTR(w) */
    /* 0x08008406: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08008408: 2076     strb     r0, [r4, #0x18] */
    *(uint8_t *)(r4 + 24) = r0;   /* store */  /* MMIO: SYSTICK.VAL(w) */
    /* 0x0800840A: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x0800840C: 8069     ldr      r0, [r0, #0x18] */
    r0 = *(uint32_t *)(r0 + 24);  /* MMIO: RCC.APB2ENR(r) */
    /* 0x0800840E: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08008410: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08008412: 02d0     beq      #0x800841a */
    if (r1 == 0) goto L_0800841A;
    /* 0x08008414: fff7e7ff bl       #0x80083e6 */
    FUN_080083E6();   /* BL -> 0x080083E6 FUN_080083E6 */
    /* 0x08008418: 04e0     b        #0x8008424 */
    goto L_08008424;

    L_0800841A:
    /* 0x0800841A: 00f0b1f8 bl       #0x8008580 */
    FUN_08008580();   /* BL -> 0x08008580 FUN_08008580 */
    /* 0x0800841E: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08008420: 00f032f9 bl       #0x8008688 */
    FUN_08008688();   /* BL -> 0x08008688 FUN_08008688 */

    L_08008424:
    /* 0x08008424: 2576     strb     r5, [r4, #0x18] */
    *(uint8_t *)(r4 + 24) = r5;   /* store */  /* MMIO: SYSTICK.VAL(w) */

    L_08008426:
    /* 0x08008426: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x08008428: 0169     ldr      r1, [r0, #0x10] */
    r1 = *(uint32_t *)(r0 + 16);  /* MMIO: RCC.APB1RSTR(r) */
    /* 0x0800842A: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x0800842C: 4907     lsls     r1, r1, #0x1d */
    r1 = r1 << 0x1d;
    /* 0x0800842E: 17d4     bmi      #0x8008460 */
    if (r1 < 0) goto L_08008460;   /* result negative */
    /* 0x08008430: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);  /* MMIO: RCC.APB2RSTR(r) */
    /* 0x08008432: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x08008434: 4907     lsls     r1, r1, #0x1d */
    r1 = r1 << 0x1d;
    /* 0x08008436: 13d4     bmi      #0x8008460 */
    if (r1 < 0) goto L_08008460;   /* result negative */
    /* 0x08008438: 0421     movs     r1, #4 */
    r1 = 4;
    /* 0x0800843A: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x0800843C: 0161     str      r1, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r1;   /* store */  /* MMIO: RCC.APB1RSTR(w) */
    /* 0x0800843E: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08008440: 2076     strb     r0, [r4, #0x18] */
    *(uint8_t *)(r4 + 24) = r0;   /* store */  /* MMIO: SYSTICK.VAL(w) */
    /* 0x08008442: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x08008444: 8069     ldr      r0, [r0, #0x18] */
    r0 = *(uint32_t *)(r0 + 24);  /* MMIO: RCC.APB2ENR(r) */
    /* 0x08008446: 8005     lsls     r0, r0, #0x16 */
    r0 = r0 << 0x16;
    /* 0x08008448: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x0800844A: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800844C: 02d0     beq      #0x8008454 */
    if (r1 == 0) goto L_08008454;
    /* 0x0800844E: fff7caff bl       #0x80083e6 */
    FUN_080083E6();   /* BL -> 0x080083E6 FUN_080083E6 */
    /* 0x08008452: 04e0     b        #0x800845e */
    goto L_0800845E;

    L_08008454:
    /* 0x08008454: 00f094f8 bl       #0x8008580 */
    FUN_08008580();   /* BL -> 0x08008580 FUN_08008580 */
    /* 0x08008458: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800845A: 00f015f9 bl       #0x8008688 */
    FUN_08008688();   /* BL -> 0x08008688 FUN_08008688 */

    L_0800845E:
    /* 0x0800845E: 2576     strb     r5, [r4, #0x18] */
    *(uint8_t *)(r4 + 24) = r5;   /* store */  /* MMIO: SYSTICK.VAL(w) */

    L_08008460:
    /* 0x08008460: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x08008462: 0169     ldr      r1, [r0, #0x10] */
    r1 = *(uint32_t *)(r0 + 16);  /* MMIO: RCC.APB1RSTR(r) */
    /* 0x08008464: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x08008466: 0907     lsls     r1, r1, #0x1c */
    r1 = r1 << 0x1c;
    /* 0x08008468: 16d4     bmi      #0x8008498 */
    if (r1 < 0) goto L_08008498;   /* result negative */
    /* 0x0800846A: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);  /* MMIO: RCC.APB2RSTR(r) */
    /* 0x0800846C: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x0800846E: 0907     lsls     r1, r1, #0x1c */
    r1 = r1 << 0x1c;
    /* 0x08008470: 12d4     bmi      #0x8008498 */
    if (r1 < 0) goto L_08008498;   /* result negative */
    /* 0x08008472: 0821     movs     r1, #8 */
    r1 = 8;
    /* 0x08008474: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x08008476: 0161     str      r1, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r1;   /* store */  /* MMIO: RCC.APB1RSTR(w) */
    /* 0x08008478: 0420     movs     r0, #4 */
    r0 = 4;
    /* 0x0800847A: 2076     strb     r0, [r4, #0x18] */
    *(uint8_t *)(r4 + 24) = r0;   /* store */  /* MMIO: SYSTICK.VAL(w) */
    /* 0x0800847C: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x0800847E: c069     ldr      r0, [r0, #0x1c] */
    r0 = *(uint32_t *)(r0 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x08008480: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08008482: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08008484: 02d0     beq      #0x800848c */
    if (r1 == 0) goto L_0800848C;
    /* 0x08008486: fff7aeff bl       #0x80083e6 */
    FUN_080083E6();   /* BL -> 0x080083E6 FUN_080083E6 */
    /* 0x0800848A: 04e0     b        #0x8008496 */
    goto L_08008496;

    L_0800848C:
    /* 0x0800848C: 00f078f8 bl       #0x8008580 */
    FUN_08008580();   /* BL -> 0x08008580 FUN_08008580 */
    /* 0x08008490: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08008492: 00f0f9f8 bl       #0x8008688 */
    FUN_08008688();   /* BL -> 0x08008688 FUN_08008688 */

    L_08008496:
    /* 0x08008496: 2576     strb     r5, [r4, #0x18] */
    *(uint8_t *)(r4 + 24) = r5;   /* store */  /* MMIO: SYSTICK.VAL(w) */

    L_08008498:
    /* 0x08008498: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x0800849A: 0169     ldr      r1, [r0, #0x10] */
    r1 = *(uint32_t *)(r0 + 16);  /* MMIO: RCC.APB1RSTR(r) */
    /* 0x0800849C: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x0800849E: c906     lsls     r1, r1, #0x1b */
    r1 = r1 << 0x1b;
    /* 0x080084A0: 17d4     bmi      #0x80084d2 */
    if (r1 < 0) goto L_080084D2;   /* result negative */
    /* 0x080084A2: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);  /* MMIO: RCC.APB2RSTR(r) */
    /* 0x080084A4: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x080084A6: c906     lsls     r1, r1, #0x1b */
    r1 = r1 << 0x1b;
    /* 0x080084A8: 13d4     bmi      #0x80084d2 */
    if (r1 < 0) goto L_080084D2;   /* result negative */
    /* 0x080084AA: 1021     movs     r1, #0x10 */
    r1 = 0x10;
    /* 0x080084AC: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x080084AE: 0161     str      r1, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r1;   /* store */  /* MMIO: RCC.APB1RSTR(w) */
    /* 0x080084B0: 0820     movs     r0, #8 */
    r0 = 8;
    /* 0x080084B2: 2076     strb     r0, [r4, #0x18] */
    *(uint8_t *)(r4 + 24) = r0;   /* store */  /* MMIO: SYSTICK.VAL(w) */
    /* 0x080084B4: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x080084B6: c069     ldr      r0, [r0, #0x1c] */
    r0 = *(uint32_t *)(r0 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x080084B8: 8005     lsls     r0, r0, #0x16 */
    r0 = r0 << 0x16;
    /* 0x080084BA: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x080084BC: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080084BE: 02d0     beq      #0x80084c6 */
    if (r1 == 0) goto L_080084C6;
    /* 0x080084C0: fff791ff bl       #0x80083e6 */
    FUN_080083E6();   /* BL -> 0x080083E6 FUN_080083E6 */
    /* 0x080084C4: 04e0     b        #0x80084d0 */
    goto L_080084D0;

    L_080084C6:
    /* 0x080084C6: 00f05bf8 bl       #0x8008580 */
    FUN_08008580();   /* BL -> 0x08008580 FUN_08008580 */
    /* 0x080084CA: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080084CC: 00f0dcf8 bl       #0x8008688 */
    FUN_08008688();   /* BL -> 0x08008688 FUN_08008688 */

    L_080084D0:
    /* 0x080084D0: 2576     strb     r5, [r4, #0x18] */
    *(uint8_t *)(r4 + 24) = r5;   /* store */  /* MMIO: SYSTICK.VAL(w) */

    L_080084D2:
    /* 0x080084D2: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x080084D4: 0169     ldr      r1, [r0, #0x10] */
    r1 = *(uint32_t *)(r0 + 16);  /* MMIO: RCC.APB1RSTR(r) */
    /* 0x080084D6: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x080084D8: c907     lsls     r1, r1, #0x1f */
    r1 = r1 << 0x1f;
    /* 0x080084DA: 08d1     bne      #0x80084ee */
    if (r1 != 0) goto L_080084EE;
    /* 0x080084DC: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);  /* MMIO: RCC.APB2RSTR(r) */
    /* 0x080084DE: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x080084E0: c907     lsls     r1, r1, #0x1f */
    r1 = r1 << 0x1f;
    /* 0x080084E2: 04d1     bne      #0x80084ee */
    if (r1 != 0) goto L_080084EE;
    /* 0x080084E4: 891e     subs     r1, r1, #2 */
    r1 = r1 - 2;
    /* 0x080084E6: 0161     str      r1, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r1;   /* store */  /* MMIO: RCC.APB1RSTR(w) */
    /* 0x080084E8: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080084EA: 00f0f7f8 bl       #0x80086dc */
    FUN_080086DC();   /* BL -> 0x080086DC FUN_080086DC */

    L_080084EE:
    /* 0x080084EE: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x080084F0: 0169     ldr      r1, [r0, #0x10] */
    r1 = *(uint32_t *)(r0 + 16);  /* MMIO: RCC.APB1RSTR(r) */
    /* 0x080084F2: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x080084F4: 0906     lsls     r1, r1, #0x18 */
    r1 = r1 << 0x18;
    /* 0x080084F6: 09d4     bmi      #0x800850c */
    if (r1 < 0) goto L_0800850C;   /* result negative */
    /* 0x080084F8: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);  /* MMIO: RCC.APB2RSTR(r) */
    /* 0x080084FA: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x080084FC: 0906     lsls     r1, r1, #0x18 */
    r1 = r1 << 0x18;
    /* 0x080084FE: 05d4     bmi      #0x800850c */
    if (r1 < 0) goto L_0800850C;   /* result negative */
    /* 0x08008500: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08008502: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x08008504: 0161     str      r1, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r1;   /* store */  /* MMIO: RCC.APB1RSTR(w) */
    /* 0x08008506: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08008508: fff748fe bl       #0x800819c */
    FUN_0800819C();   /* BL -> 0x0800819C FUN_0800819C */

    L_0800850C:
    /* 0x0800850C: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x0800850E: 0169     ldr      r1, [r0, #0x10] */
    r1 = *(uint32_t *)(r0 + 16);  /* MMIO: RCC.APB1RSTR(r) */
    /* 0x08008510: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x08008512: 4906     lsls     r1, r1, #0x19 */
    r1 = r1 << 0x19;
    /* 0x08008514: 09d4     bmi      #0x800852a */
    if (r1 < 0) goto L_0800852A;   /* result negative */
    /* 0x08008516: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);  /* MMIO: RCC.APB2RSTR(r) */
    /* 0x08008518: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x0800851A: 4906     lsls     r1, r1, #0x19 */
    r1 = r1 << 0x19;
    /* 0x0800851C: 05d4     bmi      #0x800852a */
    if (r1 < 0) goto L_0800852A;   /* result negative */
    /* 0x0800851E: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x08008520: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x08008522: 0161     str      r1, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r1;   /* store */  /* MMIO: RCC.APB1RSTR(w) */
    /* 0x08008524: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08008526: 00f0daf8 bl       #0x80086de */
    FUN_080086DE();   /* BL -> 0x080086DE FUN_080086DE */

    L_0800852A:
    /* 0x0800852A: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x0800852C: 0169     ldr      r1, [r0, #0x10] */
    r1 = *(uint32_t *)(r0 + 16);  /* MMIO: RCC.APB1RSTR(r) */
    /* 0x0800852E: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x08008530: 8906     lsls     r1, r1, #0x1a */
    r1 = r1 << 0x1a;
    /* 0x08008532: 09d4     bmi      #0x8008548 */
    if (r1 < 0) goto L_08008548;   /* result negative */
    /* 0x08008534: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);  /* MMIO: RCC.APB2RSTR(r) */
    /* 0x08008536: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x08008538: 8906     lsls     r1, r1, #0x1a */
    r1 = r1 << 0x1a;
    /* 0x0800853A: 05d4     bmi      #0x8008548 */
    if (r1 < 0) goto L_08008548;   /* result negative */
    /* 0x0800853C: 2021     movs     r1, #0x20 */
    r1 = 0x20;
    /* 0x0800853E: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x08008540: 0161     str      r1, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r1;   /* store */  /* MMIO: RCC.APB1RSTR(w) */
    /* 0x08008542: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08008544: fff72bfe bl       #0x800819e */
    FUN_0800819E();   /* BL -> 0x0800819E FUN_0800819E */

    L_08008548:
    /* 0x08008548: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
