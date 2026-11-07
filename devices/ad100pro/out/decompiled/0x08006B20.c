/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006B20
 * Name      : FUN_08006B20
 * End       : 0x08006B8E
 * Size      : 110 bytes, 51 instructions decoded in body (functions.json insns=51, blocks=5)
 * Class     : function
 * Callers   : 1  -> 0x080069AC FUN_080069AC
 * Callees   : 3  -> 0x08005C40 FUN_08005C40, 0x080070B0 FUN_080070B0, 0x080072C4 FUN_080072C4
 * MMIO      : RCC.APB2ENR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006B20(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006B20: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x08006B22: 0646     mov      r6, r0 */
    r6 = r0;
    /* 0x08006B24: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);
    /* 0x08006B26: 1a49     ldr      r1, [pc, #0x68]  ; =ADC.ISR */
    r1 = *(uint32_t *)0x08006B90;   /* = MMIO ADC+0x00 */
    /* 0x08006B28: 87b0     sub      sp, #0x1c */
    sp = sp - 0x1c;
    /* 0x08006B2A: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08006B2C: 2dd1     bne      #0x8006b8a */
    if (r0 != r1) goto L_08006B8A;
    /* 0x08006B2E: 1948     ldr      r0, [pc, #0x64]  ; =RCC.CR */
    r0 = *(uint32_t *)0x08006B94;   /* = MMIO RCC+0x00 */
    /* 0x08006B30: 8169     ldr      r1, [r0, #0x18] */
    r1 = *(uint32_t *)(r0 + 24);  /* MMIO: RCC.APB2ENR(r) */
    /* 0x08006B32: 4715     asrs     r7, r0, #0x15 */
    r7 = r0 >> 0x15;
    /* 0x08006B34: 3943     orrs     r1, r7 */
    r1 = r1 | r7;
    /* 0x08006B36: 8161     str      r1, [r0, #0x18] */
    *(uint32_t *)(r0 + 24) = r1;   /* store */  /* MMIO: RCC.APB2ENR(w) */
    /* 0x08006B38: 8069     ldr      r0, [r0, #0x18] */
    r0 = *(uint32_t *)(r0 + 24);  /* MMIO: RCC.APB2ENR(r) */
    /* 0x08006B3A: 0324     movs     r4, #3 */
    r4 = 3;
    /* 0x08006B3C: 3840     ands     r0, r7 */
    r0 = r0 & r7;
    /* 0x08006B3E: 0590     str      r0, [sp, #0x14] */
    *(uint32_t *)(sp + 20) = r0;   /* store */
    /* 0x08006B40: 0720     movs     r0, #7 */
    r0 = 7;
    /* 0x08006B42: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x08006B44: 6a46     mov      r2, sp */
    r2 = sp;
    /* 0x08006B46: 31c2     stm      r2!, {r0, r4, r5} */
    /* stm r2!, {r0, r4, r5} */
    /* 0x08006B48: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08006B4A: 1348     ldr      r0, [pc, #0x4c]  ; =GPIOC.MODER */
    r0 = *(uint32_t *)0x08006B98;   /* = MMIO GPIOC+0x00 */
    /* 0x08006B4C: 00f0bafb bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08006B50: 0820     movs     r0, #8 */
    r0 = 8;
    /* 0x08006B52: 6a46     mov      r2, sp */
    r2 = sp;
    /* 0x08006B54: 31c2     stm      r2!, {r0, r4, r5} */
    /* stm r2!, {r0, r4, r5} */
    /* 0x08006B56: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x08006B58: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08006B5A: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x08006B5C: 00f0b2fb bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08006B60: 0e48     ldr      r0, [pc, #0x38]  ; =DMA1.CH1_CCR */
    r0 = *(uint32_t *)0x08006B9C;   /* = MMIO DMA1+0x08 */
    /* 0x08006B62: 0f4c     ldr      r4, [pc, #0x3c]  ; =RAM_200003F8 */
    r4 = *(uint32_t *)0x08006BA0;   /* = RAM 0x200003F8 */
    /* 0x08006B64: 21c4     stm      r4!, {r0, r5} */
    /* stm r4!, {r0, r5} */
    /* 0x08006B66: a760     str      r7, [r4, #8] */
    *(uint32_t *)(r4 + 8) = r7;   /* store */
    /* 0x08006B68: 8020     movs     r0, #0x80 */
    r0 = 0x80;
    /* 0x08006B6A: 2560     str      r5, [r4] */
    *(uint32_t *)(r4) = r5;   /* store */
    /* 0x08006B6C: 6060     str      r0, [r4, #4] */
    *(uint32_t *)(r4 + 4) = r0;   /* store */
    /* 0x08006B6E: 2561     str      r5, [r4, #0x10] */
    *(uint32_t *)(r4 + 16) = r5;   /* store */
    /* 0x08006B70: 0001     lsls     r0, r0, #4 */
    r0 = r0 << 4;
    /* 0x08006B72: 6561     str      r5, [r4, #0x14] */
    *(uint32_t *)(r4 + 20) = r5;   /* store */
    /* 0x08006B74: e060     str      r0, [r4, #0xc] */
    *(uint32_t *)(r4 + 12) = r0;   /* store */
    /* 0x08006B76: 0a48     ldr      r0, [pc, #0x28]  ; =RAM_200003F8 */
    r0 = *(uint32_t *)0x08006BA0;   /* = RAM 0x200003F8 */
    /* 0x08006B78: 083c     subs     r4, #8 */
    r4 = r4 - 8;
    /* 0x08006B7A: 00f099fa bl       #0x80070b0 */
    FUN_080070B0();   /* BL -> 0x080070B0 FUN_080070B0 */
    /* 0x08006B7E: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08006B80: 01d0     beq      #0x8006b86 */
    if (r0 == 0) goto L_08006B86;
    /* 0x08006B82: fff75df8 bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_08006B86:
    /* 0x08006B86: f463     str      r4, [r6, #0x3c] */
    *(uint32_t *)(r6 + 60) = r4;   /* store */
    /* 0x08006B88: 6662     str      r6, [r4, #0x24] */
    *(uint32_t *)(r4 + 36) = r6;   /* store */

    L_08006B8A:
    /* 0x08006B8A: 07b0     add      sp, #0x1c */
    sp = sp + 0x1c;
    /* 0x08006B8C: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
