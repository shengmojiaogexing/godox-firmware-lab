/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800BD10
 * Name      : FUN_0800BD10
 * End       : 0x0800BD6A
 * Size      : 90 bytes, 45 instructions decoded in body (functions.json insns=45, blocks=6)
 * Class     : function
 * Callers   : 2  -> 0x08008582 FUN_08008582, 0x080085D6 FUN_080085D6
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800BD10(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800BD10: 30b5     push     {r4, r5, lr} */
    push({r4, r5, lr});
    /* 0x0800BD12: 026a     ldr      r2, [r0, #0x20] */
    r2 = *(uint32_t *)(r0 + 32);
    /* 0x0800BD14: 0123     movs     r3, #1 */
    r3 = 1;
    /* 0x0800BD16: 1b03     lsls     r3, r3, #0xc */
    r3 = r3 << 0xc;
    /* 0x0800BD18: 9a43     bics     r2, r3 */
    r2 = r2 & ~r3;
    /* 0x0800BD1A: 0262     str      r2, [r0, #0x20] */
    *(uint32_t *)(r0 + 32) = r2;   /* store */
    /* 0x0800BD1C: 046a     ldr      r4, [r0, #0x20] */
    r4 = *(uint32_t *)(r0 + 32);
    /* 0x0800BD1E: 4268     ldr      r2, [r0, #4] */
    r2 = *(uint32_t *)(r0 + 4);
    /* 0x0800BD20: c369     ldr      r3, [r0, #0x1c] */
    r3 = *(uint32_t *)(r0 + 28);
    /* 0x0800BD22: 7325     movs     r5, #0x73 */
    r5 = 0x73;
    /* 0x0800BD24: 2d02     lsls     r5, r5, #8 */
    r5 = r5 << 8;
    /* 0x0800BD26: ab43     bics     r3, r5 */
    r3 = r3 & ~r5;
    /* 0x0800BD28: 0d68     ldr      r5, [r1] */
    r5 = *(uint32_t *)(r1);
    /* 0x0800BD2A: 2d02     lsls     r5, r5, #8 */
    r5 = r5 << 8;
    /* 0x0800BD2C: 1d43     orrs     r5, r3 */
    r5 = r5 | r3;
    /* 0x0800BD2E: 0123     movs     r3, #1 */
    r3 = 1;
    /* 0x0800BD30: 5b03     lsls     r3, r3, #0xd */
    r3 = r3 << 0xd;
    /* 0x0800BD32: 9c43     bics     r4, r3 */
    r4 = r4 & ~r3;
    /* 0x0800BD34: 8b68     ldr      r3, [r1, #8] */
    r3 = *(uint32_t *)(r1 + 8);
    /* 0x0800BD36: 1b03     lsls     r3, r3, #0xc */
    r3 = r3 << 0xc;
    /* 0x0800BD38: 2343     orrs     r3, r4 */
    r3 = r3 | r4;
    /* 0x0800BD3A: 0c4c     ldr      r4, [pc, #0x30]  ; =TIM1.CR1 */
    r4 = *(uint32_t *)0x0800BD6C;   /* = MMIO TIM1+0x00 */
    /* 0x0800BD3C: a042     cmp      r0, r4 */
    /* flags = cmp(r0, r4) */
    /* 0x0800BD3E: 08d0     beq      #0x800bd52 */
    if (r0 == r4) goto L_0800BD52;
    /* 0x0800BD40: 0b4c     ldr      r4, [pc, #0x2c]  ; =TIM15.CR1 */
    r4 = *(uint32_t *)0x0800BD70;   /* = MMIO TIM15+0x00 */
    /* 0x0800BD42: a042     cmp      r0, r4 */
    /* flags = cmp(r0, r4) */
    /* 0x0800BD44: 05d0     beq      #0x800bd52 */
    if (r0 == r4) goto L_0800BD52;
    /* 0x0800BD46: 0b4c     ldr      r4, [pc, #0x2c]  ; =TIM16.CR1 */
    r4 = *(uint32_t *)0x0800BD74;   /* = MMIO TIM16+0x00 */
    /* 0x0800BD48: a042     cmp      r0, r4 */
    /* flags = cmp(r0, r4) */
    /* 0x0800BD4A: 02d0     beq      #0x800bd52 */
    if (r0 == r4) goto L_0800BD52;
    /* 0x0800BD4C: 0a4c     ldr      r4, [pc, #0x28]  ; =TIM17.CR1 */
    r4 = *(uint32_t *)0x0800BD78;   /* = MMIO TIM17+0x00 */
    /* 0x0800BD4E: a042     cmp      r0, r4 */
    /* flags = cmp(r0, r4) */
    /* 0x0800BD50: 05d1     bne      #0x800bd5e */
    if (r0 != r4) goto L_0800BD5E;

    L_0800BD52:
    /* 0x0800BD52: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x0800BD54: a403     lsls     r4, r4, #0xe */
    r4 = r4 << 0xe;
    /* 0x0800BD56: a243     bics     r2, r4 */
    r2 = r2 & ~r4;
    /* 0x0800BD58: 4c69     ldr      r4, [r1, #0x14] */
    r4 = *(uint32_t *)(r1 + 20);
    /* 0x0800BD5A: a401     lsls     r4, r4, #6 */
    r4 = r4 << 6;
    /* 0x0800BD5C: 2243     orrs     r2, r4 */
    r2 = r2 | r4;

    L_0800BD5E:
    /* 0x0800BD5E: 4260     str      r2, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r2;   /* store */
    /* 0x0800BD60: c561     str      r5, [r0, #0x1c] */
    *(uint32_t *)(r0 + 28) = r5;   /* store */
    /* 0x0800BD62: 4968     ldr      r1, [r1, #4] */
    r1 = *(uint32_t *)(r1 + 4);
    /* 0x0800BD64: 0164     str      r1, [r0, #0x40] */
    *(uint32_t *)(r0 + 64) = r1;   /* store */
    /* 0x0800BD66: 0362     str      r3, [r0, #0x20] */
    *(uint32_t *)(r0 + 32) = r3;   /* store */
    /* 0x0800BD68: 30bd     pop      {r4, r5, pc} */
    pop({r4, r5, pc});   /* return */
}
