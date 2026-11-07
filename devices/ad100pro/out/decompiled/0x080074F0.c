/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080074F0
 * Name      : FUN_080074F0
 * End       : 0x0800753E
 * Size      : 78 bytes, 37 instructions decoded in body (functions.json insns=37, blocks=7)
 * Class     : hub
 * Callers   : 19  -> 0x08009038 FUN_08009038, 0x08009046 FUN_08009046, 0x0800A934 FUN_0800A934, 0x0800AA0A FUN_0800AA0A, 0x0800ABE4 FUN_0800ABE4, 0x0800AC64 FUN_0800AC64, 0x0800B344 FUN_0800B344, 0x0800B372 FUN_0800B372, ... +11
 * Callees   : 1  -> 0x08007434 delay_or_timing
 * MMIO      : EXTI.+0x11, EXTI.IMR, EXTI.SWIER
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080074F0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080074F0: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x080074F2: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x080074F4: 007c     ldrb     r0, [r0, #0x10] */
    r0 = *(uint8_t *)(r0 + 16);
    /* 0x080074F6: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x080074F8: 08d0     beq      #0x800750c */
    if (r0 == 1) goto L_0800750C;
    /* 0x080074FA: 0126     movs     r6, #1 */
    r6 = 1;
    /* 0x080074FC: 2674     strb     r6, [r4, #0x10] */
    *(uint8_t *)(r4 + 16) = r6;   /* store */  /* MMIO: EXTI.SWIER(w) */
    /* 0x080074FE: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08007500: 6074     strb     r0, [r4, #0x11] */
    *(uint8_t *)(r4 + 17) = r0;   /* store */  /* MMIO: EXTI.+0x11(w) */
    /* 0x08007502: fff797ff bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007506: 0546     mov      r5, r0 */
    r5 = r0;
    /* 0x08007508: 0027     movs     r7, #0 */
    r7 = 0;
    /* 0x0800750A: 0ce0     b        #0x8007526 */
    goto L_08007526;

    L_0800750C:
    /* 0x0800750C: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x0800750E: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_08007510:
    /* 0x08007510: fff790ff bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08007514: 411b     subs     r1, r0, r5 */
    r1 = r0 - r5;
    /* 0x08007516: 7d20     movs     r0, #0x7d */
    r0 = 0x7d;
    /* 0x08007518: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x0800751A: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x0800751C: 03d9     bls      #0x8007526 */
    if (r1 <= r0) goto L_08007526;   /* unsigned */
    /* 0x0800751E: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x08007520: 6074     strb     r0, [r4, #0x11] */
    *(uint8_t *)(r4 + 17) = r0;   /* store */  /* MMIO: EXTI.+0x11(w) */
    /* 0x08007522: 2774     strb     r7, [r4, #0x10] */
    *(uint8_t *)(r4 + 16) = r7;   /* store */  /* MMIO: EXTI.SWIER(w) */
    /* 0x08007524: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_08007526:
    /* 0x08007526: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: EXTI.IMR(r) */
    /* 0x08007528: c068     ldr      r0, [r0, #0xc] */
    r0 = *(uint32_t *)(r0 + 12);
    /* 0x0800752A: c043     mvns     r0, r0 */
    r0 = ~r0;
    /* 0x0800752C: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x0800752E: efd5     bpl      #0x8007510 */
    if (r1 >= r0) goto L_08007510;   /* result non-negative */
    /* 0x08007530: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: EXTI.IMR(r) */
    /* 0x08007532: 0348     ldr      r0, [pc, #0xc]  ; =0x0000AAAA */
    r0 = *(uint32_t *)0x08007540;   /* = 0x0000AAAA */
    /* 0x08007534: 0860     str      r0, [r1] */
    *(uint32_t *)(r1) = r0;   /* store */  /* MMIO: EXTI.IMR(w) */
    /* 0x08007536: 6674     strb     r6, [r4, #0x11] */
    *(uint8_t *)(r4 + 17) = r6;   /* store */  /* MMIO: EXTI.+0x11(w) */
    /* 0x08007538: 2774     strb     r7, [r4, #0x10] */
    *(uint8_t *)(r4 + 16) = r7;   /* store */  /* MMIO: EXTI.SWIER(w) */
    /* 0x0800753A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800753C: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
