/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007E70
 * Name      : FUN_08007E70
 * End       : 0x08007F0A
 * Size      : 154 bytes, 75 instructions decoded in body (functions.json insns=75, blocks=8)
 * Class     : function
 * Callers   : 1  -> 0x080095F8 FUN_080095F8
 * Callees   : 2  -> 0x08007F38 FUN_08007F38, 0x0800ACBE FUN_0800ACBE
 * MMIO      : RCC.+0x1D, RCC.APB1ENR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007E70(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007E70: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08007E72: 0400     movs     r4, r0 */
    r4 = r0;
    /* 0x08007E74: 03d0     beq      #0x8007e7e */
    if (/* beq */) goto L_08007E7E;
    /* 0x08007E76: 607f     ldrb     r0, [r4, #0x1d] */
    r0 = *(uint8_t *)(r4 + 29);  /* MMIO: RCC.+0x1D(r) */
    /* 0x08007E78: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08007E7A: 02d0     beq      #0x8007e82 */
    if (r0 == 0) goto L_08007E82;
    /* 0x08007E7C: 06e0     b        #0x8007e8c */
    goto L_08007E8C;

    L_08007E7E:
    /* 0x08007E7E: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08007E80: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08007E82:
    /* 0x08007E82: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08007E84: 2077     strb     r0, [r4, #0x1c] */
    *(uint8_t *)(r4 + 28) = r0;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x08007E86: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08007E88: 00f056f8 bl       #0x8007f38 */
    FUN_08007F38();   /* BL -> 0x08007F38 FUN_08007F38 */

    L_08007E8C:
    /* 0x08007E8C: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08007E8E: 6077     strb     r0, [r4, #0x1d] */
    *(uint8_t *)(r4 + 29) = r0;   /* store */  /* MMIO: RCC.+0x1D(w) */
    /* 0x08007E90: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007E92: ca20     movs     r0, #0xca */
    r0 = 0xca;
    /* 0x08007E94: 4862     str      r0, [r1, #0x24] */
    *(uint32_t *)(r1 + 36) = r0;   /* store */  /* MMIO: EXTI.+0x24(w) */
    /* 0x08007E96: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007E98: 5320     movs     r0, #0x53 */
    r0 = 0x53;
    /* 0x08007E9A: 4862     str      r0, [r1, #0x24] */
    *(uint32_t *)(r1 + 36) = r0;   /* store */  /* MMIO: EXTI.+0x24(w) */
    /* 0x08007E9C: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08007E9E: 02f00eff bl       #0x800acbe */
    FUN_0800ACBE();   /* BL -> 0x0800ACBE FUN_0800ACBE */
    /* 0x08007EA2: 0146     mov      r1, r0 */
    r1 = r0;
    /* 0x08007EA4: ff20     movs     r0, #0xff */
    r0 = 0xff;
    /* 0x08007EA6: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x08007EA8: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007EAA: 03d0     beq      #0x8007eb4 */
    if (r1 == 0) goto L_08007EB4;
    /* 0x08007EAC: 4862     str      r0, [r1, #0x24] */
    *(uint32_t *)(r1 + 36) = r0;   /* store */  /* MMIO: EXTI.+0x24(w) */
    /* 0x08007EAE: 0420     movs     r0, #4 */
    r0 = 4;
    /* 0x08007EB0: 6077     strb     r0, [r4, #0x1d] */
    *(uint8_t *)(r4 + 29) = r0;   /* store */  /* MMIO: RCC.+0x1D(w) */
    /* 0x08007EB2: e4e7     b        #0x8007e7e */
    goto L_08007E7E;   /* loop */

    L_08007EB4:
    /* 0x08007EB4: 8a68     ldr      r2, [r1, #8] */
    r2 = *(uint32_t *)(r1 + 8);  /* MMIO: EXTI.RTSR(r) */
    /* 0x08007EB6: 154b     ldr      r3, [pc, #0x54]  ; =0xFF8FFFBF */
    r3 = *(uint32_t *)0x08007F0C;   /* = 0xFF8FFFBF */
    /* 0x08007EB8: 1a40     ands     r2, r3 */
    r2 = r2 & r3;
    /* 0x08007EBA: 8a60     str      r2, [r1, #8] */
    *(uint32_t *)(r1 + 8) = r2;   /* store */  /* MMIO: EXTI.RTSR(w) */
    /* 0x08007EBC: 2268     ldr      r2, [r4] */
    r2 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007EBE: 9168     ldr      r1, [r2, #8] */
    r1 = *(uint32_t *)(r2 + 8);
    /* 0x08007EC0: 6368     ldr      r3, [r4, #4] */
    r3 = *(uint32_t *)(r4 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x08007EC2: 6569     ldr      r5, [r4, #0x14] */
    r5 = *(uint32_t *)(r4 + 20);  /* MMIO: RCC.AHBENR(r) */
    /* 0x08007EC4: 1943     orrs     r1, r3 */
    r1 = r1 | r3;
    /* 0x08007EC6: 2369     ldr      r3, [r4, #0x10] */
    r3 = *(uint32_t *)(r4 + 16);  /* MMIO: RCC.APB1RSTR(r) */
    /* 0x08007EC8: 2b43     orrs     r3, r5 */
    r3 = r3 | r5;
    /* 0x08007ECA: 1943     orrs     r1, r3 */
    r1 = r1 | r3;
    /* 0x08007ECC: 9160     str      r1, [r2, #8] */
    *(uint32_t *)(r2 + 8) = r1;   /* store */
    /* 0x08007ECE: 2268     ldr      r2, [r4] */
    r2 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007ED0: e168     ldr      r1, [r4, #0xc] */
    r1 = *(uint32_t *)(r4 + 12);  /* MMIO: RCC.APB2RSTR(r) */
    /* 0x08007ED2: 1161     str      r1, [r2, #0x10] */
    *(uint32_t *)(r2 + 16) = r1;   /* store */
    /* 0x08007ED4: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007ED6: 0a69     ldr      r2, [r1, #0x10] */
    r2 = *(uint32_t *)(r1 + 16);  /* MMIO: EXTI.SWIER(r) */
    /* 0x08007ED8: 2389     ldrh     r3, [r4, #8] */
    r3 = *(uint16_t *)(r4 + 8);  /* MMIO: RCC.CIR(r) */
    /* 0x08007EDA: 1b04     lsls     r3, r3, #0x10 */
    r3 = r3 << 0x10;
    /* 0x08007EDC: 1a43     orrs     r2, r3 */
    r2 = r2 | r3;
    /* 0x08007EDE: 0a61     str      r2, [r1, #0x10] */
    *(uint32_t *)(r1 + 16) = r2;   /* store */  /* MMIO: EXTI.SWIER(w) */
    /* 0x08007EE0: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007EE2: ca68     ldr      r2, [r1, #0xc] */
    r2 = *(uint32_t *)(r1 + 12);  /* MMIO: EXTI.FTSR(r) */
    /* 0x08007EE4: 8023     movs     r3, #0x80 */
    r3 = 0x80;
    /* 0x08007EE6: 9a43     bics     r2, r3 */
    r2 = r2 & ~r3;
    /* 0x08007EE8: ca60     str      r2, [r1, #0xc] */
    *(uint32_t *)(r1 + 12) = r2;   /* store */  /* MMIO: EXTI.FTSR(w) */
    /* 0x08007EEA: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007EEC: 0a6c     ldr      r2, [r1, #0x40] */
    r2 = *(uint32_t *)(r1 + 64);  /* MMIO: EXTI.+0x40(r) */
    /* 0x08007EEE: db02     lsls     r3, r3, #0xb */
    r3 = r3 << 0xb;
    /* 0x08007EF0: 9a43     bics     r2, r3 */
    r2 = r2 & ~r3;
    /* 0x08007EF2: 0a64     str      r2, [r1, #0x40] */
    *(uint32_t *)(r1 + 64) = r2;   /* store */  /* MMIO: EXTI.+0x40(w) */
    /* 0x08007EF4: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007EF6: 0a6c     ldr      r2, [r1, #0x40] */
    r2 = *(uint32_t *)(r1 + 64);  /* MMIO: EXTI.+0x40(r) */
    /* 0x08007EF8: a369     ldr      r3, [r4, #0x18] */
    r3 = *(uint32_t *)(r4 + 24);  /* MMIO: RCC.APB2ENR(r) */
    /* 0x08007EFA: 1a43     orrs     r2, r3 */
    r2 = r2 | r3;
    /* 0x08007EFC: 0a64     str      r2, [r1, #0x40] */
    *(uint32_t *)(r1 + 64) = r2;   /* store */  /* MMIO: EXTI.+0x40(w) */
    /* 0x08007EFE: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007F00: 4862     str      r0, [r1, #0x24] */
    *(uint32_t *)(r1 + 36) = r0;   /* store */  /* MMIO: EXTI.+0x24(w) */
    /* 0x08007F02: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08007F04: 6077     strb     r0, [r4, #0x1d] */
    *(uint8_t *)(r4 + 29) = r0;   /* store */  /* MMIO: RCC.+0x1D(w) */
    /* 0x08007F06: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08007F08: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
