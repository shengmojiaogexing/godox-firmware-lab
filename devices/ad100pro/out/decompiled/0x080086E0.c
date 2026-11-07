/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080086E0
 * Name      : FUN_080086E0
 * End       : 0x0800872A
 * Size      : 74 bytes, 37 instructions decoded in body (functions.json insns=37, blocks=7)
 * Class     : function
 * Callers   : 1  -> 0x0800663C FUN_0800663C
 * Callees   : 0  -> -
 * MMIO      : RCC.CFGR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080086E0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080086E0: 7fb5     push     {r0, r1, r2, r3, r4, r5, r6, lr} */
    push({r0, r1, r2, r3, r4, r5, r6, lr});
    /* 0x080086E2: 1248     ldr      r0, [pc, #0x48]  ; =RAM_2000048C */
    r0 = *(uint32_t *)0x0800872C;   /* = RAM 0x2000048C */
    /* 0x080086E4: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x080086E6: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);
    /* 0x080086E8: 0091     str      r1, [sp] */
    *(uint32_t *)(sp) = r1;   /* store */
    /* 0x080086EA: c169     ldr      r1, [r0, #0x1c] */
    r1 = *(uint32_t *)(r0 + 28);
    /* 0x080086EC: 0191     str      r1, [sp, #4] */
    *(uint32_t *)(sp + 4) = r1;   /* store */
    /* 0x080086EE: c16a     ldr      r1, [r0, #0x2c] */
    r1 = *(uint32_t *)(r0 + 44);
    /* 0x080086F0: 0291     str      r1, [sp, #8] */
    *(uint32_t *)(sp + 8) = r1;   /* store */
    /* 0x080086F2: c06b     ldr      r0, [r0, #0x3c] */
    r0 = *(uint32_t *)(r0 + 60);
    /* 0x080086F4: 0390     str      r0, [sp, #0xc] */
    *(uint32_t *)(sp + 12) = r0;   /* store */
    /* 0x080086F6: 6846     mov      r0, sp */
    r0 = sp;

    L_080086F8:
    /* 0x080086F8: 0021     movs     r1, #0 */
    r1 = 0;

    L_080086FA:
    /* 0x080086FA: 8c00     lsls     r4, r1, #2 */
    r4 = r1 << 2;
    /* 0x080086FC: 2518     adds     r5, r4, r0 */
    r5 = r4 + r0;
    /* 0x080086FE: 0659     ldr      r6, [r0, r4] */
    r6 = *(uint32_t *)(r0 + r4);
    /* 0x08008700: 6b68     ldr      r3, [r5, #4] */
    r3 = *(uint32_t *)(r5 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x08008702: 9e42     cmp      r6, r3 */
    /* flags = cmp(r6, r3) */
    /* 0x08008704: 01d9     bls      #0x800870a */
    if (r6 <= r3) goto L_0800870A;   /* unsigned */
    /* 0x08008706: 6e60     str      r6, [r5, #4] */
    *(uint32_t *)(r5 + 4) = r6;   /* store */  /* MMIO: RCC.CFGR(w) */
    /* 0x08008708: 0351     str      r3, [r0, r4] */
    *(uint32_t *)(r0 + r4) = r3;   /* store */

    L_0800870A:
    /* 0x0800870A: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x0800870C: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x0800870E: 0329     cmp      r1, #3 */
    /* flags = cmp(r1, #3) */
    /* 0x08008710: f3d3     blo      #0x80086fa */
    if (r1 < 3) goto L_080086FA;   /* unsigned */
    /* 0x08008712: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x08008714: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);
    /* 0x08008716: 042a     cmp      r2, #4 */
    /* flags = cmp(r2, #4) */
    /* 0x08008718: eed3     blo      #0x80086f8 */
    if (r2 < 4) goto L_080086F8;   /* unsigned */
    /* 0x0800871A: 0299     ldr      r1, [sp, #8] */
    r1 = *(uint32_t *)(sp + 8);
    /* 0x0800871C: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x0800871E: 4018     adds     r0, r0, r1 */
    r0 = r0 + r1;
    /* 0x08008720: 4008     lsrs     r0, r0, #1 */
    r0 = r0 >> 1;
    /* 0x08008722: 0349     ldr      r1, [pc, #0xc]  ; =RAM_20000208 */
    r1 = *(uint32_t *)0x08008730;   /* = RAM 0x20000208 */
    /* 0x08008724: 0e30     adds     r0, #0xe */
    r0 = r0 + 0xe;
    /* 0x08008726: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */
    /* 0x08008728: 7fbd     pop      {r0, r1, r2, r3, r4, r5, r6, pc} */
    pop({r0, r1, r2, r3, r4, r5, r6, pc});   /* return */
}
