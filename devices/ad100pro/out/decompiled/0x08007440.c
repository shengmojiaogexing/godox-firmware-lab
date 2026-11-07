/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007440
 * Name      : FUN_08007440
 * End       : 0x080074E2
 * Size      : 162 bytes, 78 instructions decoded in body (functions.json insns=78, blocks=15)
 * Class     : function
 * Callers   : 1  -> 0x080095CC FUN_080095CC
 * Callees   : 2  -> 0x08007434 delay_or_timing, 0x080074EC FUN_080074EC
 * MMIO      : EXTI.+0x11, EXTI.IMR, EXTI.SWIER
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007440(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007440: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08007442: 0400     movs     r4, r0 */
    r4 = r0;
    /* 0x08007444: 0cd0     beq      #0x8007460 */
    if (/* beq */) goto L_08007460;
    /* 0x08007446: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: EXTI.IMR(r) */
    /* 0x08007448: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);
    /* 0x0800744A: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x0800744C: c907     lsls     r1, r1, #0x1f */
    r1 = r1 << 0x1f;
    /* 0x0800744E: 09d1     bne      #0x8007464 */
    if (/* bne */) goto L_08007464;
    /* 0x08007450: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);
    /* 0x08007452: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x08007454: 8907     lsls     r1, r1, #0x1e */
    r1 = r1 << 0x1e;
    /* 0x08007456: 05d4     bmi      #0x8007464 */
    if (/* bmi */) goto L_08007464;   /* result negative */
    /* 0x08007458: c068     ldr      r0, [r0, #0xc] */
    r0 = *(uint32_t *)(r0 + 12);
    /* 0x0800745A: c043     mvns     r0, r0 */
    r0 = ~r0;
    /* 0x0800745C: 4007     lsls     r0, r0, #0x1d */
    r0 = r0 << 0x1d;
    /* 0x0800745E: 01d4     bmi      #0x8007464 */
    if (/* bmi */) goto L_08007464;   /* result negative */

    L_08007460:
    /* 0x08007460: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08007462: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08007464:
    /* 0x08007464: 607c     ldrb     r0, [r4, #0x11] */
    r0 = *(uint8_t *)(r4 + 17);  /* MMIO: EXTI.+0x11(r) */
    /* 0x08007466: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08007468: 03d1     bne      #0x8007472 */
    if (r0 != 0) goto L_08007472;
    /* 0x0800746A: 2074     strb     r0, [r4, #0x10] */
    *(uint8_t *)(r4 + 16) = r0;   /* store */  /* MMIO: EXTI.SWIER(w) */
    /* 0x0800746C: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800746E: 00f03df8 bl       #0x80074ec */
    FUN_080074EC();   /* BL -> 0x080074EC FUN_080074EC */

    L_08007472:
    /* 0x08007472: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08007474: 6074     strb     r0, [r4, #0x11] */
    *(uint8_t *)(r4 + 17) = r0;   /* store */  /* MMIO: EXTI.+0x11(w) */
    /* 0x08007476: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: EXTI.IMR(r) */
    /* 0x08007478: 1a48     ldr      r0, [pc, #0x68]  ; =0x00005555 */
    r0 = *(uint32_t *)0x080074E4;   /* = 0x00005555 */
    /* 0x0800747A: 0860     str      r0, [r1] */
    *(uint32_t *)(r1) = r0;   /* store */  /* MMIO: EXTI.IMR(w) */
    /* 0x0800747C: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: EXTI.IMR(r) */
    /* 0x0800747E: 4168     ldr      r1, [r0, #4] */
    r1 = *(uint32_t *)(r0 + 4);
    /* 0x08007480: 6268     ldr      r2, [r4, #4] */
    r2 = *(uint32_t *)(r4 + 4);  /* MMIO: EXTI.EMR(r) */
    /* 0x08007482: c908     lsrs     r1, r1, #3 */
    r1 = r1 >> 3;
    /* 0x08007484: c900     lsls     r1, r1, #3 */
    r1 = r1 << 3;
    /* 0x08007486: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x08007488: 4160     str      r1, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r1;   /* store */
    /* 0x0800748A: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: EXTI.IMR(r) */
    /* 0x0800748C: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);
    /* 0x0800748E: a268     ldr      r2, [r4, #8] */
    r2 = *(uint32_t *)(r4 + 8);  /* MMIO: EXTI.RTSR(r) */
    /* 0x08007490: 090b     lsrs     r1, r1, #0xc */
    r1 = r1 >> 0xc;
    /* 0x08007492: 0903     lsls     r1, r1, #0xc */
    r1 = r1 << 0xc;
    /* 0x08007494: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x08007496: 8160     str      r1, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r1;   /* store */
    /* 0x08007498: 1348     ldr      r0, [pc, #0x4c]  ; =0x00000FFF */
    r0 = *(uint32_t *)0x080074E8;   /* = 0x00000FFF */
    /* 0x0800749A: e168     ldr      r1, [r4, #0xc] */
    r1 = *(uint32_t *)(r4 + 12);  /* MMIO: EXTI.FTSR(r) */
    /* 0x0800749C: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x0800749E: 03d1     bne      #0x80074a8 */
    if (r1 != r0) goto L_080074A8;
    /* 0x080074A0: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: EXTI.IMR(r) */
    /* 0x080074A2: 0969     ldr      r1, [r1, #0x10] */
    r1 = *(uint32_t *)(r1 + 16);  /* MMIO: EXTI.SWIER(r) */
    /* 0x080074A4: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x080074A6: 18d0     beq      #0x80074da */
    if (r1 == r0) goto L_080074DA;

    L_080074A8:
    /* 0x080074A8: fff7c4ff bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x080074AC: 7d26     movs     r6, #0x7d */
    r6 = 0x7d;
    /* 0x080074AE: 0546     mov      r5, r0 */
    r5 = r0;
    /* 0x080074B0: f600     lsls     r6, r6, #3 */
    r6 = r6 << 3;
    /* 0x080074B2: 07e0     b        #0x80074c4 */
    goto L_080074C4;

    L_080074B4:
    /* 0x080074B4: fff7beff bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x080074B8: 401b     subs     r0, r0, r5 */
    r0 = r0 - r5;
    /* 0x080074BA: b042     cmp      r0, r6 */
    /* flags = cmp(r0, r6) */
    /* 0x080074BC: 02d9     bls      #0x80074c4 */
    if (r0 <= r6) goto L_080074C4;   /* unsigned */
    /* 0x080074BE: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x080074C0: 6074     strb     r0, [r4, #0x11] */
    *(uint8_t *)(r4 + 17) = r0;   /* store */  /* MMIO: EXTI.+0x11(w) */
    /* 0x080074C2: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_080074C4:
    /* 0x080074C4: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: EXTI.IMR(r) */
    /* 0x080074C6: c068     ldr      r0, [r0, #0xc] */
    r0 = *(uint32_t *)(r0 + 12);
    /* 0x080074C8: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080074CA: f3d1     bne      #0x80074b4 */
    if (r0 != 0) goto L_080074B4;
    /* 0x080074CC: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: EXTI.IMR(r) */
    /* 0x080074CE: 0169     ldr      r1, [r0, #0x10] */
    r1 = *(uint32_t *)(r0 + 16);
    /* 0x080074D0: e268     ldr      r2, [r4, #0xc] */
    r2 = *(uint32_t *)(r4 + 12);  /* MMIO: EXTI.FTSR(r) */
    /* 0x080074D2: 090b     lsrs     r1, r1, #0xc */
    r1 = r1 >> 0xc;
    /* 0x080074D4: 0903     lsls     r1, r1, #0xc */
    r1 = r1 << 0xc;
    /* 0x080074D6: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x080074D8: 0161     str      r1, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r1;   /* store */

    L_080074DA:
    /* 0x080074DA: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x080074DC: 6074     strb     r0, [r4, #0x11] */
    *(uint8_t *)(r4 + 17) = r0;   /* store */  /* MMIO: EXTI.+0x11(w) */
    /* 0x080074DE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080074E0: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
