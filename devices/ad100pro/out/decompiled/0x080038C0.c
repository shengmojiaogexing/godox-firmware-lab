/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080038C0
 * Name      : FUN_080038C0
 * End       : 0x08003A82
 * Size      : 450 bytes, 219 instructions decoded in body (functions.json insns=219, blocks=54)
 * Class     : function
 * Callers   : 2  -> 0x0800CF74 main, 0x0800D014 FUN_0800D014
 * Callees   : 5  -> 0x0800339C FUN_0800339C, 0x080033F4 FUN_080033F4, 0x0800341C FUN_0800341C, 0x08003C30 FUN_08003C30, 0x0800741C FUN_0800741C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080038C0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080038C0: feb5     push     {r1, r2, r3, r4, r5, r6, r7, lr} */
    push({r1, r2, r3, r4, r5, r6, r7, lr});
    /* 0x080038C2: 7048     ldr      r0, [pc, #0x1c0]  ; =RAM_200001B2 */
    r0 = *(uint32_t *)0x08003A84;   /* = RAM 0x200001B2 */
    /* 0x080038C4: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x080038C6: 1a28     cmp      r0, #0x1a */
    /* flags = cmp(r0, #0x1a) */
    /* 0x080038C8: 2fd0     beq      #0x800392a */
    if (r0 == 0x1a) goto L_0800392A;
    /* 0x080038CA: 6f48     ldr      r0, [pc, #0x1bc]  ; =RAM_200001BC */
    r0 = *(uint32_t *)0x08003A88;   /* = RAM 0x200001BC */
    /* 0x080038CC: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x080038CE: 1a28     cmp      r0, #0x1a */
    /* flags = cmp(r0, #0x1a) */
    /* 0x080038D0: 2bd0     beq      #0x800392a */
    if (r0 == 0x1a) goto L_0800392A;
    /* 0x080038D2: 6e48     ldr      r0, [pc, #0x1b8]  ; =RAM_200001F2 */
    r0 = *(uint32_t *)0x08003A8C;   /* = RAM 0x200001F2 */
    /* 0x080038D4: 0027     movs     r7, #0 */
    r7 = 0;
    /* 0x080038D6: 0488     ldrh     r4, [r0] */
    r4 = *(uint16_t *)(r0);
    /* 0x080038D8: 6d48     ldr      r0, [pc, #0x1b4]  ; =RAM_200001CD */
    r0 = *(uint32_t *)0x08003A90;   /* = RAM 0x200001CD */
    /* 0x080038DA: 6e4d     ldr      r5, [pc, #0x1b8]  ; =RAM_2000010A */
    r5 = *(uint32_t *)0x08003A94;   /* = RAM 0x2000010A */
    /* 0x080038DC: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x080038DE: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x080038E0: 16d1     bne      #0x8003910 */
    if (r0 != 1) goto L_08003910;
    /* 0x080038E2: 6889     ldrh     r0, [r5, #0xa] */
    r0 = *(uint16_t *)(r5 + 10);
    /* 0x080038E4: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x080038E6: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x080038E8: 6881     strh     r0, [r5, #0xa] */
    *(uint16_t *)(r5 + 10) = r0;   /* store */
    /* 0x080038EA: c828     cmp      r0, #0xc8 */
    /* flags = cmp(r0, #0xc8) */
    /* 0x080038EC: 10d9     bls      #0x8003910 */
    if (r0 <= 0xc8) goto L_08003910;   /* unsigned */
    /* 0x080038EE: 6f81     strh     r7, [r5, #0xa] */
    *(uint16_t *)(r5 + 10) = r7;   /* store */
    /* 0x080038F0: 2889     ldrh     r0, [r5, #8] */
    r0 = *(uint16_t *)(r5 + 8);
    /* 0x080038F2: 694e     ldr      r6, [pc, #0x1a4]  ; =RAM_200001E2 */
    r6 = *(uint32_t *)0x08003A98;   /* = RAM 0x200001E2 */
    /* 0x080038F4: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080038F6: 0ad0     beq      #0x800390e */
    if (r0 == 0) goto L_0800390E;
    /* 0x080038F8: 3222     movs     r2, #0x32 */
    r2 = 0x32;
    /* 0x080038FA: 1421     movs     r1, #0x14 */
    r1 = 0x14;
    /* 0x080038FC: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x080038FE: 00f097f9 bl       #0x8003c30 */
    FUN_08003C30();   /* BL -> 0x08003C30 FUN_08003C30 */
    /* 0x08003902: 2889     ldrh     r0, [r5, #8] */
    r0 = *(uint16_t *)(r5 + 8);
    /* 0x08003904: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x08003906: 2881     strh     r0, [r5, #8] */
    *(uint16_t *)(r5 + 8) = r0;   /* store */
    /* 0x08003908: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800390A: 3070     strb     r0, [r6] */
    *(uint8_t *)(r6) = r0;   /* store */
    /* 0x0800390C: 00e0     b        #0x8003910 */
    goto L_08003910;

    L_0800390E:
    /* 0x0800390E: 3770     strb     r7, [r6] */
    *(uint8_t *)(r6) = r7;   /* store */

    L_08003910:
    /* 0x08003910: 6048     ldr      r0, [pc, #0x180]  ; =RAM_2000010A */
    r0 = *(uint32_t *)0x08003A94;   /* = RAM 0x2000010A */
    /* 0x08003912: 8178     ldrb     r1, [r0, #2] */
    r1 = *(uint8_t *)(r0 + 2);
    /* 0x08003914: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x08003916: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x08003918: 8170     strb     r1, [r0, #2] */
    *(uint8_t *)(r0 + 2) = r1;   /* store */
    /* 0x0800391A: 1929     cmp      r1, #0x19 */
    /* flags = cmp(r1, #0x19) */
    /* 0x0800391C: 76d3     blo      #0x8003a0c */
    if (r1 < 0x19) goto L_08003A0C;   /* unsigned */
    /* 0x0800391E: 0146     mov      r1, r0 */
    r1 = r0;
    /* 0x08003920: 8f70     strb     r7, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r7;   /* store */
    /* 0x08003922: ca78     ldrb     r2, [r1, #3] */
    r2 = *(uint8_t *)(r1 + 3);
    /* 0x08003924: 032a     cmp      r2, #3 */
    /* flags = cmp(r2, #3) */
    /* 0x08003926: 01d9     bls      #0x800392c */
    if (r2 <= 3) goto L_0800392C;   /* unsigned */
    /* 0x08003928: cf70     strb     r7, [r1, #3] */
    *(uint8_t *)(r1 + 3) = r7;   /* store */

    L_0800392A:
    /* 0x0800392A: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */

    L_0800392C:
    /* 0x0800392C: 5300     lsls     r3, r2, #1 */
    r3 = r2 << 1;
    /* 0x0800392E: 5948     ldr      r0, [pc, #0x164]  ; =RAM_2000010A */
    r0 = *(uint32_t *)0x08003A94;   /* = RAM 0x2000010A */
    /* 0x08003930: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x08003932: 0c30     adds     r0, #0xc */
    r0 = r0 + 0xc;
    /* 0x08003934: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);
    /* 0x08003936: c452     strh     r4, [r0, r3] */
    *(uint16_t *)(r0 + r3) = r4;   /* store */
    /* 0x08003938: ca70     strb     r2, [r1, #3] */
    *(uint8_t *)(r1 + 3) = r2;   /* store */
    /* 0x0800393A: 032a     cmp      r2, #3 */
    /* flags = cmp(r2, #3) */
    /* 0x0800393C: 66d9     bls      #0x8003a0c */
    if (r2 <= 3) goto L_08003A0C;   /* unsigned */
    /* 0x0800393E: 5549     ldr      r1, [pc, #0x154]  ; =RAM_2000010A */
    r1 = *(uint32_t *)0x08003A94;   /* = RAM 0x2000010A */
    /* 0x08003940: cf70     strb     r7, [r1, #3] */
    *(uint8_t *)(r1 + 3) = r7;   /* store */
    /* 0x08003942: 5649     ldr      r1, [pc, #0x158]  ; =RAM_200001CC */
    r1 = *(uint32_t *)0x08003A9C;   /* = RAM 0x200001CC */
    /* 0x08003944: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08003946: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x08003948: 02d0     beq      #0x8003950 */
    if (r1 == 0) goto L_08003950;
    /* 0x0800394A: 544a     ldr      r2, [pc, #0x150]  ; =RAM_200001CC */
    r2 = *(uint32_t *)0x08003A9C;   /* = RAM 0x200001CC */
    /* 0x0800394C: 491e     subs     r1, r1, #1 */
    r1 = r1 - 1;
    /* 0x0800394E: 1170     strb     r1, [r2] */
    *(uint8_t *)(r2) = r1;   /* store */

    L_08003950:
    /* 0x08003950: 0022     movs     r2, #0 */
    r2 = 0;

    L_08003952:
    /* 0x08003952: 0021     movs     r1, #0 */
    r1 = 0;

    L_08003954:
    /* 0x08003954: 4c00     lsls     r4, r1, #1 */
    r4 = r1 << 1;
    /* 0x08003956: 2518     adds     r5, r4, r0 */
    r5 = r4 + r0;
    /* 0x08003958: 065b     ldrh     r6, [r0, r4] */
    r6 = *(uint16_t *)(r0 + r4);
    /* 0x0800395A: 6b88     ldrh     r3, [r5, #2] */
    r3 = *(uint16_t *)(r5 + 2);
    /* 0x0800395C: 9e42     cmp      r6, r3 */
    /* flags = cmp(r6, r3) */
    /* 0x0800395E: 01d9     bls      #0x8003964 */
    if (r6 <= r3) goto L_08003964;   /* unsigned */
    /* 0x08003960: 6e80     strh     r6, [r5, #2] */
    *(uint16_t *)(r5 + 2) = r6;   /* store */
    /* 0x08003962: 0353     strh     r3, [r0, r4] */
    *(uint16_t *)(r0 + r4) = r3;   /* store */

    L_08003964:
    /* 0x08003964: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x08003966: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x08003968: 0329     cmp      r1, #3 */
    /* flags = cmp(r1, #3) */
    /* 0x0800396A: f3d3     blo      #0x8003954 */
    if (r1 < 3) goto L_08003954;   /* unsigned */
    /* 0x0800396C: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x0800396E: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);
    /* 0x08003970: 042a     cmp      r2, #4 */
    /* flags = cmp(r2, #4) */
    /* 0x08003972: eed3     blo      #0x8003952 */
    if (r2 < 4) goto L_08003952;   /* unsigned */
    /* 0x08003974: 4188     ldrh     r1, [r0, #2] */
    r1 = *(uint16_t *)(r0 + 2);
    /* 0x08003976: 8088     ldrh     r0, [r0, #4] */
    r0 = *(uint16_t *)(r0 + 4);
    /* 0x08003978: 0818     adds     r0, r1, r0 */
    r0 = r1 + r0;
    /* 0x0800397A: 4408     lsrs     r4, r0, #1 */
    r4 = r0 >> 1;
    /* 0x0800397C: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x0800397E: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08003980: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x08003982: 03f04bfd bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x08003986: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08003988: 67d1     bne      #0x8003a5a */
    if (r0 != 1) goto L_08003A5A;
    /* 0x0800398A: 4448     ldr      r0, [pc, #0x110]  ; =RAM_200001CC */
    r0 = *(uint32_t *)0x08003A9C;   /* = RAM 0x200001CC */
    /* 0x0800398C: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800398E: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08003990: 63d1     bne      #0x8003a5a */
    if (r0 != 0) goto L_08003A5A;
    /* 0x08003992: 4348     ldr      r0, [pc, #0x10c]  ; =RAM_20000235 */
    r0 = *(uint32_t *)0x08003AA0;   /* = RAM 0x20000235 */
    /* 0x08003994: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08003996: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08003998: 5fd1     bne      #0x8003a5a */
    if (r0 != 0) goto L_08003A5A;
    /* 0x0800399A: 4248     ldr      r0, [pc, #0x108]  ; =RAM_200001BB */
    r0 = *(uint32_t *)0x08003AA4;   /* = RAM 0x200001BB */
    /* 0x0800399C: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800399E: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080039A0: 5bd1     bne      #0x8003a5a */
    if (r0 != 0) goto L_08003A5A;
    /* 0x080039A2: 3c4d     ldr      r5, [pc, #0xf0]  ; =RAM_2000010A */
    r5 = *(uint32_t *)0x08003A94;   /* = RAM 0x2000010A */
    /* 0x080039A4: a888     ldrh     r0, [r5, #4] */
    r0 = *(uint16_t *)(r5 + 4);
    /* 0x080039A6: 411d     adds     r1, r0, #5 */
    r1 = r0 + 5;
    /* 0x080039A8: 8c42     cmp      r4, r1 */
    /* flags = cmp(r4, r1) */
    /* 0x080039AA: 56d2     bhs      #0x8003a5a */
    if (r4 >= r1) goto L_08003A5A;   /* unsigned */
    /* 0x080039AC: 3e4e     ldr      r6, [pc, #0xf8]  ; =RAM_200001CE */
    r6 = *(uint32_t *)0x08003AA8;   /* = RAM 0x200001CE */
    /* 0x080039AE: 401f     subs     r0, r0, #5 */
    r0 = r0 - 5;
    /* 0x080039B0: 8442     cmp      r4, r0 */
    /* flags = cmp(r4, r0) */
    /* 0x080039B2: 52dd     ble      #0x8003a5a */
    if (r4 <= r0) goto L_08003A5A;   /* signed */
    /* 0x080039B4: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080039B6: fff7f1fc bl       #0x800339c */
    FUN_0800339C();   /* BL -> 0x0800339C FUN_0800339C */
    /* 0x080039BA: 3c4a     ldr      r2, [pc, #0xf0]  ; =0x66666666 */
    r2 = *(uint32_t *)0x08003AAC;   /* = 0x66666666 */
    /* 0x080039BC: 3c4b     ldr      r3, [pc, #0xf0]  ; =0x407F8666 */
    r3 = *(uint32_t *)0x08003AB0;   /* = 0x407F8666 */
    /* 0x080039BE: 0191     str      r1, [sp, #4] */
    *(uint32_t *)(sp + 4) = r1;   /* store */
    /* 0x080039C0: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x080039C2: fff72bfd bl       #0x800341c */
    FUN_0800341C();   /* BL -> 0x0800341C FUN_0800341C */
    /* 0x080039C6: 00d2     bhs      #0x80039ca */
    if (r4 >= r0) goto L_080039CA;   /* unsigned */
    /* 0x080039C8: 2f70     strb     r7, [r5] */
    *(uint8_t *)(r5) = r7;   /* store */

    L_080039CA:
    /* 0x080039CA: 384a     ldr      r2, [pc, #0xe0]  ; =0x66666666 */
    r2 = *(uint32_t *)0x08003AAC;   /* = 0x66666666 */
    /* 0x080039CC: 384b     ldr      r3, [pc, #0xe0]  ; =0x407F8666 */
    r3 = *(uint32_t *)0x08003AB0;   /* = 0x407F8666 */
    /* 0x080039CE: 0199     ldr      r1, [sp, #4] */
    r1 = *(uint32_t *)(sp + 4);
    /* 0x080039D0: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x080039D2: fff70ffd bl       #0x80033f4 */
    FUN_080033F4();   /* BL -> 0x080033F4 FUN_080033F4 */
    /* 0x080039D6: 0bd2     bhs      #0x80039f0 */
    if (r4 >= r0) goto L_080039F0;   /* unsigned */
    /* 0x080039D8: 3770     strb     r7, [r6] */
    *(uint8_t *)(r6) = r7;   /* store */
    /* 0x080039DA: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);
    /* 0x080039DC: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x080039DE: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x080039E0: 2870     strb     r0, [r5] */
    *(uint8_t *)(r5) = r0;   /* store */
    /* 0x080039E2: 0628     cmp      r0, #6 */
    /* flags = cmp(r0, #6) */
    /* 0x080039E4: 29d9     bls      #0x8003a3a */
    if (r0 <= 6) goto L_08003A3A;   /* unsigned */
    /* 0x080039E6: 2a49     ldr      r1, [pc, #0xa8]  ; =RAM_200001CD */
    r1 = *(uint32_t *)0x08003A90;   /* = RAM 0x200001CD */
    /* 0x080039E8: 2f70     strb     r7, [r5] */
    *(uint8_t *)(r5) = r7;   /* store */
    /* 0x080039EA: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x080039EC: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x080039EE: 24e0     b        #0x8003a3a */
    goto L_08003A3A;

    L_080039F0:
    /* 0x080039F0: a988     ldrh     r1, [r5, #4] */
    r1 = *(uint16_t *)(r5 + 4);
    /* 0x080039F2: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x080039F4: 0322     movs     r2, #3 */
    r2 = 3;
    /* 0x080039F6: 8c42     cmp      r4, r1 */
    /* flags = cmp(r4, r1) */
    /* 0x080039F8: 0cd9     bls      #0x8003a14 */
    if (r4 <= r1) goto L_08003A14;   /* unsigned */
    /* 0x080039FA: 2e49     ldr      r1, [pc, #0xb8]  ; =0x00000217 */
    r1 = *(uint32_t *)0x08003AB4;   /* = 0x00000217 */
    /* 0x080039FC: 8c42     cmp      r4, r1 */
    /* flags = cmp(r4, r1) */
    /* 0x080039FE: 0fd3     blo      #0x8003a20 */
    if (r4 < r1) goto L_08003A20;   /* unsigned */
    /* 0x08003A00: 2c49     ldr      r1, [pc, #0xb0]  ; =0x00000217 */
    r1 = *(uint32_t *)0x08003AB4;   /* = 0x00000217 */
    /* 0x08003A02: 1731     adds     r1, #0x17 */
    r1 = r1 + 0x17;
    /* 0x08003A04: 8c42     cmp      r4, r1 */
    /* flags = cmp(r4, r1) */
    /* 0x08003A06: 02d2     bhs      #0x8003a0e */
    if (r4 >= r1) goto L_08003A0E;   /* unsigned */

    L_08003A08:
    /* 0x08003A08: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08003A0A: 13e0     b        #0x8003a34 */
    goto L_08003A34;

    L_08003A0C:
    /* 0x08003A0C: 27e0     b        #0x8003a5e */
    goto L_08003A5E;

    L_08003A0E:
    /* 0x08003A0E: 2949     ldr      r1, [pc, #0xa4]  ; =0x00000217 */
    r1 = *(uint32_t *)0x08003AB4;   /* = 0x00000217 */
    /* 0x08003A10: 3631     adds     r1, #0x36 */
    r1 = r1 + 0x36;
    /* 0x08003A12: 0de0     b        #0x8003a30 */
    goto L_08003A30;

    L_08003A14:
    /* 0x08003A14: 8c42     cmp      r4, r1 */
    /* flags = cmp(r4, r1) */
    /* 0x08003A16: 10d2     bhs      #0x8003a3a */
    if (r4 >= r1) goto L_08003A3A;   /* unsigned */
    /* 0x08003A18: 2649     ldr      r1, [pc, #0x98]  ; =0x00000217 */
    r1 = *(uint32_t *)0x08003AB4;   /* = 0x00000217 */
    /* 0x08003A1A: 0839     subs     r1, #8 */
    r1 = r1 - 8;
    /* 0x08003A1C: 8c42     cmp      r4, r1 */
    /* flags = cmp(r4, r1) */
    /* 0x08003A1E: 01d2     bhs      #0x8003a24 */
    if (r4 >= r1) goto L_08003A24;   /* unsigned */

    L_08003A20:
    /* 0x08003A20: 3770     strb     r7, [r6] */
    *(uint8_t *)(r6) = r7;   /* store */
    /* 0x08003A22: 0ae0     b        #0x8003a3a */
    goto L_08003A3A;

    L_08003A24:
    /* 0x08003A24: 2349     ldr      r1, [pc, #0x8c]  ; =0x00000217 */
    r1 = *(uint32_t *)0x08003AB4;   /* = 0x00000217 */
    /* 0x08003A26: 0f31     adds     r1, #0xf */
    r1 = r1 + 0xf;
    /* 0x08003A28: 8c42     cmp      r4, r1 */
    /* flags = cmp(r4, r1) */
    /* 0x08003A2A: edd3     blo      #0x8003a08 */
    if (r4 < r1) goto L_08003A08;   /* unsigned */
    /* 0x08003A2C: 2149     ldr      r1, [pc, #0x84]  ; =0x00000217 */
    r1 = *(uint32_t *)0x08003AB4;   /* = 0x00000217 */
    /* 0x08003A2E: 2f31     adds     r1, #0x2f */
    r1 = r1 + 0x2f;

    L_08003A30:
    /* 0x08003A30: 8c42     cmp      r4, r1 */
    /* flags = cmp(r4, r1) */
    /* 0x08003A32: 01d2     bhs      #0x8003a38 */
    if (r4 >= r1) goto L_08003A38;   /* unsigned */

    L_08003A34:
    /* 0x08003A34: 3070     strb     r0, [r6] */
    *(uint8_t *)(r6) = r0;   /* store */
    /* 0x08003A36: 00e0     b        #0x8003a3a */
    goto L_08003A3A;

    L_08003A38:
    /* 0x08003A38: 3270     strb     r2, [r6] */
    *(uint8_t *)(r6) = r2;   /* store */

    L_08003A3A:
    /* 0x08003A3A: 1f4a     ldr      r2, [pc, #0x7c]  ; =0xA3D70A3D */
    r2 = *(uint32_t *)0x08003AB8;   /* = 0xA3D70A3D */
    /* 0x08003A3C: 1f4b     ldr      r3, [pc, #0x7c]  ; =0x40807D70 */
    r3 = *(uint32_t *)0x08003ABC;   /* = 0x40807D70 */
    /* 0x08003A3E: 0199     ldr      r1, [sp, #4] */
    r1 = *(uint32_t *)(sp + 4);
    /* 0x08003A40: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x08003A42: fff7ebfc bl       #0x800341c */
    FUN_0800341C();   /* BL -> 0x0800341C FUN_0800341C */
    /* 0x08003A46: 08d2     bhs      #0x8003a5a */
    if (r4 >= r1) goto L_08003A5A;   /* unsigned */
    /* 0x08003A48: 6878     ldrb     r0, [r5, #1] */
    r0 = *(uint8_t *)(r5 + 1);
    /* 0x08003A4A: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08003A4C: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08003A4E: 6870     strb     r0, [r5, #1] */
    *(uint8_t *)(r5 + 1) = r0;   /* store */
    /* 0x08003A50: 0328     cmp      r0, #3 */
    /* flags = cmp(r0, #3) */
    /* 0x08003A52: 02d3     blo      #0x8003a5a */
    if (r0 < 3) goto L_08003A5A;   /* unsigned */
    /* 0x08003A54: 0e48     ldr      r0, [pc, #0x38]  ; =RAM_200001CD */
    r0 = *(uint32_t *)0x08003A90;   /* = RAM 0x200001CD */
    /* 0x08003A56: 6f70     strb     r7, [r5, #1] */
    *(uint8_t *)(r5 + 1) = r7;   /* store */
    /* 0x08003A58: 0770     strb     r7, [r0] */
    *(uint8_t *)(r0) = r7;   /* store */

    L_08003A5A:
    /* 0x08003A5A: 0e48     ldr      r0, [pc, #0x38]  ; =RAM_2000010A */
    r0 = *(uint32_t *)0x08003A94;   /* = RAM 0x2000010A */
    /* 0x08003A5C: 8480     strh     r4, [r0, #4] */
    *(uint16_t *)(r0 + 4) = r4;   /* store */

    L_08003A5E:
    /* 0x08003A5E: 0c48     ldr      r0, [pc, #0x30]  ; =RAM_200001CD */
    r0 = *(uint32_t *)0x08003A90;   /* = RAM 0x200001CD */
    /* 0x08003A60: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08003A62: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08003A64: 0b48     ldr      r0, [pc, #0x2c]  ; =RAM_2000010A */
    r0 = *(uint32_t *)0x08003A94;   /* = RAM 0x2000010A */
    /* 0x08003A66: 0ad1     bne      #0x8003a7e */
    if (r0 != 1) goto L_08003A7E;
    /* 0x08003A68: c188     ldrh     r1, [r0, #6] */
    r1 = *(uint16_t *)(r0 + 6);
    /* 0x08003A6A: 154a     ldr      r2, [pc, #0x54]  ; =0x00002EE0 */
    r2 = *(uint32_t *)0x08003AC0;   /* = 0x00002EE0 */
    /* 0x08003A6C: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x08003A6E: 89b2     uxth     r1, r1 */
    r1 = uxth(r1);
    /* 0x08003A70: c180     strh     r1, [r0, #6] */
    *(uint16_t *)(r0 + 6) = r1;   /* store */
    /* 0x08003A72: 9142     cmp      r1, r2 */
    /* flags = cmp(r1, r2) */
    /* 0x08003A74: 00d8     bhi      #0x8003a78 */
    if (r1 > r2) goto L_08003A78;   /* unsigned */
    /* 0x08003A76: 58e7     b        #0x800392a */
    goto L_0800392A;   /* loop */

    L_08003A78:
    /* 0x08003A78: 124a     ldr      r2, [pc, #0x48]  ; =RAM_200001D4 */
    r2 = *(uint32_t *)0x08003AC4;   /* = RAM 0x200001D4 */
    /* 0x08003A7A: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08003A7C: 1170     strb     r1, [r2] */
    *(uint8_t *)(r2) = r1;   /* store */

    L_08003A7E:
    /* 0x08003A7E: c780     strh     r7, [r0, #6] */
    *(uint16_t *)(r0 + 6) = r7;   /* store */
    /* 0x08003A80: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */
}
