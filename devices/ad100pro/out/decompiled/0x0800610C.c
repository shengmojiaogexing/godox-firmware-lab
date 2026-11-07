/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800610C
 * Name      : FUN_0800610C
 * End       : 0x0800631C
 * Size      : 528 bytes, 243 instructions decoded in body (functions.json insns=243, blocks=47)
 * Class     : hub
 * Callers   : 1  -> 0x0800642C FUN_0800642C
 * Callees   : 9  -> 0x080030F0 FUN_080030F0, 0x0800311C FUN_0800311C, 0x08003168 lookup_or_pad, 0x080032BC FUN_080032BC, 0x080032CC FUN_080032CC, 0x0800339C FUN_0800339C, 0x080033B8 FUN_080033B8, 0x0800341C FUN_0800341C, ... +1
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800610C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800610C: feb5     push     {r1, r2, r3, r4, r5, r6, r7, lr} */
    push({r1, r2, r3, r4, r5, r6, r7, lr});
    /* 0x0800610E: 83a0     adr      r0, #0x20c */
    r0 = 0x00000000;   /* address */
    /* 0x08006110: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);
    /* 0x08006112: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08006114: 8248     ldr      r0, [pc, #0x208]  ; =RAM_20000208 */
    r0 = *(uint32_t *)0x08006320;   /* = RAM 0x20000208 */
    /* 0x08006116: 0126     movs     r6, #1 */
    r6 = 1;
    /* 0x08006118: 0788     ldrh     r7, [r0] */
    r7 = *(uint16_t *)(r0);
    /* 0x0800611A: 8248     ldr      r0, [pc, #0x208]  ; =RAM_200001B6 */
    r0 = *(uint32_t *)0x08006324;   /* = RAM 0x200001B6 */
    /* 0x0800611C: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x0800611E: 0780     strh     r7, [r0] */
    *(uint16_t *)(r0) = r7;   /* store */
    /* 0x08006120: 8148     ldr      r0, [pc, #0x204]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x08006328;   /* = RAM 0x20000175 */
    /* 0x08006122: 824c     ldr      r4, [pc, #0x208]  ; =RAM_2000014A */
    r4 = *(uint32_t *)0x0800632C;   /* = RAM 0x2000014A */
    /* 0x08006124: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08006126: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x08006128: 890f     lsrs     r1, r1, #0x1e */
    r1 = r1 >> 0x1e;
    /* 0x0800612A: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x0800612C: 03d1     bne      #0x8006136 */
    if (r1 != 1) goto L_08006136;
    /* 0x0800612E: 8049     ldr      r1, [pc, #0x200]  ; =RAM_20000480 */
    r1 = *(uint32_t *)0x08006330;   /* = RAM 0x20000480 */
    /* 0x08006130: 8979     ldrb     r1, [r1, #6] */
    r1 = *(uint8_t *)(r1 + 6);
    /* 0x08006132: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x08006134: 0bd0     beq      #0x800614e */
    if (r1 == 1) goto L_0800614E;

    L_08006136:
    /* 0x08006136: ff21     movs     r1, #0xff */
    r1 = 0xff;
    /* 0x08006138: fe31     adds     r1, #0xfe */
    r1 = r1 + 0xfe;
    /* 0x0800613A: 6182     strh     r1, [r4, #0x12] */
    *(uint16_t *)(r4 + 18) = r1;   /* store */
    /* 0x0800613C: a582     strh     r5, [r4, #0x14] */
    *(uint16_t *)(r4 + 20) = r5;   /* store */
    /* 0x0800613E: 0922     movs     r2, #9 */
    r2 = 9;
    /* 0x08006140: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08006142: 0a70     strb     r2, [r1] */
    *(uint8_t *)(r1) = r2;   /* store */
    /* 0x08006144: 0522     movs     r2, #5 */
    r2 = 5;
    /* 0x08006146: 4a70     strb     r2, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r2;   /* store */
    /* 0x08006148: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x0800614A: 31d0     beq      #0x80061b0 */
    if (r1 == 1) goto L_080061B0;
    /* 0x0800614C: 3ce0     b        #0x80061c8 */
    goto L_080061C8;

    L_0800614E:
    /* 0x0800614E: 7a49     ldr      r1, [pc, #0x1e8]  ; =RAM_2000017D */
    r1 = *(uint32_t *)0x08006338;   /* = RAM 0x2000017D */
    /* 0x08006150: 7848     ldr      r0, [pc, #0x1e0]  ; =RAM_2000019C */
    r0 = *(uint32_t *)0x08006334;   /* = RAM 0x2000019C */
    /* 0x08006152: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08006154: 405c     ldrb     r0, [r0, r1] */
    r0 = *(uint8_t *)(r0 + r1);
    /* 0x08006156: ff28     cmp      r0, #0xff */
    /* flags = cmp(r0, #0xff) */
    /* 0x08006158: 19d0     beq      #0x800618e */
    if (r0 == 0xff) goto L_0800618E;
    /* 0x0800615A: 5021     movs     r1, #0x50 */
    r1 = 0x50;
    /* 0x0800615C: 081a     subs     r0, r1, r0 */
    r0 = r1 - r0;
    /* 0x0800615E: 7749     ldr      r1, [pc, #0x1dc]  ; =RAM_200001AA */
    r1 = *(uint32_t *)0x0800633C;   /* = RAM 0x200001AA */
    /* 0x08006160: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08006162: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08006164: 7649     ldr      r1, [pc, #0x1d8]  ; =flash_0800D216 */
    r1 = *(uint32_t *)0x08006340;   /* = flash data/const 0x0800D216 */
    /* 0x08006166: 8000     lsls     r0, r0, #2 */
    r0 = r0 << 2;
    /* 0x08006168: 085a     ldrh     r0, [r1, r0] */
    r0 = *(uint16_t *)(r1 + r0);
    /* 0x0800616A: 2082     strh     r0, [r4, #0x10] */
    *(uint16_t *)(r4 + 16) = r0;   /* store */
    /* 0x0800616C: 7548     ldr      r0, [pc, #0x1d4]  ; =RAM_20000167 */
    r0 = *(uint32_t *)0x08006344;   /* = RAM 0x20000167 */
    /* 0x0800616E: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08006170: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08006172: 02d1     bne      #0x800617a */
    if (r0 != 1) goto L_0800617A;
    /* 0x08006174: 7448     ldr      r0, [pc, #0x1d0]  ; =RAM_2000018C */
    r0 = *(uint32_t *)0x08006348;   /* = RAM 0x2000018C */
    /* 0x08006176: 0088     ldrh     r0, [r0] */
    r0 = *(uint16_t *)(r0);
    /* 0x08006178: 2082     strh     r0, [r4, #0x10] */
    *(uint16_t *)(r4 + 16) = r0;   /* store */

    L_0800617A:
    /* 0x0800617A: 208a     ldrh     r0, [r4, #0x10] */
    r0 = *(uint16_t *)(r4 + 16);
    /* 0x0800617C: fdf70ef9 bl       #0x800339c */
    FUN_0800339C();   /* BL -> 0x0800339C FUN_0800339C */
    /* 0x08006180: 724a     ldr      r2, [pc, #0x1c8]  ; =0x5810624E */
    r2 = *(uint32_t *)0x0800634C;   /* = 0x5810624E */
    /* 0x08006182: 734b     ldr      r3, [pc, #0x1cc]  ; =0x3FF8B439 */
    r3 = *(uint32_t *)0x08006350;   /* = 0x3FF8B439 */
    /* 0x08006184: fdf7a2f8 bl       #0x80032cc */
    FUN_080032CC();   /* BL -> 0x080032CC FUN_080032CC */
    /* 0x08006188: fdf716f9 bl       #0x80033b8 */
    FUN_080033B8();   /* BL -> 0x080033B8 FUN_080033B8 */
    /* 0x0800618C: 6082     strh     r0, [r4, #0x12] */
    *(uint16_t *)(r4 + 18) = r0;   /* store */

    L_0800618E:
    /* 0x0800618E: e078     ldrb     r0, [r4, #3] */
    r0 = *(uint8_t *)(r4 + 3);
    /* 0x08006190: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08006192: 09d1     bne      #0x80061a8 */
    if (r0 != 1) goto L_080061A8;
    /* 0x08006194: 608a     ldrh     r0, [r4, #0x12] */
    r0 = *(uint16_t *)(r4 + 18);
    /* 0x08006196: 8742     cmp      r7, r0 */
    /* flags = cmp(r7, r0) */
    /* 0x08006198: 06d9     bls      #0x80061a8 */
    if (r7 <= r0) goto L_080061A8;   /* unsigned */
    /* 0x0800619A: 381a     subs     r0, r7, r0 */
    r0 = r7 - r0;
    /* 0x0800619C: 0f21     movs     r1, #0xf */
    r1 = 0xf;
    /* 0x0800619E: fcf7bdff bl       #0x800311c */
    FUN_0800311C();   /* BL -> 0x0800311C FUN_0800311C */
    /* 0x080061A2: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x080061A4: a082     strh     r0, [r4, #0x14] */
    *(uint16_t *)(r4 + 20) = r0;   /* store */
    /* 0x080061A6: 00e0     b        #0x80061aa */
    goto L_080061AA;

    L_080061A8:
    /* 0x080061A8: a582     strh     r5, [r4, #0x14] */
    *(uint16_t *)(r4 + 20) = r5;   /* store */

    L_080061AA:
    /* 0x080061AA: 1e20     movs     r0, #0x1e */
    r0 = 0x1e;
    /* 0x080061AC: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x080061AE: 08e0     b        #0x80061c2 */
    goto L_080061C2;

    L_080061B0:
    /* 0x080061B0: 6848     ldr      r0, [pc, #0x1a0]  ; =RAM_20000185 */
    r0 = *(uint32_t *)0x08006354;   /* = RAM 0x20000185 */
    /* 0x080061B2: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x080061B4: 0628     cmp      r0, #6 */
    /* flags = cmp(r0, #6) */
    /* 0x080061B6: 07d1     bne      #0x80061c8 */
    if (r0 != 6) goto L_080061C8;
    /* 0x080061B8: 6748     ldr      r0, [pc, #0x19c]  ; =RAM_2000013D */
    r0 = *(uint32_t *)0x08006358;   /* = RAM 0x2000013D */
    /* 0x080061BA: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x080061BC: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080061BE: 03d1     bne      #0x80061c8 */
    if (r0 != 0) goto L_080061C8;
    /* 0x080061C0: 1e20     movs     r0, #0x1e */
    r0 = 0x1e;

    L_080061C2:
    /* 0x080061C2: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x080061C4: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x080061C6: 4870     strb     r0, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r0;   /* store */

    L_080061C8:
    /* 0x080061C8: 608a     ldrh     r0, [r4, #0x12] */
    r0 = *(uint16_t *)(r4 + 18);
    /* 0x080061CA: fdf7e7f8 bl       #0x800339c */
    FUN_0800339C();   /* BL -> 0x0800339C FUN_0800339C */
    /* 0x080061CE: 634a     ldr      r2, [pc, #0x18c]  ; =0x47AE147B */
    r2 = *(uint32_t *)0x0800635C;   /* = 0x47AE147B */
    /* 0x080061D0: 634b     ldr      r3, [pc, #0x18c]  ; =0x3F947AE1 */
    r3 = *(uint32_t *)0x08006360;   /* = 0x3F947AE1 */
    /* 0x080061D2: 0291     str      r1, [sp, #8] */
    *(uint32_t *)(sp + 8) = r1;   /* store */
    /* 0x080061D4: 0190     str      r0, [sp, #4] */
    *(uint32_t *)(sp + 4) = r0;   /* store */
    /* 0x080061D6: fdf779f8 bl       #0x80032cc */
    FUN_080032CC();   /* BL -> 0x080032CC FUN_080032CC */
    /* 0x080061DA: 029b     ldr      r3, [sp, #8] */
    r3 = *(uint32_t *)(sp + 8);
    /* 0x080061DC: 019a     ldr      r2, [sp, #4] */
    r2 = *(uint32_t *)(sp + 4);
    /* 0x080061DE: fcf7c3ff bl       #0x8003168 */
    lookup_or_pad();   /* BL -> 0x08003168 lookup_or_pad */
    /* 0x080061E2: 0290     str      r0, [sp, #8] */
    *(uint32_t *)(sp + 8) = r0;   /* store */
    /* 0x080061E4: 0191     str      r1, [sp, #4] */
    *(uint32_t *)(sp + 4) = r1;   /* store */
    /* 0x080061E6: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x080061E8: fdf7d8f8 bl       #0x800339c */
    FUN_0800339C();   /* BL -> 0x0800339C FUN_0800339C */
    /* 0x080061EC: 019b     ldr      r3, [sp, #4] */
    r3 = *(uint32_t *)(sp + 4);
    /* 0x080061EE: 029a     ldr      r2, [sp, #8] */
    r2 = *(uint32_t *)(sp + 8);
    /* 0x080061F0: fdf714f9 bl       #0x800341c */
    FUN_0800341C();   /* BL -> 0x0800341C FUN_0800341C */
    /* 0x080061F4: 13d2     bhs      #0x800621e */
    if (r0 >= 0) goto L_0800621E;   /* unsigned */
    /* 0x080061F6: 207a     ldrb     r0, [r4, #8] */
    r0 = *(uint8_t *)(r4 + 8);
    /* 0x080061F8: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x080061FA: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x080061FC: 2072     strb     r0, [r4, #8] */
    *(uint8_t *)(r4 + 8) = r0;   /* store */
    /* 0x080061FE: 0328     cmp      r0, #3 */
    /* flags = cmp(r0, #3) */
    /* 0x08006200: 0cd9     bls      #0x800621c */
    if (r0 <= 3) goto L_0800621C;   /* unsigned */
    /* 0x08006202: 6571     strb     r5, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r5;   /* store */
    /* 0x08006204: a671     strb     r6, [r4, #6] */
    *(uint8_t *)(r4 + 6) = r6;   /* store */
    /* 0x08006206: e573     strb     r5, [r4, #0xf] */
    *(uint8_t *)(r4 + 15) = r5;   /* store */
    /* 0x08006208: fef7ecf9 bl       #0x80045e4 */
    FUN_080045E4();   /* BL -> 0x080045E4 FUN_080045E4 */
    /* 0x0800620C: e079     ldrb     r0, [r4, #7] */
    r0 = *(uint8_t *)(r4 + 7);
    /* 0x0800620E: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08006210: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08006212: e071     strb     r0, [r4, #7] */
    *(uint8_t *)(r4 + 7) = r0;   /* store */
    /* 0x08006214: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x08006216: 7ed9     bls      #0x8006316 */
    if (r0 <= 0xa) goto L_08006316;   /* unsigned */
    /* 0x08006218: e571     strb     r5, [r4, #7] */
    *(uint8_t *)(r4 + 7) = r5;   /* store */
    /* 0x0800621A: e670     strb     r6, [r4, #3] */
    *(uint8_t *)(r4 + 3) = r6;   /* store */

    L_0800621C:
    /* 0x0800621C: 7be0     b        #0x8006316 */
    goto L_08006316;

    L_0800621E:
    /* 0x0800621E: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x08006220: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08006222: 06d0     beq      #0x8006232 */
    if (r0 == 0) goto L_08006232;
    /* 0x08006224: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x08006226: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x08006228: 000e     lsrs     r0, r0, #0x18 */
    r0 = r0 >> 0x18;
    /* 0x0800622A: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800622C: 73d1     bne      #0x8006316 */
    if (r0 != 0) goto L_08006316;
    /* 0x0800622E: e573     strb     r5, [r4, #0xf] */
    *(uint8_t *)(r4 + 15) = r5;   /* store */
    /* 0x08006230: 71e0     b        #0x8006316 */
    goto L_08006316;

    L_08006232:
    /* 0x08006232: 3c48     ldr      r0, [pc, #0xf0]  ; =RAM_200001B6 */
    r0 = *(uint32_t *)0x08006324;   /* = RAM 0x200001B6 */
    /* 0x08006234: 0088     ldrh     r0, [r0] */
    r0 = *(uint16_t *)(r0);
    /* 0x08006236: 0190     str      r0, [sp, #4] */
    *(uint32_t *)(sp + 4) = r0;   /* store */
    /* 0x08006238: 678a     ldrh     r7, [r4, #0x12] */
    r7 = *(uint16_t *)(r4 + 18);
    /* 0x0800623A: b842     cmp      r0, r7 */
    /* flags = cmp(r0, r7) */
    /* 0x0800623C: 0ad9     bls      #0x8006254 */
    if (r0 <= r7) goto L_08006254;   /* unsigned */
    /* 0x0800623E: 207a     ldrb     r0, [r4, #8] */
    r0 = *(uint8_t *)(r4 + 8);
    /* 0x08006240: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08006242: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08006244: 2072     strb     r0, [r4, #8] */
    *(uint8_t *)(r4 + 8) = r0;   /* store */
    /* 0x08006246: 0328     cmp      r0, #3 */
    /* flags = cmp(r0, #3) */
    /* 0x08006248: 65d9     bls      #0x8006316 */
    if (r0 <= 3) goto L_08006316;   /* unsigned */
    /* 0x0800624A: e571     strb     r5, [r4, #7] */
    *(uint8_t *)(r4 + 7) = r5;   /* store */
    /* 0x0800624C: 6571     strb     r5, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r5;   /* store */
    /* 0x0800624E: a671     strb     r6, [r4, #6] */
    *(uint8_t *)(r4 + 6) = r6;   /* store */
    /* 0x08006250: e573     strb     r5, [r4, #0xf] */
    *(uint8_t *)(r4 + 15) = r5;   /* store */
    /* 0x08006252: 0ee0     b        #0x8006272 */
    goto L_08006272;

    L_08006254:
    /* 0x08006254: 6421     movs     r1, #0x64 */
    r1 = 0x64;
    /* 0x08006256: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x08006258: fcf74aff bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x0800625C: 391a     subs     r1, r7, r0 */
    r1 = r7 - r0;
    /* 0x0800625E: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x08006260: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x08006262: 09da     bge      #0x8006278 */
    if (r1 >= r0) goto L_08006278;   /* signed */
    /* 0x08006264: e571     strb     r5, [r4, #7] */
    *(uint8_t *)(r4 + 7) = r5;   /* store */
    /* 0x08006266: e570     strb     r5, [r4, #3] */
    *(uint8_t *)(r4 + 3) = r5;   /* store */
    /* 0x08006268: 6571     strb     r5, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r5;   /* store */
    /* 0x0800626A: a079     ldrb     r0, [r4, #6] */
    r0 = *(uint8_t *)(r4 + 6);
    /* 0x0800626C: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800626E: 00d1     bne      #0x8006272 */
    if (r0 != 0) goto L_08006272;
    /* 0x08006270: e673     strb     r6, [r4, #0xf] */
    *(uint8_t *)(r4 + 15) = r6;   /* store */

    L_08006272:
    /* 0x08006272: fef7b7f9 bl       #0x80045e4 */
    FUN_080045E4();   /* BL -> 0x080045E4 FUN_080045E4 */
    /* 0x08006276: 4ee0     b        #0x8006316 */
    goto L_08006316;

    L_08006278:
    /* 0x08006278: 3221     movs     r1, #0x32 */
    r1 = 0x32;
    /* 0x0800627A: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800627C: fcf738ff bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x08006280: fdf78cf8 bl       #0x800339c */
    FUN_0800339C();   /* BL -> 0x0800339C FUN_0800339C */
    /* 0x08006284: 374a     ldr      r2, [pc, #0xdc]  ; =0xEB851EB9 */
    r2 = *(uint32_t *)0x08006364;   /* = 0xEB851EB9 */
    /* 0x08006286: 384b     ldr      r3, [pc, #0xe0]  ; =0x407FD851 */
    r3 = *(uint32_t *)0x08006368;   /* = 0x407FD851 */
    /* 0x08006288: fdf718f8 bl       #0x80032bc */
    FUN_080032BC();   /* BL -> 0x080032BC FUN_080032BC */
    /* 0x0800628C: 0746     mov      r7, r0 */
    r7 = r0;
    /* 0x0800628E: 0291     str      r1, [sp, #8] */
    *(uint32_t *)(sp + 8) = r1;   /* store */
    /* 0x08006290: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x08006292: fdf783f8 bl       #0x800339c */
    FUN_0800339C();   /* BL -> 0x0800339C FUN_0800339C */
    /* 0x08006296: 3a46     mov      r2, r7 */
    r2 = r7;
    /* 0x08006298: 029b     ldr      r3, [sp, #8] */
    r3 = *(uint32_t *)(sp + 8);
    /* 0x0800629A: fdf7bff8 bl       #0x800341c */
    FUN_0800341C();   /* BL -> 0x0800341C FUN_0800341C */
    /* 0x0800629E: 0bd2     bhs      #0x80062b8 */
    if (r0 >= 0) goto L_080062B8;   /* unsigned */
    /* 0x080062A0: e571     strb     r5, [r4, #7] */
    *(uint8_t *)(r4 + 7) = r5;   /* store */
    /* 0x080062A2: e570     strb     r5, [r4, #3] */
    *(uint8_t *)(r4 + 3) = r5;   /* store */
    /* 0x080062A4: 6571     strb     r5, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r5;   /* store */
    /* 0x080062A6: e673     strb     r6, [r4, #0xf] */
    *(uint8_t *)(r4 + 15) = r6;   /* store */
    /* 0x080062A8: a079     ldrb     r0, [r4, #6] */
    r0 = *(uint8_t *)(r4 + 6);
    /* 0x080062AA: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x080062AC: 01d1     bne      #0x80062b2 */
    if (r0 != 1) goto L_080062B2;
    /* 0x080062AE: 0420     movs     r0, #4 */
    r0 = 4;
    /* 0x080062B0: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */

    L_080062B2:
    /* 0x080062B2: fef797f9 bl       #0x80045e4 */
    FUN_080045E4();   /* BL -> 0x080045E4 FUN_080045E4 */
    /* 0x080062B6: 2de0     b        #0x8006314 */
    goto L_08006314;

    L_080062B8:
    /* 0x080062B8: 6846     mov      r0, sp */
    r0 = sp;
    /* 0x080062BA: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x080062BC: 608a     ldrh     r0, [r4, #0x12] */
    r0 = *(uint16_t *)(r4 + 18);
    /* 0x080062BE: 0190     str      r0, [sp, #4] */
    *(uint32_t *)(sp + 4) = r0;   /* store */
    /* 0x080062C0: 0746     mov      r7, r0 */
    r7 = r0;
    /* 0x080062C2: fcf715ff bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x080062C6: 391a     subs     r1, r7, r0 */
    r1 = r7 - r0;
    /* 0x080062C8: 1648     ldr      r0, [pc, #0x58]  ; =RAM_200001B6 */
    r0 = *(uint32_t *)0x08006324;   /* = RAM 0x200001B6 */
    /* 0x080062CA: 0788     ldrh     r7, [r0] */
    r7 = *(uint16_t *)(r0);
    /* 0x080062CC: b942     cmp      r1, r7 */
    /* flags = cmp(r1, r7) */
    /* 0x080062CE: 11da     bge      #0x80062f4 */
    if (r1 >= r7) goto L_080062F4;   /* signed */
    /* 0x080062D0: 6079     ldrb     r0, [r4, #5] */
    r0 = *(uint8_t *)(r4 + 5);
    /* 0x080062D2: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x080062D4: 06d1     bne      #0x80062e4 */
    if (r0 != 1) goto L_080062E4;
    /* 0x080062D6: 2079     ldrb     r0, [r4, #4] */
    r0 = *(uint8_t *)(r4 + 4);
    /* 0x080062D8: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x080062DA: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x080062DC: 2071     strb     r0, [r4, #4] */
    *(uint8_t *)(r4 + 4) = r0;   /* store */
    /* 0x080062DE: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x080062E0: 03d9     bls      #0x80062ea */
    if (r0 <= 0xa) goto L_080062EA;   /* unsigned */
    /* 0x080062E2: 2571     strb     r5, [r4, #4] */
    *(uint8_t *)(r4 + 4) = r5;   /* store */

    L_080062E4:
    /* 0x080062E4: 6571     strb     r5, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r5;   /* store */
    /* 0x080062E6: fef77df9 bl       #0x80045e4 */
    FUN_080045E4();   /* BL -> 0x080045E4 FUN_080045E4 */

    L_080062EA:
    /* 0x080062EA: a571     strb     r5, [r4, #6] */
    *(uint8_t *)(r4 + 6) = r5;   /* store */
    /* 0x080062EC: e673     strb     r6, [r4, #0xf] */
    *(uint8_t *)(r4 + 15) = r6;   /* store */
    /* 0x080062EE: e570     strb     r5, [r4, #3] */
    *(uint8_t *)(r4 + 3) = r5;   /* store */
    /* 0x080062F0: e571     strb     r5, [r4, #7] */
    *(uint8_t *)(r4 + 7) = r5;   /* store */
    /* 0x080062F2: 0fe0     b        #0x8006314 */
    goto L_08006314;

    L_080062F4:
    /* 0x080062F4: 6846     mov      r0, sp */
    r0 = sp;
    /* 0x080062F6: 4178     ldrb     r1, [r0, #1] */
    r1 = *(uint8_t *)(r0 + 1);
    /* 0x080062F8: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x080062FA: fcf7f9fe bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x080062FE: 0199     ldr      r1, [sp, #4] */
    r1 = *(uint32_t *)(sp + 4);
    /* 0x08006300: 081a     subs     r0, r1, r0 */
    r0 = r1 - r0;
    /* 0x08006302: b842     cmp      r0, r7 */
    /* flags = cmp(r0, r7) */
    /* 0x08006304: 01db     blt      #0x800630a */
    if (r0 < r7) goto L_0800630A;   /* signed */
    /* 0x08006306: 1948     ldr      r0, [pc, #0x64]  ; =RAM_200001B8 */
    r0 = *(uint32_t *)0x0800636C;   /* = RAM 0x200001B8 */
    /* 0x08006308: 0570     strb     r5, [r0] */
    *(uint8_t *)(r0) = r5;   /* store */

    L_0800630A:
    /* 0x0800630A: 6671     strb     r6, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r6;   /* store */
    /* 0x0800630C: a571     strb     r5, [r4, #6] */
    *(uint8_t *)(r4 + 6) = r5;   /* store */
    /* 0x0800630E: e673     strb     r6, [r4, #0xf] */
    *(uint8_t *)(r4 + 15) = r6;   /* store */
    /* 0x08006310: e570     strb     r5, [r4, #3] */
    *(uint8_t *)(r4 + 3) = r5;   /* store */
    /* 0x08006312: 2571     strb     r5, [r4, #4] */
    *(uint8_t *)(r4 + 4) = r5;   /* store */

    L_08006314:
    /* 0x08006314: 2572     strb     r5, [r4, #8] */
    *(uint8_t *)(r4 + 8) = r5;   /* store */

    L_08006316:
    /* 0x08006316: a079     ldrb     r0, [r4, #6] */
    r0 = *(uint8_t *)(r4 + 6);
    /* 0x08006318: 6070     strb     r0, [r4, #1] */
    *(uint8_t *)(r4 + 1) = r0;   /* store */
    /* 0x0800631A: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */
}
