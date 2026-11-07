/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006E2C
 * Name      : FUN_08006E2C
 * End       : 0x080070AC
 * Size      : 640 bytes, 320 instructions decoded in body (functions.json insns=320, blocks=95)
 * Class     : function
 * Callers   : 1  -> 0x08004790 IRQ9_Handler
 * Callees   : 0  -> -
 * MMIO      : DMA1.IFCR, DMA1.ISR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006E2C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006E2C: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x08006E2E: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x08006E30: 9e48     ldr      r0, [pc, #0x278]  ; =DMA1.ISR */
    r0 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006E32: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: DMA1.ISR(r) */
    /* 0x08006E34: 9d4a     ldr      r2, [pc, #0x274]  ; =DMA1.ISR */
    r2 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006E36: 9d4e     ldr      r6, [pc, #0x274]  ; =DMA1.ISR */
    r6 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006E38: 9c4f     ldr      r7, [pc, #0x270]  ; =DMA1.ISR */
    r7 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006E3A: 0832     adds     r2, #8 */
    r2 = r2 + 8;
    /* 0x08006E3C: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x08006E3E: 1304     lsls     r3, r2, #0x10 */
    r3 = r2 << 0x10;
    /* 0x08006E40: 1c36     adds     r6, #0x1c */
    r6 = r6 + 0x1c;
    /* 0x08006E42: 3037     adds     r7, #0x30 */
    r7 = r7 + 0x30;
    /* 0x08006E44: 9042     cmp      r0, r2 */
    /* flags = cmp(r0, r2) */
    /* 0x08006E46: 01d1     bne      #0x8006e4c */
    if (r0 != r2) goto L_08006E4C;
    /* 0x08006E48: 0822     movs     r2, #8 */
    r2 = 8;
    /* 0x08006E4A: 1ee0     b        #0x8006e8a */
    goto L_08006E8A;

    L_08006E4C:
    /* 0x08006E4C: b042     cmp      r0, r6 */
    /* flags = cmp(r0, r6) */
    /* 0x08006E4E: 01d1     bne      #0x8006e54 */
    if (r0 != r6) goto L_08006E54;
    /* 0x08006E50: 8022     movs     r2, #0x80 */
    r2 = 0x80;
    /* 0x08006E52: 1ae0     b        #0x8006e8a */
    goto L_08006E8A;

    L_08006E54:
    /* 0x08006E54: b842     cmp      r0, r7 */
    /* flags = cmp(r0, r7) */
    /* 0x08006E56: 02d1     bne      #0x8006e5e */
    if (r0 != r7) goto L_08006E5E;
    /* 0x08006E58: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08006E5A: d202     lsls     r2, r2, #0xb */
    r2 = r2 << 0xb;
    /* 0x08006E5C: 15e0     b        #0x8006e8a */
    goto L_08006E8A;

    L_08006E5E:
    /* 0x08006E5E: 934a     ldr      r2, [pc, #0x24c]  ; =DMA1.ISR */
    r2 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006E60: 4432     adds     r2, #0x44 */
    r2 = r2 + 0x44;
    /* 0x08006E62: 9042     cmp      r0, r2 */
    /* flags = cmp(r0, r2) */
    /* 0x08006E64: 02d1     bne      #0x8006e6c */
    if (r0 != r2) goto L_08006E6C;
    /* 0x08006E66: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08006E68: d203     lsls     r2, r2, #0xf */
    r2 = r2 << 0xf;
    /* 0x08006E6A: 0ee0     b        #0x8006e8a */
    goto L_08006E8A;

    L_08006E6C:
    /* 0x08006E6C: 8f4a     ldr      r2, [pc, #0x23c]  ; =DMA1.ISR */
    r2 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006E6E: 5832     adds     r2, #0x58 */
    r2 = r2 + 0x58;
    /* 0x08006E70: 9042     cmp      r0, r2 */
    /* flags = cmp(r0, r2) */
    /* 0x08006E72: 01d1     bne      #0x8006e78 */
    if (r0 != r2) goto L_08006E78;
    /* 0x08006E74: 1a46     mov      r2, r3 */
    r2 = r3;
    /* 0x08006E76: 08e0     b        #0x8006e8a */
    goto L_08006E8A;

    L_08006E78:
    /* 0x08006E78: 8c4a     ldr      r2, [pc, #0x230]  ; =DMA1.ISR */
    r2 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006E7A: 6c32     adds     r2, #0x6c */
    r2 = r2 + 0x6c;
    /* 0x08006E7C: 9042     cmp      r0, r2 */
    /* flags = cmp(r0, r2) */
    /* 0x08006E7E: 02d1     bne      #0x8006e86 */
    if (r0 != r2) goto L_08006E86;
    /* 0x08006E80: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08006E82: d205     lsls     r2, r2, #0x17 */
    r2 = r2 << 0x17;
    /* 0x08006E84: 01e0     b        #0x8006e8a */
    goto L_08006E8A;

    L_08006E86:
    /* 0x08006E86: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08006E88: d206     lsls     r2, r2, #0x1b */
    r2 = r2 << 0x1b;

    L_08006E8A:
    /* 0x08006E8A: 2546     mov      r5, r4 */
    r5 = r4;
    /* 0x08006E8C: 1140     ands     r1, r2 */
    r1 = r1 & r2;
    /* 0x08006E8E: 2035     adds     r5, #0x20 */
    r5 = r5 + 0x20;
    /* 0x08006E90: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x08006E92: 3cd0     beq      #0x8006f0e */
    if (r1 == 0) goto L_08006F0E;
    /* 0x08006E94: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: DMA1.ISR(r) */
    /* 0x08006E96: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x08006E98: 0907     lsls     r1, r1, #0x1c */
    r1 = r1 << 0x1c;
    /* 0x08006E9A: 38d4     bmi      #0x8006f0e */
    if (r1 < 0) goto L_08006F0E;   /* result negative */
    /* 0x08006E9C: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: DMA1.ISR(r) */
    /* 0x08006E9E: 0822     movs     r2, #8 */
    r2 = 8;
    /* 0x08006EA0: 9143     bics     r1, r2 */
    r1 = r1 & ~r2;
    /* 0x08006EA2: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: DMA1.ISR(w) */
    /* 0x08006EA4: 8149     ldr      r1, [pc, #0x204]  ; =DMA1.ISR */
    r1 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006EA6: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x08006EA8: 0831     adds     r1, #8 */
    r1 = r1 + 8;
    /* 0x08006EAA: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08006EAC: 01d1     bne      #0x8006eb2 */
    if (r0 != r1) goto L_08006EB2;
    /* 0x08006EAE: 0820     movs     r0, #8 */
    r0 = 8;
    /* 0x08006EB0: 1ee0     b        #0x8006ef0 */
    goto L_08006EF0;

    L_08006EB2:
    /* 0x08006EB2: b042     cmp      r0, r6 */
    /* flags = cmp(r0, r6) */
    /* 0x08006EB4: 01d1     bne      #0x8006eba */
    if (r0 != r6) goto L_08006EBA;
    /* 0x08006EB6: 8020     movs     r0, #0x80 */
    r0 = 0x80;
    /* 0x08006EB8: 1ae0     b        #0x8006ef0 */
    goto L_08006EF0;

    L_08006EBA:
    /* 0x08006EBA: b842     cmp      r0, r7 */
    /* flags = cmp(r0, r7) */
    /* 0x08006EBC: 02d1     bne      #0x8006ec4 */
    if (r0 != r7) goto L_08006EC4;
    /* 0x08006EBE: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08006EC0: c002     lsls     r0, r0, #0xb */
    r0 = r0 << 0xb;
    /* 0x08006EC2: 15e0     b        #0x8006ef0 */
    goto L_08006EF0;

    L_08006EC4:
    /* 0x08006EC4: 7949     ldr      r1, [pc, #0x1e4]  ; =DMA1.ISR */
    r1 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006EC6: 4431     adds     r1, #0x44 */
    r1 = r1 + 0x44;
    /* 0x08006EC8: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08006ECA: 02d1     bne      #0x8006ed2 */
    if (r0 != r1) goto L_08006ED2;
    /* 0x08006ECC: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08006ECE: c003     lsls     r0, r0, #0xf */
    r0 = r0 << 0xf;
    /* 0x08006ED0: 0ee0     b        #0x8006ef0 */
    goto L_08006EF0;

    L_08006ED2:
    /* 0x08006ED2: 7649     ldr      r1, [pc, #0x1d8]  ; =DMA1.ISR */
    r1 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006ED4: 5831     adds     r1, #0x58 */
    r1 = r1 + 0x58;
    /* 0x08006ED6: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08006ED8: 01d1     bne      #0x8006ede */
    if (r0 != r1) goto L_08006EDE;
    /* 0x08006EDA: 1846     mov      r0, r3 */
    r0 = r3;
    /* 0x08006EDC: 08e0     b        #0x8006ef0 */
    goto L_08006EF0;

    L_08006EDE:
    /* 0x08006EDE: 7349     ldr      r1, [pc, #0x1cc]  ; =DMA1.ISR */
    r1 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006EE0: 6c31     adds     r1, #0x6c */
    r1 = r1 + 0x6c;
    /* 0x08006EE2: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08006EE4: 02d1     bne      #0x8006eec */
    if (r0 != r1) goto L_08006EEC;
    /* 0x08006EE6: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08006EE8: c005     lsls     r0, r0, #0x17 */
    r0 = r0 << 0x17;
    /* 0x08006EEA: 01e0     b        #0x8006ef0 */
    goto L_08006EF0;

    L_08006EEC:
    /* 0x08006EEC: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08006EEE: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;

    L_08006EF0:
    /* 0x08006EF0: 6e49     ldr      r1, [pc, #0x1b8]  ; =DMA1.ISR */
    r1 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006EF2: 4860     str      r0, [r1, #4] */
    *(uint32_t *)(r1 + 4) = r0;   /* store */  /* MMIO: DMA1.IFCR(w) */
    /* 0x08006EF4: a06b     ldr      r0, [r4, #0x38] */
    r0 = *(uint32_t *)(r4 + 56);
    /* 0x08006EF6: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08006EF8: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08006EFA: a063     str      r0, [r4, #0x38] */
    *(uint32_t *)(r4 + 56) = r0;   /* store */
    /* 0x08006EFC: 0420     movs     r0, #4 */
    r0 = 4;
    /* 0x08006EFE: 6870     strb     r0, [r5, #1] */
    *(uint8_t *)(r5 + 1) = r0;   /* store */
    /* 0x08006F00: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006F02: 2870     strb     r0, [r5] */
    *(uint8_t *)(r5) = r0;   /* store */
    /* 0x08006F04: 216b     ldr      r1, [r4, #0x30] */
    r1 = *(uint32_t *)(r4 + 48);
    /* 0x08006F06: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x08006F08: 01d0     beq      #0x8006f0e */
    if (r1 == 0) goto L_08006F0E;
    /* 0x08006F0A: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08006F0C: 8847     blx      r1 */
    ((void(*)())r1)();   /* indirect call */

    L_08006F0E:
    /* 0x08006F0E: 6748     ldr      r0, [pc, #0x19c]  ; =DMA1.ISR */
    r0 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006F10: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: DMA1.ISR(r) */
    /* 0x08006F12: 664a     ldr      r2, [pc, #0x198]  ; =DMA1.ISR */
    r2 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006F14: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x08006F16: 0832     adds     r2, #8 */
    r2 = r2 + 8;
    /* 0x08006F18: 1315     asrs     r3, r2, #0x14 */
    r3 = r2 >> 0x14;
    /* 0x08006F1A: 9042     cmp      r0, r2 */
    /* flags = cmp(r0, r2) */
    /* 0x08006F1C: 01d1     bne      #0x8006f22 */
    if (r0 != r2) goto L_08006F22;
    /* 0x08006F1E: 0422     movs     r2, #4 */
    r2 = 4;
    /* 0x08006F20: 1ee0     b        #0x8006f60 */
    goto L_08006F60;

    L_08006F22:
    /* 0x08006F22: b042     cmp      r0, r6 */
    /* flags = cmp(r0, r6) */
    /* 0x08006F24: 01d1     bne      #0x8006f2a */
    if (r0 != r6) goto L_08006F2A;
    /* 0x08006F26: 4022     movs     r2, #0x40 */
    r2 = 0x40;
    /* 0x08006F28: 1ae0     b        #0x8006f60 */
    goto L_08006F60;

    L_08006F2A:
    /* 0x08006F2A: b842     cmp      r0, r7 */
    /* flags = cmp(r0, r7) */
    /* 0x08006F2C: 01d1     bne      #0x8006f32 */
    if (r0 != r7) goto L_08006F32;
    /* 0x08006F2E: 1a46     mov      r2, r3 */
    r2 = r3;
    /* 0x08006F30: 16e0     b        #0x8006f60 */
    goto L_08006F60;

    L_08006F32:
    /* 0x08006F32: 5e4a     ldr      r2, [pc, #0x178]  ; =DMA1.ISR */
    r2 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006F34: 4432     adds     r2, #0x44 */
    r2 = r2 + 0x44;
    /* 0x08006F36: 9042     cmp      r0, r2 */
    /* flags = cmp(r0, r2) */
    /* 0x08006F38: 02d1     bne      #0x8006f40 */
    if (r0 != r2) goto L_08006F40;
    /* 0x08006F3A: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08006F3C: 9203     lsls     r2, r2, #0xe */
    r2 = r2 << 0xe;
    /* 0x08006F3E: 0fe0     b        #0x8006f60 */
    goto L_08006F60;

    L_08006F40:
    /* 0x08006F40: 5a4a     ldr      r2, [pc, #0x168]  ; =DMA1.ISR */
    r2 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006F42: 5832     adds     r2, #0x58 */
    r2 = r2 + 0x58;
    /* 0x08006F44: 9042     cmp      r0, r2 */
    /* flags = cmp(r0, r2) */
    /* 0x08006F46: 02d1     bne      #0x8006f4e */
    if (r0 != r2) goto L_08006F4E;
    /* 0x08006F48: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08006F4A: 9204     lsls     r2, r2, #0x12 */
    r2 = r2 << 0x12;
    /* 0x08006F4C: 08e0     b        #0x8006f60 */
    goto L_08006F60;

    L_08006F4E:
    /* 0x08006F4E: 574a     ldr      r2, [pc, #0x15c]  ; =DMA1.ISR */
    r2 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006F50: 6c32     adds     r2, #0x6c */
    r2 = r2 + 0x6c;
    /* 0x08006F52: 9042     cmp      r0, r2 */
    /* flags = cmp(r0, r2) */
    /* 0x08006F54: 02d1     bne      #0x8006f5c */
    if (r0 != r2) goto L_08006F5C;
    /* 0x08006F56: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08006F58: 9205     lsls     r2, r2, #0x16 */
    r2 = r2 << 0x16;
    /* 0x08006F5A: 01e0     b        #0x8006f60 */
    goto L_08006F60;

    L_08006F5C:
    /* 0x08006F5C: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08006F5E: 9206     lsls     r2, r2, #0x1a */
    r2 = r2 << 0x1a;

    L_08006F60:
    /* 0x08006F60: 1142     tst      r1, r2 */
    /* flags = tst(r1, r2) */
    /* 0x08006F62: 39d0     beq      #0x8006fd8 */
    if ((r1 & r2)) goto L_08006FD8;   /* test bits */
    /* 0x08006F64: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: DMA1.ISR(r) */
    /* 0x08006F66: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x08006F68: 4907     lsls     r1, r1, #0x1d */
    r1 = r1 << 0x1d;
    /* 0x08006F6A: 35d4     bmi      #0x8006fd8 */
    if ((r1 & r2)) goto L_08006FD8;   /* result negative */
    /* 0x08006F6C: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: DMA1.ISR(r) */
    /* 0x08006F6E: 8906     lsls     r1, r1, #0x1a */
    r1 = r1 << 0x1a;
    /* 0x08006F70: 03d4     bmi      #0x8006f7a */
    if ((r1 & r2)) goto L_08006F7A;   /* result negative */
    /* 0x08006F72: 0268     ldr      r2, [r0] */
    r2 = *(uint32_t *)(r0);  /* MMIO: DMA1.ISR(r) */
    /* 0x08006F74: 0421     movs     r1, #4 */
    r1 = 4;
    /* 0x08006F76: 8a43     bics     r2, r1 */
    r2 = r2 & ~r1;
    /* 0x08006F78: 0260     str      r2, [r0] */
    *(uint32_t *)(r0) = r2;   /* store */  /* MMIO: DMA1.ISR(w) */

    L_08006F7A:
    /* 0x08006F7A: 4c49     ldr      r1, [pc, #0x130]  ; =DMA1.ISR */
    r1 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006F7C: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x08006F7E: 0831     adds     r1, #8 */
    r1 = r1 + 8;
    /* 0x08006F80: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08006F82: 01d1     bne      #0x8006f88 */
    if (r0 != r1) goto L_08006F88;
    /* 0x08006F84: 0420     movs     r0, #4 */
    r0 = 4;
    /* 0x08006F86: 1ee0     b        #0x8006fc6 */
    goto L_08006FC6;

    L_08006F88:
    /* 0x08006F88: b042     cmp      r0, r6 */
    /* flags = cmp(r0, r6) */
    /* 0x08006F8A: 01d1     bne      #0x8006f90 */
    if (r0 != r6) goto L_08006F90;
    /* 0x08006F8C: 4020     movs     r0, #0x40 */
    r0 = 0x40;
    /* 0x08006F8E: 1ae0     b        #0x8006fc6 */
    goto L_08006FC6;

    L_08006F90:
    /* 0x08006F90: b842     cmp      r0, r7 */
    /* flags = cmp(r0, r7) */
    /* 0x08006F92: 01d1     bne      #0x8006f98 */
    if (r0 != r7) goto L_08006F98;
    /* 0x08006F94: 1846     mov      r0, r3 */
    r0 = r3;
    /* 0x08006F96: 16e0     b        #0x8006fc6 */
    goto L_08006FC6;

    L_08006F98:
    /* 0x08006F98: 4449     ldr      r1, [pc, #0x110]  ; =DMA1.ISR */
    r1 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006F9A: 4431     adds     r1, #0x44 */
    r1 = r1 + 0x44;
    /* 0x08006F9C: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08006F9E: 02d1     bne      #0x8006fa6 */
    if (r0 != r1) goto L_08006FA6;
    /* 0x08006FA0: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08006FA2: 8003     lsls     r0, r0, #0xe */
    r0 = r0 << 0xe;
    /* 0x08006FA4: 0fe0     b        #0x8006fc6 */
    goto L_08006FC6;

    L_08006FA6:
    /* 0x08006FA6: 4149     ldr      r1, [pc, #0x104]  ; =DMA1.ISR */
    r1 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006FA8: 5831     adds     r1, #0x58 */
    r1 = r1 + 0x58;
    /* 0x08006FAA: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08006FAC: 02d1     bne      #0x8006fb4 */
    if (r0 != r1) goto L_08006FB4;
    /* 0x08006FAE: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08006FB0: 8004     lsls     r0, r0, #0x12 */
    r0 = r0 << 0x12;
    /* 0x08006FB2: 08e0     b        #0x8006fc6 */
    goto L_08006FC6;

    L_08006FB4:
    /* 0x08006FB4: 3d49     ldr      r1, [pc, #0xf4]  ; =DMA1.ISR */
    r1 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006FB6: 6c31     adds     r1, #0x6c */
    r1 = r1 + 0x6c;
    /* 0x08006FB8: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08006FBA: 02d1     bne      #0x8006fc2 */
    if (r0 != r1) goto L_08006FC2;
    /* 0x08006FBC: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08006FBE: 8005     lsls     r0, r0, #0x16 */
    r0 = r0 << 0x16;
    /* 0x08006FC0: 01e0     b        #0x8006fc6 */
    goto L_08006FC6;

    L_08006FC2:
    /* 0x08006FC2: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08006FC4: 8006     lsls     r0, r0, #0x1a */
    r0 = r0 << 0x1a;

    L_08006FC6:
    /* 0x08006FC6: 3949     ldr      r1, [pc, #0xe4]  ; =DMA1.ISR */
    r1 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006FC8: 4860     str      r0, [r1, #4] */
    *(uint32_t *)(r1 + 4) = r0;   /* store */  /* MMIO: DMA1.IFCR(w) */
    /* 0x08006FCA: 1120     movs     r0, #0x11 */
    r0 = 0x11;
    /* 0x08006FCC: 6870     strb     r0, [r5, #1] */
    *(uint8_t *)(r5 + 1) = r0;   /* store */
    /* 0x08006FCE: e16a     ldr      r1, [r4, #0x2c] */
    r1 = *(uint32_t *)(r4 + 44);
    /* 0x08006FD0: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x08006FD2: 01d0     beq      #0x8006fd8 */
    if (r1 == 0) goto L_08006FD8;
    /* 0x08006FD4: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08006FD6: 8847     blx      r1 */
    ((void(*)())r1)();   /* indirect call */

    L_08006FD8:
    /* 0x08006FD8: 3448     ldr      r0, [pc, #0xd0]  ; =DMA1.ISR */
    r0 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006FDA: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: DMA1.ISR(r) */
    /* 0x08006FDC: 334a     ldr      r2, [pc, #0xcc]  ; =DMA1.ISR */
    r2 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006FDE: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x08006FE0: 0832     adds     r2, #8 */
    r2 = r2 + 8;
    /* 0x08006FE2: 5315     asrs     r3, r2, #0x15 */
    r3 = r2 >> 0x15;
    /* 0x08006FE4: 9042     cmp      r0, r2 */
    /* flags = cmp(r0, r2) */
    /* 0x08006FE6: 01d1     bne      #0x8006fec */
    if (r0 != r2) goto L_08006FEC;
    /* 0x08006FE8: 0222     movs     r2, #2 */
    r2 = 2;
    /* 0x08006FEA: 1ee0     b        #0x800702a */
    goto L_0800702A;

    L_08006FEC:
    /* 0x08006FEC: b042     cmp      r0, r6 */
    /* flags = cmp(r0, r6) */
    /* 0x08006FEE: 01d1     bne      #0x8006ff4 */
    if (r0 != r6) goto L_08006FF4;
    /* 0x08006FF0: 2022     movs     r2, #0x20 */
    r2 = 0x20;
    /* 0x08006FF2: 1ae0     b        #0x800702a */
    goto L_0800702A;

    L_08006FF4:
    /* 0x08006FF4: b842     cmp      r0, r7 */
    /* flags = cmp(r0, r7) */
    /* 0x08006FF6: 01d1     bne      #0x8006ffc */
    if (r0 != r7) goto L_08006FFC;
    /* 0x08006FF8: 1a46     mov      r2, r3 */
    r2 = r3;
    /* 0x08006FFA: 16e0     b        #0x800702a */
    goto L_0800702A;

    L_08006FFC:
    /* 0x08006FFC: 2b4a     ldr      r2, [pc, #0xac]  ; =DMA1.ISR */
    r2 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08006FFE: 4432     adds     r2, #0x44 */
    r2 = r2 + 0x44;
    /* 0x08007000: 9042     cmp      r0, r2 */
    /* flags = cmp(r0, r2) */
    /* 0x08007002: 02d1     bne      #0x800700a */
    if (r0 != r2) goto L_0800700A;
    /* 0x08007004: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08007006: 5203     lsls     r2, r2, #0xd */
    r2 = r2 << 0xd;
    /* 0x08007008: 0fe0     b        #0x800702a */
    goto L_0800702A;

    L_0800700A:
    /* 0x0800700A: 284a     ldr      r2, [pc, #0xa0]  ; =DMA1.ISR */
    r2 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x0800700C: 5832     adds     r2, #0x58 */
    r2 = r2 + 0x58;
    /* 0x0800700E: 9042     cmp      r0, r2 */
    /* flags = cmp(r0, r2) */
    /* 0x08007010: 02d1     bne      #0x8007018 */
    if (r0 != r2) goto L_08007018;
    /* 0x08007012: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08007014: 5204     lsls     r2, r2, #0x11 */
    r2 = r2 << 0x11;
    /* 0x08007016: 08e0     b        #0x800702a */
    goto L_0800702A;

    L_08007018:
    /* 0x08007018: 244a     ldr      r2, [pc, #0x90]  ; =DMA1.ISR */
    r2 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x0800701A: 6c32     adds     r2, #0x6c */
    r2 = r2 + 0x6c;
    /* 0x0800701C: 9042     cmp      r0, r2 */
    /* flags = cmp(r0, r2) */
    /* 0x0800701E: 02d1     bne      #0x8007026 */
    if (r0 != r2) goto L_08007026;
    /* 0x08007020: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08007022: 5205     lsls     r2, r2, #0x15 */
    r2 = r2 << 0x15;
    /* 0x08007024: 01e0     b        #0x800702a */
    goto L_0800702A;

    L_08007026:
    /* 0x08007026: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08007028: 5206     lsls     r2, r2, #0x19 */
    r2 = r2 << 0x19;

    L_0800702A:
    /* 0x0800702A: 1142     tst      r1, r2 */
    /* flags = tst(r1, r2) */
    /* 0x0800702C: 3dd0     beq      #0x80070aa */
    if ((r1 & r2)) goto L_080070AA;   /* test bits */
    /* 0x0800702E: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: DMA1.ISR(r) */
    /* 0x08007030: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x08007032: 8907     lsls     r1, r1, #0x1e */
    r1 = r1 << 0x1e;
    /* 0x08007034: 39d4     bmi      #0x80070aa */
    if ((r1 & r2)) goto L_080070AA;   /* result negative */
    /* 0x08007036: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: DMA1.ISR(r) */
    /* 0x08007038: 8906     lsls     r1, r1, #0x1a */
    r1 = r1 << 0x1a;
    /* 0x0800703A: 03d4     bmi      #0x8007044 */
    if ((r1 & r2)) goto L_08007044;   /* result negative */
    /* 0x0800703C: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: DMA1.ISR(r) */
    /* 0x0800703E: 0222     movs     r2, #2 */
    r2 = 2;
    /* 0x08007040: 9143     bics     r1, r2 */
    r1 = r1 & ~r2;
    /* 0x08007042: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: DMA1.ISR(w) */

    L_08007044:
    /* 0x08007044: 1949     ldr      r1, [pc, #0x64]  ; =DMA1.ISR */
    r1 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08007046: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x08007048: 0831     adds     r1, #8 */
    r1 = r1 + 8;
    /* 0x0800704A: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x0800704C: 01d1     bne      #0x8007052 */
    if (r0 != r1) goto L_08007052;
    /* 0x0800704E: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08007050: 1ee0     b        #0x8007090 */
    goto L_08007090;

    L_08007052:
    /* 0x08007052: b042     cmp      r0, r6 */
    /* flags = cmp(r0, r6) */
    /* 0x08007054: 01d1     bne      #0x800705a */
    if (r0 != r6) goto L_0800705A;
    /* 0x08007056: 2020     movs     r0, #0x20 */
    r0 = 0x20;
    /* 0x08007058: 1ae0     b        #0x8007090 */
    goto L_08007090;

    L_0800705A:
    /* 0x0800705A: b842     cmp      r0, r7 */
    /* flags = cmp(r0, r7) */
    /* 0x0800705C: 01d1     bne      #0x8007062 */
    if (r0 != r7) goto L_08007062;
    /* 0x0800705E: 1846     mov      r0, r3 */
    r0 = r3;
    /* 0x08007060: 16e0     b        #0x8007090 */
    goto L_08007090;

    L_08007062:
    /* 0x08007062: 1249     ldr      r1, [pc, #0x48]  ; =DMA1.ISR */
    r1 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08007064: 4431     adds     r1, #0x44 */
    r1 = r1 + 0x44;
    /* 0x08007066: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08007068: 02d1     bne      #0x8007070 */
    if (r0 != r1) goto L_08007070;
    /* 0x0800706A: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800706C: 4003     lsls     r0, r0, #0xd */
    r0 = r0 << 0xd;
    /* 0x0800706E: 0fe0     b        #0x8007090 */
    goto L_08007090;

    L_08007070:
    /* 0x08007070: 0e49     ldr      r1, [pc, #0x38]  ; =DMA1.ISR */
    r1 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08007072: 5831     adds     r1, #0x58 */
    r1 = r1 + 0x58;
    /* 0x08007074: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08007076: 02d1     bne      #0x800707e */
    if (r0 != r1) goto L_0800707E;
    /* 0x08007078: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800707A: 4004     lsls     r0, r0, #0x11 */
    r0 = r0 << 0x11;
    /* 0x0800707C: 08e0     b        #0x8007090 */
    goto L_08007090;

    L_0800707E:
    /* 0x0800707E: 0b49     ldr      r1, [pc, #0x2c]  ; =DMA1.ISR */
    r1 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08007080: 6c31     adds     r1, #0x6c */
    r1 = r1 + 0x6c;
    /* 0x08007082: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08007084: 02d1     bne      #0x800708c */
    if (r0 != r1) goto L_0800708C;
    /* 0x08007086: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08007088: 4005     lsls     r0, r0, #0x15 */
    r0 = r0 << 0x15;
    /* 0x0800708A: 01e0     b        #0x8007090 */
    goto L_08007090;

    L_0800708C:
    /* 0x0800708C: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800708E: 4006     lsls     r0, r0, #0x19 */
    r0 = r0 << 0x19;

    L_08007090:
    /* 0x08007090: 0649     ldr      r1, [pc, #0x18]  ; =DMA1.ISR */
    r1 = *(uint32_t *)0x080070AC;   /* = MMIO DMA1+0x00 */
    /* 0x08007092: 4860     str      r0, [r1, #4] */
    *(uint32_t *)(r1 + 4) = r0;   /* store */  /* MMIO: DMA1.IFCR(w) */
    /* 0x08007094: a06b     ldr      r0, [r4, #0x38] */
    r0 = *(uint32_t *)(r4 + 56);
    /* 0x08007096: a063     str      r0, [r4, #0x38] */
    *(uint32_t *)(r4 + 56) = r0;   /* store */
    /* 0x08007098: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800709A: 6870     strb     r0, [r5, #1] */
    *(uint8_t *)(r5 + 1) = r0;   /* store */
    /* 0x0800709C: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800709E: 2870     strb     r0, [r5] */
    *(uint8_t *)(r5) = r0;   /* store */
    /* 0x080070A0: a16a     ldr      r1, [r4, #0x28] */
    r1 = *(uint32_t *)(r4 + 40);
    /* 0x080070A2: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x080070A4: 01d0     beq      #0x80070aa */
    if (r1 == 0) goto L_080070AA;
    /* 0x080070A6: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080070A8: 8847     blx      r1 */
    ((void(*)())r1)();   /* indirect call */

    L_080070AA:
    /* 0x080070AA: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
