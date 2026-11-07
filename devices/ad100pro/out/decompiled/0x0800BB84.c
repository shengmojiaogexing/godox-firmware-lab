/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800BB84
 * Name      : FUN_0800BB84
 * End       : 0x0800BBFC
 * Size      : 120 bytes, 60 instructions decoded in body (functions.json insns=60, blocks=11)
 * Class     : function
 * Callers   : 1  -> 0x08008582 FUN_08008582
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800BB84(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800BB84: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x0800BB86: 026a     ldr      r2, [r0, #0x20] */
    r2 = *(uint32_t *)(r0 + 32);
    /* 0x0800BB88: 5208     lsrs     r2, r2, #1 */
    r2 = r2 >> 1;
    /* 0x0800BB8A: 5200     lsls     r2, r2, #1 */
    r2 = r2 << 1;
    /* 0x0800BB8C: 0262     str      r2, [r0, #0x20] */
    *(uint32_t *)(r0 + 32) = r2;   /* store */
    /* 0x0800BB8E: 056a     ldr      r5, [r0, #0x20] */
    r5 = *(uint32_t *)(r0 + 32);
    /* 0x0800BB90: 4368     ldr      r3, [r0, #4] */
    r3 = *(uint32_t *)(r0 + 4);
    /* 0x0800BB92: 8269     ldr      r2, [r0, #0x18] */
    r2 = *(uint32_t *)(r0 + 24);
    /* 0x0800BB94: 7024     movs     r4, #0x70 */
    r4 = 0x70;
    /* 0x0800BB96: a243     bics     r2, r4 */
    r2 = r2 & ~r4;
    /* 0x0800BB98: 9208     lsrs     r2, r2, #2 */
    r2 = r2 >> 2;
    /* 0x0800BB9A: 0c68     ldr      r4, [r1] */
    r4 = *(uint32_t *)(r1);
    /* 0x0800BB9C: 9200     lsls     r2, r2, #2 */
    r2 = r2 << 2;
    /* 0x0800BB9E: 1443     orrs     r4, r2 */
    r4 = r4 | r2;
    /* 0x0800BBA0: 0222     movs     r2, #2 */
    r2 = 2;
    /* 0x0800BBA2: 9543     bics     r5, r2 */
    r5 = r5 & ~r2;
    /* 0x0800BBA4: 8a68     ldr      r2, [r1, #8] */
    r2 = *(uint32_t *)(r1 + 8);
    /* 0x0800BBA6: 164e     ldr      r6, [pc, #0x58]  ; =TIM15.CR1 */
    r6 = *(uint32_t *)0x0800BC00;   /* = MMIO TIM15+0x00 */
    /* 0x0800BBA8: 2a43     orrs     r2, r5 */
    r2 = r2 | r5;
    /* 0x0800BBAA: 144d     ldr      r5, [pc, #0x50]  ; =TIM1.CR1 */
    r5 = *(uint32_t *)0x0800BBFC;   /* = MMIO TIM1+0x00 */
    /* 0x0800BBAC: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x0800BBAE: 07d0     beq      #0x800bbc0 */
    if (r0 == r5) goto L_0800BBC0;
    /* 0x0800BBB0: b042     cmp      r0, r6 */
    /* flags = cmp(r0, r6) */
    /* 0x0800BBB2: 05d0     beq      #0x800bbc0 */
    if (r0 == r6) goto L_0800BBC0;
    /* 0x0800BBB4: 134f     ldr      r7, [pc, #0x4c]  ; =TIM16.CR1 */
    r7 = *(uint32_t *)0x0800BC04;   /* = MMIO TIM16+0x00 */
    /* 0x0800BBB6: b842     cmp      r0, r7 */
    /* flags = cmp(r0, r7) */
    /* 0x0800BBB8: 02d0     beq      #0x800bbc0 */
    if (r0 == r7) goto L_0800BBC0;
    /* 0x0800BBBA: 134f     ldr      r7, [pc, #0x4c]  ; =TIM17.CR1 */
    r7 = *(uint32_t *)0x0800BC08;   /* = MMIO TIM17+0x00 */
    /* 0x0800BBBC: b842     cmp      r0, r7 */
    /* flags = cmp(r0, r7) */
    /* 0x0800BBBE: 06d1     bne      #0x800bbce */
    if (r0 != r7) goto L_0800BBCE;

    L_0800BBC0:
    /* 0x0800BBC0: 0827     movs     r7, #8 */
    r7 = 8;
    /* 0x0800BBC2: ba43     bics     r2, r7 */
    r2 = r2 & ~r7;
    /* 0x0800BBC4: cf68     ldr      r7, [r1, #0xc] */
    r7 = *(uint32_t *)(r1 + 12);
    /* 0x0800BBC6: 1743     orrs     r7, r2 */
    r7 = r7 | r2;
    /* 0x0800BBC8: 0422     movs     r2, #4 */
    r2 = 4;
    /* 0x0800BBCA: 9743     bics     r7, r2 */
    r7 = r7 & ~r2;
    /* 0x0800BBCC: 3a46     mov      r2, r7 */
    r2 = r7;

    L_0800BBCE:
    /* 0x0800BBCE: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x0800BBD0: 07d0     beq      #0x800bbe2 */
    if (r0 == r5) goto L_0800BBE2;
    /* 0x0800BBD2: b042     cmp      r0, r6 */
    /* flags = cmp(r0, r6) */
    /* 0x0800BBD4: 05d0     beq      #0x800bbe2 */
    if (r0 == r6) goto L_0800BBE2;
    /* 0x0800BBD6: 0b4d     ldr      r5, [pc, #0x2c]  ; =TIM16.CR1 */
    r5 = *(uint32_t *)0x0800BC04;   /* = MMIO TIM16+0x00 */
    /* 0x0800BBD8: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x0800BBDA: 02d0     beq      #0x800bbe2 */
    if (r0 == r5) goto L_0800BBE2;
    /* 0x0800BBDC: 0a4d     ldr      r5, [pc, #0x28]  ; =TIM17.CR1 */
    r5 = *(uint32_t *)0x0800BC08;   /* = MMIO TIM17+0x00 */
    /* 0x0800BBDE: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x0800BBE0: 06d1     bne      #0x800bbf0 */
    if (r0 != r5) goto L_0800BBF0;

    L_0800BBE2:
    /* 0x0800BBE2: 0325     movs     r5, #3 */
    r5 = 3;
    /* 0x0800BBE4: 2d02     lsls     r5, r5, #8 */
    r5 = r5 << 8;
    /* 0x0800BBE6: ab43     bics     r3, r5 */
    r3 = r3 & ~r5;
    /* 0x0800BBE8: 4d69     ldr      r5, [r1, #0x14] */
    r5 = *(uint32_t *)(r1 + 20);
    /* 0x0800BBEA: 1d43     orrs     r5, r3 */
    r5 = r5 | r3;
    /* 0x0800BBEC: 8b69     ldr      r3, [r1, #0x18] */
    r3 = *(uint32_t *)(r1 + 24);
    /* 0x0800BBEE: 2b43     orrs     r3, r5 */
    r3 = r3 | r5;

    L_0800BBF0:
    /* 0x0800BBF0: 4360     str      r3, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r3;   /* store */
    /* 0x0800BBF2: 8461     str      r4, [r0, #0x18] */
    *(uint32_t *)(r0 + 24) = r4;   /* store */
    /* 0x0800BBF4: 4968     ldr      r1, [r1, #4] */
    r1 = *(uint32_t *)(r1 + 4);
    /* 0x0800BBF6: 4163     str      r1, [r0, #0x34] */
    *(uint32_t *)(r0 + 52) = r1;   /* store */
    /* 0x0800BBF8: 0262     str      r2, [r0, #0x20] */
    *(uint32_t *)(r0 + 32) = r2;   /* store */
    /* 0x0800BBFA: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
