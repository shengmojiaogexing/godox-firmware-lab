/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800BDC4
 * Name      : FUN_0800BDC4
 * End       : 0x0800C058
 * Size      : 660 bytes, 277 instructions decoded in body (functions.json insns=277, blocks=35)
 * Class     : hub
 * Callers   : 9  -> 0x0800ABE4 FUN_0800ABE4, 0x0800AC0A FUN_0800AC0A, 0x0800C080 FUN_0800C080, 0x0800C0C8 FUN_0800C0C8, 0x0800C116 FUN_0800C116, 0x0800C124 FUN_0800C124, 0x0800C13A FUN_0800C13A, 0x0800C14E FUN_0800C14E, ... +1
 * Callees   : 4  -> 0x080030F0 FUN_080030F0, 0x08004C0C FUN_08004C0C, 0x08004EE4 FUN_08004EE4, 0x0800C82C FUN_0800C82C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800BDC4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800BDC4: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x0800BDC6: 3024     movs     r4, #0x30 */
    r4 = 0x30;
    /* 0x0800BDC8: 3725     movs     r5, #0x37 */
    r5 = 0x37;
    /* 0x0800BDCA: 4826     movs     r6, #0x48 */
    r6 = 0x48;
    /* 0x0800BDCC: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800BDCE: 57d0     beq      #0x800be80 */
    if (r0 == 0) goto L_0800BE80;
    /* 0x0800BDD0: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800BDD2: 7cd1     bne      #0x800bece */
    if (r0 != 1) goto L_0800BECE;
    /* 0x0800BDD4: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800BDD6: f8f719ff bl       #0x8004c0c */
    FUN_08004C0C();   /* BL -> 0x08004C0C FUN_08004C0C */
    /* 0x0800BDDA: 0127     movs     r7, #1 */
    r7 = 1;
    /* 0x0800BDDC: 2221     movs     r1, #0x22 */
    r1 = 0x22;
    /* 0x0800BDDE: 1d22     movs     r2, #0x1d */
    r2 = 0x1d;
    /* 0x0800BDE0: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800BDE2: f9f77ff8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BDE6: 2921     movs     r1, #0x29 */
    r1 = 0x29;
    /* 0x0800BDE8: 0e22     movs     r2, #0xe */
    r2 = 0xe;
    /* 0x0800BDEA: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800BDEC: f9f77af8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BDF0: 2146     mov      r1, r4 */
    r1 = r4;
    /* 0x0800BDF2: 1c22     movs     r2, #0x1c */
    r2 = 0x1c;
    /* 0x0800BDF4: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800BDF6: f9f775f8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BDFA: 2946     mov      r1, r5 */
    r1 = r5;
    /* 0x0800BDFC: 1d22     movs     r2, #0x1d */
    r2 = 0x1d;
    /* 0x0800BDFE: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800BE00: f9f770f8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BE04: 4221     movs     r1, #0x42 */
    r1 = 0x42;
    /* 0x0800BE06: 1922     movs     r2, #0x19 */
    r2 = 0x19;
    /* 0x0800BE08: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800BE0A: f9f76bf8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BE0E: 4921     movs     r1, #0x49 */
    r1 = 0x49;
    /* 0x0800BE10: 1b22     movs     r2, #0x1b */
    r2 = 0x1b;
    /* 0x0800BE12: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800BE14: f9f766f8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BE18: 5021     movs     r1, #0x50 */
    r1 = 0x50;
    /* 0x0800BE1A: 1822     movs     r2, #0x18 */
    r2 = 0x18;
    /* 0x0800BE1C: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800BE1E: f9f761f8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BE22: 5721     movs     r1, #0x57 */
    r1 = 0x57;
    /* 0x0800BE24: 1022     movs     r2, #0x10 */
    r2 = 0x10;
    /* 0x0800BE26: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800BE28: f9f75cf8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BE2C: 8a4f     ldr      r7, [pc, #0x228]  ; =RAM_200001ED */
    r7 = *(uint32_t *)0x0800C058;   /* = RAM 0x200001ED */
    /* 0x0800BE2E: 2524     movs     r4, #0x25 */
    r4 = 0x25;
    /* 0x0800BE30: 3878     ldrb     r0, [r7] */
    r0 = *(uint8_t *)(r7);
    /* 0x0800BE32: 2c25     movs     r5, #0x2c */
    r5 = 0x2c;
    /* 0x0800BE34: 0f28     cmp      r0, #0xf */
    /* flags = cmp(r0, #0xf) */
    /* 0x0800BE36: 22d0     beq      #0x800be7e */
    if (r0 == 0xf) goto L_0800BE7E;
    /* 0x0800BE38: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x0800BE3A: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800BE3C: 1722     movs     r2, #0x17 */
    r2 = 0x17;
    /* 0x0800BE3E: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x0800BE40: f9f750f8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BE44: 0821     movs     r1, #8 */
    r1 = 8;
    /* 0x0800BE46: 1d22     movs     r2, #0x1d */
    r2 = 0x1d;
    /* 0x0800BE48: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800BE4A: f9f74bf8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BE4E: 0f21     movs     r1, #0xf */
    r1 = 0xf;
    /* 0x0800BE50: 0c22     movs     r2, #0xc */
    r2 = 0xc;
    /* 0x0800BE52: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800BE54: f9f746f8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BE58: 1621     movs     r1, #0x16 */
    r1 = 0x16;
    /* 0x0800BE5A: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800BE5C: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800BE5E: f9f741f8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BE62: 1e21     movs     r1, #0x1e */
    r1 = 0x1e;
    /* 0x0800BE64: 4822     movs     r2, #0x48 */
    r2 = 0x48;
    /* 0x0800BE66: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800BE68: f9f73cf8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BE6C: 3878     ldrb     r0, [r7] */
    r0 = *(uint8_t *)(r7);
    /* 0x0800BE6E: ff28     cmp      r0, #0xff */
    /* flags = cmp(r0, #0xff) */
    /* 0x0800BE70: 49d0     beq      #0x800bf06 */
    if (r0 == 0xff) goto L_0800BF06;
    /* 0x0800BE72: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800BE74: 4fd0     beq      #0x800bf16 */
    if (r0 == 0) goto L_0800BF16;
    /* 0x0800BE76: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800BE78: 55d0     beq      #0x800bf26 */
    if (r0 == 1) goto L_0800BF26;
    /* 0x0800BE7A: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x0800BE7C: 5bd0     beq      #0x800bf36 */
    if (r0 == 2) goto L_0800BF36;

    L_0800BE7E:
    /* 0x0800BE7E: 64e0     b        #0x800bf4a */
    goto L_0800BF4A;

    L_0800BE80:
    /* 0x0800BE80: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800BE82: f8f7c3fe bl       #0x8004c0c */
    FUN_08004C0C();   /* BL -> 0x08004C0C FUN_08004C0C */
    /* 0x0800BE86: 0127     movs     r7, #1 */
    r7 = 1;
    /* 0x0800BE88: 2921     movs     r1, #0x29 */
    r1 = 0x29;
    /* 0x0800BE8A: 1d22     movs     r2, #0x1d */
    r2 = 0x1d;
    /* 0x0800BE8C: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800BE8E: f9f729f8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BE92: 2146     mov      r1, r4 */
    r1 = r4;
    /* 0x0800BE94: 0e22     movs     r2, #0xe */
    r2 = 0xe;
    /* 0x0800BE96: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800BE98: f9f724f8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BE9C: 2946     mov      r1, r5 */
    r1 = r5;
    /* 0x0800BE9E: 1c22     movs     r2, #0x1c */
    r2 = 0x1c;
    /* 0x0800BEA0: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800BEA2: f9f71ff8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BEA6: 3e21     movs     r1, #0x3e */
    r1 = 0x3e;
    /* 0x0800BEA8: 1d22     movs     r2, #0x1d */
    r2 = 0x1d;
    /* 0x0800BEAA: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800BEAC: f9f71af8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BEB0: 3146     mov      r1, r6 */
    r1 = r6;
    /* 0x0800BEB2: 1b22     movs     r2, #0x1b */
    r2 = 0x1b;
    /* 0x0800BEB4: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800BEB6: f9f715f8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BEBA: 4f21     movs     r1, #0x4f */
    r1 = 0x4f;
    /* 0x0800BEBC: 0f22     movs     r2, #0xf */
    r2 = 0xf;
    /* 0x0800BEBE: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800BEC0: f9f710f8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BEC4: 0324     movs     r4, #3 */
    r4 = 3;
    /* 0x0800BEC6: 3121     movs     r1, #0x31 */
    r1 = 0x31;
    /* 0x0800BEC8: 0c22     movs     r2, #0xc */
    r2 = 0xc;
    /* 0x0800BECA: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800BECC: 00e0     b        #0x800bed0 */
    goto L_0800BED0;

    L_0800BECE:
    /* 0x0800BECE: b6e0     b        #0x800c03e */
    goto L_0800C03E;

    L_0800BED0:
    /* 0x0800BED0: f9f708f8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BED4: 3821     movs     r1, #0x38 */
    r1 = 0x38;
    /* 0x0800BED6: 1122     movs     r2, #0x11 */
    r2 = 0x11;
    /* 0x0800BED8: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800BEDA: f9f703f8 bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BEDE: 5f4d     ldr      r5, [pc, #0x17c]  ; =RAM_20000183 */
    r5 = *(uint32_t *)0x0800C05C;   /* = RAM 0x20000183 */
    /* 0x0800BEE0: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x0800BEE2: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);
    /* 0x0800BEE4: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800BEE6: f7f703f9 bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x0800BEEA: c2b2     uxtb     r2, r0 */
    r2 = uxtb(r0);
    /* 0x0800BEEC: 3f21     movs     r1, #0x3f */
    r1 = 0x3f;
    /* 0x0800BEEE: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800BEF0: f8f7f8ff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BEF4: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);
    /* 0x0800BEF6: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x0800BEF8: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800BEFA: f7f7f9f8 bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x0800BEFE: 0a46     mov      r2, r1 */
    r2 = r1;
    /* 0x0800BF00: 4621     movs     r1, #0x46 */
    r1 = 0x46;
    /* 0x0800BF02: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800BF04: 8de0     b        #0x800c022 */
    goto L_0800C022;

    L_0800BF06:
    /* 0x0800BF06: 2146     mov      r1, r4 */
    r1 = r4;
    /* 0x0800BF08: 3e22     movs     r2, #0x3e */
    r2 = 0x3e;
    /* 0x0800BF0A: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800BF0C: f8f7eaff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BF10: 2946     mov      r1, r5 */
    r1 = r5;
    /* 0x0800BF12: 3e22     movs     r2, #0x3e */
    r2 = 0x3e;
    /* 0x0800BF14: 16e0     b        #0x800bf44 */
    goto L_0800BF44;

    L_0800BF16:
    /* 0x0800BF16: 2146     mov      r1, r4 */
    r1 = r4;
    /* 0x0800BF18: 1822     movs     r2, #0x18 */
    r2 = 0x18;
    /* 0x0800BF1A: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800BF1C: f8f7e2ff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BF20: 2946     mov      r1, r5 */
    r1 = r5;
    /* 0x0800BF22: 1422     movs     r2, #0x14 */
    r2 = 0x14;
    /* 0x0800BF24: 0ee0     b        #0x800bf44 */
    goto L_0800BF44;

    L_0800BF26:
    /* 0x0800BF26: 2146     mov      r1, r4 */
    r1 = r4;
    /* 0x0800BF28: 0e22     movs     r2, #0xe */
    r2 = 0xe;
    /* 0x0800BF2A: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800BF2C: f8f7daff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BF30: 2946     mov      r1, r5 */
    r1 = r5;
    /* 0x0800BF32: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800BF34: 06e0     b        #0x800bf44 */
    goto L_0800BF44;

    L_0800BF36:
    /* 0x0800BF36: 2146     mov      r1, r4 */
    r1 = r4;
    /* 0x0800BF38: 0e22     movs     r2, #0xe */
    r2 = 0xe;
    /* 0x0800BF3A: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800BF3C: f8f7d2ff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BF40: 2946     mov      r1, r5 */
    r1 = r5;
    /* 0x0800BF42: 0222     movs     r2, #2 */
    r2 = 2;

    L_0800BF44:
    /* 0x0800BF44: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800BF46: f8f7cdff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */

    L_0800BF4A:
    /* 0x0800BF4A: 454f     ldr      r7, [pc, #0x114]  ; =RAM_200001EE */
    r7 = *(uint32_t *)0x0800C060;   /* = RAM 0x200001EE */
    /* 0x0800BF4C: 3878     ldrb     r0, [r7] */
    r0 = *(uint8_t *)(r7);
    /* 0x0800BF4E: 0f28     cmp      r0, #0xf */
    /* flags = cmp(r0, #0xf) */
    /* 0x0800BF50: 43d0     beq      #0x800bfda */
    if (r0 == 0xf) goto L_0800BFDA;
    /* 0x0800BF52: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x0800BF54: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x0800BF56: 1722     movs     r2, #0x17 */
    r2 = 0x17;
    /* 0x0800BF58: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x0800BF5A: f8f7c3ff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BF5E: 3146     mov      r1, r6 */
    r1 = r6;
    /* 0x0800BF60: 1d22     movs     r2, #0x1d */
    r2 = 0x1d;
    /* 0x0800BF62: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800BF64: f8f7beff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BF68: 4f21     movs     r1, #0x4f */
    r1 = 0x4f;
    /* 0x0800BF6A: 0c22     movs     r2, #0xc */
    r2 = 0xc;
    /* 0x0800BF6C: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800BF6E: f8f7b9ff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BF72: 5621     movs     r1, #0x56 */
    r1 = 0x56;
    /* 0x0800BF74: 0222     movs     r2, #2 */
    r2 = 2;
    /* 0x0800BF76: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800BF78: f8f7b4ff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BF7C: 5e21     movs     r1, #0x5e */
    r1 = 0x5e;
    /* 0x0800BF7E: 4822     movs     r2, #0x48 */
    r2 = 0x48;
    /* 0x0800BF80: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800BF82: f8f7afff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BF86: 3878     ldrb     r0, [r7] */
    r0 = *(uint8_t *)(r7);
    /* 0x0800BF88: 6521     movs     r1, #0x65 */
    r1 = 0x65;
    /* 0x0800BF8A: 6c26     movs     r6, #0x6c */
    r6 = 0x6c;
    /* 0x0800BF8C: ff28     cmp      r0, #0xff */
    /* flags = cmp(r0, #0xff) */
    /* 0x0800BF8E: 06d0     beq      #0x800bf9e */
    if (r0 == 0xff) goto L_0800BF9E;
    /* 0x0800BF90: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800BF92: 0bd0     beq      #0x800bfac */
    if (r0 == 0) goto L_0800BFAC;
    /* 0x0800BF94: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800BF96: 10d0     beq      #0x800bfba */
    if (r0 == 1) goto L_0800BFBA;
    /* 0x0800BF98: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x0800BF9A: 15d0     beq      #0x800bfc8 */
    if (r0 == 2) goto L_0800BFC8;
    /* 0x0800BF9C: 1de0     b        #0x800bfda */
    goto L_0800BFDA;

    L_0800BF9E:
    /* 0x0800BF9E: 3e22     movs     r2, #0x3e */
    r2 = 0x3e;
    /* 0x0800BFA0: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800BFA2: f8f79fff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BFA6: 3146     mov      r1, r6 */
    r1 = r6;
    /* 0x0800BFA8: 3e22     movs     r2, #0x3e */
    r2 = 0x3e;
    /* 0x0800BFAA: 13e0     b        #0x800bfd4 */
    goto L_0800BFD4;

    L_0800BFAC:
    /* 0x0800BFAC: 1822     movs     r2, #0x18 */
    r2 = 0x18;
    /* 0x0800BFAE: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800BFB0: f8f798ff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BFB4: 3146     mov      r1, r6 */
    r1 = r6;
    /* 0x0800BFB6: 1422     movs     r2, #0x14 */
    r2 = 0x14;
    /* 0x0800BFB8: 0ce0     b        #0x800bfd4 */
    goto L_0800BFD4;

    L_0800BFBA:
    /* 0x0800BFBA: 0e22     movs     r2, #0xe */
    r2 = 0xe;
    /* 0x0800BFBC: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800BFBE: f8f791ff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BFC2: 3146     mov      r1, r6 */
    r1 = r6;
    /* 0x0800BFC4: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800BFC6: 05e0     b        #0x800bfd4 */
    goto L_0800BFD4;

    L_0800BFC8:
    /* 0x0800BFC8: 0e22     movs     r2, #0xe */
    r2 = 0xe;
    /* 0x0800BFCA: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800BFCC: f8f78aff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BFD0: 3146     mov      r1, r6 */
    r1 = r6;
    /* 0x0800BFD2: 0222     movs     r2, #2 */
    r2 = 2;

    L_0800BFD4:
    /* 0x0800BFD4: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800BFD6: f8f785ff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */

    L_0800BFDA:
    /* 0x0800BFDA: 224e     ldr      r6, [pc, #0x88]  ; =RAM_200001F0 */
    r6 = *(uint32_t *)0x0800C064;   /* = RAM 0x200001F0 */
    /* 0x0800BFDC: 3078     ldrb     r0, [r6] */
    r0 = *(uint8_t *)(r6);
    /* 0x0800BFDE: 0f28     cmp      r0, #0xf */
    /* flags = cmp(r0, #0xf) */
    /* 0x0800BFE0: 21d0     beq      #0x800c026 */
    if (r0 == 0xf) goto L_0800C026;
    /* 0x0800BFE2: 0527     movs     r7, #5 */
    r7 = 5;
    /* 0x0800BFE4: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800BFE6: 0f22     movs     r2, #0xf */
    r2 = 0xf;
    /* 0x0800BFE8: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800BFEA: f8f77bff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BFEE: 0721     movs     r1, #7 */
    r1 = 7;
    /* 0x0800BFF0: 0a22     movs     r2, #0xa */
    r2 = 0xa;
    /* 0x0800BFF2: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800BFF4: f8f776ff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800BFF8: 0e21     movs     r1, #0xe */
    r1 = 0xe;
    /* 0x0800BFFA: 1722     movs     r2, #0x17 */
    r2 = 0x17;
    /* 0x0800BFFC: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800BFFE: f8f771ff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800C002: 1821     movs     r1, #0x18 */
    r1 = 0x18;
    /* 0x0800C004: 4822     movs     r2, #0x48 */
    r2 = 0x48;
    /* 0x0800C006: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800C008: f8f76cff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800C00C: 3078     ldrb     r0, [r6] */
    r0 = *(uint8_t *)(r6);
    /* 0x0800C00E: 2146     mov      r1, r4 */
    r1 = r4;
    /* 0x0800C010: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800C012: 15d0     beq      #0x800c040 */
    if (r0 == 0) goto L_0800C040;
    /* 0x0800C014: 1822     movs     r2, #0x18 */
    r2 = 0x18;
    /* 0x0800C016: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800C018: f8f764ff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800C01C: 2946     mov      r1, r5 */
    r1 = r5;
    /* 0x0800C01E: 1722     movs     r2, #0x17 */
    r2 = 0x17;

    L_0800C020:
    /* 0x0800C020: 3846     mov      r0, r7 */
    r0 = r7;

    L_0800C022:
    /* 0x0800C022: f8f75fff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */

    L_0800C026:
    /* 0x0800C026: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C028: 00f000fc bl       #0x800c82c */
    FUN_0800C82C();   /* BL -> 0x0800C82C FUN_0800C82C */
    /* 0x0800C02C: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800C02E: 00f0fdfb bl       #0x800c82c */
    FUN_0800C82C();   /* BL -> 0x0800C82C FUN_0800C82C */
    /* 0x0800C032: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x0800C034: 00f0fafb bl       #0x800c82c */
    FUN_0800C82C();   /* BL -> 0x0800C82C FUN_0800C82C */
    /* 0x0800C038: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x0800C03A: 00f0f7fb bl       #0x800c82c */
    FUN_0800C82C();   /* BL -> 0x0800C82C FUN_0800C82C */

    L_0800C03E:
    /* 0x0800C03E: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_0800C040:
    /* 0x0800C040: 1822     movs     r2, #0x18 */
    r2 = 0x18;
    /* 0x0800C042: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800C044: f8f74eff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800C048: 2946     mov      r1, r5 */
    r1 = r5;
    /* 0x0800C04A: 0f22     movs     r2, #0xf */
    r2 = 0xf;
    /* 0x0800C04C: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800C04E: f8f749ff bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x0800C052: 3321     movs     r1, #0x33 */
    r1 = 0x33;
    /* 0x0800C054: 0f22     movs     r2, #0xf */
    r2 = 0xf;
    /* 0x0800C056: e3e7     b        #0x800c020 */
    goto L_0800C020;   /* loop */
}
