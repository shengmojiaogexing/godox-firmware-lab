/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006BA4
 * Name      : FUN_08006BA4
 * End       : 0x08006C34
 * Size      : 144 bytes, 70 instructions decoded in body (functions.json insns=70, blocks=9)
 * Class     : function
 * Callers   : 3  -> 0x080035A8 FUN_080035A8, 0x0800C4E8 FUN_0800C4E8, 0x0800C578 FUN_0800C578
 * Callees   : 2  -> 0x08003828 FUN_08003828, 0x08007104 FUN_08007104
 * MMIO      : DMA1.CH1_CCR, DMA1.CH1_CPAR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006BA4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006BA4: f7b5     push     {r0, r1, r2, r4, r5, r6, r7, lr} */
    push({r0, r1, r2, r4, r5, r6, r7, lr});
    /* 0x08006BA6: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x08006BA8: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);  /* MMIO: DMA1.CH1_CCR(r) */
    /* 0x08006BAA: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x08006BAC: 8068     ldr      r0, [r0, #8] */
    r0 = *(uint32_t *)(r0 + 8);  /* MMIO: DMA1.CH1_CPAR(r) */
    /* 0x08006BAE: 4007     lsls     r0, r0, #0x1d */
    r0 = r0 << 0x1d;
    /* 0x08006BB0: 3dd4     bmi      #0x8006c2e */
    if (/* bmi */) goto L_08006C2E;   /* result negative */
    /* 0x08006BB2: 2646     mov      r6, r4 */
    r6 = r4;
    /* 0x08006BB4: 4036     adds     r6, #0x40 */
    r6 = r6 + 0x40;
    /* 0x08006BB6: 3078     ldrb     r0, [r6] */
    r0 = *(uint8_t *)(r6);
    /* 0x08006BB8: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08006BBA: 0ad0     beq      #0x8006bd2 */
    if (r0 == 1) goto L_08006BD2;
    /* 0x08006BBC: 0127     movs     r7, #1 */
    r7 = 1;
    /* 0x08006BBE: 3770     strb     r7, [r6] */
    *(uint8_t *)(r6) = r7;   /* store */
    /* 0x08006BC0: e069     ldr      r0, [r4, #0x1c] */
    r0 = *(uint32_t *)(r4 + 28);
    /* 0x08006BC2: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08006BC4: 07d0     beq      #0x8006bd6 */
    if (r0 == 1) goto L_08006BD6;
    /* 0x08006BC6: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08006BC8: fcf72efe bl       #0x8003828 */
    FUN_08003828();   /* BL -> 0x08003828 FUN_08003828 */
    /* 0x08006BCC: 0500     movs     r5, r0 */
    r5 = r0;
    /* 0x08006BCE: 02d0     beq      #0x8006bd6 */
    if (r0 == 1) goto L_08006BD6;
    /* 0x08006BD0: 2ee0     b        #0x8006c30 */
    goto L_08006C30;

    L_08006BD2:
    /* 0x08006BD2: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08006BD4: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */

    L_08006BD6:
    /* 0x08006BD6: 606c     ldr      r0, [r4, #0x44] */
    r0 = *(uint32_t *)(r4 + 68);
    /* 0x08006BD8: 1649     ldr      r1, [pc, #0x58]  ; =0xFFFFF1FE */
    r1 = *(uint32_t *)0x08006C34;   /* = 0xFFFFF1FE */
    /* 0x08006BDA: 0840     ands     r0, r1 */
    r0 = r0 & r1;
    /* 0x08006BDC: ff21     movs     r1, #0xff */
    r1 = 0xff;
    /* 0x08006BDE: 0131     adds     r1, #1 */
    r1 = r1 + 1;
    /* 0x08006BE0: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08006BE2: 6064     str      r0, [r4, #0x44] */
    *(uint32_t *)(r4 + 68) = r0;   /* store */
    /* 0x08006BE4: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006BE6: a064     str      r0, [r4, #0x48] */
    *(uint32_t *)(r4 + 72) = r0;   /* store */
    /* 0x08006BE8: 3070     strb     r0, [r6] */
    *(uint8_t *)(r6) = r0;   /* store */
    /* 0x08006BEA: e16b     ldr      r1, [r4, #0x3c] */
    r1 = *(uint32_t *)(r4 + 60);
    /* 0x08006BEC: 1248     ldr      r0, [pc, #0x48]  ; =func_08003732 */
    r0 = *(uint32_t *)0x08006C38;   /* = flash data/const 0x08003733 */
    /* 0x08006BEE: 8862     str      r0, [r1, #0x28] */
    *(uint32_t *)(r1 + 40) = r0;   /* store */
    /* 0x08006BF0: e16b     ldr      r1, [r4, #0x3c] */
    r1 = *(uint32_t *)(r4 + 60);
    /* 0x08006BF2: 1248     ldr      r0, [pc, #0x48]  ; =func_080037B8 */
    r0 = *(uint32_t *)0x08006C3C;   /* = flash data/const 0x080037B9 */
    /* 0x08006BF4: c862     str      r0, [r1, #0x2c] */
    *(uint32_t *)(r1 + 44) = r0;   /* store */
    /* 0x08006BF6: e16b     ldr      r1, [r4, #0x3c] */
    r1 = *(uint32_t *)(r4 + 60);
    /* 0x08006BF8: 1148     ldr      r0, [pc, #0x44]  ; =func_0800379E */
    r0 = *(uint32_t *)0x08006C40;   /* = flash data/const 0x0800379F */
    /* 0x08006BFA: 0863     str      r0, [r1, #0x30] */
    *(uint32_t *)(r1 + 48) = r0;   /* store */
    /* 0x08006BFC: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);
    /* 0x08006BFE: 1c20     movs     r0, #0x1c */
    r0 = 0x1c;
    /* 0x08006C00: 0860     str      r0, [r1] */
    *(uint32_t *)(r1) = r0;   /* store */
    /* 0x08006C02: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x08006C04: 4168     ldr      r1, [r0, #4] */
    r1 = *(uint32_t *)(r0 + 4);
    /* 0x08006C06: 1022     movs     r2, #0x10 */
    r2 = 0x10;
    /* 0x08006C08: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x08006C0A: 4160     str      r1, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r1;   /* store */
    /* 0x08006C0C: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x08006C0E: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);
    /* 0x08006C10: 3943     orrs     r1, r7 */
    r1 = r1 | r7;
    /* 0x08006C12: c160     str      r1, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r1;   /* store */
    /* 0x08006C14: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);
    /* 0x08006C16: e06b     ldr      r0, [r4, #0x3c] */
    r0 = *(uint32_t *)(r4 + 60);
    /* 0x08006C18: 4031     adds     r1, #0x40 */
    r1 = r1 + 0x40;
    /* 0x08006C1A: 029b     ldr      r3, [sp, #8] */
    r3 = *(uint32_t *)(sp + 8);
    /* 0x08006C1C: 019a     ldr      r2, [sp, #4] */
    r2 = *(uint32_t *)(sp + 4);
    /* 0x08006C1E: 00f071fa bl       #0x8007104 */
    FUN_08007104();   /* BL -> 0x08007104 FUN_08007104 */
    /* 0x08006C22: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x08006C24: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);
    /* 0x08006C26: 0422     movs     r2, #4 */
    r2 = 4;
    /* 0x08006C28: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x08006C2A: 8160     str      r1, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r1;   /* store */
    /* 0x08006C2C: 00e0     b        #0x8006c30 */
    goto L_08006C30;

    L_08006C2E:
    /* 0x08006C2E: 0225     movs     r5, #2 */
    r5 = 2;

    L_08006C30:
    /* 0x08006C30: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08006C32: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */
}
