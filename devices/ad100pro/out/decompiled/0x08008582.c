/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008582
 * Name      : FUN_08008582
 * End       : 0x08008652
 * Size      : 208 bytes, 100 instructions decoded in body (functions.json insns=100, blocks=14)
 * Class     : function
 * Callers   : 1  -> 0x08009768 FUN_08009768
 * Callees   : 4  -> 0x0800BB84 FUN_0800BB84, 0x0800BC0C FUN_0800BC0C, 0x0800BC8C FUN_0800BC8C, 0x0800BD10 FUN_0800BD10
 * MMIO      : RCC.APB1RSTR, RCC.APB2ENR, SYS.+0x0
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08008582(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08008582: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x08008584: 0646     mov      r6, r0 */
    r6 = r0;
    /* 0x08008586: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x08008588: 2036     adds     r6, #0x20 */
    r6 = r6 + 0x20;
    /* 0x0800858A: 307e     ldrb     r0, [r6, #0x18] */
    r0 = *(uint8_t *)(r6 + 24);
    /* 0x0800858C: 0d46     mov      r5, r1 */
    r5 = r1;
    /* 0x0800858E: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08008590: 0dd0     beq      #0x80085ae */
    if (r0 == 1) goto L_080085AE;
    /* 0x08008592: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08008594: 3076     strb     r0, [r6, #0x18] */
    *(uint8_t *)(r6 + 24) = r0;   /* store */
    /* 0x08008596: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08008598: 7076     strb     r0, [r6, #0x19] */
    *(uint8_t *)(r6 + 25) = r0;   /* store */
    /* 0x0800859A: 0427     movs     r7, #4 */
    r7 = 4;
    /* 0x0800859C: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x0800859E: 08d0     beq      #0x80085b2 */
    if (r2 == 0) goto L_080085B2;
    /* 0x080085A0: 042a     cmp      r2, #4 */
    /* flags = cmp(r2, #4) */
    /* 0x080085A2: 16d0     beq      #0x80085d2 */
    if (r2 == 4) goto L_080085D2;
    /* 0x080085A4: 082a     cmp      r2, #8 */
    /* flags = cmp(r2, #8) */
    /* 0x080085A6: 2ad0     beq      #0x80085fe */
    if (r2 == 8) goto L_080085FE;
    /* 0x080085A8: 0c2a     cmp      r2, #0xc */
    /* flags = cmp(r2, #0xc) */
    /* 0x080085AA: 4dd1     bne      #0x8008648 */
    if (r2 != 0xc) goto L_08008648;
    /* 0x080085AC: 37e0     b        #0x800861e */
    goto L_0800861E;

    L_080085AE:
    /* 0x080085AE: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x080085B0: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_080085B2:
    /* 0x080085B2: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x080085B4: 03f0e6fa bl       #0x800bb84 */
    FUN_0800BB84();   /* BL -> 0x0800BB84 FUN_0800BB84 */
    /* 0x080085B8: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x080085BA: 8169     ldr      r1, [r0, #0x18] */
    r1 = *(uint32_t *)(r0 + 24);  /* MMIO: RCC.APB2ENR(r) */
    /* 0x080085BC: 0822     movs     r2, #8 */
    r2 = 8;
    /* 0x080085BE: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x080085C0: 8161     str      r1, [r0, #0x18] */
    *(uint32_t *)(r0 + 24) = r1;   /* store */  /* MMIO: RCC.APB2ENR(w) */
    /* 0x080085C2: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x080085C4: 8169     ldr      r1, [r0, #0x18] */
    r1 = *(uint32_t *)(r0 + 24);  /* MMIO: RCC.APB2ENR(r) */
    /* 0x080085C6: b943     bics     r1, r7 */
    r1 = r1 & ~r7;
    /* 0x080085C8: 8161     str      r1, [r0, #0x18] */
    *(uint32_t *)(r0 + 24) = r1;   /* store */  /* MMIO: RCC.APB2ENR(w) */
    /* 0x080085CA: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x080085CC: 8169     ldr      r1, [r0, #0x18] */
    r1 = *(uint32_t *)(r0 + 24);  /* MMIO: RCC.APB2ENR(r) */
    /* 0x080085CE: 2a69     ldr      r2, [r5, #0x10] */
    r2 = *(uint32_t *)(r5 + 16);  /* MMIO: RCC.APB1RSTR(r) */
    /* 0x080085D0: 12e0     b        #0x80085f8 */
    goto L_080085F8;

    L_080085D2:
    /* 0x080085D2: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x080085D4: 03f01afb bl       #0x800bc0c */
    FUN_0800BC0C();   /* BL -> 0x0800BC0C FUN_0800BC0C */
    /* 0x080085D8: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x080085DA: 8269     ldr      r2, [r0, #0x18] */
    r2 = *(uint32_t *)(r0 + 24);  /* MMIO: RCC.APB2ENR(r) */
    /* 0x080085DC: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x080085DE: c902     lsls     r1, r1, #0xb */
    r1 = r1 << 0xb;
    /* 0x080085E0: 0a43     orrs     r2, r1 */
    r2 = r2 | r1;
    /* 0x080085E2: 8261     str      r2, [r0, #0x18] */
    *(uint32_t *)(r0 + 24) = r2;   /* store */  /* MMIO: RCC.APB2ENR(w) */
    /* 0x080085E4: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x080085E6: 8169     ldr      r1, [r0, #0x18] */
    r1 = *(uint32_t *)(r0 + 24);  /* MMIO: RCC.APB2ENR(r) */
    /* 0x080085E8: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x080085EA: 9202     lsls     r2, r2, #0xa */
    r2 = r2 << 0xa;
    /* 0x080085EC: 9143     bics     r1, r2 */
    r1 = r1 & ~r2;
    /* 0x080085EE: 8161     str      r1, [r0, #0x18] */
    *(uint32_t *)(r0 + 24) = r1;   /* store */  /* MMIO: RCC.APB2ENR(w) */
    /* 0x080085F0: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x080085F2: 8169     ldr      r1, [r0, #0x18] */
    r1 = *(uint32_t *)(r0 + 24);  /* MMIO: RCC.APB2ENR(r) */
    /* 0x080085F4: 2a69     ldr      r2, [r5, #0x10] */
    r2 = *(uint32_t *)(r5 + 16);  /* MMIO: RCC.APB1RSTR(r) */
    /* 0x080085F6: 1202     lsls     r2, r2, #8 */
    r2 = r2 << 8;

    L_080085F8:
    /* 0x080085F8: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x080085FA: 8161     str      r1, [r0, #0x18] */
    *(uint32_t *)(r0 + 24) = r1;   /* store */  /* MMIO: RCC.APB2ENR(w) */
    /* 0x080085FC: 24e0     b        #0x8008648 */
    goto L_08008648;

    L_080085FE:
    /* 0x080085FE: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x08008600: 03f044fb bl       #0x800bc8c */
    FUN_0800BC8C();   /* BL -> 0x0800BC8C FUN_0800BC8C */
    /* 0x08008604: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x08008606: c269     ldr      r2, [r0, #0x1c] */
    r2 = *(uint32_t *)(r0 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x08008608: 0821     movs     r1, #8 */
    r1 = 8;
    /* 0x0800860A: 0a43     orrs     r2, r1 */
    r2 = r2 | r1;
    /* 0x0800860C: c261     str      r2, [r0, #0x1c] */
    *(uint32_t *)(r0 + 28) = r2;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x0800860E: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x08008610: c169     ldr      r1, [r0, #0x1c] */
    r1 = *(uint32_t *)(r0 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x08008612: b943     bics     r1, r7 */
    r1 = r1 & ~r7;
    /* 0x08008614: c161     str      r1, [r0, #0x1c] */
    *(uint32_t *)(r0 + 28) = r1;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x08008616: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x08008618: c169     ldr      r1, [r0, #0x1c] */
    r1 = *(uint32_t *)(r0 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x0800861A: 2a69     ldr      r2, [r5, #0x10] */
    r2 = *(uint32_t *)(r5 + 16);  /* MMIO: RCC.APB1RSTR(r) */
    /* 0x0800861C: 12e0     b        #0x8008644 */
    goto L_08008644;

    L_0800861E:
    /* 0x0800861E: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x08008620: 03f076fb bl       #0x800bd10 */
    FUN_0800BD10();   /* BL -> 0x0800BD10 FUN_0800BD10 */
    /* 0x08008624: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x08008626: c169     ldr      r1, [r0, #0x1c] */
    r1 = *(uint32_t *)(r0 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x08008628: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800862A: d202     lsls     r2, r2, #0xb */
    r2 = r2 << 0xb;
    /* 0x0800862C: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x0800862E: c161     str      r1, [r0, #0x1c] */
    *(uint32_t *)(r0 + 28) = r1;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x08008630: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x08008632: c269     ldr      r2, [r0, #0x1c] */
    r2 = *(uint32_t *)(r0 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x08008634: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08008636: 8902     lsls     r1, r1, #0xa */
    r1 = r1 << 0xa;
    /* 0x08008638: 8a43     bics     r2, r1 */
    r2 = r2 & ~r1;
    /* 0x0800863A: c261     str      r2, [r0, #0x1c] */
    *(uint32_t *)(r0 + 28) = r2;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x0800863C: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x0800863E: c169     ldr      r1, [r0, #0x1c] */
    r1 = *(uint32_t *)(r0 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x08008640: 2a69     ldr      r2, [r5, #0x10] */
    r2 = *(uint32_t *)(r5 + 16);  /* MMIO: RCC.APB1RSTR(r) */
    /* 0x08008642: 1202     lsls     r2, r2, #8 */
    r2 = r2 << 8;

    L_08008644:
    /* 0x08008644: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x08008646: c161     str      r1, [r0, #0x1c] */
    *(uint32_t *)(r0 + 28) = r1;   /* store */  /* MMIO: RCC.APB1ENR(w) */

    L_08008648:
    /* 0x08008648: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800864A: 7076     strb     r0, [r6, #0x19] */
    *(uint8_t *)(r6 + 25) = r0;   /* store */
    /* 0x0800864C: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800864E: 3076     strb     r0, [r6, #0x18] */
    *(uint8_t *)(r6 + 24) = r0;   /* store */
    /* 0x08008650: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
