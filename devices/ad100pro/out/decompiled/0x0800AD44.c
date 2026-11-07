/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800AD44
 * Name      : FUN_0800AD44
 * End       : 0x0800AF3E
 * Size      : 506 bytes, 247 instructions decoded in body (functions.json insns=247, blocks=109)
 * Class     : function
 * Callers   : 2  -> 0x0800CF74 main, 0x0800CFEE FUN_0800CFEE
 * Callees   : 5  -> 0x08004B74 FUN_08004B74, 0x08005DFC FUN_08005DFC, 0x08005E8C FUN_08005E8C, 0x0800B588 FUN_0800B588, 0x0800CE6C FUN_0800CE6C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800AD44(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800AD44: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x0800AD46: 0027     movs     r7, #0 */
    r7 = 0;
    /* 0x0800AD48: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x0800AD4A: 7d4d     ldr      r5, [pc, #0x1f4]  ; =RAM_20000120 */
    r5 = *(uint32_t *)0x0800AF40;   /* = RAM 0x20000120 */
    /* 0x0800AD4C: 1420     movs     r0, #0x14 */
    r0 = 0x14;
    /* 0x0800AD4E: 2870     strb     r0, [r5] */
    *(uint8_t *)(r5) = r0;   /* store */
    /* 0x0800AD50: 3e46     mov      r6, r7 */
    r6 = r7;

    L_0800AD52:
    /* 0x0800AD52: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800AD54: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x0800AD56: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800AD58: 2870     strb     r0, [r5] */
    *(uint8_t *)(r5) = r0;   /* store */
    /* 0x0800AD5A: 6421     movs     r1, #0x64 */
    r1 = 0x64;
    /* 0x0800AD5C: 4843     muls     r0, r1, r0 */
    r0 = r1 * r0;
    /* 0x0800AD5E: 7949     ldr      r1, [pc, #0x1e4]  ; =0x0801F000 */
    r1 = *(uint32_t *)0x0800AF44;   /* = 0x0801F000 */
    /* 0x0800AD60: 4018     adds     r0, r0, r1 */
    r0 = r0 + r1;
    /* 0x0800AD62: fbf793f8 bl       #0x8005e8c */
    FUN_08005E8C();   /* BL -> 0x08005E8C FUN_08005E8C */
    /* 0x0800AD66: 7848     ldr      r0, [pc, #0x1e0]  ; =RAM_20000434 */
    r0 = *(uint32_t *)0x0800AF48;   /* = RAM 0x20000434 */
    /* 0x0800AD68: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800AD6A: 5c28     cmp      r0, #0x5c */
    /* flags = cmp(r0, #0x5c) */
    /* 0x0800AD6C: 07d0     beq      #0x800ad7e */
    if (r0 == 0x5c) goto L_0800AD7E;
    /* 0x0800AD6E: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);
    /* 0x0800AD70: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800AD72: 32d0     beq      #0x800adda */
    if (r0 == 0) goto L_0800ADDA;
    /* 0x0800AD74: 002c     cmp      r4, #0 */
    /* flags = cmp(r4, #0) */
    /* 0x0800AD76: ecd1     bne      #0x800ad52 */
    if (r4 != 0) goto L_0800AD52;

    L_0800AD78:
    /* 0x0800AD78: 002f     cmp      r7, #0 */
    /* flags = cmp(r7, #0) */
    /* 0x0800AD7A: 30d0     beq      #0x800adde */
    if (r7 == 0) goto L_0800ADDE;
    /* 0x0800AD7C: 00e0     b        #0x800ad80 */
    goto L_0800AD80;

    L_0800AD7E:
    /* 0x0800AD7E: 0024     movs     r4, #0 */
    r4 = 0;

    L_0800AD80:
    /* 0x0800AD80: 02f074f8 bl       #0x800ce6c */
    FUN_0800CE6C();   /* BL -> 0x0800CE6C FUN_0800CE6C */
    /* 0x0800AD84: 7148     ldr      r0, [pc, #0x1c4]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800AF4C;   /* = RAM 0x20000175 */
    /* 0x0800AD86: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x0800AD88: 8907     lsls     r1, r1, #0x1e */
    r1 = r1 << 0x1e;
    /* 0x0800AD8A: 890f     lsrs     r1, r1, #0x1e */
    r1 = r1 >> 0x1e;
    /* 0x0800AD8C: 0229     cmp      r1, #2 */
    /* flags = cmp(r1, #2) */
    /* 0x0800AD8E: 00d9     bls      #0x800ad92 */
    if (r1 <= 2) goto L_0800AD92;   /* unsigned */
    /* 0x0800AD90: 0670     strb     r6, [r0] */
    *(uint8_t *)(r0) = r6;   /* store */

    L_0800AD92:
    /* 0x0800AD92: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x0800AD94: 6f4a     ldr      r2, [pc, #0x1bc]  ; =RAM_200001A2 */
    r2 = *(uint32_t *)0x0800AF54;   /* = RAM 0x200001A2 */
    /* 0x0800AD96: c906     lsls     r1, r1, #0x1b */
    r1 = r1 << 0x1b;
    /* 0x0800AD98: c90e     lsrs     r1, r1, #0x1b */
    r1 = r1 >> 0x1b;
    /* 0x0800AD9A: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x0800AD9C: 6c48     ldr      r0, [pc, #0x1b0]  ; =RAM_2000019C */
    r0 = *(uint32_t *)0x0800AF50;   /* = RAM 0x2000019C */
    /* 0x0800AD9E: 5021     movs     r1, #0x50 */
    r1 = 0x50;

    L_0800ADA0:
    /* 0x0800ADA0: 035d     ldrb     r3, [r0, r4] */
    r3 = *(uint8_t *)(r0 + r4);
    /* 0x0800ADA2: 502b     cmp      r3, #0x50 */
    /* flags = cmp(r3, #0x50) */
    /* 0x0800ADA4: 00d9     bls      #0x800ada8 */
    if (r3 <= 0x50) goto L_0800ADA8;   /* unsigned */
    /* 0x0800ADA6: 0155     strb     r1, [r0, r4] */
    *(uint8_t *)(r0 + r4) = r1;   /* store */

    L_0800ADA8:
    /* 0x0800ADA8: 135d     ldrb     r3, [r2, r4] */
    r3 = *(uint8_t *)(r2 + r4);
    /* 0x0800ADAA: 143b     subs     r3, #0x14 */
    r3 = r3 - 0x14;
    /* 0x0800ADAC: 3d2b     cmp      r3, #0x3d */
    /* flags = cmp(r3, #0x3d) */
    /* 0x0800ADAE: 00d3     blo      #0x800adb2 */
    if (r3 < 0x3d) goto L_0800ADB2;   /* unsigned */
    /* 0x0800ADB0: 1155     strb     r1, [r2, r4] */
    *(uint8_t *)(r2 + r4) = r1;   /* store */

    L_0800ADB2:
    /* 0x0800ADB2: 641c     adds     r4, r4, #1 */
    r4 = r4 + 1;
    /* 0x0800ADB4: e4b2     uxtb     r4, r4 */
    r4 = uxtb(r4);
    /* 0x0800ADB6: 062c     cmp      r4, #6 */
    /* flags = cmp(r4, #6) */
    /* 0x0800ADB8: f2d3     blo      #0x800ada0 */
    if (r4 < 6) goto L_0800ADA0;   /* unsigned */
    /* 0x0800ADBA: 674a     ldr      r2, [pc, #0x19c]  ; =RAM_20000166 */
    r2 = *(uint32_t *)0x0800AF58;   /* = RAM 0x20000166 */
    /* 0x0800ADBC: 0521     movs     r1, #5 */
    r1 = 5;
    /* 0x0800ADBE: 1078     ldrb     r0, [r2] */
    r0 = *(uint8_t *)(r2);
    /* 0x0800ADC0: 1428     cmp      r0, #0x14 */
    /* flags = cmp(r0, #0x14) */
    /* 0x0800ADC2: 24d9     bls      #0x800ae0e */
    if (r0 <= 0x14) goto L_0800AE0E;   /* unsigned */
    /* 0x0800ADC4: 3228     cmp      r0, #0x32 */
    /* flags = cmp(r0, #0x32) */
    /* 0x0800ADC6: 22d0     beq      #0x800ae0e */
    if (r0 == 0x32) goto L_0800AE0E;
    /* 0x0800ADC8: 14dc     bgt      #0x800adf4 */
    if (r0 > 0x32) goto L_0800ADF4;   /* signed */
    /* 0x0800ADCA: 2328     cmp      r0, #0x23 */
    /* flags = cmp(r0, #0x23) */
    /* 0x0800ADCC: 1fd0     beq      #0x800ae0e */
    if (r0 == 0x23) goto L_0800AE0E;
    /* 0x0800ADCE: 0cdc     bgt      #0x800adea */
    if (r0 > 0x23) goto L_0800ADEA;   /* signed */
    /* 0x0800ADD0: 1928     cmp      r0, #0x19 */
    /* flags = cmp(r0, #0x19) */
    /* 0x0800ADD2: 1cd0     beq      #0x800ae0e */
    if (r0 == 0x19) goto L_0800AE0E;
    /* 0x0800ADD4: 1e28     cmp      r0, #0x1e */
    /* flags = cmp(r0, #0x1e) */
    /* 0x0800ADD6: 19d1     bne      #0x800ae0c */
    if (r0 != 0x1e) goto L_0800AE0C;
    /* 0x0800ADD8: 19e0     b        #0x800ae0e */
    goto L_0800AE0E;

    L_0800ADDA:
    /* 0x0800ADDA: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x0800ADDC: cce7     b        #0x800ad78 */
    goto L_0800AD78;   /* loop */

    L_0800ADDE:
    /* 0x0800ADDE: f9f7c9fe bl       #0x8004b74 */
    FUN_08004B74();   /* BL -> 0x08004B74 FUN_08004B74 */
    /* 0x0800ADE2: fbf70bf8 bl       #0x8005dfc */
    FUN_08005DFC();   /* BL -> 0x08005DFC FUN_08005DFC */
    /* 0x0800ADE6: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x0800ADE8: a5e0     b        #0x800af36 */
    goto L_0800AF36;

    L_0800ADEA:
    /* 0x0800ADEA: 2828     cmp      r0, #0x28 */
    /* flags = cmp(r0, #0x28) */
    /* 0x0800ADEC: 0fd0     beq      #0x800ae0e */
    if (r0 == 0x28) goto L_0800AE0E;
    /* 0x0800ADEE: 2d28     cmp      r0, #0x2d */
    /* flags = cmp(r0, #0x2d) */
    /* 0x0800ADF0: 0cd1     bne      #0x800ae0c */
    if (r0 != 0x2d) goto L_0800AE0C;
    /* 0x0800ADF2: 0ce0     b        #0x800ae0e */
    goto L_0800AE0E;

    L_0800ADF4:
    /* 0x0800ADF4: 5028     cmp      r0, #0x50 */
    /* flags = cmp(r0, #0x50) */
    /* 0x0800ADF6: 0ad0     beq      #0x800ae0e */
    if (r0 == 0x50) goto L_0800AE0E;
    /* 0x0800ADF8: 04dc     bgt      #0x800ae04 */
    if (r0 > 0x50) goto L_0800AE04;   /* signed */
    /* 0x0800ADFA: 3c28     cmp      r0, #0x3c */
    /* flags = cmp(r0, #0x3c) */
    /* 0x0800ADFC: 07d0     beq      #0x800ae0e */
    if (r0 == 0x3c) goto L_0800AE0E;
    /* 0x0800ADFE: 4628     cmp      r0, #0x46 */
    /* flags = cmp(r0, #0x46) */
    /* 0x0800AE00: 04d1     bne      #0x800ae0c */
    if (r0 != 0x46) goto L_0800AE0C;
    /* 0x0800AE02: 04e0     b        #0x800ae0e */
    goto L_0800AE0E;

    L_0800AE04:
    /* 0x0800AE04: 5a28     cmp      r0, #0x5a */
    /* flags = cmp(r0, #0x5a) */
    /* 0x0800AE06: 02d0     beq      #0x800ae0e */
    if (r0 == 0x5a) goto L_0800AE0E;
    /* 0x0800AE08: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x0800AE0A: 00d0     beq      #0x800ae0e */
    if (r0 == 0x64) goto L_0800AE0E;

    L_0800AE0C:
    /* 0x0800AE0C: 1170     strb     r1, [r2] */
    *(uint8_t *)(r2) = r1;   /* store */

    L_0800AE0E:
    /* 0x0800AE0E: 534a     ldr      r2, [pc, #0x14c]  ; =RAM_20000165 */
    r2 = *(uint32_t *)0x0800AF5C;   /* = RAM 0x20000165 */
    /* 0x0800AE10: 1078     ldrb     r0, [r2] */
    r0 = *(uint8_t *)(r2);
    /* 0x0800AE12: 1428     cmp      r0, #0x14 */
    /* flags = cmp(r0, #0x14) */
    /* 0x0800AE14: 1cd9     bls      #0x800ae50 */
    if (r0 <= 0x14) goto L_0800AE50;   /* unsigned */
    /* 0x0800AE16: 3228     cmp      r0, #0x32 */
    /* flags = cmp(r0, #0x32) */
    /* 0x0800AE18: 1ad0     beq      #0x800ae50 */
    if (r0 == 0x32) goto L_0800AE50;
    /* 0x0800AE1A: 0cdc     bgt      #0x800ae36 */
    if (r0 > 0x32) goto L_0800AE36;   /* signed */
    /* 0x0800AE1C: 2328     cmp      r0, #0x23 */
    /* flags = cmp(r0, #0x23) */
    /* 0x0800AE1E: 17d0     beq      #0x800ae50 */
    if (r0 == 0x23) goto L_0800AE50;
    /* 0x0800AE20: 04dc     bgt      #0x800ae2c */
    if (r0 > 0x23) goto L_0800AE2C;   /* signed */
    /* 0x0800AE22: 1928     cmp      r0, #0x19 */
    /* flags = cmp(r0, #0x19) */
    /* 0x0800AE24: 14d0     beq      #0x800ae50 */
    if (r0 == 0x19) goto L_0800AE50;
    /* 0x0800AE26: 1e28     cmp      r0, #0x1e */
    /* flags = cmp(r0, #0x1e) */
    /* 0x0800AE28: 11d1     bne      #0x800ae4e */
    if (r0 != 0x1e) goto L_0800AE4E;
    /* 0x0800AE2A: 11e0     b        #0x800ae50 */
    goto L_0800AE50;

    L_0800AE2C:
    /* 0x0800AE2C: 2828     cmp      r0, #0x28 */
    /* flags = cmp(r0, #0x28) */
    /* 0x0800AE2E: 0fd0     beq      #0x800ae50 */
    if (r0 == 0x28) goto L_0800AE50;
    /* 0x0800AE30: 2d28     cmp      r0, #0x2d */
    /* flags = cmp(r0, #0x2d) */
    /* 0x0800AE32: 0cd1     bne      #0x800ae4e */
    if (r0 != 0x2d) goto L_0800AE4E;
    /* 0x0800AE34: 0ce0     b        #0x800ae50 */
    goto L_0800AE50;

    L_0800AE36:
    /* 0x0800AE36: 5028     cmp      r0, #0x50 */
    /* flags = cmp(r0, #0x50) */
    /* 0x0800AE38: 0ad0     beq      #0x800ae50 */
    if (r0 == 0x50) goto L_0800AE50;
    /* 0x0800AE3A: 04dc     bgt      #0x800ae46 */
    if (r0 > 0x50) goto L_0800AE46;   /* signed */
    /* 0x0800AE3C: 3c28     cmp      r0, #0x3c */
    /* flags = cmp(r0, #0x3c) */
    /* 0x0800AE3E: 07d0     beq      #0x800ae50 */
    if (r0 == 0x3c) goto L_0800AE50;
    /* 0x0800AE40: 4628     cmp      r0, #0x46 */
    /* flags = cmp(r0, #0x46) */
    /* 0x0800AE42: 04d1     bne      #0x800ae4e */
    if (r0 != 0x46) goto L_0800AE4E;
    /* 0x0800AE44: 04e0     b        #0x800ae50 */
    goto L_0800AE50;

    L_0800AE46:
    /* 0x0800AE46: 5a28     cmp      r0, #0x5a */
    /* flags = cmp(r0, #0x5a) */
    /* 0x0800AE48: 02d0     beq      #0x800ae50 */
    if (r0 == 0x5a) goto L_0800AE50;
    /* 0x0800AE4A: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x0800AE4C: 00d0     beq      #0x800ae50 */
    if (r0 == 0x64) goto L_0800AE50;

    L_0800AE4E:
    /* 0x0800AE4E: 1170     strb     r1, [r2] */
    *(uint8_t *)(r2) = r1;   /* store */

    L_0800AE50:
    /* 0x0800AE50: 4349     ldr      r1, [pc, #0x10c]  ; =RAM_200001B1 */
    r1 = *(uint32_t *)0x0800AF60;   /* = RAM 0x200001B1 */
    /* 0x0800AE52: 0878     ldrb     r0, [r1] */
    r0 = *(uint8_t *)(r1);
    /* 0x0800AE54: 1828     cmp      r0, #0x18 */
    /* flags = cmp(r0, #0x18) */
    /* 0x0800AE56: 2ad0     beq      #0x800aeae */
    if (r0 == 0x18) goto L_0800AEAE;
    /* 0x0800AE58: 14dc     bgt      #0x800ae84 */
    if (r0 > 0x18) goto L_0800AE84;   /* signed */
    /* 0x0800AE5A: 0b28     cmp      r0, #0xb */
    /* flags = cmp(r0, #0xb) */
    /* 0x0800AE5C: 27d0     beq      #0x800aeae */
    if (r0 == 0xb) goto L_0800AEAE;
    /* 0x0800AE5E: 08dc     bgt      #0x800ae72 */
    if (r0 > 0xb) goto L_0800AE72;   /* signed */
    /* 0x0800AE60: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800AE62: 24d0     beq      #0x800aeae */
    if (r0 == 0) goto L_0800AEAE;
    /* 0x0800AE64: 0328     cmp      r0, #3 */
    /* flags = cmp(r0, #3) */
    /* 0x0800AE66: 22d0     beq      #0x800aeae */
    if (r0 == 3) goto L_0800AEAE;
    /* 0x0800AE68: 0528     cmp      r0, #5 */
    /* flags = cmp(r0, #5) */
    /* 0x0800AE6A: 20d0     beq      #0x800aeae */
    if (r0 == 5) goto L_0800AEAE;
    /* 0x0800AE6C: 0828     cmp      r0, #8 */
    /* flags = cmp(r0, #8) */
    /* 0x0800AE6E: 1dd1     bne      #0x800aeac */
    if (r0 != 8) goto L_0800AEAC;
    /* 0x0800AE70: 1de0     b        #0x800aeae */
    goto L_0800AEAE;

    L_0800AE72:
    /* 0x0800AE72: 0d28     cmp      r0, #0xd */
    /* flags = cmp(r0, #0xd) */
    /* 0x0800AE74: 1bd0     beq      #0x800aeae */
    if (r0 == 0xd) goto L_0800AEAE;
    /* 0x0800AE76: 1028     cmp      r0, #0x10 */
    /* flags = cmp(r0, #0x10) */
    /* 0x0800AE78: 19d0     beq      #0x800aeae */
    if (r0 == 0x10) goto L_0800AEAE;
    /* 0x0800AE7A: 1328     cmp      r0, #0x13 */
    /* flags = cmp(r0, #0x13) */
    /* 0x0800AE7C: 17d0     beq      #0x800aeae */
    if (r0 == 0x13) goto L_0800AEAE;
    /* 0x0800AE7E: 1528     cmp      r0, #0x15 */
    /* flags = cmp(r0, #0x15) */
    /* 0x0800AE80: 14d1     bne      #0x800aeac */
    if (r0 != 0x15) goto L_0800AEAC;
    /* 0x0800AE82: 14e0     b        #0x800aeae */
    goto L_0800AEAE;

    L_0800AE84:
    /* 0x0800AE84: f328     cmp      r0, #0xf3 */
    /* flags = cmp(r0, #0xf3) */
    /* 0x0800AE86: 12d0     beq      #0x800aeae */
    if (r0 == 0xf3) goto L_0800AEAE;
    /* 0x0800AE88: 08dc     bgt      #0x800ae9c */
    if (r0 > 0xf3) goto L_0800AE9C;   /* signed */
    /* 0x0800AE8A: e828     cmp      r0, #0xe8 */
    /* flags = cmp(r0, #0xe8) */
    /* 0x0800AE8C: 0fd0     beq      #0x800aeae */
    if (r0 == 0xe8) goto L_0800AEAE;
    /* 0x0800AE8E: eb28     cmp      r0, #0xeb */
    /* flags = cmp(r0, #0xeb) */
    /* 0x0800AE90: 0dd0     beq      #0x800aeae */
    if (r0 == 0xeb) goto L_0800AEAE;
    /* 0x0800AE92: ed28     cmp      r0, #0xed */
    /* flags = cmp(r0, #0xed) */
    /* 0x0800AE94: 0bd0     beq      #0x800aeae */
    if (r0 == 0xed) goto L_0800AEAE;
    /* 0x0800AE96: f028     cmp      r0, #0xf0 */
    /* flags = cmp(r0, #0xf0) */
    /* 0x0800AE98: 08d1     bne      #0x800aeac */
    if (r0 != 0xf0) goto L_0800AEAC;
    /* 0x0800AE9A: 08e0     b        #0x800aeae */
    goto L_0800AEAE;

    L_0800AE9C:
    /* 0x0800AE9C: f528     cmp      r0, #0xf5 */
    /* flags = cmp(r0, #0xf5) */
    /* 0x0800AE9E: 06d0     beq      #0x800aeae */
    if (r0 == 0xf5) goto L_0800AEAE;
    /* 0x0800AEA0: f828     cmp      r0, #0xf8 */
    /* flags = cmp(r0, #0xf8) */
    /* 0x0800AEA2: 04d0     beq      #0x800aeae */
    if (r0 == 0xf8) goto L_0800AEAE;
    /* 0x0800AEA4: fb28     cmp      r0, #0xfb */
    /* flags = cmp(r0, #0xfb) */
    /* 0x0800AEA6: 02d0     beq      #0x800aeae */
    if (r0 == 0xfb) goto L_0800AEAE;
    /* 0x0800AEA8: fd28     cmp      r0, #0xfd */
    /* flags = cmp(r0, #0xfd) */
    /* 0x0800AEAA: 00d0     beq      #0x800aeae */
    if (r0 == 0xfd) goto L_0800AEAE;

    L_0800AEAC:
    /* 0x0800AEAC: 0e70     strb     r6, [r1] */
    *(uint8_t *)(r1) = r6;   /* store */

    L_0800AEAE:
    /* 0x0800AEAE: 2d4a     ldr      r2, [pc, #0xb4]  ; =flash_0800DA2E */
    r2 = *(uint32_t *)0x0800AF64;   /* = flash data/const 0x0800DA2E */
    /* 0x0800AEB0: 2d48     ldr      r0, [pc, #0xb4]  ; =RAM_20000480 */
    r0 = *(uint32_t *)0x0800AF68;   /* = RAM 0x20000480 */
    /* 0x0800AEB2: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800AEB4: 3446     mov      r4, r6 */
    r4 = r6;

    L_0800AEB6:
    /* 0x0800AEB6: 435c     ldrb     r3, [r0, r1] */
    r3 = *(uint8_t *)(r0 + r1);
    /* 0x0800AEB8: 565c     ldrb     r6, [r2, r1] */
    r6 = *(uint8_t *)(r2 + r1);
    /* 0x0800AEBA: b342     cmp      r3, r6 */
    /* flags = cmp(r3, r6) */
    /* 0x0800AEBC: 00d9     bls      #0x800aec0 */
    if (r3 <= r6) goto L_0800AEC0;   /* unsigned */
    /* 0x0800AEBE: 4454     strb     r4, [r0, r1] */
    *(uint8_t *)(r0 + r1) = r4;   /* store */

    L_0800AEC0:
    /* 0x0800AEC0: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x0800AEC2: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x0800AEC4: 0829     cmp      r1, #8 */
    /* flags = cmp(r1, #8) */
    /* 0x0800AEC6: f6d3     blo      #0x800aeb6 */
    if (r1 < 8) goto L_0800AEB6;   /* unsigned */
    /* 0x0800AEC8: 0179     ldrb     r1, [r0, #4] */
    r1 = *(uint8_t *)(r0 + 4);
    /* 0x0800AECA: 2848     ldr      r0, [pc, #0xa0]  ; =RAM_2000017C */
    r0 = *(uint32_t *)0x0800AF6C;   /* = RAM 0x2000017C */
    /* 0x0800AECC: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x0800AECE: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x0800AED0: 00d9     bls      #0x800aed4 */
    if (r1 <= 1) goto L_0800AED4;   /* unsigned */
    /* 0x0800AED2: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */

    L_0800AED4:
    /* 0x0800AED4: 2648     ldr      r0, [pc, #0x98]  ; =RAM_20000183 */
    r0 = *(uint32_t *)0x0800AF70;   /* = RAM 0x20000183 */
    /* 0x0800AED6: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x0800AED8: 1f29     cmp      r1, #0x1f */
    /* flags = cmp(r1, #0x1f) */
    /* 0x0800AEDA: 00d9     bls      #0x800aede */
    if (r1 <= 0x1f) goto L_0800AEDE;   /* unsigned */
    /* 0x0800AEDC: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */

    L_0800AEDE:
    /* 0x0800AEDE: 2548     ldr      r0, [pc, #0x94]  ; =RAM_2000017D */
    r0 = *(uint32_t *)0x0800AF74;   /* = RAM 0x2000017D */
    /* 0x0800AEE0: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x0800AEE2: 0529     cmp      r1, #5 */
    /* flags = cmp(r1, #5) */
    /* 0x0800AEE4: 00d9     bls      #0x800aee8 */
    if (r1 <= 5) goto L_0800AEE8;   /* unsigned */
    /* 0x0800AEE6: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */

    L_0800AEE8:
    /* 0x0800AEE8: 2348     ldr      r0, [pc, #0x8c]  ; =RAM_20000231 */
    r0 = *(uint32_t *)0x0800AF78;   /* = RAM 0x20000231 */
    /* 0x0800AEEA: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x0800AEEC: 0429     cmp      r1, #4 */
    /* flags = cmp(r1, #4) */
    /* 0x0800AEEE: 01d9     bls      #0x800aef4 */
    if (r1 <= 4) goto L_0800AEF4;   /* unsigned */
    /* 0x0800AEF0: 0421     movs     r1, #4 */
    r1 = 4;
    /* 0x0800AEF2: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */

    L_0800AEF4:
    /* 0x0800AEF4: 2148     ldr      r0, [pc, #0x84]  ; =RAM_20000169 */
    r0 = *(uint32_t *)0x0800AF7C;   /* = RAM 0x20000169 */
    /* 0x0800AEF6: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x0800AEF8: 0929     cmp      r1, #9 */
    /* flags = cmp(r1, #9) */
    /* 0x0800AEFA: 00d9     bls      #0x800aefe */
    if (r1 <= 9) goto L_0800AEFE;   /* unsigned */
    /* 0x0800AEFC: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */

    L_0800AEFE:
    /* 0x0800AEFE: 2048     ldr      r0, [pc, #0x80]  ; =RAM_2000016A */
    r0 = *(uint32_t *)0x0800AF80;   /* = RAM 0x2000016A */
    /* 0x0800AF00: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x0800AF02: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x0800AF04: 00d9     bls      #0x800af08 */
    if (r1 <= 1) goto L_0800AF08;   /* unsigned */
    /* 0x0800AF06: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */

    L_0800AF08:
    /* 0x0800AF08: 1e48     ldr      r0, [pc, #0x78]  ; =RAM_2000016B */
    r0 = *(uint32_t *)0x0800AF84;   /* = RAM 0x2000016B */
    /* 0x0800AF0A: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x0800AF0C: 2d29     cmp      r1, #0x2d */
    /* flags = cmp(r1, #0x2d) */
    /* 0x0800AF0E: 0fd0     beq      #0x800af30 */
    if (r1 == 0x2d) goto L_0800AF30;
    /* 0x0800AF10: 06dc     bgt      #0x800af20 */
    if (r1 > 0x2d) goto L_0800AF20;   /* signed */
    /* 0x0800AF12: 1e29     cmp      r1, #0x1e */
    /* flags = cmp(r1, #0x1e) */
    /* 0x0800AF14: 0cd0     beq      #0x800af30 */
    if (r1 == 0x1e) goto L_0800AF30;
    /* 0x0800AF16: 2329     cmp      r1, #0x23 */
    /* flags = cmp(r1, #0x23) */
    /* 0x0800AF18: 0ad0     beq      #0x800af30 */
    if (r1 == 0x23) goto L_0800AF30;
    /* 0x0800AF1A: 2829     cmp      r1, #0x28 */
    /* flags = cmp(r1, #0x28) */
    /* 0x0800AF1C: 06d1     bne      #0x800af2c */
    if (r1 != 0x28) goto L_0800AF2C;
    /* 0x0800AF1E: 07e0     b        #0x800af30 */
    goto L_0800AF30;

    L_0800AF20:
    /* 0x0800AF20: 3229     cmp      r1, #0x32 */
    /* flags = cmp(r1, #0x32) */
    /* 0x0800AF22: 05d0     beq      #0x800af30 */
    if (r1 == 0x32) goto L_0800AF30;
    /* 0x0800AF24: 3729     cmp      r1, #0x37 */
    /* flags = cmp(r1, #0x37) */
    /* 0x0800AF26: 03d0     beq      #0x800af30 */
    if (r1 == 0x37) goto L_0800AF30;
    /* 0x0800AF28: 3c29     cmp      r1, #0x3c */
    /* flags = cmp(r1, #0x3c) */
    /* 0x0800AF2A: 01d0     beq      #0x800af30 */
    if (r1 == 0x3c) goto L_0800AF30;

    L_0800AF2C:
    /* 0x0800AF2C: 3721     movs     r1, #0x37 */
    r1 = 0x37;
    /* 0x0800AF2E: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */

    L_0800AF30:
    /* 0x0800AF30: faf764ff bl       #0x8005dfc */
    FUN_08005DFC();   /* BL -> 0x08005DFC FUN_08005DFC */
    /* 0x0800AF34: 2046     mov      r0, r4 */
    r0 = r4;

    L_0800AF36:
    /* 0x0800AF36: 2870     strb     r0, [r5] */
    *(uint8_t *)(r5) = r0;   /* store */
    /* 0x0800AF38: 00f026fb bl       #0x800b588 */
    FUN_0800B588();   /* BL -> 0x0800B588 FUN_0800B588 */
    /* 0x0800AF3C: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
