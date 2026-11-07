/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800941C
 * Name      : FUN_0800941C
 * End       : 0x0800959C
 * Size      : 384 bytes, 170 instructions decoded in body (functions.json insns=170, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x0800CF74 main
 * Callees   : 4  -> 0x080072C4 FUN_080072C4, 0x08007426 FUN_08007426, 0x0800765C FUN_0800765C, 0x08007670 FUN_08007670
 * MMIO      : RCC.AHBENR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800941C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800941C: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x0800941E: 5f48     ldr      r0, [pc, #0x17c]  ; =RCC.CR */
    r0 = *(uint32_t *)0x0800959C;   /* = MMIO RCC+0x00 */
    /* 0x08009420: 87b0     sub      sp, #0x1c */
    sp = sp - 0x1c;
    /* 0x08009422: 4269     ldr      r2, [r0, #0x14] */
    r2 = *(uint32_t *)(r0 + 20);  /* MMIO: RCC.AHBENR(r) */
    /* 0x08009424: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08009426: c904     lsls     r1, r1, #0x13 */
    r1 = r1 << 0x13;
    /* 0x08009428: 0a43     orrs     r2, r1 */
    r2 = r2 | r1;
    /* 0x0800942A: 4261     str      r2, [r0, #0x14] */
    *(uint32_t *)(r0 + 20) = r2;   /* store */  /* MMIO: RCC.AHBENR(w) */
    /* 0x0800942C: 4269     ldr      r2, [r0, #0x14] */
    r2 = *(uint32_t *)(r0 + 20);  /* MMIO: RCC.AHBENR(r) */
    /* 0x0800942E: 0a40     ands     r2, r1 */
    r2 = r2 & r1;
    /* 0x08009430: 0592     str      r2, [sp, #0x14] */
    *(uint32_t *)(sp + 20) = r2;   /* store */
    /* 0x08009432: 4269     ldr      r2, [r0, #0x14] */
    r2 = *(uint32_t *)(r0 + 20);  /* MMIO: RCC.AHBENR(r) */
    /* 0x08009434: c900     lsls     r1, r1, #3 */
    r1 = r1 << 3;
    /* 0x08009436: 0a43     orrs     r2, r1 */
    r2 = r2 | r1;
    /* 0x08009438: 4261     str      r2, [r0, #0x14] */
    *(uint32_t *)(r0 + 20) = r2;   /* store */  /* MMIO: RCC.AHBENR(w) */
    /* 0x0800943A: 4269     ldr      r2, [r0, #0x14] */
    r2 = *(uint32_t *)(r0 + 20);  /* MMIO: RCC.AHBENR(r) */
    /* 0x0800943C: 0a40     ands     r2, r1 */
    r2 = r2 & r1;
    /* 0x0800943E: 0592     str      r2, [sp, #0x14] */
    *(uint32_t *)(sp + 20) = r2;   /* store */
    /* 0x08009440: 4269     ldr      r2, [r0, #0x14] */
    r2 = *(uint32_t *)(r0 + 20);  /* MMIO: RCC.AHBENR(r) */
    /* 0x08009442: 4911     asrs     r1, r1, #5 */
    r1 = r1 >> 5;
    /* 0x08009444: 0a43     orrs     r2, r1 */
    r2 = r2 | r1;
    /* 0x08009446: 4261     str      r2, [r0, #0x14] */
    *(uint32_t *)(r0 + 20) = r2;   /* store */  /* MMIO: RCC.AHBENR(w) */
    /* 0x08009448: 4269     ldr      r2, [r0, #0x14] */
    r2 = *(uint32_t *)(r0 + 20);  /* MMIO: RCC.AHBENR(r) */
    /* 0x0800944A: 0a40     ands     r2, r1 */
    r2 = r2 & r1;
    /* 0x0800944C: 0592     str      r2, [sp, #0x14] */
    *(uint32_t *)(sp + 20) = r2;   /* store */
    /* 0x0800944E: 4269     ldr      r2, [r0, #0x14] */
    r2 = *(uint32_t *)(r0 + 20);  /* MMIO: RCC.AHBENR(r) */
    /* 0x08009450: 4900     lsls     r1, r1, #1 */
    r1 = r1 << 1;
    /* 0x08009452: 0a43     orrs     r2, r1 */
    r2 = r2 | r1;
    /* 0x08009454: 4261     str      r2, [r0, #0x14] */
    *(uint32_t *)(r0 + 20) = r2;   /* store */  /* MMIO: RCC.AHBENR(w) */
    /* 0x08009456: 4269     ldr      r2, [r0, #0x14] */
    r2 = *(uint32_t *)(r0 + 20);  /* MMIO: RCC.AHBENR(r) */
    /* 0x08009458: 0a40     ands     r2, r1 */
    r2 = r2 & r1;
    /* 0x0800945A: 0592     str      r2, [sp, #0x14] */
    *(uint32_t *)(sp + 20) = r2;   /* store */
    /* 0x0800945C: 4269     ldr      r2, [r0, #0x14] */
    r2 = *(uint32_t *)(r0 + 20);  /* MMIO: RCC.AHBENR(r) */
    /* 0x0800945E: 8900     lsls     r1, r1, #2 */
    r1 = r1 << 2;
    /* 0x08009460: 0a43     orrs     r2, r1 */
    r2 = r2 | r1;
    /* 0x08009462: 4261     str      r2, [r0, #0x14] */
    *(uint32_t *)(r0 + 20) = r2;   /* store */  /* MMIO: RCC.AHBENR(w) */
    /* 0x08009464: 4069     ldr      r0, [r0, #0x14] */
    r0 = *(uint32_t *)(r0 + 20);  /* MMIO: RCC.AHBENR(r) */
    /* 0x08009466: 4f4e     ldr      r6, [pc, #0x13c]  ; =GPIOC.MODER */
    r6 = *(uint32_t *)0x080095A4;   /* = MMIO GPIOC+0x00 */
    /* 0x08009468: 0840     ands     r0, r1 */
    r0 = r0 & r1;
    /* 0x0800946A: 0590     str      r0, [sp, #0x14] */
    *(uint32_t *)(sp + 20) = r0;   /* store */
    /* 0x0800946C: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800946E: 4c49     ldr      r1, [pc, #0x130]  ; =0x00002330 */
    r1 = *(uint32_t *)0x080095A0;   /* = 0x00002330 */
    /* 0x08009470: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x08009472: fdf7d8ff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009476: 4c4f     ldr      r7, [pc, #0x130]  ; =GPIOF.MODER */
    r7 = *(uint32_t *)0x080095A8;   /* = MMIO GPIOF+0x00 */
    /* 0x08009478: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800947A: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800947C: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800947E: fdf7d2ff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009482: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08009484: 4821     movs     r1, #0x48 */
    r1 = 0x48;
    /* 0x08009486: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x08009488: fdf7cdff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800948C: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800948E: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x08009490: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x08009492: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x08009494: fdf7c7ff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009498: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x0800949A: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800949C: 4349     ldr      r1, [pc, #0x10c]  ; =0x00000602 */
    r1 = *(uint32_t *)0x080095AC;   /* = 0x00000602 */
    /* 0x0800949E: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x080094A0: fdf7c1ff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x080094A4: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x080094A6: 4249     ldr      r1, [pc, #0x108]  ; =0x00003C04 */
    r1 = *(uint32_t *)0x080095B0;   /* = 0x00003C04 */
    /* 0x080094A8: 4248     ldr      r0, [pc, #0x108]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x080095B4;   /* = MMIO GPIOB+0x00 */
    /* 0x080094AA: fdf7bcff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x080094AE: 0f21     movs     r1, #0xf */
    r1 = 0xf;
    /* 0x080094B0: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x080094B2: 8901     lsls     r1, r1, #6 */
    r1 = r1 << 6;
    /* 0x080094B4: 3f48     ldr      r0, [pc, #0xfc]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x080095B4;   /* = MMIO GPIOB+0x00 */
    /* 0x080094B6: fdf7b6ff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x080094BA: 3948     ldr      r0, [pc, #0xe4]  ; =0x00002330 */
    r0 = *(uint32_t *)0x080095A0;   /* = 0x00002330 */
    /* 0x080094BC: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x080094BE: 4830     adds     r0, #0x48 */
    r0 = r0 + 0x48;
    /* 0x080094C0: 0125     movs     r5, #1 */
    r5 = 1;
    /* 0x080094C2: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x080094C4: 0294     str      r4, [sp, #8] */
    *(uint32_t *)(sp + 8) = r4;   /* store */
    /* 0x080094C6: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x080094C8: 0195     str      r5, [sp, #4] */
    *(uint32_t *)(sp + 4) = r5;   /* store */
    /* 0x080094CA: 0394     str      r4, [sp, #0xc] */
    *(uint32_t *)(sp + 12) = r4;   /* store */
    /* 0x080094CC: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x080094CE: fdf7f9fe bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x080094D2: 3948     ldr      r0, [pc, #0xe4]  ; =0x0000DC80 */
    r0 = *(uint32_t *)0x080095B8;   /* = 0x0000DC80 */
    /* 0x080094D4: 6a46     mov      r2, sp */
    r2 = sp;
    /* 0x080094D6: 31c2     stm      r2!, {r0, r4, r5} */
    /* stm r2!, {r0, r4, r5} */
    /* 0x080094D8: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x080094DA: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x080094DC: fdf7f2fe bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x080094E0: 0095     str      r5, [sp] */
    *(uint32_t *)(sp) = r5;   /* store */
    /* 0x080094E2: 0294     str      r4, [sp, #8] */
    *(uint32_t *)(sp + 8) = r4;   /* store */
    /* 0x080094E4: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x080094E6: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x080094E8: 0195     str      r5, [sp, #4] */
    *(uint32_t *)(sp + 4) = r5;   /* store */
    /* 0x080094EA: 0394     str      r4, [sp, #0xc] */
    *(uint32_t *)(sp + 12) = r4;   /* store */
    /* 0x080094EC: fdf7eafe bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x080094F0: 2e48     ldr      r0, [pc, #0xb8]  ; =0x00000602 */
    r0 = *(uint32_t *)0x080095AC;   /* = 0x00000602 */
    /* 0x080094F2: 0294     str      r4, [sp, #8] */
    *(uint32_t *)(sp + 8) = r4;   /* store */
    /* 0x080094F4: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x080094F6: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x080094F8: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x080094FA: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x080094FC: 0195     str      r5, [sp, #4] */
    *(uint32_t *)(sp + 4) = r5;   /* store */
    /* 0x080094FE: 0394     str      r4, [sp, #0xc] */
    *(uint32_t *)(sp + 12) = r4;   /* store */
    /* 0x08009500: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x08009502: fdf7dffe bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08009506: 2d48     ldr      r0, [pc, #0xb4]  ; =0x00008004 */
    r0 = *(uint32_t *)0x080095BC;   /* = 0x00008004 */
    /* 0x08009508: 6a46     mov      r2, sp */
    r2 = sp;
    /* 0x0800950A: 31c2     stm      r2!, {r0, r4, r5} */
    /* stm r2!, {r0, r4, r5} */
    /* 0x0800950C: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x0800950E: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08009510: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x08009512: fdf7d7fe bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08009516: 4020     movs     r0, #0x40 */
    r0 = 0x40;
    /* 0x08009518: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x0800951A: 294e     ldr      r6, [pc, #0xa4]  ; =0x10210000 */
    r6 = *(uint32_t *)0x080095C0;   /* = 0x10210000 */
    /* 0x0800951C: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x0800951E: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08009520: 0196     str      r6, [sp, #4] */
    *(uint32_t *)(sp + 4) = r6;   /* store */
    /* 0x08009522: 0295     str      r5, [sp, #8] */
    *(uint32_t *)(sp + 8) = r5;   /* store */
    /* 0x08009524: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x08009526: fdf7cdfe bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x0800952A: 0095     str      r5, [sp] */
    *(uint32_t *)(sp) = r5;   /* store */
    /* 0x0800952C: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x0800952E: 2148     ldr      r0, [pc, #0x84]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x080095B4;   /* = MMIO GPIOB+0x00 */
    /* 0x08009530: 0196     str      r6, [sp, #4] */
    *(uint32_t *)(sp + 4) = r6;   /* store */
    /* 0x08009532: 0295     str      r5, [sp, #8] */
    *(uint32_t *)(sp + 8) = r5;   /* store */
    /* 0x08009534: fdf7c6fe bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08009538: 3a20     movs     r0, #0x3a */
    r0 = 0x3a;
    /* 0x0800953A: 6a46     mov      r2, sp */
    r2 = sp;
    /* 0x0800953C: 31c2     stm      r2!, {r0, r4, r5} */
    /* stm r2!, {r0, r4, r5} */
    /* 0x0800953E: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08009540: 1c48     ldr      r0, [pc, #0x70]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x080095B4;   /* = MMIO GPIOB+0x00 */
    /* 0x08009542: fdf7bffe bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08009546: 1f48     ldr      r0, [pc, #0x7c]  ; =0x00003FC4 */
    r0 = *(uint32_t *)0x080095C4;   /* = 0x00003FC4 */
    /* 0x08009548: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x0800954A: 0294     str      r4, [sp, #8] */
    *(uint32_t *)(sp + 8) = r4;   /* store */
    /* 0x0800954C: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x0800954E: 0195     str      r5, [sp, #4] */
    *(uint32_t *)(sp + 4) = r5;   /* store */
    /* 0x08009550: 0394     str      r4, [sp, #0xc] */
    *(uint32_t *)(sp + 12) = r4;   /* store */
    /* 0x08009552: 1848     ldr      r0, [pc, #0x60]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x080095B4;   /* = MMIO GPIOB+0x00 */
    /* 0x08009554: fdf7b6fe bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08009558: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x0800955A: 8003     lsls     r0, r0, #0xe */
    r0 = r0 << 0xe;
    /* 0x0800955C: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x0800955E: 0194     str      r4, [sp, #4] */
    *(uint32_t *)(sp + 4) = r4;   /* store */
    /* 0x08009560: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08009562: 0294     str      r4, [sp, #8] */
    *(uint32_t *)(sp + 8) = r4;   /* store */
    /* 0x08009564: 1348     ldr      r0, [pc, #0x4c]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x080095B4;   /* = MMIO GPIOB+0x00 */
    /* 0x08009566: fdf7adfe bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x0800956A: 0420     movs     r0, #4 */
    r0 = 4;
    /* 0x0800956C: 6a46     mov      r2, sp */
    r2 = sp;
    /* 0x0800956E: 31c2     stm      r2!, {r0, r4, r5} */
    /* stm r2!, {r0, r4, r5} */
    /* 0x08009570: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08009572: 1548     ldr      r0, [pc, #0x54]  ; =GPIOD.MODER */
    r0 = *(uint32_t *)0x080095C8;   /* = MMIO GPIOD+0x00 */
    /* 0x08009574: fdf7a6fe bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08009578: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800957A: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x0800957C: 0520     movs     r0, #5 */
    r0 = 5;
    /* 0x0800957E: fef777f8 bl       #0x8007670 */
    FUN_08007670();   /* BL -> 0x08007670 FUN_08007670 */
    /* 0x08009582: 0520     movs     r0, #5 */
    r0 = 5;
    /* 0x08009584: fef76af8 bl       #0x800765c */
    FUN_0800765C();   /* BL -> 0x0800765C FUN_0800765C */
    /* 0x08009588: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800958A: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x0800958C: 0720     movs     r0, #7 */
    r0 = 7;
    /* 0x0800958E: fef76ff8 bl       #0x8007670 */
    FUN_08007670();   /* BL -> 0x08007670 FUN_08007670 */
    /* 0x08009592: 0720     movs     r0, #7 */
    r0 = 7;
    /* 0x08009594: fef762f8 bl       #0x800765c */
    FUN_0800765C();   /* BL -> 0x0800765C FUN_0800765C */
    /* 0x08009598: 07b0     add      sp, #0x1c */
    sp = sp + 0x1c;
    /* 0x0800959A: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
