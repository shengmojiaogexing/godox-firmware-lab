/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800BC8C
 * Name      : FUN_0800BC8C
 * End       : 0x0800BCFE
 * Size      : 114 bytes, 57 instructions decoded in body (functions.json insns=57, blocks=7)
 * Class     : function
 * Callers   : 2  -> 0x08008582 FUN_08008582, 0x080085D6 FUN_080085D6
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800BC8C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800BC8C: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800BC8E: 026a     ldr      r2, [r0, #0x20] */
    r2 = *(uint32_t *)(r0 + 32);
    /* 0x0800BC90: ff23     movs     r3, #0xff */
    r3 = 0xff;
    /* 0x0800BC92: 0133     adds     r3, #1 */
    r3 = r3 + 1;
    /* 0x0800BC94: 9a43     bics     r2, r3 */
    r2 = r2 & ~r3;
    /* 0x0800BC96: 0262     str      r2, [r0, #0x20] */
    *(uint32_t *)(r0 + 32) = r2;   /* store */
    /* 0x0800BC98: 056a     ldr      r5, [r0, #0x20] */
    r5 = *(uint32_t *)(r0 + 32);
    /* 0x0800BC9A: 4368     ldr      r3, [r0, #4] */
    r3 = *(uint32_t *)(r0 + 4);
    /* 0x0800BC9C: c269     ldr      r2, [r0, #0x1c] */
    r2 = *(uint32_t *)(r0 + 28);
    /* 0x0800BC9E: 7024     movs     r4, #0x70 */
    r4 = 0x70;
    /* 0x0800BCA0: a243     bics     r2, r4 */
    r2 = r2 & ~r4;
    /* 0x0800BCA2: 9208     lsrs     r2, r2, #2 */
    r2 = r2 >> 2;
    /* 0x0800BCA4: 0c68     ldr      r4, [r1] */
    r4 = *(uint32_t *)(r1);
    /* 0x0800BCA6: 9200     lsls     r2, r2, #2 */
    r2 = r2 << 2;
    /* 0x0800BCA8: 1443     orrs     r4, r2 */
    r4 = r4 | r2;
    /* 0x0800BCAA: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800BCAC: 5202     lsls     r2, r2, #9 */
    r2 = r2 << 9;
    /* 0x0800BCAE: 9543     bics     r5, r2 */
    r5 = r5 & ~r2;
    /* 0x0800BCB0: 8a68     ldr      r2, [r1, #8] */
    r2 = *(uint32_t *)(r1 + 8);
    /* 0x0800BCB2: 1202     lsls     r2, r2, #8 */
    r2 = r2 << 8;
    /* 0x0800BCB4: 2a43     orrs     r2, r5 */
    r2 = r2 | r5;
    /* 0x0800BCB6: 124d     ldr      r5, [pc, #0x48]  ; =TIM1.CR1 */
    r5 = *(uint32_t *)0x0800BD00;   /* = MMIO TIM1+0x00 */
    /* 0x0800BCB8: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x0800BCBA: 08d1     bne      #0x800bcce */
    if (r0 != r5) goto L_0800BCCE;
    /* 0x0800BCBC: 1546     mov      r5, r2 */
    r5 = r2;
    /* 0x0800BCBE: ca68     ldr      r2, [r1, #0xc] */
    r2 = *(uint32_t *)(r1 + 12);
    /* 0x0800BCC0: c614     asrs     r6, r0, #0x13 */
    r6 = r0 >> 0x13;
    /* 0x0800BCC2: b543     bics     r5, r6 */
    r5 = r5 & ~r6;
    /* 0x0800BCC4: 1202     lsls     r2, r2, #8 */
    r2 = r2 << 8;
    /* 0x0800BCC6: 2a43     orrs     r2, r5 */
    r2 = r2 | r5;
    /* 0x0800BCC8: 0515     asrs     r5, r0, #0x14 */
    r5 = r0 >> 0x14;
    /* 0x0800BCCA: aa43     bics     r2, r5 */
    r2 = r2 & ~r5;
    /* 0x0800BCCC: 08e0     b        #0x800bce0 */
    goto L_0800BCE0;

    L_0800BCCE:
    /* 0x0800BCCE: 0d4d     ldr      r5, [pc, #0x34]  ; =TIM15.CR1 */
    r5 = *(uint32_t *)0x0800BD04;   /* = MMIO TIM15+0x00 */
    /* 0x0800BCD0: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x0800BCD2: 05d0     beq      #0x800bce0 */
    if (r0 == r5) goto L_0800BCE0;
    /* 0x0800BCD4: 0c4d     ldr      r5, [pc, #0x30]  ; =TIM16.CR1 */
    r5 = *(uint32_t *)0x0800BD08;   /* = MMIO TIM16+0x00 */
    /* 0x0800BCD6: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x0800BCD8: 02d0     beq      #0x800bce0 */
    if (r0 == r5) goto L_0800BCE0;
    /* 0x0800BCDA: 0c4d     ldr      r5, [pc, #0x30]  ; =TIM17.CR1 */
    r5 = *(uint32_t *)0x0800BD0C;   /* = MMIO TIM17+0x00 */
    /* 0x0800BCDC: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x0800BCDE: 08d1     bne      #0x800bcf2 */
    if (r0 != r5) goto L_0800BCF2;

    L_0800BCE0:
    /* 0x0800BCE0: 0325     movs     r5, #3 */
    r5 = 3;
    /* 0x0800BCE2: 2d03     lsls     r5, r5, #0xc */
    r5 = r5 << 0xc;
    /* 0x0800BCE4: ab43     bics     r3, r5 */
    r3 = r3 & ~r5;
    /* 0x0800BCE6: 4d69     ldr      r5, [r1, #0x14] */
    r5 = *(uint32_t *)(r1 + 20);
    /* 0x0800BCE8: 2d01     lsls     r5, r5, #4 */
    r5 = r5 << 4;
    /* 0x0800BCEA: 1d43     orrs     r5, r3 */
    r5 = r5 | r3;
    /* 0x0800BCEC: 8b69     ldr      r3, [r1, #0x18] */
    r3 = *(uint32_t *)(r1 + 24);
    /* 0x0800BCEE: 1b01     lsls     r3, r3, #4 */
    r3 = r3 << 4;
    /* 0x0800BCF0: 2b43     orrs     r3, r5 */
    r3 = r3 | r5;

    L_0800BCF2:
    /* 0x0800BCF2: 4360     str      r3, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r3;   /* store */
    /* 0x0800BCF4: c461     str      r4, [r0, #0x1c] */
    *(uint32_t *)(r0 + 28) = r4;   /* store */
    /* 0x0800BCF6: 4968     ldr      r1, [r1, #4] */
    r1 = *(uint32_t *)(r1 + 4);
    /* 0x0800BCF8: c163     str      r1, [r0, #0x3c] */
    *(uint32_t *)(r0 + 60) = r1;   /* store */
    /* 0x0800BCFA: 0262     str      r2, [r0, #0x20] */
    *(uint32_t *)(r0 + 32) = r2;   /* store */
    /* 0x0800BCFC: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
