/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007544
 * Name      : FUN_08007544
 * End       : 0x080075B0
 * Size      : 108 bytes, 52 instructions decoded in body (functions.json insns=52, blocks=11)
 * Class     : function
 * Callers   : 2  -> 0x0800CF74 main, 0x0800CFDE FUN_0800CFDE
 * Callees   : 1  -> 0x08007434 delay_or_timing
 * MMIO      : EXTI.+0x11, EXTI.FTSR, EXTI.IMR, EXTI.SWIER
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007544(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007544: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x08007546: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x08007548: 007c     ldrb     r0, [r0, #0x10] */
    r0 = *(uint8_t *)(r0 + 16);
    /* 0x0800754A: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800754C: 12d0     beq      #0x8007574 */
    if (r0 == 1) goto L_08007574;
    /* 0x0800754E: 0126     movs     r6, #1 */
    r6 = 1;
    /* 0x08007550: 2674     strb     r6, [r4, #0x10] */
    *(uint8_t *)(r4 + 16) = r6;   /* store */  /* MMIO: EXTI.SWIER(w) */
    /* 0x08007552: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08007554: 6074     strb     r0, [r4, #0x11] */
    *(uint8_t *)(r4 + 17) = r0;   /* store */  /* MMIO: EXTI.+0x11(w) */
    /* 0x08007556: 1649     ldr      r1, [pc, #0x58]  ; =0x00000FFF */
    r1 = *(uint32_t *)0x080075B0;   /* = 0x00000FFF */
    /* 0x08007558: e068     ldr      r0, [r4, #0xc] */
    r0 = *(uint32_t *)(r4 + 12);  /* MMIO: EXTI.FTSR(r) */
    /* 0x0800755A: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x0800755C: 02d1     bne      #0x8007564 */
    if (r0 != r1) goto L_08007564;
    /* 0x0800755E: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: EXTI.IMR(r) */
    /* 0x08007560: 1448     ldr      r0, [pc, #0x50]  ; =0x0000AAAA */
    r0 = *(uint32_t *)0x080075B4;   /* = 0x0000AAAA */
    /* 0x08007562: 0860     str      r0, [r1] */
    *(uint32_t *)(r1) = r0;   /* store */

    L_08007564:
    /* 0x08007564: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: EXTI.IMR(r) */
    /* 0x08007566: 1448     ldr      r0, [pc, #0x50]  ; =0x0000CCCC */
    r0 = *(uint32_t *)0x080075B8;   /* = 0x0000CCCC */
    /* 0x08007568: 0860     str      r0, [r1] */
    *(uint32_t *)(r1) = r0;   /* store */
    /* 0x0800756A: fff763ff bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x0800756E: 0546     mov      r5, r0 */
    r5 = r0;
    /* 0x08007570: 0027     movs     r7, #0 */
    r7 = 0;
    /* 0x08007572: 0ce0     b        #0x800758e */
    goto L_0800758E;

    L_08007574:
    /* 0x08007574: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08007576: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_08007578:
    /* 0x08007578: fff75cff bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x0800757C: 411b     subs     r1, r0, r5 */
    r1 = r0 - r5;
    /* 0x0800757E: 7d20     movs     r0, #0x7d */
    r0 = 0x7d;
    /* 0x08007580: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x08007582: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x08007584: 03d9     bls      #0x800758e */
    if (r1 <= r0) goto L_0800758E;   /* unsigned */
    /* 0x08007586: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x08007588: 6074     strb     r0, [r4, #0x11] */
    *(uint8_t *)(r4 + 17) = r0;   /* store */  /* MMIO: EXTI.+0x11(w) */
    /* 0x0800758A: 2774     strb     r7, [r4, #0x10] */
    *(uint8_t *)(r4 + 16) = r7;   /* store */  /* MMIO: EXTI.SWIER(w) */
    /* 0x0800758C: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_0800758E:
    /* 0x0800758E: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: EXTI.IMR(r) */
    /* 0x08007590: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);
    /* 0x08007592: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x08007594: c907     lsls     r1, r1, #0x1f */
    r1 = r1 << 0x1f;
    /* 0x08007596: 07d1     bne      #0x80075a8 */
    if (r1 != r0) goto L_080075A8;
    /* 0x08007598: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);
    /* 0x0800759A: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x0800759C: 8907     lsls     r1, r1, #0x1e */
    r1 = r1 << 0x1e;
    /* 0x0800759E: 03d4     bmi      #0x80075a8 */
    if (r1 < r0) goto L_080075A8;   /* result negative */
    /* 0x080075A0: c068     ldr      r0, [r0, #0xc] */
    r0 = *(uint32_t *)(r0 + 12);
    /* 0x080075A2: c043     mvns     r0, r0 */
    r0 = ~r0;
    /* 0x080075A4: 4007     lsls     r0, r0, #0x1d */
    r0 = r0 << 0x1d;
    /* 0x080075A6: e7d5     bpl      #0x8007578 */
    if (r1 >= r0) goto L_08007578;   /* result non-negative */

    L_080075A8:
    /* 0x080075A8: 6674     strb     r6, [r4, #0x11] */
    *(uint8_t *)(r4 + 17) = r6;   /* store */  /* MMIO: EXTI.+0x11(w) */
    /* 0x080075AA: 2774     strb     r7, [r4, #0x10] */
    *(uint8_t *)(r4 + 16) = r7;   /* store */  /* MMIO: EXTI.SWIER(w) */
    /* 0x080075AC: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080075AE: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
