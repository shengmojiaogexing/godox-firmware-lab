/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007104
 * Name      : FUN_08007104
 * End       : 0x08007152
 * Size      : 78 bytes, 38 instructions decoded in body (functions.json insns=38, blocks=3)
 * Class     : function
 * Callers   : 1  -> 0x08006BA4 FUN_08006BA4
 * Callees   : 1  -> 0x080047A0 FUN_080047A0
 * MMIO      : DMA1.ISR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007104(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007104: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x08007106: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x08007108: 2030     adds     r0, #0x20 */
    r0 = r0 + 0x20;
    /* 0x0800710A: 0578     ldrb     r5, [r0] */
    r5 = *(uint8_t *)(r0);  /* MMIO: DMA1.ISR(r) */
    /* 0x0800710C: 012d     cmp      r5, #1 */
    /* flags = cmp(r5, #1) */
    /* 0x0800710E: 1ed0     beq      #0x800714e */
    if (r5 == 1) goto L_0800714E;
    /* 0x08007110: 0125     movs     r5, #1 */
    r5 = 1;
    /* 0x08007112: 0570     strb     r5, [r0] */
    *(uint8_t *)(r0) = r5;   /* store */  /* MMIO: DMA1.ISR(w) */
    /* 0x08007114: 0226     movs     r6, #2 */
    r6 = 2;
    /* 0x08007116: 4670     strb     r6, [r0, #1] */
    *(uint8_t *)(r0 + 1) = r6;   /* store */  /* MMIO: DMA1.ISR(w) */
    /* 0x08007118: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x0800711A: 0768     ldr      r7, [r0] */
    r7 = *(uint32_t *)(r0);  /* MMIO: DMA1.ISR(r) */
    /* 0x0800711C: af43     bics     r7, r5 */
    r7 = r7 & ~r5;
    /* 0x0800711E: 0760     str      r7, [r0] */
    *(uint32_t *)(r0) = r7;   /* store */  /* MMIO: DMA1.ISR(w) */
    /* 0x08007120: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08007122: fdf73dfb bl       #0x80047a0 */
    FUN_080047A0();   /* BL -> 0x080047A0 FUN_080047A0 */
    /* 0x08007126: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x08007128: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: DMA1.ISR(r) */
    /* 0x0800712A: 3143     orrs     r1, r6 */
    r1 = r1 | r6;
    /* 0x0800712C: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: DMA1.ISR(w) */
    /* 0x0800712E: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x08007130: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: DMA1.ISR(r) */
    /* 0x08007132: 0422     movs     r2, #4 */
    r2 = 4;
    /* 0x08007134: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x08007136: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: DMA1.ISR(w) */
    /* 0x08007138: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x0800713A: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: DMA1.ISR(r) */
    /* 0x0800713C: 0822     movs     r2, #8 */
    r2 = 8;
    /* 0x0800713E: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x08007140: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: DMA1.ISR(w) */
    /* 0x08007142: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x08007144: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: DMA1.ISR(r) */
    /* 0x08007146: 2943     orrs     r1, r5 */
    r1 = r1 | r5;
    /* 0x08007148: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: DMA1.ISR(w) */
    /* 0x0800714A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800714C: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_0800714E:
    /* 0x0800714E: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08007150: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
