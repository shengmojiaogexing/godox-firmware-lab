/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800BC0C
 * Name      : FUN_0800BC0C
 * End       : 0x0800BC7A
 * Size      : 110 bytes, 55 instructions decoded in body (functions.json insns=55, blocks=7)
 * Class     : function
 * Callers   : 1  -> 0x08008582 FUN_08008582
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800BC0C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800BC0C: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800BC0E: 026a     ldr      r2, [r0, #0x20] */
    r2 = *(uint32_t *)(r0 + 32);
    /* 0x0800BC10: 1023     movs     r3, #0x10 */
    r3 = 0x10;
    /* 0x0800BC12: 9a43     bics     r2, r3 */
    r2 = r2 & ~r3;
    /* 0x0800BC14: 0262     str      r2, [r0, #0x20] */
    *(uint32_t *)(r0 + 32) = r2;   /* store */
    /* 0x0800BC16: 056a     ldr      r5, [r0, #0x20] */
    r5 = *(uint32_t *)(r0 + 32);
    /* 0x0800BC18: 4268     ldr      r2, [r0, #4] */
    r2 = *(uint32_t *)(r0 + 4);
    /* 0x0800BC1A: 8369     ldr      r3, [r0, #0x18] */
    r3 = *(uint32_t *)(r0 + 24);
    /* 0x0800BC1C: 7324     movs     r4, #0x73 */
    r4 = 0x73;
    /* 0x0800BC1E: 2402     lsls     r4, r4, #8 */
    r4 = r4 << 8;
    /* 0x0800BC20: a343     bics     r3, r4 */
    r3 = r3 & ~r4;
    /* 0x0800BC22: 0c68     ldr      r4, [r1] */
    r4 = *(uint32_t *)(r1);
    /* 0x0800BC24: 2402     lsls     r4, r4, #8 */
    r4 = r4 << 8;
    /* 0x0800BC26: 1c43     orrs     r4, r3 */
    r4 = r4 | r3;
    /* 0x0800BC28: 2023     movs     r3, #0x20 */
    r3 = 0x20;
    /* 0x0800BC2A: 9d43     bics     r5, r3 */
    r5 = r5 & ~r3;
    /* 0x0800BC2C: 8b68     ldr      r3, [r1, #8] */
    r3 = *(uint32_t *)(r1 + 8);
    /* 0x0800BC2E: 1b01     lsls     r3, r3, #4 */
    r3 = r3 << 4;
    /* 0x0800BC30: 2b43     orrs     r3, r5 */
    r3 = r3 | r5;
    /* 0x0800BC32: 124d     ldr      r5, [pc, #0x48]  ; =TIM1.CR1 */
    r5 = *(uint32_t *)0x0800BC7C;   /* = MMIO TIM1+0x00 */
    /* 0x0800BC34: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x0800BC36: 08d1     bne      #0x800bc4a */
    if (r0 != r5) goto L_0800BC4A;
    /* 0x0800BC38: 1d46     mov      r5, r3 */
    r5 = r3;
    /* 0x0800BC3A: cb68     ldr      r3, [r1, #0xc] */
    r3 = *(uint32_t *)(r1 + 12);
    /* 0x0800BC3C: 8026     movs     r6, #0x80 */
    r6 = 0x80;
    /* 0x0800BC3E: b543     bics     r5, r6 */
    r5 = r5 & ~r6;
    /* 0x0800BC40: 1b01     lsls     r3, r3, #4 */
    r3 = r3 << 4;
    /* 0x0800BC42: 2b43     orrs     r3, r5 */
    r3 = r3 | r5;
    /* 0x0800BC44: 4025     movs     r5, #0x40 */
    r5 = 0x40;
    /* 0x0800BC46: ab43     bics     r3, r5 */
    r3 = r3 & ~r5;
    /* 0x0800BC48: 08e0     b        #0x800bc5c */
    goto L_0800BC5C;

    L_0800BC4A:
    /* 0x0800BC4A: 0d4d     ldr      r5, [pc, #0x34]  ; =TIM15.CR1 */
    r5 = *(uint32_t *)0x0800BC80;   /* = MMIO TIM15+0x00 */
    /* 0x0800BC4C: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x0800BC4E: 05d0     beq      #0x800bc5c */
    if (r0 == r5) goto L_0800BC5C;
    /* 0x0800BC50: 0c4d     ldr      r5, [pc, #0x30]  ; =TIM16.CR1 */
    r5 = *(uint32_t *)0x0800BC84;   /* = MMIO TIM16+0x00 */
    /* 0x0800BC52: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x0800BC54: 02d0     beq      #0x800bc5c */
    if (r0 == r5) goto L_0800BC5C;
    /* 0x0800BC56: 0c4d     ldr      r5, [pc, #0x30]  ; =TIM17.CR1 */
    r5 = *(uint32_t *)0x0800BC88;   /* = MMIO TIM17+0x00 */
    /* 0x0800BC58: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x0800BC5A: 08d1     bne      #0x800bc6e */
    if (r0 != r5) goto L_0800BC6E;

    L_0800BC5C:
    /* 0x0800BC5C: 0325     movs     r5, #3 */
    r5 = 3;
    /* 0x0800BC5E: ad02     lsls     r5, r5, #0xa */
    r5 = r5 << 0xa;
    /* 0x0800BC60: aa43     bics     r2, r5 */
    r2 = r2 & ~r5;
    /* 0x0800BC62: 4d69     ldr      r5, [r1, #0x14] */
    r5 = *(uint32_t *)(r1 + 20);
    /* 0x0800BC64: ad00     lsls     r5, r5, #2 */
    r5 = r5 << 2;
    /* 0x0800BC66: 1543     orrs     r5, r2 */
    r5 = r5 | r2;
    /* 0x0800BC68: 8a69     ldr      r2, [r1, #0x18] */
    r2 = *(uint32_t *)(r1 + 24);
    /* 0x0800BC6A: 9200     lsls     r2, r2, #2 */
    r2 = r2 << 2;
    /* 0x0800BC6C: 2a43     orrs     r2, r5 */
    r2 = r2 | r5;

    L_0800BC6E:
    /* 0x0800BC6E: 4260     str      r2, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r2;   /* store */
    /* 0x0800BC70: 8461     str      r4, [r0, #0x18] */
    *(uint32_t *)(r0 + 24) = r4;   /* store */
    /* 0x0800BC72: 4968     ldr      r1, [r1, #4] */
    r1 = *(uint32_t *)(r1 + 4);
    /* 0x0800BC74: 8163     str      r1, [r0, #0x38] */
    *(uint32_t *)(r0 + 56) = r1;   /* store */
    /* 0x0800BC76: 0362     str      r3, [r0, #0x20] */
    *(uint32_t *)(r0 + 32) = r3;   /* store */
    /* 0x0800BC78: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
