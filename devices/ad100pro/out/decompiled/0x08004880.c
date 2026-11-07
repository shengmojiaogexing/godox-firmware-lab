/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004880
 * Name      : FUN_08004880
 * End       : 0x08004A92
 * Size      : 530 bytes, 260 instructions decoded in body (functions.json insns=260, blocks=108)
 * Class     : function
 * Callers   : 3  -> 0x080047CC FUN_080047CC, 0x0800481A FUN_0800481A, 0x08004836 FUN_08004836
 * Callees   : 4  -> 0x080038AC FUN_080038AC, 0x0800B578 FUN_0800B578, 0x0800B758 FUN_0800B758, 0x0800B7F0 FUN_0800B7F0
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004880(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004880: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08004882: 7d20     movs     r0, #0x7d */
    r0 = 0x7d;
    /* 0x08004884: 834c     ldr      r4, [pc, #0x20c]  ; =RAM_20000164 */
    r4 = *(uint32_t *)0x08004A94;   /* = RAM 0x20000164 */
    /* 0x08004886: 0001     lsls     r0, r0, #4 */
    r0 = r0 << 4;
    /* 0x08004888: 6086     strh     r0, [r4, #0x32] */
    *(uint16_t *)(r4 + 50) = r0;   /* store */
    /* 0x0800488A: fff70ff8 bl       #0x80038ac */
    FUN_080038AC();   /* BL -> 0x080038AC FUN_080038AC */
    /* 0x0800488E: 06f0afff bl       #0x800b7f0 */
    FUN_0800B7F0();   /* BL -> 0x0800B7F0 FUN_0800B7F0 */
    /* 0x08004892: 06f071fe bl       #0x800b578 */
    FUN_0800B578();   /* BL -> 0x0800B578 FUN_0800B578 */
    /* 0x08004896: 8049     ldr      r1, [pc, #0x200]  ; =RAM_200001E3 */
    r1 = *(uint32_t *)0x08004A98;   /* = RAM 0x200001E3 */
    /* 0x08004898: 0878     ldrb     r0, [r1] */
    r0 = *(uint8_t *)(r1);
    /* 0x0800489A: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800489C: 02d0     beq      #0x80048a4 */
    if (r0 == 0) goto L_080048A4;
    /* 0x0800489E: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x080048A0: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */

    L_080048A2:
    /* 0x080048A2: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_080048A4:
    /* 0x080048A4: 207a     ldrb     r0, [r4, #8] */
    r0 = *(uint8_t *)(r4 + 8);
    /* 0x080048A6: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080048A8: 05d0     beq      #0x80048b6 */
    if (r0 == 0) goto L_080048B6;
    /* 0x080048AA: 608d     ldrh     r0, [r4, #0x2a] */
    r0 = *(uint16_t *)(r4 + 42);
    /* 0x080048AC: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x080048AE: f8d9     bls      #0x80048a2 */
    if (r0 <= 0xa) goto L_080048A2;   /* unsigned */
    /* 0x080048B0: 0a38     subs     r0, #0xa */
    r0 = r0 - 0xa;
    /* 0x080048B2: 6085     strh     r0, [r4, #0x2a] */
    *(uint16_t *)(r4 + 42) = r0;   /* store */

    L_080048B4:
    /* 0x080048B4: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_080048B6:
    /* 0x080048B6: 607d     ldrb     r0, [r4, #0x15] */
    r0 = *(uint8_t *)(r4 + 21);
    /* 0x080048B8: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x080048BA: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x080048BC: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x080048BE: 0cd0     beq      #0x80048da */
    if (r0 == 1) goto L_080048DA;
    /* 0x080048C0: e07c     ldrb     r0, [r4, #0x13] */
    r0 = *(uint8_t *)(r4 + 19);
    /* 0x080048C2: 1e25     movs     r5, #0x1e */
    r5 = 0x1e;
    /* 0x080048C4: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x080048C6: 6fd0     beq      #0x80049a8 */
    if (r0 == 0xa) goto L_080049A8;
    /* 0x080048C8: 24dc     bgt      #0x8004914 */
    if (r0 > 0xa) goto L_08004914;   /* signed */
    /* 0x080048CA: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080048CC: 27d0     beq      #0x800491e */
    if (r0 == 0) goto L_0800491E;
    /* 0x080048CE: 5a22     movs     r2, #0x5a */
    r2 = 0x5a;
    /* 0x080048D0: 0328     cmp      r0, #3 */
    /* flags = cmp(r0, #3) */
    /* 0x080048D2: 60d0     beq      #0x8004996 */
    if (r0 == 3) goto L_08004996;
    /* 0x080048D4: 0428     cmp      r0, #4 */
    /* flags = cmp(r0, #4) */
    /* 0x080048D6: 2ad1     bne      #0x800492e */
    if (r0 != 4) goto L_0800492E;
    /* 0x080048D8: 72e0     b        #0x80049c0 */
    goto L_080049C0;

    L_080048DA:
    /* 0x080048DA: a07d     ldrb     r0, [r4, #0x16] */
    r0 = *(uint8_t *)(r4 + 22);
    /* 0x080048DC: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080048DE: 11d0     beq      #0x8004904 */
    if (r0 == 0) goto L_08004904;
    /* 0x080048E0: a175     strb     r1, [r4, #0x16] */
    *(uint8_t *)(r4 + 22) = r1;   /* store */
    /* 0x080048E2: 6e49     ldr      r1, [pc, #0x1b8]  ; =RAM_20000480 */
    r1 = *(uint32_t *)0x08004A9C;   /* = RAM 0x20000480 */
    /* 0x080048E4: e07d     ldrb     r0, [r4, #0x17] */
    r0 = *(uint8_t *)(r4 + 23);
    /* 0x080048E6: 6e4c     ldr      r4, [pc, #0x1b8]  ; =flash_0800DA2E */
    r4 = *(uint32_t *)0x08004AA0;   /* = flash data/const 0x0800DA2E */
    /* 0x080048E8: 0b5c     ldrb     r3, [r1, r0] */
    r3 = *(uint8_t *)(r1 + r0);
    /* 0x080048EA: 5b1c     adds     r3, r3, #1 */
    r3 = r3 + 1;
    /* 0x080048EC: dbb2     uxtb     r3, r3 */
    r3 = uxtb(r3);
    /* 0x080048EE: 0b54     strb     r3, [r1, r0] */
    *(uint8_t *)(r1 + r0) = r3;   /* store */
    /* 0x080048F0: 245c     ldrb     r4, [r4, r0] */
    r4 = *(uint8_t *)(r4 + r0);
    /* 0x080048F2: a342     cmp      r3, r4 */
    /* flags = cmp(r3, r4) */
    /* 0x080048F4: 00d9     bls      #0x80048f8 */
    if (r3 <= r4) goto L_080048F8;   /* unsigned */
    /* 0x080048F6: 0a54     strb     r2, [r1, r0] */
    *(uint8_t *)(r1 + r0) = r2;   /* store */

    L_080048F8:
    /* 0x080048F8: 0528     cmp      r0, #5 */
    /* flags = cmp(r0, #5) */
    /* 0x080048FA: dbd1     bne      #0x80048b4 */
    if (r0 != 5) goto L_080048B4;
    /* 0x080048FC: 4879     ldrb     r0, [r1, #5] */
    r0 = *(uint8_t *)(r1 + 5);
    /* 0x080048FE: 06f02bff bl       #0x800b758 */
    FUN_0800B758();   /* BL -> 0x0800B758 FUN_0800B758 */

    L_08004902:
    /* 0x08004902: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08004904:
    /* 0x08004904: e07d     ldrb     r0, [r4, #0x17] */
    r0 = *(uint8_t *)(r4 + 23);
    /* 0x08004906: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08004908: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800490A: e075     strb     r0, [r4, #0x17] */
    *(uint8_t *)(r4 + 23) = r0;   /* store */
    /* 0x0800490C: 0528     cmp      r0, #5 */
    /* flags = cmp(r0, #5) */
    /* 0x0800490E: f8d9     bls      #0x8004902 */
    if (r0 <= 5) goto L_08004902;   /* unsigned */
    /* 0x08004910: e275     strb     r2, [r4, #0x17] */
    *(uint8_t *)(r4 + 23) = r2;   /* store */

    L_08004912:
    /* 0x08004912: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08004914:
    /* 0x08004914: 0b28     cmp      r0, #0xb */
    /* flags = cmp(r0, #0xb) */
    /* 0x08004916: 74d0     beq      #0x8004a02 */
    if (r0 == 0xb) goto L_08004A02;
    /* 0x08004918: 0c28     cmp      r0, #0xc */
    /* flags = cmp(r0, #0xc) */
    /* 0x0800491A: 7fd1     bne      #0x8004a1c */
    if (r0 != 0xc) goto L_08004A1C;
    /* 0x0800491C: 77e0     b        #0x8004a0e */
    goto L_08004A0E;

    L_0800491E:
    /* 0x0800491E: 607c     ldrb     r0, [r4, #0x11] */
    r0 = *(uint8_t *)(r4 + 17);
    /* 0x08004920: 5023     movs     r3, #0x50 */
    r3 = 0x50;
    /* 0x08004922: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08004924: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x08004926: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08004928: 02d0     beq      #0x8004930 */
    if (r0 == 1) goto L_08004930;
    /* 0x0800492A: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x0800492C: 16d0     beq      #0x800495c */
    if (r0 == 2) goto L_0800495C;

    L_0800492E:
    /* 0x0800492E: 75e0     b        #0x8004a1c */
    goto L_08004A1C;

    L_08004930:
    /* 0x08004930: 5848     ldr      r0, [pc, #0x160]  ; =RAM_20000164 */
    r0 = *(uint32_t *)0x08004A94;   /* = RAM 0x20000164 */
    /* 0x08004932: 617e     ldrb     r1, [r4, #0x19] */
    r1 = *(uint8_t *)(r4 + 25);
    /* 0x08004934: 3830     adds     r0, #0x38 */
    r0 = r0 + 0x38;
    /* 0x08004936: 425c     ldrb     r2, [r0, r1] */
    r2 = *(uint8_t *)(r0 + r1);
    /* 0x08004938: ff2a     cmp      r2, #0xff */
    /* flags = cmp(r2, #0xff) */
    /* 0x0800493A: 07d0     beq      #0x800494c */
    if (r2 == 0xff) goto L_0800494C;
    /* 0x0800493C: a58d     ldrh     r5, [r4, #0x2c] */
    r5 = *(uint16_t *)(r4 + 44);
    /* 0x0800493E: 202d     cmp      r5, #0x20 */
    /* flags = cmp(r5, #0x20) */
    /* 0x08004940: 06d0     beq      #0x8004950 */
    if (r5 == 0x20) goto L_08004950;
    /* 0x08004942: 502a     cmp      r2, #0x50 */
    /* flags = cmp(r2, #0x50) */
    /* 0x08004944: 6ad2     bhs      #0x8004a1c */
    if (r2 >= 0x50) goto L_08004A1C;   /* unsigned */
    /* 0x08004946: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x08004948: 4254     strb     r2, [r0, r1] */
    *(uint8_t *)(r0 + r1) = r2;   /* store */
    /* 0x0800494A: 67e0     b        #0x8004a1c */
    goto L_08004A1C;

    L_0800494C:
    /* 0x0800494C: 4354     strb     r3, [r0, r1] */
    *(uint8_t *)(r0 + r1) = r3;   /* store */
    /* 0x0800494E: 65e0     b        #0x8004a1c */
    goto L_08004A1C;

    L_08004950:
    /* 0x08004950: 0a32     adds     r2, #0xa */
    r2 = r2 + 0xa;
    /* 0x08004952: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);
    /* 0x08004954: 4254     strb     r2, [r0, r1] */
    *(uint8_t *)(r0 + r1) = r2;   /* store */
    /* 0x08004956: 502a     cmp      r2, #0x50 */
    /* flags = cmp(r2, #0x50) */
    /* 0x08004958: f8d8     bhi      #0x800494c */
    if (r2 > 0x50) goto L_0800494C;   /* unsigned */
    /* 0x0800495A: 5fe0     b        #0x8004a1c */
    goto L_08004A1C;

    L_0800495C:
    /* 0x0800495C: 4d49     ldr      r1, [pc, #0x134]  ; =RAM_20000164 */
    r1 = *(uint32_t *)0x08004A94;   /* = RAM 0x20000164 */
    /* 0x0800495E: 607e     ldrb     r0, [r4, #0x19] */
    r0 = *(uint8_t *)(r4 + 25);
    /* 0x08004960: 3e31     adds     r1, #0x3e */
    r1 = r1 + 0x3e;
    /* 0x08004962: 0a5c     ldrb     r2, [r1, r0] */
    r2 = *(uint8_t *)(r1 + r0);
    /* 0x08004964: 322a     cmp      r2, #0x32 */
    /* flags = cmp(r2, #0x32) */
    /* 0x08004966: 0dd0     beq      #0x8004984 */
    if (r2 == 0x32) goto L_08004984;
    /* 0x08004968: 06dc     bgt      #0x8004978 */
    if (r2 > 0x32) goto L_08004978;   /* signed */
    /* 0x0800496A: 142a     cmp      r2, #0x14 */
    /* flags = cmp(r2, #0x14) */
    /* 0x0800496C: 11d0     beq      #0x8004992 */
    if (r2 == 0x14) goto L_08004992;
    /* 0x0800496E: 1e2a     cmp      r2, #0x1e */
    /* flags = cmp(r2, #0x1e) */
    /* 0x08004970: 0cd0     beq      #0x800498c */
    if (r2 == 0x1e) goto L_0800498C;
    /* 0x08004972: 282a     cmp      r2, #0x28 */
    /* flags = cmp(r2, #0x28) */
    /* 0x08004974: 02d1     bne      #0x800497c */
    if (r2 != 0x28) goto L_0800497C;
    /* 0x08004976: 07e0     b        #0x8004988 */
    goto L_08004988;

    L_08004978:
    /* 0x08004978: 3c2a     cmp      r2, #0x3c */
    /* flags = cmp(r2, #0x3c) */
    /* 0x0800497A: 01d0     beq      #0x8004980 */
    if (r2 == 0x3c) goto L_08004980;

    L_0800497C:
    /* 0x0800497C: 0b54     strb     r3, [r1, r0] */
    *(uint8_t *)(r1 + r0) = r3;   /* store */
    /* 0x0800497E: 4de0     b        #0x8004a1c */
    goto L_08004A1C;

    L_08004980:
    /* 0x08004980: 4622     movs     r2, #0x46 */
    r2 = 0x46;
    /* 0x08004982: 04e0     b        #0x800498e */
    goto L_0800498E;

    L_08004984:
    /* 0x08004984: 3c22     movs     r2, #0x3c */
    r2 = 0x3c;
    /* 0x08004986: 02e0     b        #0x800498e */
    goto L_0800498E;

    L_08004988:
    /* 0x08004988: 3222     movs     r2, #0x32 */
    r2 = 0x32;
    /* 0x0800498A: 00e0     b        #0x800498e */
    goto L_0800498E;

    L_0800498C:
    /* 0x0800498C: 2822     movs     r2, #0x28 */
    r2 = 0x28;

    L_0800498E:
    /* 0x0800498E: 0a54     strb     r2, [r1, r0] */
    *(uint8_t *)(r1 + r0) = r2;   /* store */
    /* 0x08004990: 44e0     b        #0x8004a1c */
    goto L_08004A1C;

    L_08004992:
    /* 0x08004992: 0d54     strb     r5, [r1, r0] */
    *(uint8_t *)(r1 + r0) = r5;   /* store */
    /* 0x08004994: 42e0     b        #0x8004a1c */
    goto L_08004A1C;

    L_08004996:
    /* 0x08004996: a078     ldrb     r0, [r4, #2] */
    r0 = *(uint8_t *)(r4 + 2);
    /* 0x08004998: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800499A: 01d8     bhi      #0x80049a0 */
    if (r0 > 1) goto L_080049A0;   /* unsigned */
    /* 0x0800499C: a170     strb     r1, [r4, #2] */
    *(uint8_t *)(r4 + 2) = r1;   /* store */
    /* 0x0800499E: 3de0     b        #0x8004a1c */
    goto L_08004A1C;

    L_080049A0:
    /* 0x080049A0: 1428     cmp      r0, #0x14 */
    /* flags = cmp(r0, #0x14) */
    /* 0x080049A2: 02d8     bhi      #0x80049aa */
    if (r0 > 0x14) goto L_080049AA;   /* unsigned */
    /* 0x080049A4: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x080049A6: 07e0     b        #0x80049b8 */
    goto L_080049B8;

    L_080049A8:
    /* 0x080049A8: 1ee0     b        #0x80049e8 */
    goto L_080049E8;

    L_080049AA:
    /* 0x080049AA: 3228     cmp      r0, #0x32 */
    /* flags = cmp(r0, #0x32) */
    /* 0x080049AC: 01d8     bhi      #0x80049b2 */
    if (r0 > 0x32) goto L_080049B2;   /* unsigned */
    /* 0x080049AE: 401f     subs     r0, r0, #5 */
    r0 = r0 - 5;
    /* 0x080049B0: 02e0     b        #0x80049b8 */
    goto L_080049B8;

    L_080049B2:
    /* 0x080049B2: 6328     cmp      r0, #0x63 */
    /* flags = cmp(r0, #0x63) */
    /* 0x080049B4: 02d0     beq      #0x80049bc */
    if (r0 == 0x63) goto L_080049BC;
    /* 0x080049B6: 0a38     subs     r0, #0xa */
    r0 = r0 - 0xa;

    L_080049B8:
    /* 0x080049B8: a070     strb     r0, [r4, #2] */
    *(uint8_t *)(r4 + 2) = r0;   /* store */
    /* 0x080049BA: 2fe0     b        #0x8004a1c */
    goto L_08004A1C;

    L_080049BC:
    /* 0x080049BC: a270     strb     r2, [r4, #2] */
    *(uint8_t *)(r4 + 2) = r2;   /* store */
    /* 0x080049BE: 2de0     b        #0x8004a1c */
    goto L_08004A1C;

    L_080049C0:
    /* 0x080049C0: 6078     ldrb     r0, [r4, #1] */
    r0 = *(uint8_t *)(r4 + 1);
    /* 0x080049C2: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x080049C4: 01d8     bhi      #0x80049ca */
    if (r0 > 1) goto L_080049CA;   /* unsigned */
    /* 0x080049C6: 6170     strb     r1, [r4, #1] */
    *(uint8_t *)(r4 + 1) = r1;   /* store */
    /* 0x080049C8: 28e0     b        #0x8004a1c */
    goto L_08004A1C;

    L_080049CA:
    /* 0x080049CA: 1428     cmp      r0, #0x14 */
    /* flags = cmp(r0, #0x14) */
    /* 0x080049CC: 01d8     bhi      #0x80049d2 */
    if (r0 > 0x14) goto L_080049D2;   /* unsigned */
    /* 0x080049CE: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x080049D0: 06e0     b        #0x80049e0 */
    goto L_080049E0;

    L_080049D2:
    /* 0x080049D2: 3228     cmp      r0, #0x32 */
    /* flags = cmp(r0, #0x32) */
    /* 0x080049D4: 01d8     bhi      #0x80049da */
    if (r0 > 0x32) goto L_080049DA;   /* unsigned */
    /* 0x080049D6: 401f     subs     r0, r0, #5 */
    r0 = r0 - 5;
    /* 0x080049D8: 02e0     b        #0x80049e0 */
    goto L_080049E0;

    L_080049DA:
    /* 0x080049DA: 6328     cmp      r0, #0x63 */
    /* flags = cmp(r0, #0x63) */
    /* 0x080049DC: 02d0     beq      #0x80049e4 */
    if (r0 == 0x63) goto L_080049E4;
    /* 0x080049DE: 0a38     subs     r0, #0xa */
    r0 = r0 - 0xa;

    L_080049E0:
    /* 0x080049E0: 6070     strb     r0, [r4, #1] */
    *(uint8_t *)(r4 + 1) = r0;   /* store */
    /* 0x080049E2: 1be0     b        #0x8004a1c */
    goto L_08004A1C;

    L_080049E4:
    /* 0x080049E4: 6270     strb     r2, [r4, #1] */
    *(uint8_t *)(r4 + 1) = r2;   /* store */
    /* 0x080049E6: 19e0     b        #0x8004a1c */
    goto L_08004A1C;

    L_080049E8:
    /* 0x080049E8: e07f     ldrb     r0, [r4, #0x1f] */
    r0 = *(uint8_t *)(r4 + 31);
    /* 0x080049EA: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080049EC: 04d0     beq      #0x80049f8 */
    if (r0 == 0) goto L_080049F8;
    /* 0x080049EE: 1f28     cmp      r0, #0x1f */
    /* flags = cmp(r0, #0x1f) */
    /* 0x080049F0: 04d9     bls      #0x80049fc */
    if (r0 <= 0x1f) goto L_080049FC;   /* unsigned */
    /* 0x080049F2: e277     strb     r2, [r4, #0x1f] */
    *(uint8_t *)(r4 + 31) = r2;   /* store */
    /* 0x080049F4: 12e0     b        #0x8004a1c */
    goto L_08004A1C;

    L_080049F8:
    /* 0x080049F8: 1f20     movs     r0, #0x1f */
    r0 = 0x1f;
    /* 0x080049FA: 00e0     b        #0x80049fe */
    goto L_080049FE;

    L_080049FC:
    /* 0x080049FC: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;

    L_080049FE:
    /* 0x080049FE: e077     strb     r0, [r4, #0x1f] */
    *(uint8_t *)(r4 + 31) = r0;   /* store */
    /* 0x08004A00: 0ce0     b        #0x8004a1c */
    goto L_08004A1C;

    L_08004A02:
    /* 0x08004A02: 6079     ldrb     r0, [r4, #5] */
    r0 = *(uint8_t *)(r4 + 5);
    /* 0x08004A04: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08004A06: 09d0     beq      #0x8004a1c */
    if (r0 == 0) goto L_08004A1C;
    /* 0x08004A08: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x08004A0A: 6071     strb     r0, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r0;   /* store */
    /* 0x08004A0C: 06e0     b        #0x8004a1c */
    goto L_08004A1C;

    L_08004A0E:
    /* 0x08004A0E: e079     ldrb     r0, [r4, #7] */
    r0 = *(uint8_t *)(r4 + 7);
    /* 0x08004A10: 2328     cmp      r0, #0x23 */
    /* flags = cmp(r0, #0x23) */
    /* 0x08004A12: 01d8     bhi      #0x8004a18 */
    if (r0 > 0x23) goto L_08004A18;   /* unsigned */
    /* 0x08004A14: e571     strb     r5, [r4, #7] */
    *(uint8_t *)(r4 + 7) = r5;   /* store */
    /* 0x08004A16: 01e0     b        #0x8004a1c */
    goto L_08004A1C;

    L_08004A18:
    /* 0x08004A18: 401f     subs     r0, r0, #5 */
    r0 = r0 - 5;
    /* 0x08004A1A: e071     strb     r0, [r4, #7] */
    *(uint8_t *)(r4 + 7) = r0;   /* store */

    L_08004A1C:
    /* 0x08004A1C: 607c     ldrb     r0, [r4, #0x11] */
    r0 = *(uint8_t *)(r4 + 17);
    /* 0x08004A1E: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x08004A20: e08d     ldrh     r0, [r4, #0x2e] */
    r0 = *(uint16_t *)(r4 + 46);
    /* 0x08004A22: 27d4     bmi      #0x8004a74 */
    if (r0 < 0x23) goto L_08004A74;   /* result negative */
    /* 0x08004A24: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x08004A26: 01d8     bhi      #0x8004a2c */
    if (r0 > 0xa) goto L_08004A2C;   /* unsigned */
    /* 0x08004A28: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x08004A2A: 19e0     b        #0x8004a60 */
    goto L_08004A60;

    L_08004A2C:
    /* 0x08004A2C: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x08004A2E: 01d2     bhs      #0x8004a34 */
    if (r0 >= 0x64) goto L_08004A34;   /* unsigned */
    /* 0x08004A30: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x08004A32: 15e0     b        #0x8004a60 */
    goto L_08004A60;

    L_08004A34:
    /* 0x08004A34: c828     cmp      r0, #0xc8 */
    /* flags = cmp(r0, #0xc8) */
    /* 0x08004A36: 01d2     bhs      #0x8004a3c */
    if (r0 >= 0xc8) goto L_08004A3C;   /* unsigned */
    /* 0x08004A38: 801e     subs     r0, r0, #2 */
    r0 = r0 - 2;
    /* 0x08004A3A: 11e0     b        #0x8004a60 */
    goto L_08004A60;

    L_08004A3C:
    /* 0x08004A3C: ff21     movs     r1, #0xff */
    r1 = 0xff;
    /* 0x08004A3E: f531     adds     r1, #0xf5 */
    r1 = r1 + 0xf5;
    /* 0x08004A40: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08004A42: 01d2     bhs      #0x8004a48 */
    if (r0 >= r1) goto L_08004A48;   /* unsigned */
    /* 0x08004A44: 401f     subs     r0, r0, #5 */
    r0 = r0 - 5;
    /* 0x08004A46: 0be0     b        #0x8004a60 */
    goto L_08004A60;

    L_08004A48:
    /* 0x08004A48: 7d21     movs     r1, #0x7d */
    r1 = 0x7d;
    /* 0x08004A4A: c900     lsls     r1, r1, #3 */
    r1 = r1 << 3;
    /* 0x08004A4C: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08004A4E: 01d2     bhs      #0x8004a54 */
    if (r0 >= r1) goto L_08004A54;   /* unsigned */
    /* 0x08004A50: 0a38     subs     r0, #0xa */
    r0 = r0 - 0xa;
    /* 0x08004A52: 05e0     b        #0x8004a60 */
    goto L_08004A60;

    L_08004A54:
    /* 0x08004A54: 1349     ldr      r1, [pc, #0x4c]  ; =0x00000BB8 */
    r1 = *(uint32_t *)0x08004AA4;   /* = 0x00000BB8 */
    /* 0x08004A56: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08004A58: 01d2     bhs      #0x8004a5e */
    if (r0 >= r1) goto L_08004A5E;   /* unsigned */
    /* 0x08004A5A: 3238     subs     r0, #0x32 */
    r0 = r0 - 0x32;
    /* 0x08004A5C: 00e0     b        #0x8004a60 */
    goto L_08004A60;

    L_08004A5E:
    /* 0x08004A5E: 6438     subs     r0, #0x64 */
    r0 = r0 - 0x64;

    L_08004A60:
    /* 0x08004A60: e085     strh     r0, [r4, #0x2e] */
    *(uint16_t *)(r4 + 46) = r0;   /* store */
    /* 0x08004A62: a08d     ldrh     r0, [r4, #0x2c] */
    r0 = *(uint16_t *)(r4 + 44);
    /* 0x08004A64: 2028     cmp      r0, #0x20 */
    /* flags = cmp(r0, #0x20) */
    /* 0x08004A66: 00d0     beq      #0x8004a6a */
    if (r0 == 0x20) goto L_08004A6A;

    L_08004A68:
    /* 0x08004A68: 53e7     b        #0x8004912 */
    goto L_08004912;   /* loop */

    L_08004A6A:
    /* 0x08004A6A: e08d     ldrh     r0, [r4, #0x2e] */
    r0 = *(uint16_t *)(r4 + 46);
    /* 0x08004A6C: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x08004A6E: fbd9     bls      #0x8004a68 */
    if (r0 <= 0x64) goto L_08004A68;   /* unsigned */
    /* 0x08004A70: 6438     subs     r0, #0x64 */
    r0 = r0 - 0x64;
    /* 0x08004A72: 02e0     b        #0x8004a7a */
    goto L_08004A7A;

    L_08004A74:
    /* 0x08004A74: 0528     cmp      r0, #5 */
    /* flags = cmp(r0, #5) */
    /* 0x08004A76: 02d8     bhi      #0x8004a7e */
    if (r0 > 5) goto L_08004A7E;   /* unsigned */
    /* 0x08004A78: 0520     movs     r0, #5 */
    r0 = 5;

    L_08004A7A:
    /* 0x08004A7A: e085     strh     r0, [r4, #0x2e] */
    *(uint16_t *)(r4 + 46) = r0;   /* store */
    /* 0x08004A7C: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08004A7E:
    /* 0x08004A7E: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x08004A80: 01d2     bhs      #0x8004a86 */
    if (r0 >= 0x64) goto L_08004A86;   /* unsigned */
    /* 0x08004A82: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x08004A84: f9e7     b        #0x8004a7a */
    goto L_08004A7A;   /* loop */

    L_08004A86:
    /* 0x08004A86: c828     cmp      r0, #0xc8 */
    /* flags = cmp(r0, #0xc8) */
    /* 0x08004A88: 01d2     bhs      #0x8004a8e */
    if (r0 >= 0xc8) goto L_08004A8E;   /* unsigned */
    /* 0x08004A8A: 801e     subs     r0, r0, #2 */
    r0 = r0 - 2;
    /* 0x08004A8C: f5e7     b        #0x8004a7a */
    goto L_08004A7A;   /* loop */

    L_08004A8E:
    /* 0x08004A8E: c01e     subs     r0, r0, #3 */
    r0 = r0 - 3;
    /* 0x08004A90: f3e7     b        #0x8004a7a */
    goto L_08004A7A;   /* loop */
}
