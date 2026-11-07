/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080070B0
 * Name      : FUN_080070B0
 * End       : 0x080070FE
 * Size      : 78 bytes, 39 instructions decoded in body (functions.json insns=39, blocks=6)
 * Class     : function
 * Callers   : 1  -> 0x08006B20 FUN_08006B20
 * Callees   : 0  -> -
 * MMIO      : DMA1.CH1_CCR, DMA1.CH1_CMAR, DMA1.CH1_CNDTR, DMA1.CH1_CPAR, DMA1.CH1_None, DMA1.CH2_ISR, DMA1.CH3_ISR, DMA1.IFCR, DMA1.ISR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080070B0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080070B0: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x080070B2: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080070B4: 06d0     beq      #0x80070c4 */
    if (r0 == 0) goto L_080070C4;
    /* 0x080070B6: 0146     mov      r1, r0 */
    r1 = r0;
    /* 0x080070B8: 2031     adds     r1, #0x20 */
    r1 = r1 + 0x20;
    /* 0x080070BA: 4a78     ldrb     r2, [r1, #1] */
    r2 = *(uint8_t *)(r1 + 1);  /* MMIO: DMA1.ISR(r) */
    /* 0x080070BC: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x080070BE: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x080070C0: 02d0     beq      #0x80070c8 */
    if (r2 == 0) goto L_080070C8;
    /* 0x080070C2: 02e0     b        #0x80070ca */
    goto L_080070CA;

    L_080070C4:
    /* 0x080070C4: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x080070C6: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */

    L_080070C8:
    /* 0x080070C8: 0c70     strb     r4, [r1] */
    *(uint8_t *)(r1) = r4;   /* store */  /* MMIO: DMA1.ISR(w) */

    L_080070CA:
    /* 0x080070CA: 0222     movs     r2, #2 */
    r2 = 2;
    /* 0x080070CC: 4a70     strb     r2, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r2;   /* store */  /* MMIO: DMA1.ISR(w) */
    /* 0x080070CE: 0568     ldr      r5, [r0] */
    r5 = *(uint32_t *)(r0);  /* MMIO: DMA1.ISR(r) */
    /* 0x080070D0: 2b68     ldr      r3, [r5] */
    r3 = *(uint32_t *)(r5);
    /* 0x080070D2: 0b4a     ldr      r2, [pc, #0x2c]  ; =0xFFFFC00F */
    r2 = *(uint32_t *)0x08007100;   /* = 0xFFFFC00F */
    /* 0x080070D4: 8668     ldr      r6, [r0, #8] */
    r6 = *(uint32_t *)(r0 + 8);  /* MMIO: DMA1.CH1_CCR(r) */
    /* 0x080070D6: 1340     ands     r3, r2 */
    r3 = r3 & r2;
    /* 0x080070D8: 4268     ldr      r2, [r0, #4] */
    r2 = *(uint32_t *)(r0 + 4);  /* MMIO: DMA1.IFCR(r) */
    /* 0x080070DA: 0769     ldr      r7, [r0, #0x10] */
    r7 = *(uint32_t *)(r0 + 16);  /* MMIO: DMA1.CH1_CPAR(r) */
    /* 0x080070DC: 3243     orrs     r2, r6 */
    r2 = r2 | r6;
    /* 0x080070DE: c668     ldr      r6, [r0, #0xc] */
    r6 = *(uint32_t *)(r0 + 12);  /* MMIO: DMA1.CH1_CNDTR(r) */
    /* 0x080070E0: 3e43     orrs     r6, r7 */
    r6 = r6 | r7;
    /* 0x080070E2: 3243     orrs     r2, r6 */
    r2 = r2 | r6;
    /* 0x080070E4: 4669     ldr      r6, [r0, #0x14] */
    r6 = *(uint32_t *)(r0 + 20);  /* MMIO: DMA1.CH1_CMAR(r) */
    /* 0x080070E6: 3243     orrs     r2, r6 */
    r2 = r2 | r6;
    /* 0x080070E8: 8669     ldr      r6, [r0, #0x18] */
    r6 = *(uint32_t *)(r0 + 24);  /* MMIO: DMA1.CH1_None(r) */
    /* 0x080070EA: 3243     orrs     r2, r6 */
    r2 = r2 | r6;
    /* 0x080070EC: c669     ldr      r6, [r0, #0x1c] */
    r6 = *(uint32_t *)(r0 + 28);  /* MMIO: DMA1.CH2_ISR(r) */
    /* 0x080070EE: 3243     orrs     r2, r6 */
    r2 = r2 | r6;
    /* 0x080070F0: 1a43     orrs     r2, r3 */
    r2 = r2 | r3;
    /* 0x080070F2: 2a60     str      r2, [r5] */
    *(uint32_t *)(r5) = r2;   /* store */
    /* 0x080070F4: 8463     str      r4, [r0, #0x38] */
    *(uint32_t *)(r0 + 56) = r4;   /* store */  /* MMIO: DMA1.CH3_ISR(w) */
    /* 0x080070F6: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x080070F8: 4870     strb     r0, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r0;   /* store */  /* MMIO: DMA1.ISR(w) */
    /* 0x080070FA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080070FC: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
