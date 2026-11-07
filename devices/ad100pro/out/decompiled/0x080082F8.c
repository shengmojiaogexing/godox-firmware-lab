/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080082F8
 * Name      : FUN_080082F8
 * End       : 0x080083E6
 * Size      : 238 bytes, 113 instructions decoded in body (functions.json insns=113, blocks=29)
 * Class     : function
 * Callers   : 5  -> 0x080096A8 FUN_080096A8, 0x08009708 FUN_08009708, 0x08009768 FUN_08009768, 0x08009D84 FUN_08009D84, 0x08009DB8 FUN_08009DB8
 * Callees   : 4  -> 0x0800BB5E FUN_0800BB5E, 0x0800BB74 FUN_0800BB74, 0x0800BD7C FUN_0800BD7C, 0x0800BD9E FUN_0800BD9E
 * MMIO      : RCC.+0x19, RCC.APB2ENR, RCC.CIR, SYS.+0x0
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080082F8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080082F8: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x080082FA: 0546     mov      r5, r0 */
    r5 = r0;
    /* 0x080082FC: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x080082FE: 2035     adds     r5, #0x20 */
    r5 = r5 + 0x20;
    /* 0x08008300: 287e     ldrb     r0, [r5, #0x18] */
    r0 = *(uint8_t *)(r5 + 24);  /* MMIO: RCC.APB2ENR(r) */
    /* 0x08008302: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08008304: 17d0     beq      #0x8008336 */
    if (r0 == 1) goto L_08008336;
    /* 0x08008306: 0127     movs     r7, #1 */
    r7 = 1;
    /* 0x08008308: 2f76     strb     r7, [r5, #0x18] */
    *(uint8_t *)(r5 + 24) = r7;   /* store */  /* MMIO: RCC.APB2ENR(w) */
    /* 0x0800830A: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x0800830C: 6876     strb     r0, [r5, #0x19] */
    *(uint8_t *)(r5 + 25) = r0;   /* store */  /* MMIO: RCC.+0x19(w) */
    /* 0x0800830E: 2268     ldr      r2, [r4] */
    r2 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x08008310: 9068     ldr      r0, [r2, #8] */
    r0 = *(uint32_t *)(r2 + 8);
    /* 0x08008312: 7726     movs     r6, #0x77 */
    r6 = 0x77;
    /* 0x08008314: ff23     movs     r3, #0xff */
    r3 = 0xff;
    /* 0x08008316: b043     bics     r0, r6 */
    r0 = r0 & ~r6;
    /* 0x08008318: 1b02     lsls     r3, r3, #8 */
    r3 = r3 << 8;
    /* 0x0800831A: 9843     bics     r0, r3 */
    r0 = r0 & ~r3;
    /* 0x0800831C: 9060     str      r0, [r2, #8] */
    *(uint32_t *)(r2 + 8) = r0;   /* store */
    /* 0x0800831E: 0868     ldr      r0, [r1] */
    r0 = *(uint32_t *)(r1);
    /* 0x08008320: 5028     cmp      r0, #0x50 */
    /* flags = cmp(r0, #0x50) */
    /* 0x08008322: 37d0     beq      #0x8008394 */
    if (r0 == 0x50) goto L_08008394;
    /* 0x08008324: 0edc     bgt      #0x8008344 */
    if (r0 > 0x50) goto L_08008344;   /* signed */
    /* 0x08008326: 2028     cmp      r0, #0x20 */
    /* flags = cmp(r0, #0x20) */
    /* 0x08008328: 57d0     beq      #0x80083da */
    if (r0 == 0x20) goto L_080083DA;
    /* 0x0800832A: 06dc     bgt      #0x800833a */
    if (r0 > 0x20) goto L_0800833A;   /* signed */
    /* 0x0800832C: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800832E: 4ed0     beq      #0x80083ce */
    if (r0 == 0) goto L_080083CE;
    /* 0x08008330: 1028     cmp      r0, #0x10 */
    /* flags = cmp(r0, #0x10) */
    /* 0x08008332: 40d1     bne      #0x80083b6 */
    if (r0 != 0x10) goto L_080083B6;
    /* 0x08008334: 4ee0     b        #0x80083d4 */
    goto L_080083D4;

    L_08008336:
    /* 0x08008336: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08008338: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_0800833A:
    /* 0x0800833A: 3028     cmp      r0, #0x30 */
    /* flags = cmp(r0, #0x30) */
    /* 0x0800833C: 50d0     beq      #0x80083e0 */
    if (r0 == 0x30) goto L_080083E0;
    /* 0x0800833E: 4028     cmp      r0, #0x40 */
    /* flags = cmp(r0, #0x40) */
    /* 0x08008340: 39d1     bne      #0x80083b6 */
    if (r0 != 0x40) goto L_080083B6;
    /* 0x08008342: 3ce0     b        #0x80083be */
    goto L_080083BE;

    L_08008344:
    /* 0x08008344: 6028     cmp      r0, #0x60 */
    /* flags = cmp(r0, #0x60) */
    /* 0x08008346: 2dd0     beq      #0x80083a4 */
    if (r0 == 0x60) goto L_080083A4;
    /* 0x08008348: 7028     cmp      r0, #0x70 */
    /* flags = cmp(r0, #0x70) */
    /* 0x0800834A: 17d0     beq      #0x800837c */
    if (r0 == 0x70) goto L_0800837C;
    /* 0x0800834C: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800834E: 1203     lsls     r2, r2, #0xc */
    r2 = r2 << 0xc;
    /* 0x08008350: 801a     subs     r0, r0, r2 */
    r0 = r0 - r2;
    /* 0x08008352: 0ed0     beq      #0x8008372 */
    if (r0 == 0x70) goto L_08008372;
    /* 0x08008354: 9042     cmp      r0, r2 */
    /* flags = cmp(r0, r2) */
    /* 0x08008356: 2ed1     bne      #0x80083b6 */
    if (r0 != r2) goto L_080083B6;
    /* 0x08008358: cb68     ldr      r3, [r1, #0xc] */
    r3 = *(uint32_t *)(r1 + 12);
    /* 0x0800835A: 4a68     ldr      r2, [r1, #4] */
    r2 = *(uint32_t *)(r1 + 4);
    /* 0x0800835C: 8968     ldr      r1, [r1, #8] */
    r1 = *(uint32_t *)(r1 + 8);
    /* 0x0800835E: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x08008360: 03f0fdfb bl       #0x800bb5e */
    FUN_0800BB5E();   /* BL -> 0x0800BB5E FUN_0800BB5E */
    /* 0x08008364: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x08008366: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);  /* MMIO: RCC.CIR(r) */
    /* 0x08008368: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800836A: 9203     lsls     r2, r2, #0xe */
    r2 = r2 << 0xe;
    /* 0x0800836C: 1143     orrs     r1, r2 */
    r1 = r1 | r2;

    L_0800836E:
    /* 0x0800836E: 8160     str      r1, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r1;   /* store */  /* MMIO: RCC.CIR(w) */
    /* 0x08008370: 21e0     b        #0x80083b6 */
    goto L_080083B6;

    L_08008372:
    /* 0x08008372: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x08008374: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);  /* MMIO: RCC.CIR(r) */
    /* 0x08008376: c908     lsrs     r1, r1, #3 */
    r1 = r1 >> 3;
    /* 0x08008378: c900     lsls     r1, r1, #3 */
    r1 = r1 << 3;
    /* 0x0800837A: f8e7     b        #0x800836e */
    goto L_0800836E;   /* loop */

    L_0800837C:
    /* 0x0800837C: cb68     ldr      r3, [r1, #0xc] */
    r3 = *(uint32_t *)(r1 + 12);
    /* 0x0800837E: 4a68     ldr      r2, [r1, #4] */
    r2 = *(uint32_t *)(r1 + 4);
    /* 0x08008380: 8968     ldr      r1, [r1, #8] */
    r1 = *(uint32_t *)(r1 + 8);
    /* 0x08008382: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x08008384: 03f0ebfb bl       #0x800bb5e */
    FUN_0800BB5E();   /* BL -> 0x0800BB5E FUN_0800BB5E */
    /* 0x08008388: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x0800838A: 8868     ldr      r0, [r1, #8] */
    r0 = *(uint32_t *)(r1 + 8);
    /* 0x0800838C: b043     bics     r0, r6 */
    r0 = r0 & ~r6;
    /* 0x0800838E: 3043     orrs     r0, r6 */
    r0 = r0 | r6;
    /* 0x08008390: 8860     str      r0, [r1, #8] */
    *(uint32_t *)(r1 + 8) = r0;   /* store */
    /* 0x08008392: 10e0     b        #0x80083b6 */
    goto L_080083B6;

    L_08008394:
    /* 0x08008394: ca68     ldr      r2, [r1, #0xc] */
    r2 = *(uint32_t *)(r1 + 12);
    /* 0x08008396: 4968     ldr      r1, [r1, #4] */
    r1 = *(uint32_t *)(r1 + 4);
    /* 0x08008398: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x0800839A: 03f0effc bl       #0x800bd7c */
    FUN_0800BD7C();   /* BL -> 0x0800BD7C FUN_0800BD7C */
    /* 0x0800839E: 5021     movs     r1, #0x50 */
    r1 = 0x50;
    /* 0x080083A0: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x080083A2: 06e0     b        #0x80083b2 */
    goto L_080083B2;

    L_080083A4:
    /* 0x080083A4: ca68     ldr      r2, [r1, #0xc] */
    r2 = *(uint32_t *)(r1 + 12);
    /* 0x080083A6: 4968     ldr      r1, [r1, #4] */
    r1 = *(uint32_t *)(r1 + 4);
    /* 0x080083A8: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x080083AA: 03f0f8fc bl       #0x800bd9e */
    FUN_0800BD9E();   /* BL -> 0x0800BD9E FUN_0800BD9E */
    /* 0x080083AE: 6021     movs     r1, #0x60 */
    r1 = 0x60;
    /* 0x080083B0: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */

    L_080083B2:
    /* 0x080083B2: 03f0dffb bl       #0x800bb74 */
    FUN_0800BB74();   /* BL -> 0x0800BB74 FUN_0800BB74 */

    L_080083B6:
    /* 0x080083B6: 6f76     strb     r7, [r5, #0x19] */
    *(uint8_t *)(r5 + 25) = r7;   /* store */  /* MMIO: RCC.+0x19(w) */
    /* 0x080083B8: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080083BA: 2876     strb     r0, [r5, #0x18] */
    *(uint8_t *)(r5 + 24) = r0;   /* store */  /* MMIO: RCC.APB2ENR(w) */
    /* 0x080083BC: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_080083BE:
    /* 0x080083BE: ca68     ldr      r2, [r1, #0xc] */
    r2 = *(uint32_t *)(r1 + 12);
    /* 0x080083C0: 4968     ldr      r1, [r1, #4] */
    r1 = *(uint32_t *)(r1 + 4);
    /* 0x080083C2: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x080083C4: 03f0dafc bl       #0x800bd7c */
    FUN_0800BD7C();   /* BL -> 0x0800BD7C FUN_0800BD7C */
    /* 0x080083C8: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x080083CA: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x080083CC: f1e7     b        #0x80083b2 */
    goto L_080083B2;   /* loop */

    L_080083CE:
    /* 0x080083CE: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x080083D0: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x080083D2: eee7     b        #0x80083b2 */
    goto L_080083B2;   /* loop */

    L_080083D4:
    /* 0x080083D4: 1021     movs     r1, #0x10 */
    r1 = 0x10;
    /* 0x080083D6: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x080083D8: ebe7     b        #0x80083b2 */
    goto L_080083B2;   /* loop */

    L_080083DA:
    /* 0x080083DA: 2021     movs     r1, #0x20 */
    r1 = 0x20;
    /* 0x080083DC: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x080083DE: e8e7     b        #0x80083b2 */
    goto L_080083B2;   /* loop */

    L_080083E0:
    /* 0x080083E0: 3021     movs     r1, #0x30 */
    r1 = 0x30;
    /* 0x080083E2: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x080083E4: e5e7     b        #0x80083b2 */
    goto L_080083B2;   /* loop */
}
