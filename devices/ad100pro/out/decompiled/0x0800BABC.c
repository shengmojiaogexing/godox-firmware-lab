/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800BABC
 * Name      : FUN_0800BABC
 * End       : 0x0800BB30
 * Size      : 116 bytes, 58 instructions decoded in body (functions.json insns=58, blocks=18)
 * Class     : function
 * Callers   : 4  -> 0x080081E8 FUN_080081E8, 0x08008206 FUN_08008206, 0x08008652 FUN_08008652, 0x08008670 FUN_08008670
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800BABC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800BABC: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x0800BABE: 0268     ldr      r2, [r0] */
    r2 = *(uint32_t *)(r0);
    /* 0x0800BAC0: 1b4b     ldr      r3, [pc, #0x6c]  ; =TIM1.CR1 */
    r3 = *(uint32_t *)0x0800BB30;   /* = MMIO TIM1+0x00 */
    /* 0x0800BAC2: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x0800BAC4: a407     lsls     r4, r4, #0x1e */
    r4 = r4 << 0x1e;
    /* 0x0800BAC6: 1b4d     ldr      r5, [pc, #0x6c]  ; =TIM3.CR1 */
    r5 = *(uint32_t *)0x0800BB34;   /* = MMIO TIM3+0x00 */
    /* 0x0800BAC8: 9842     cmp      r0, r3 */
    /* flags = cmp(r0, r3) */
    /* 0x0800BACA: 03d0     beq      #0x800bad4 */
    if (r0 == r3) goto L_0800BAD4;
    /* 0x0800BACC: a042     cmp      r0, r4 */
    /* flags = cmp(r0, r4) */
    /* 0x0800BACE: 01d0     beq      #0x800bad4 */
    if (r0 == r4) goto L_0800BAD4;
    /* 0x0800BAD0: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x0800BAD2: 03d1     bne      #0x800badc */
    if (r0 != r5) goto L_0800BADC;

    L_0800BAD4:
    /* 0x0800BAD4: 7026     movs     r6, #0x70 */
    r6 = 0x70;
    /* 0x0800BAD6: b243     bics     r2, r6 */
    r2 = r2 & ~r6;
    /* 0x0800BAD8: 4e68     ldr      r6, [r1, #4] */
    r6 = *(uint32_t *)(r1 + 4);
    /* 0x0800BADA: 3243     orrs     r2, r6 */
    r2 = r2 | r6;

    L_0800BADC:
    /* 0x0800BADC: 164e     ldr      r6, [pc, #0x58]  ; =TIM15.CR1 */
    r6 = *(uint32_t *)0x0800BB38;   /* = MMIO TIM15+0x00 */
    /* 0x0800BADE: 174f     ldr      r7, [pc, #0x5c]  ; =TIM16.CR1 */
    r7 = *(uint32_t *)0x0800BB3C;   /* = MMIO TIM16+0x00 */
    /* 0x0800BAE0: 9842     cmp      r0, r3 */
    /* flags = cmp(r0, r3) */
    /* 0x0800BAE2: 0dd0     beq      #0x800bb00 */
    if (r0 == r3) goto L_0800BB00;
    /* 0x0800BAE4: a042     cmp      r0, r4 */
    /* flags = cmp(r0, r4) */
    /* 0x0800BAE6: 0bd0     beq      #0x800bb00 */
    if (r0 == r4) goto L_0800BB00;
    /* 0x0800BAE8: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x0800BAEA: 09d0     beq      #0x800bb00 */
    if (r0 == r5) goto L_0800BB00;
    /* 0x0800BAEC: 144c     ldr      r4, [pc, #0x50]  ; =TIM14.CR1 */
    r4 = *(uint32_t *)0x0800BB40;   /* = MMIO TIM14+0x00 */
    /* 0x0800BAEE: a042     cmp      r0, r4 */
    /* flags = cmp(r0, r4) */
    /* 0x0800BAF0: 06d0     beq      #0x800bb00 */
    if (r0 == r4) goto L_0800BB00;
    /* 0x0800BAF2: b042     cmp      r0, r6 */
    /* flags = cmp(r0, r6) */
    /* 0x0800BAF4: 04d0     beq      #0x800bb00 */
    if (r0 == r6) goto L_0800BB00;
    /* 0x0800BAF6: b842     cmp      r0, r7 */
    /* flags = cmp(r0, r7) */
    /* 0x0800BAF8: 02d0     beq      #0x800bb00 */
    if (r0 == r7) goto L_0800BB00;
    /* 0x0800BAFA: 124c     ldr      r4, [pc, #0x48]  ; =TIM17.CR1 */
    r4 = *(uint32_t *)0x0800BB44;   /* = MMIO TIM17+0x00 */
    /* 0x0800BAFC: a042     cmp      r0, r4 */
    /* flags = cmp(r0, r4) */
    /* 0x0800BAFE: 04d1     bne      #0x800bb0a */
    if (r0 != r4) goto L_0800BB0A;

    L_0800BB00:
    /* 0x0800BB00: 0324     movs     r4, #3 */
    r4 = 3;
    /* 0x0800BB02: 2402     lsls     r4, r4, #8 */
    r4 = r4 << 8;
    /* 0x0800BB04: a243     bics     r2, r4 */
    r2 = r2 & ~r4;
    /* 0x0800BB06: cc68     ldr      r4, [r1, #0xc] */
    r4 = *(uint32_t *)(r1 + 12);
    /* 0x0800BB08: 2243     orrs     r2, r4 */
    r2 = r2 | r4;

    L_0800BB0A:
    /* 0x0800BB0A: 0260     str      r2, [r0] */
    *(uint32_t *)(r0) = r2;   /* store */
    /* 0x0800BB0C: 8a68     ldr      r2, [r1, #8] */
    r2 = *(uint32_t *)(r1 + 8);
    /* 0x0800BB0E: c262     str      r2, [r0, #0x2c] */
    *(uint32_t *)(r0 + 44) = r2;   /* store */
    /* 0x0800BB10: 0a68     ldr      r2, [r1] */
    r2 = *(uint32_t *)(r1);
    /* 0x0800BB12: 8262     str      r2, [r0, #0x28] */
    *(uint32_t *)(r0 + 40) = r2;   /* store */
    /* 0x0800BB14: 9842     cmp      r0, r3 */
    /* flags = cmp(r0, r3) */
    /* 0x0800BB16: 06d0     beq      #0x800bb26 */
    if (r0 == r3) goto L_0800BB26;
    /* 0x0800BB18: b042     cmp      r0, r6 */
    /* flags = cmp(r0, r6) */
    /* 0x0800BB1A: 04d0     beq      #0x800bb26 */
    if (r0 == r6) goto L_0800BB26;
    /* 0x0800BB1C: b842     cmp      r0, r7 */
    /* flags = cmp(r0, r7) */
    /* 0x0800BB1E: 02d0     beq      #0x800bb26 */
    if (r0 == r7) goto L_0800BB26;
    /* 0x0800BB20: 084a     ldr      r2, [pc, #0x20]  ; =TIM17.CR1 */
    r2 = *(uint32_t *)0x0800BB44;   /* = MMIO TIM17+0x00 */
    /* 0x0800BB22: 9042     cmp      r0, r2 */
    /* flags = cmp(r0, r2) */
    /* 0x0800BB24: 01d1     bne      #0x800bb2a */
    if (r0 != r2) goto L_0800BB2A;

    L_0800BB26:
    /* 0x0800BB26: 0969     ldr      r1, [r1, #0x10] */
    r1 = *(uint32_t *)(r1 + 16);
    /* 0x0800BB28: 0163     str      r1, [r0, #0x30] */
    *(uint32_t *)(r0 + 48) = r1;   /* store */

    L_0800BB2A:
    /* 0x0800BB2A: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800BB2C: 4161     str      r1, [r0, #0x14] */
    *(uint32_t *)(r0 + 20) = r1;   /* store */
    /* 0x0800BB2E: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
