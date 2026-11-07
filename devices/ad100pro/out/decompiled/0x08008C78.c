/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008C78
 * Name      : GPIO_init
 * End       : 0x08008CF6
 * Size      : 126 bytes, 61 instructions decoded in body (functions.json insns=61, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x08004AF0 FUN_08004AF0
 * Callees   : 1  -> 0x08007426 FUN_08007426
 * MMIO      : GPIOB.BRR, GPIOB.MODER, GPIOB.PUPDR, GPIOC.MODER
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void GPIO_init(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08008C78: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x08008C7A: 1f4d     ldr      r5, [pc, #0x7c]  ; =GPIOC.MODER */
    r5 = *(uint32_t *)0x08008CF8;   /* = MMIO GPIOC+0x00 */
    /* 0x08008C7C: 2868     ldr      r0, [r5] */
    r0 = *(uint32_t *)(r5);  /* MMIO: GPIOC.MODER(r) */
    /* 0x08008C7E: 0126     movs     r6, #1 */
    r6 = 1;
    /* 0x08008C80: 7603     lsls     r6, r6, #0xd */
    r6 = r6 << 0xd;
    /* 0x08008C82: b043     bics     r0, r6 */
    r0 = r0 & ~r6;
    /* 0x08008C84: 2860     str      r0, [r5] */
    *(uint32_t *)(r5) = r0;   /* store */  /* MMIO: GPIOC.MODER(w) */
    /* 0x08008C86: 2868     ldr      r0, [r5] */
    r0 = *(uint32_t *)(r5);  /* MMIO: GPIOC.MODER(r) */
    /* 0x08008C88: 7710     asrs     r7, r6, #1 */
    r7 = r6 >> 1;
    /* 0x08008C8A: 3843     orrs     r0, r7 */
    r0 = r0 | r7;
    /* 0x08008C8C: 2860     str      r0, [r5] */
    *(uint32_t *)(r5) = r0;   /* store */  /* MMIO: GPIOC.MODER(w) */
    /* 0x08008C8E: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08008C90: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x08008C92: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08008C94: fef7c7fb bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08008C98: 184c     ldr      r4, [pc, #0x60]  ; =GPIOB.MODER */
    r4 = *(uint32_t *)0x08008CFC;   /* = MMIO GPIOB+0x00 */
    /* 0x08008C9A: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: GPIOB.MODER(r) */
    /* 0x08008C9C: 6104     lsls     r1, r4, #0x11 */
    r1 = r4 << 0x11;
    /* 0x08008C9E: 8843     bics     r0, r1 */
    r0 = r0 & ~r1;
    /* 0x08008CA0: 2060     str      r0, [r4] */
    *(uint32_t *)(r4) = r0;   /* store */  /* MMIO: GPIOB.MODER(w) */
    /* 0x08008CA2: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: GPIOB.MODER(r) */
    /* 0x08008CA4: 4910     asrs     r1, r1, #1 */
    r1 = r1 >> 1;
    /* 0x08008CA6: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08008CA8: 2060     str      r0, [r4] */
    *(uint32_t *)(r4) = r0;   /* store */  /* MMIO: GPIOB.MODER(w) */
    /* 0x08008CAA: a662     str      r6, [r4, #0x28] */
    *(uint32_t *)(r4 + 40) = r6;   /* store */  /* MMIO: GPIOB.BRR(w) */
    /* 0x08008CAC: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: GPIOB.MODER(r) */
    /* 0x08008CAE: 4910     asrs     r1, r1, #1 */
    r1 = r1 >> 1;
    /* 0x08008CB0: 8843     bics     r0, r1 */
    r0 = r0 & ~r1;
    /* 0x08008CB2: 2060     str      r0, [r4] */
    *(uint32_t *)(r4) = r0;   /* store */  /* MMIO: GPIOB.MODER(w) */
    /* 0x08008CB4: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: GPIOB.MODER(r) */
    /* 0x08008CB6: 4910     asrs     r1, r1, #1 */
    r1 = r1 >> 1;
    /* 0x08008CB8: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08008CBA: 2060     str      r0, [r4] */
    *(uint32_t *)(r4) = r0;   /* store */  /* MMIO: GPIOB.MODER(w) */
    /* 0x08008CBC: a762     str      r7, [r4, #0x28] */
    *(uint32_t *)(r4 + 40) = r7;   /* store */  /* MMIO: GPIOB.BRR(w) */
    /* 0x08008CBE: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: GPIOB.MODER(r) */
    /* 0x08008CC0: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x08008CC2: 0007     lsls     r0, r0, #0x1c */
    r0 = r0 << 0x1c;
    /* 0x08008CC4: 8143     bics     r1, r0 */
    r1 = r1 & ~r0;
    /* 0x08008CC6: 2160     str      r1, [r4] */
    *(uint32_t *)(r4) = r1;   /* store */  /* MMIO: GPIOB.MODER(w) */
    /* 0x08008CC8: e168     ldr      r1, [r4, #0xc] */
    r1 = *(uint32_t *)(r4 + 12);  /* MMIO: GPIOB.PUPDR(r) */
    /* 0x08008CCA: 8143     bics     r1, r0 */
    r1 = r1 & ~r0;
    /* 0x08008CCC: e160     str      r1, [r4, #0xc] */
    *(uint32_t *)(r4 + 12) = r1;   /* store */  /* MMIO: GPIOB.PUPDR(w) */
    /* 0x08008CCE: e068     ldr      r0, [r4, #0xc] */
    r0 = *(uint32_t *)(r4 + 12);  /* MMIO: GPIOB.PUPDR(r) */
    /* 0x08008CD0: e104     lsls     r1, r4, #0x13 */
    r1 = r4 << 0x13;
    /* 0x08008CD2: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08008CD4: e060     str      r0, [r4, #0xc] */
    *(uint32_t *)(r4 + 12) = r0;   /* store */  /* MMIO: GPIOB.PUPDR(w) */
    /* 0x08008CD6: 2868     ldr      r0, [r5] */
    r0 = *(uint32_t *)(r5);  /* MMIO: GPIOC.MODER(r) */
    /* 0x08008CD8: 0913     asrs     r1, r1, #0xc */
    r1 = r1 >> 0xc;
    /* 0x08008CDA: 8843     bics     r0, r1 */
    r0 = r0 & ~r1;
    /* 0x08008CDC: 2860     str      r0, [r5] */
    *(uint32_t *)(r5) = r0;   /* store */  /* MMIO: GPIOC.MODER(w) */
    /* 0x08008CDE: 2868     ldr      r0, [r5] */
    r0 = *(uint32_t *)(r5);  /* MMIO: GPIOC.MODER(r) */
    /* 0x08008CE0: 4910     asrs     r1, r1, #1 */
    r1 = r1 >> 1;
    /* 0x08008CE2: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08008CE4: 2860     str      r0, [r5] */
    *(uint32_t *)(r5) = r0;   /* store */  /* MMIO: GPIOC.MODER(w) */
    /* 0x08008CE6: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08008CE8: 0912     asrs     r1, r1, #8 */
    r1 = r1 >> 8;
    /* 0x08008CEA: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08008CEC: fef79bfb bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08008CF0: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: GPIOB.MODER(r) */
    /* 0x08008CF2: 2060     str      r0, [r4] */
    *(uint32_t *)(r4) = r0;   /* store */  /* MMIO: GPIOB.MODER(w) */
    /* 0x08008CF4: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
