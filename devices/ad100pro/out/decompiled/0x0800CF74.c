/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CF74
 * Name      : main
 * End       : 0x0800D066
 * Size      : 242 bytes, 83 instructions decoded in body (functions.json insns=83, blocks=5)
 * Class     : hub
 * Callers   : 0  -> -
 * Callees   : 35  -> 0x080035A8 FUN_080035A8, 0x080038C0 FUN_080038C0, 0x08003BAC FUN_08003BAC, 0x080047CC FUN_080047CC, 0x0800642C FUN_0800642C, 0x080065CC FUN_080065CC, 0x080074F0 FUN_080074F0, 0x08007544 FUN_08007544, ... +27
 * MMIO      : GPIOB.MODER, RCC.APB1ENR, SYSCFG.CFGR1
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void main(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CF74: 08b5     push     {r3, lr} */
    push({r3, lr});
    /* 0x0800CF76: 3c4a     ldr      r2, [pc, #0xf0]  ; =flash_08003000 */
    r2 = *(uint32_t *)0x0800D068;   /* = vector_table (0x08003000) */
    /* 0x0800CF78: 3c4b     ldr      r3, [pc, #0xf0]  ; =RAM_20000000 */
    r3 = *(uint32_t *)0x0800D06C;   /* = RAM_base (0x20000000) */
    /* 0x0800CF7A: 0020     movs     r0, #0 */
    r0 = 0;

    L_0800CF7C:
    /* 0x0800CF7C: 8100     lsls     r1, r0, #2 */
    r1 = r0 << 2;
    /* 0x0800CF7E: 8c18     adds     r4, r1, r2 */
    r4 = r1 + r2;
    /* 0x0800CF80: 2468     ldr      r4, [r4] */
    r4 = *(uint32_t *)(r4);  /* MMIO: GPIOB.MODER(r) */
    /* 0x0800CF82: 5c50     str      r4, [r3, r1] */
    *(uint32_t *)(r3 + r1) = r4;   /* store */
    /* 0x0800CF84: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800CF86: 3028     cmp      r0, #0x30 */
    /* flags = cmp(r0, #0x30) */
    /* 0x0800CF88: f8d3     blo      #0x800cf7c */
    if (r0 < 0x30) goto L_0800CF7C;   /* unsigned */
    /* 0x0800CF8A: 3948     ldr      r0, [pc, #0xe4]  ; =SYSCFG.CFGR1 */
    r0 = *(uint32_t *)0x0800D070;   /* = MMIO SYSCFG+0x00 */
    /* 0x0800CF8C: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: SYSCFG.CFGR1(r) */
    /* 0x0800CF8E: 0322     movs     r2, #3 */
    r2 = 3;
    /* 0x0800CF90: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x0800CF92: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: SYSCFG.CFGR1(w) */
    /* 0x0800CF94: faf71afb bl       #0x80075cc */
    FUN_080075CC();   /* BL -> 0x080075CC FUN_080075CC */
    /* 0x0800CF98: fef7aafc bl       #0x800b8f0 */
    FUN_0800B8F0();   /* BL -> 0x0800B8F0 FUN_0800B8F0 */
    /* 0x0800CF9C: fcf73efa bl       #0x800941c */
    FUN_0800941C();   /* BL -> 0x0800941C FUN_0800941C */
    /* 0x0800CFA0: fcf728fa bl       #0x80093f4 */
    FUN_080093F4();   /* BL -> 0x080093F4 FUN_080093F4 */
    /* 0x0800CFA4: fcf76afb bl       #0x800967c */
    FUN_0800967C();   /* BL -> 0x0800967C FUN_0800967C */
    /* 0x0800CFA8: fcf7b2f9 bl       #0x8009310 */
    FUN_08009310();   /* BL -> 0x08009310 FUN_08009310 */
    /* 0x0800CFAC: fcf70efb bl       #0x80095cc */
    FUN_080095CC();   /* BL -> 0x080095CC FUN_080095CC */
    /* 0x0800CFB0: fcf722fb bl       #0x80095f8 */
    FUN_080095F8();   /* BL -> 0x080095F8 FUN_080095F8 */
    /* 0x0800CFB4: fcf7a8fb bl       #0x8009708 */
    FUN_08009708();   /* BL -> 0x08009708 FUN_08009708 */
    /* 0x0800CFB8: fcf7d6fb bl       #0x8009768 */
    FUN_08009768();   /* BL -> 0x08009768 FUN_08009768 */
    /* 0x0800CFBC: fcf774fb bl       #0x80096a8 */
    FUN_080096A8();   /* BL -> 0x080096A8 FUN_080096A8 */
    /* 0x0800CFC0: fcf7f2f9 bl       #0x80093a8 */
    FUN_080093A8();   /* BL -> 0x080093A8 FUN_080093A8 */
    /* 0x0800CFC4: 2b4c     ldr      r4, [pc, #0xac]  ; =RCC.CR */
    r4 = *(uint32_t *)0x0800D074;   /* = MMIO RCC+0x00 */
    /* 0x0800CFC6: e169     ldr      r1, [r4, #0x1c] */
    r1 = *(uint32_t *)(r4 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x0800CFC8: 2004     lsls     r0, r4, #0x10 */
    r0 = r4 << 0x10;
    /* 0x0800CFCA: 0143     orrs     r1, r0 */
    r1 = r1 | r0;
    /* 0x0800CFCC: e161     str      r1, [r4, #0x1c] */
    *(uint32_t *)(r4 + 28) = r1;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x0800CFCE: e169     ldr      r1, [r4, #0x1c] */
    r1 = *(uint32_t *)(r4 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x0800CFD0: 0140     ands     r1, r0 */
    r1 = r1 & r0;
    /* 0x0800CFD2: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800CFD4: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800CFD8: 2748     ldr      r0, [pc, #0x9c]  ; =RAM_20000274 */
    r0 = *(uint32_t *)0x0800D078;   /* = RAM 0x20000274 */
    /* 0x0800CFDA: 0091     str      r1, [sp] */
    *(uint32_t *)(sp) = r1;   /* store */
    /* 0x0800CFDC: faf798ff bl       #0x8007f10 */
    FUN_08007F10();   /* BL -> 0x08007F10 FUN_08007F10 */
    /* 0x0800CFE0: 2648     ldr      r0, [pc, #0x98]  ; =RAM_20000384 */
    r0 = *(uint32_t *)0x0800D07C;   /* = RAM 0x20000384 */
    /* 0x0800CFE2: faf7affa bl       #0x8007544 */
    FUN_08007544();   /* BL -> 0x08007544 FUN_08007544 */
    /* 0x0800CFE6: 2548     ldr      r0, [pc, #0x94]  ; =RAM_20000384 */
    r0 = *(uint32_t *)0x0800D07C;   /* = RAM 0x20000384 */
    /* 0x0800CFE8: faf782fa bl       #0x80074f0 */
    FUN_080074F0();   /* BL -> 0x080074F0 FUN_080074F0 */
    /* 0x0800CFEC: fdf790fe bl       #0x800ad10 */
    FUN_0800AD10();   /* BL -> 0x0800AD10 FUN_0800AD10 */
    /* 0x0800CFF0: fdf7a8fe bl       #0x800ad44 */
    FUN_0800AD44();   /* BL -> 0x0800AD44 FUN_0800AD44 */
    /* 0x0800CFF4: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800CFF6: fdf79dfc bl       #0x800a934 */
    FUN_0800A934();   /* BL -> 0x0800A934 FUN_0800A934 */
    /* 0x0800CFFA: fdf7f3fd bl       #0x800abe4 */
    FUN_0800ABE4();   /* BL -> 0x0800ABE4 FUN_0800ABE4 */
    /* 0x0800CFFE: 606a     ldr      r0, [r4, #0x24] */
    r0 = *(uint32_t *)(r4 + 36);  /* MMIO: RCC.CSR(r) */
    /* 0x0800D000: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800D002: c905     lsls     r1, r1, #0x17 */
    r1 = r1 << 0x17;
    /* 0x0800D004: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x0800D006: 6062     str      r0, [r4, #0x24] */
    *(uint32_t *)(r4 + 36) = r0;   /* store */  /* MMIO: RCC.CSR(w) */
    /* 0x0800D008: 1d48     ldr      r0, [pc, #0x74]  ; =RAM_2000021B */
    r0 = *(uint32_t *)0x0800D080;   /* = RAM 0x2000021B */
    /* 0x0800D00A: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x0800D00C: 1d4d     ldr      r5, [pc, #0x74]  ; =RAM_200001E1 */
    r5 = *(uint32_t *)0x0800D084;   /* = RAM 0x200001E1 */
    /* 0x0800D00E: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */

    L_0800D010:
    /* 0x0800D010: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800D012: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800D016: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);
    /* 0x0800D018: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800D01A: f9d1     bne      #0x800d010 */
    if (r0 != 1) goto L_0800D010;
    /* 0x0800D01C: 2c70     strb     r4, [r5] */
    *(uint8_t *)(r5) = r4;   /* store */
    /* 0x0800D01E: 1748     ldr      r0, [pc, #0x5c]  ; =RAM_20000384 */
    r0 = *(uint32_t *)0x0800D07C;   /* = RAM 0x20000384 */
    /* 0x0800D020: faf766fa bl       #0x80074f0 */
    FUN_080074F0();   /* BL -> 0x080074F0 FUN_080074F0 */
    /* 0x0800D024: fbf76cfe bl       #0x8008d00 */
    dispatcher_large();   /* BL -> 0x08008D00 dispatcher_large */
    /* 0x0800D028: f7f7d0fb bl       #0x80047cc */
    FUN_080047CC();   /* BL -> 0x080047CC FUN_080047CC */
    /* 0x0800D02C: fcf764fc bl       #0x80098f8 */
    FUN_080098F8();   /* BL -> 0x080098F8 FUN_080098F8 */
    /* 0x0800D030: f6f7bafa bl       #0x80035a8 */
    FUN_080035A8();   /* BL -> 0x080035A8 FUN_080035A8 */
    /* 0x0800D034: f9f7faf9 bl       #0x800642c */
    FUN_0800642C();   /* BL -> 0x0800642C FUN_0800642C */
    /* 0x0800D038: f6f742fc bl       #0x80038c0 */
    FUN_080038C0();   /* BL -> 0x080038C0 FUN_080038C0 */
    /* 0x0800D03C: 00f02cf8 bl       #0x800d098 */
    FUN_0800D098();   /* BL -> 0x0800D098 FUN_0800D098 */
    /* 0x0800D040: f9f7c4fa bl       #0x80065cc */
    FUN_080065CC();   /* BL -> 0x080065CC FUN_080065CC */
    /* 0x0800D044: fbf792fc bl       #0x800896c */
    FUN_0800896C();   /* BL -> 0x0800896C FUN_0800896C */
    /* 0x0800D048: fef7a4f9 bl       #0x800b394 */
    FUN_0800B394();   /* BL -> 0x0800B394 FUN_0800B394 */
    /* 0x0800D04C: fdf7f0f8 bl       #0x800a230 */
    FUN_0800A230();   /* BL -> 0x0800A230 FUN_0800A230 */
    /* 0x0800D050: f6f7acfd bl       #0x8003bac */
    FUN_08003BAC();   /* BL -> 0x08003BAC FUN_08003BAC */
    /* 0x0800D054: fff7befb bl       #0x800c7d4 */
    FUN_0800C7D4();   /* BL -> 0x0800C7D4 FUN_0800C7D4 */
    /* 0x0800D058: fef7aafb bl       #0x800b7b0 */
    FUN_0800B7B0();   /* BL -> 0x0800B7B0 FUN_0800B7B0 */
    /* 0x0800D05C: fcf7ccfb bl       #0x80097f8 */
    FUN_080097F8();   /* BL -> 0x080097F8 FUN_080097F8 */
    /* 0x0800D060: fef7d4fb bl       #0x800b80c */
    FUN_0800B80C();   /* BL -> 0x0800B80C FUN_0800B80C */
    /* 0x0800D064: d4e7     b        #0x800d010 */
    goto L_0800D010;   /* loop */
}
