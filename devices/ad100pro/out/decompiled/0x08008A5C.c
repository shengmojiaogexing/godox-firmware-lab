/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008A5C
 * Name      : FUN_08008A5C
 * End       : 0x08008C66
 * Size      : 522 bytes, 257 instructions decoded in body (functions.json insns=257, blocks=100)
 * Class     : function
 * Callers   : 3  -> 0x080047CC FUN_080047CC, 0x080047EC FUN_080047EC, 0x0800481A FUN_0800481A
 * Callees   : 4  -> 0x080038AC FUN_080038AC, 0x0800B578 FUN_0800B578, 0x0800B758 FUN_0800B758, 0x0800B7F0 FUN_0800B7F0
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08008A5C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08008A5C: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08008A5E: 7d20     movs     r0, #0x7d */
    r0 = 0x7d;
    /* 0x08008A60: 814c     ldr      r4, [pc, #0x204]  ; =RAM_20000164 */
    r4 = *(uint32_t *)0x08008C68;   /* = RAM 0x20000164 */
    /* 0x08008A62: 0001     lsls     r0, r0, #4 */
    r0 = r0 << 4;
    /* 0x08008A64: 6086     strh     r0, [r4, #0x32] */
    *(uint16_t *)(r4 + 50) = r0;   /* store */
    /* 0x08008A66: faf721ff bl       #0x80038ac */
    FUN_080038AC();   /* BL -> 0x080038AC FUN_080038AC */
    /* 0x08008A6A: 02f0c1fe bl       #0x800b7f0 */
    FUN_0800B7F0();   /* BL -> 0x0800B7F0 FUN_0800B7F0 */
    /* 0x08008A6E: 02f083fd bl       #0x800b578 */
    FUN_0800B578();   /* BL -> 0x0800B578 FUN_0800B578 */
    /* 0x08008A72: 207a     ldrb     r0, [r4, #8] */
    r0 = *(uint8_t *)(r4 + 8);
    /* 0x08008A74: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008A76: 07d0     beq      #0x8008a88 */
    if (r0 == 0) goto L_08008A88;
    /* 0x08008A78: 608d     ldrh     r0, [r4, #0x2a] */
    r0 = *(uint16_t *)(r4 + 42);
    /* 0x08008A7A: e121     movs     r1, #0xe1 */
    r1 = 0xe1;
    /* 0x08008A7C: 0901     lsls     r1, r1, #4 */
    r1 = r1 << 4;
    /* 0x08008A7E: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08008A80: 01d2     bhs      #0x8008a86 */
    if (r0 >= r1) goto L_08008A86;   /* unsigned */
    /* 0x08008A82: 0a30     adds     r0, #0xa */
    r0 = r0 + 0xa;
    /* 0x08008A84: 6085     strh     r0, [r4, #0x2a] */
    *(uint16_t *)(r4 + 42) = r0;   /* store */

    L_08008A86:
    /* 0x08008A86: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08008A88:
    /* 0x08008A88: 607d     ldrb     r0, [r4, #0x15] */
    r0 = *(uint8_t *)(r4 + 21);
    /* 0x08008A8A: 0521     movs     r1, #5 */
    r1 = 5;
    /* 0x08008A8C: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08008A8E: 0cd0     beq      #0x8008aaa */
    if (r0 == 1) goto L_08008AAA;
    /* 0x08008A90: e07c     ldrb     r0, [r4, #0x13] */
    r0 = *(uint8_t *)(r4 + 19);
    /* 0x08008A92: 0026     movs     r6, #0 */
    r6 = 0;
    /* 0x08008A94: 3c25     movs     r5, #0x3c */
    r5 = 0x3c;
    /* 0x08008A96: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x08008A98: 71d0     beq      #0x8008b7e */
    if (r0 == 0xa) goto L_08008B7E;
    /* 0x08008A9A: 23dc     bgt      #0x8008ae4 */
    if (r0 > 0xa) goto L_08008AE4;   /* signed */
    /* 0x08008A9C: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008A9E: 26d0     beq      #0x8008aee */
    if (r0 == 0) goto L_08008AEE;
    /* 0x08008AA0: 0328     cmp      r0, #3 */
    /* flags = cmp(r0, #3) */
    /* 0x08008AA2: 63d0     beq      #0x8008b6c */
    if (r0 == 3) goto L_08008B6C;
    /* 0x08008AA4: 0428     cmp      r0, #4 */
    /* flags = cmp(r0, #4) */
    /* 0x08008AA6: 2ad1     bne      #0x8008afe */
    if (r0 != 4) goto L_08008AFE;
    /* 0x08008AA8: 72e0     b        #0x8008b90 */
    goto L_08008B90;

    L_08008AAA:
    /* 0x08008AAA: a07d     ldrb     r0, [r4, #0x16] */
    r0 = *(uint8_t *)(r4 + 22);
    /* 0x08008AAC: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008AAE: 0ed0     beq      #0x8008ace */
    if (r0 == 0) goto L_08008ACE;
    /* 0x08008AB0: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08008AB2: a075     strb     r0, [r4, #0x16] */
    *(uint8_t *)(r4 + 22) = r0;   /* store */
    /* 0x08008AB4: 6d48     ldr      r0, [pc, #0x1b4]  ; =RAM_20000480 */
    r0 = *(uint32_t *)0x08008C6C;   /* = RAM 0x20000480 */
    /* 0x08008AB6: e17d     ldrb     r1, [r4, #0x17] */
    r1 = *(uint8_t *)(r4 + 23);
    /* 0x08008AB8: 425c     ldrb     r2, [r0, r1] */
    r2 = *(uint8_t *)(r0 + r1);
    /* 0x08008ABA: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x08008ABC: 0fd0     beq      #0x8008ade */
    if (r2 == 0) goto L_08008ADE;
    /* 0x08008ABE: 521e     subs     r2, r2, #1 */
    r2 = r2 - 1;

    L_08008AC0:
    /* 0x08008AC0: 4254     strb     r2, [r0, r1] */
    *(uint8_t *)(r0 + r1) = r2;   /* store */
    /* 0x08008AC2: 0529     cmp      r1, #5 */
    /* flags = cmp(r1, #5) */
    /* 0x08008AC4: dfd1     bne      #0x8008a86 */
    if (r1 != 5) goto L_08008A86;
    /* 0x08008AC6: 4079     ldrb     r0, [r0, #5] */
    r0 = *(uint8_t *)(r0 + 5);
    /* 0x08008AC8: 02f046fe bl       #0x800b758 */
    FUN_0800B758();   /* BL -> 0x0800B758 FUN_0800B758 */
    /* 0x08008ACC: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08008ACE:
    /* 0x08008ACE: e07d     ldrb     r0, [r4, #0x17] */
    r0 = *(uint8_t *)(r4 + 23);
    /* 0x08008AD0: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008AD2: 02d0     beq      #0x8008ada */
    if (r0 == 0) goto L_08008ADA;
    /* 0x08008AD4: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x08008AD6: e075     strb     r0, [r4, #0x17] */
    *(uint8_t *)(r4 + 23) = r0;   /* store */
    /* 0x08008AD8: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08008ADA:
    /* 0x08008ADA: e175     strb     r1, [r4, #0x17] */
    *(uint8_t *)(r4 + 23) = r1;   /* store */

    L_08008ADC:
    /* 0x08008ADC: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08008ADE:
    /* 0x08008ADE: 644a     ldr      r2, [pc, #0x190]  ; =flash_0800DA2E */
    r2 = *(uint32_t *)0x08008C70;   /* = flash data/const 0x0800DA2E */
    /* 0x08008AE0: 525c     ldrb     r2, [r2, r1] */
    r2 = *(uint8_t *)(r2 + r1);
    /* 0x08008AE2: ede7     b        #0x8008ac0 */
    goto L_08008AC0;   /* loop */

    L_08008AE4:
    /* 0x08008AE4: 0b28     cmp      r0, #0xb */
    /* flags = cmp(r0, #0xb) */
    /* 0x08008AE6: 6cd0     beq      #0x8008bc2 */
    if (r0 == 0xb) goto L_08008BC2;
    /* 0x08008AE8: 0c28     cmp      r0, #0xc */
    /* flags = cmp(r0, #0xc) */
    /* 0x08008AEA: 7ad1     bne      #0x8008be2 */
    if (r0 != 0xc) goto L_08008BE2;
    /* 0x08008AEC: 72e0     b        #0x8008bd4 */
    goto L_08008BD4;

    L_08008AEE:
    /* 0x08008AEE: 607c     ldrb     r0, [r4, #0x11] */
    r0 = *(uint8_t *)(r4 + 17);
    /* 0x08008AF0: 5023     movs     r3, #0x50 */
    r3 = 0x50;
    /* 0x08008AF2: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08008AF4: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x08008AF6: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08008AF8: 02d0     beq      #0x8008b00 */
    if (r0 == 1) goto L_08008B00;
    /* 0x08008AFA: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08008AFC: 16d0     beq      #0x8008b2c */
    if (r0 == 2) goto L_08008B2C;

    L_08008AFE:
    /* 0x08008AFE: 70e0     b        #0x8008be2 */
    goto L_08008BE2;

    L_08008B00:
    /* 0x08008B00: 5948     ldr      r0, [pc, #0x164]  ; =RAM_20000164 */
    r0 = *(uint32_t *)0x08008C68;   /* = RAM 0x20000164 */
    /* 0x08008B02: 657e     ldrb     r5, [r4, #0x19] */
    r5 = *(uint8_t *)(r4 + 25);
    /* 0x08008B04: 3830     adds     r0, #0x38 */
    r0 = r0 + 0x38;
    /* 0x08008B06: 425d     ldrb     r2, [r0, r5] */
    r2 = *(uint8_t *)(r0 + r5);
    /* 0x08008B08: ff2a     cmp      r2, #0xff */
    /* flags = cmp(r2, #0xff) */
    /* 0x08008B0A: 07d0     beq      #0x8008b1c */
    if (r2 == 0xff) goto L_08008B1C;
    /* 0x08008B0C: a38d     ldrh     r3, [r4, #0x2c] */
    r3 = *(uint16_t *)(r4 + 44);
    /* 0x08008B0E: 202b     cmp      r3, #0x20 */
    /* flags = cmp(r3, #0x20) */
    /* 0x08008B10: 06d0     beq      #0x8008b20 */
    if (r3 == 0x20) goto L_08008B20;
    /* 0x08008B12: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x08008B14: 65d0     beq      #0x8008be2 */
    if (r2 == 0) goto L_08008BE2;
    /* 0x08008B16: 521e     subs     r2, r2, #1 */
    r2 = r2 - 1;

    L_08008B18:
    /* 0x08008B18: 4255     strb     r2, [r0, r5] */
    *(uint8_t *)(r0 + r5) = r2;   /* store */
    /* 0x08008B1A: 62e0     b        #0x8008be2 */
    goto L_08008BE2;

    L_08008B1C:
    /* 0x08008B1C: 4355     strb     r3, [r0, r5] */
    *(uint8_t *)(r0 + r5) = r3;   /* store */
    /* 0x08008B1E: 60e0     b        #0x8008be2 */
    goto L_08008BE2;

    L_08008B20:
    /* 0x08008B20: 0a2a     cmp      r2, #0xa */
    /* flags = cmp(r2, #0xa) */
    /* 0x08008B22: 01d2     bhs      #0x8008b28 */
    if (r2 >= 0xa) goto L_08008B28;   /* unsigned */
    /* 0x08008B24: 4655     strb     r6, [r0, r5] */
    *(uint8_t *)(r0 + r5) = r6;   /* store */
    /* 0x08008B26: 5ce0     b        #0x8008be2 */
    goto L_08008BE2;

    L_08008B28:
    /* 0x08008B28: 0a3a     subs     r2, #0xa */
    r2 = r2 - 0xa;
    /* 0x08008B2A: f5e7     b        #0x8008b18 */
    goto L_08008B18;   /* loop */

    L_08008B2C:
    /* 0x08008B2C: 4e48     ldr      r0, [pc, #0x138]  ; =RAM_20000164 */
    r0 = *(uint32_t *)0x08008C68;   /* = RAM 0x20000164 */
    /* 0x08008B2E: 627e     ldrb     r2, [r4, #0x19] */
    r2 = *(uint8_t *)(r4 + 25);
    /* 0x08008B30: 3e30     adds     r0, #0x3e */
    r0 = r0 + 0x3e;
    /* 0x08008B32: 865c     ldrb     r6, [r0, r2] */
    r6 = *(uint8_t *)(r0 + r2);
    /* 0x08008B34: 322e     cmp      r6, #0x32 */
    /* flags = cmp(r6, #0x32) */
    /* 0x08008B36: 12d0     beq      #0x8008b5e */
    if (r6 == 0x32) goto L_08008B5E;
    /* 0x08008B38: 07dc     bgt      #0x8008b4a */
    if (r6 > 0x32) goto L_08008B4A;   /* signed */
    /* 0x08008B3A: 1425     movs     r5, #0x14 */
    r5 = 0x14;
    /* 0x08008B3C: 142e     cmp      r6, #0x14 */
    /* flags = cmp(r6, #0x14) */
    /* 0x08008B3E: 13d0     beq      #0x8008b68 */
    if (r6 == 0x14) goto L_08008B68;
    /* 0x08008B40: 1e2e     cmp      r6, #0x1e */
    /* flags = cmp(r6, #0x1e) */
    /* 0x08008B42: 11d0     beq      #0x8008b68 */
    if (r6 == 0x1e) goto L_08008B68;
    /* 0x08008B44: 282e     cmp      r6, #0x28 */
    /* flags = cmp(r6, #0x28) */
    /* 0x08008B46: 0dd1     bne      #0x8008b64 */
    if (r6 != 0x28) goto L_08008B64;
    /* 0x08008B48: 0be0     b        #0x8008b62 */
    goto L_08008B62;

    L_08008B4A:
    /* 0x08008B4A: 3c2e     cmp      r6, #0x3c */
    /* flags = cmp(r6, #0x3c) */
    /* 0x08008B4C: 05d0     beq      #0x8008b5a */
    if (r6 == 0x3c) goto L_08008B5A;
    /* 0x08008B4E: 462e     cmp      r6, #0x46 */
    /* flags = cmp(r6, #0x46) */
    /* 0x08008B50: 0ad0     beq      #0x8008b68 */
    if (r6 == 0x46) goto L_08008B68;
    /* 0x08008B52: 502e     cmp      r6, #0x50 */
    /* flags = cmp(r6, #0x50) */
    /* 0x08008B54: 06d1     bne      #0x8008b64 */
    if (r6 != 0x50) goto L_08008B64;
    /* 0x08008B56: 4623     movs     r3, #0x46 */
    r3 = 0x46;
    /* 0x08008B58: 04e0     b        #0x8008b64 */
    goto L_08008B64;

    L_08008B5A:
    /* 0x08008B5A: 3223     movs     r3, #0x32 */
    r3 = 0x32;
    /* 0x08008B5C: 02e0     b        #0x8008b64 */
    goto L_08008B64;

    L_08008B5E:
    /* 0x08008B5E: 2823     movs     r3, #0x28 */
    r3 = 0x28;
    /* 0x08008B60: 00e0     b        #0x8008b64 */
    goto L_08008B64;

    L_08008B62:
    /* 0x08008B62: 1e23     movs     r3, #0x1e */
    r3 = 0x1e;

    L_08008B64:
    /* 0x08008B64: 8354     strb     r3, [r0, r2] */
    *(uint8_t *)(r0 + r2) = r3;   /* store */
    /* 0x08008B66: 3ce0     b        #0x8008be2 */
    goto L_08008BE2;

    L_08008B68:
    /* 0x08008B68: 8554     strb     r5, [r0, r2] */
    *(uint8_t *)(r0 + r2) = r5;   /* store */
    /* 0x08008B6A: 3ae0     b        #0x8008be2 */
    goto L_08008BE2;

    L_08008B6C:
    /* 0x08008B6C: a078     ldrb     r0, [r4, #2] */
    r0 = *(uint8_t *)(r4 + 2);
    /* 0x08008B6E: 1428     cmp      r0, #0x14 */
    /* flags = cmp(r0, #0x14) */
    /* 0x08008B70: 01d2     bhs      #0x8008b76 */
    if (r0 >= 0x14) goto L_08008B76;   /* unsigned */
    /* 0x08008B72: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08008B74: 05e0     b        #0x8008b82 */
    goto L_08008B82;

    L_08008B76:
    /* 0x08008B76: 3228     cmp      r0, #0x32 */
    /* flags = cmp(r0, #0x32) */
    /* 0x08008B78: 02d2     bhs      #0x8008b80 */
    if (r0 >= 0x32) goto L_08008B80;   /* unsigned */
    /* 0x08008B7A: 401d     adds     r0, r0, #5 */
    r0 = r0 + 5;
    /* 0x08008B7C: 01e0     b        #0x8008b82 */
    goto L_08008B82;

    L_08008B7E:
    /* 0x08008B7E: 18e0     b        #0x8008bb2 */
    goto L_08008BB2;

    L_08008B80:
    /* 0x08008B80: 0a30     adds     r0, #0xa */
    r0 = r0 + 0xa;

    L_08008B82:
    /* 0x08008B82: a070     strb     r0, [r4, #2] */
    *(uint8_t *)(r4 + 2) = r0;   /* store */
    /* 0x08008B84: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08008B86: 6328     cmp      r0, #0x63 */
    /* flags = cmp(r0, #0x63) */
    /* 0x08008B88: 2bd3     blo      #0x8008be2 */
    if (r0 < 0x63) goto L_08008BE2;   /* unsigned */
    /* 0x08008B8A: 6320     movs     r0, #0x63 */
    r0 = 0x63;
    /* 0x08008B8C: a070     strb     r0, [r4, #2] */
    *(uint8_t *)(r4 + 2) = r0;   /* store */
    /* 0x08008B8E: 28e0     b        #0x8008be2 */
    goto L_08008BE2;

    L_08008B90:
    /* 0x08008B90: 6078     ldrb     r0, [r4, #1] */
    r0 = *(uint8_t *)(r4 + 1);
    /* 0x08008B92: 1428     cmp      r0, #0x14 */
    /* flags = cmp(r0, #0x14) */
    /* 0x08008B94: 01d2     bhs      #0x8008b9a */
    if (r0 >= 0x14) goto L_08008B9A;   /* unsigned */
    /* 0x08008B96: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08008B98: 04e0     b        #0x8008ba4 */
    goto L_08008BA4;

    L_08008B9A:
    /* 0x08008B9A: 3228     cmp      r0, #0x32 */
    /* flags = cmp(r0, #0x32) */
    /* 0x08008B9C: 01d2     bhs      #0x8008ba2 */
    if (r0 >= 0x32) goto L_08008BA2;   /* unsigned */
    /* 0x08008B9E: 401d     adds     r0, r0, #5 */
    r0 = r0 + 5;
    /* 0x08008BA0: 00e0     b        #0x8008ba4 */
    goto L_08008BA4;

    L_08008BA2:
    /* 0x08008BA2: 0a30     adds     r0, #0xa */
    r0 = r0 + 0xa;

    L_08008BA4:
    /* 0x08008BA4: 6070     strb     r0, [r4, #1] */
    *(uint8_t *)(r4 + 1) = r0;   /* store */
    /* 0x08008BA6: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08008BA8: 5a28     cmp      r0, #0x5a */
    /* flags = cmp(r0, #0x5a) */
    /* 0x08008BAA: 1ad3     blo      #0x8008be2 */
    if (r0 < 0x5a) goto L_08008BE2;   /* unsigned */
    /* 0x08008BAC: 5a20     movs     r0, #0x5a */
    r0 = 0x5a;
    /* 0x08008BAE: 6070     strb     r0, [r4, #1] */
    *(uint8_t *)(r4 + 1) = r0;   /* store */
    /* 0x08008BB0: 17e0     b        #0x8008be2 */
    goto L_08008BE2;

    L_08008BB2:
    /* 0x08008BB2: e07f     ldrb     r0, [r4, #0x1f] */
    r0 = *(uint8_t *)(r4 + 31);
    /* 0x08008BB4: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08008BB6: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08008BB8: e077     strb     r0, [r4, #0x1f] */
    *(uint8_t *)(r4 + 31) = r0;   /* store */
    /* 0x08008BBA: 1f28     cmp      r0, #0x1f */
    /* flags = cmp(r0, #0x1f) */
    /* 0x08008BBC: 11d9     bls      #0x8008be2 */
    if (r0 <= 0x1f) goto L_08008BE2;   /* unsigned */
    /* 0x08008BBE: e677     strb     r6, [r4, #0x1f] */
    *(uint8_t *)(r4 + 31) = r6;   /* store */
    /* 0x08008BC0: 0fe0     b        #0x8008be2 */
    goto L_08008BE2;

    L_08008BC2:
    /* 0x08008BC2: 6079     ldrb     r0, [r4, #5] */
    r0 = *(uint8_t *)(r4 + 5);
    /* 0x08008BC4: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08008BC6: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08008BC8: 6071     strb     r0, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r0;   /* store */
    /* 0x08008BCA: 0928     cmp      r0, #9 */
    /* flags = cmp(r0, #9) */
    /* 0x08008BCC: 09d9     bls      #0x8008be2 */
    if (r0 <= 9) goto L_08008BE2;   /* unsigned */
    /* 0x08008BCE: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x08008BD0: 6071     strb     r0, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r0;   /* store */
    /* 0x08008BD2: 06e0     b        #0x8008be2 */
    goto L_08008BE2;

    L_08008BD4:
    /* 0x08008BD4: e079     ldrb     r0, [r4, #7] */
    r0 = *(uint8_t *)(r4 + 7);
    /* 0x08008BD6: 401d     adds     r0, r0, #5 */
    r0 = r0 + 5;
    /* 0x08008BD8: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08008BDA: e071     strb     r0, [r4, #7] */
    *(uint8_t *)(r4 + 7) = r0;   /* store */
    /* 0x08008BDC: 3c28     cmp      r0, #0x3c */
    /* flags = cmp(r0, #0x3c) */
    /* 0x08008BDE: 00d9     bls      #0x8008be2 */
    if (r0 <= 0x3c) goto L_08008BE2;   /* unsigned */
    /* 0x08008BE0: e571     strb     r5, [r4, #7] */
    *(uint8_t *)(r4 + 7) = r5;   /* store */

    L_08008BE2:
    /* 0x08008BE2: 607c     ldrb     r0, [r4, #0x11] */
    r0 = *(uint8_t *)(r4 + 17);
    /* 0x08008BE4: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x08008BE6: e08d     ldrh     r0, [r4, #0x2e] */
    r0 = *(uint16_t *)(r4 + 46);
    /* 0x08008BE8: 28d4     bmi      #0x8008c3c */
    if (r0 < 0x3c) goto L_08008C3C;   /* result negative */
    /* 0x08008BEA: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x08008BEC: 01d2     bhs      #0x8008bf2 */
    if (r0 >= 0x64) goto L_08008BF2;   /* unsigned */
    /* 0x08008BEE: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08008BF0: 15e0     b        #0x8008c1e */
    goto L_08008C1E;

    L_08008BF2:
    /* 0x08008BF2: c828     cmp      r0, #0xc8 */
    /* flags = cmp(r0, #0xc8) */
    /* 0x08008BF4: 01d2     bhs      #0x8008bfa */
    if (r0 >= 0xc8) goto L_08008BFA;   /* unsigned */
    /* 0x08008BF6: 801c     adds     r0, r0, #2 */
    r0 = r0 + 2;
    /* 0x08008BF8: 11e0     b        #0x8008c1e */
    goto L_08008C1E;

    L_08008BFA:
    /* 0x08008BFA: ff21     movs     r1, #0xff */
    r1 = 0xff;
    /* 0x08008BFC: f531     adds     r1, #0xf5 */
    r1 = r1 + 0xf5;
    /* 0x08008BFE: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08008C00: 01d2     bhs      #0x8008c06 */
    if (r0 >= r1) goto L_08008C06;   /* unsigned */
    /* 0x08008C02: 401d     adds     r0, r0, #5 */
    r0 = r0 + 5;
    /* 0x08008C04: 0be0     b        #0x8008c1e */
    goto L_08008C1E;

    L_08008C06:
    /* 0x08008C06: 7d21     movs     r1, #0x7d */
    r1 = 0x7d;
    /* 0x08008C08: c900     lsls     r1, r1, #3 */
    r1 = r1 << 3;
    /* 0x08008C0A: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08008C0C: 01d2     bhs      #0x8008c12 */
    if (r0 >= r1) goto L_08008C12;   /* unsigned */
    /* 0x08008C0E: 0a30     adds     r0, #0xa */
    r0 = r0 + 0xa;
    /* 0x08008C10: 05e0     b        #0x8008c1e */
    goto L_08008C1E;

    L_08008C12:
    /* 0x08008C12: 1849     ldr      r1, [pc, #0x60]  ; =0x00000BB8 */
    r1 = *(uint32_t *)0x08008C74;   /* = 0x00000BB8 */
    /* 0x08008C14: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08008C16: 01d2     bhs      #0x8008c1c */
    if (r0 >= r1) goto L_08008C1C;   /* unsigned */
    /* 0x08008C18: 3230     adds     r0, #0x32 */
    r0 = r0 + 0x32;
    /* 0x08008C1A: 00e0     b        #0x8008c1e */
    goto L_08008C1E;

    L_08008C1C:
    /* 0x08008C1C: 6430     adds     r0, #0x64 */
    r0 = r0 + 0x64;

    L_08008C1E:
    /* 0x08008C1E: e085     strh     r0, [r4, #0x2e] */
    *(uint16_t *)(r4 + 46) = r0;   /* store */
    /* 0x08008C20: a08d     ldrh     r0, [r4, #0x2c] */
    r0 = *(uint16_t *)(r4 + 44);
    /* 0x08008C22: 2028     cmp      r0, #0x20 */
    /* flags = cmp(r0, #0x20) */
    /* 0x08008C24: 02d1     bne      #0x8008c2c */
    if (r0 != 0x20) goto L_08008C2C;
    /* 0x08008C26: e08d     ldrh     r0, [r4, #0x2e] */
    r0 = *(uint16_t *)(r4 + 46);
    /* 0x08008C28: 6430     adds     r0, #0x64 */
    r0 = r0 + 0x64;
    /* 0x08008C2A: e085     strh     r0, [r4, #0x2e] */
    *(uint16_t *)(r4 + 46) = r0;   /* store */

    L_08008C2C:
    /* 0x08008C2C: e18d     ldrh     r1, [r4, #0x2e] */
    r1 = *(uint16_t *)(r4 + 46);
    /* 0x08008C2E: af20     movs     r0, #0xaf */
    r0 = 0xaf;
    /* 0x08008C30: 4001     lsls     r0, r0, #5 */
    r0 = r0 << 5;
    /* 0x08008C32: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x08008C34: 00d8     bhi      #0x8008c38 */
    if (r1 > r0) goto L_08008C38;   /* unsigned */
    /* 0x08008C36: 51e7     b        #0x8008adc */
    goto L_08008ADC;   /* loop */

    L_08008C38:
    /* 0x08008C38: e085     strh     r0, [r4, #0x2e] */
    *(uint16_t *)(r4 + 46) = r0;   /* store */

    L_08008C3A:
    /* 0x08008C3A: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08008C3C:
    /* 0x08008C3C: 0528     cmp      r0, #5 */
    /* flags = cmp(r0, #5) */
    /* 0x08008C3E: 10d3     blo      #0x8008c62 */
    if (r0 < 5) goto L_08008C62;   /* unsigned */
    /* 0x08008C40: ff21     movs     r1, #0xff */
    r1 = 0xff;
    /* 0x08008C42: 2d31     adds     r1, #0x2d */
    r1 = r1 + 0x2d;
    /* 0x08008C44: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x08008C46: 01d2     bhs      #0x8008c4c */
    if (r0 >= 0x64) goto L_08008C4C;   /* unsigned */
    /* 0x08008C48: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08008C4A: 06e0     b        #0x8008c5a */
    goto L_08008C5A;

    L_08008C4C:
    /* 0x08008C4C: c828     cmp      r0, #0xc8 */
    /* flags = cmp(r0, #0xc8) */
    /* 0x08008C4E: 01d2     bhs      #0x8008c54 */
    if (r0 >= 0xc8) goto L_08008C54;   /* unsigned */
    /* 0x08008C50: 801c     adds     r0, r0, #2 */
    r0 = r0 + 2;
    /* 0x08008C52: 02e0     b        #0x8008c5a */
    goto L_08008C5A;

    L_08008C54:
    /* 0x08008C54: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08008C56: 01d2     bhs      #0x8008c5c */
    if (r0 >= r1) goto L_08008C5C;   /* unsigned */
    /* 0x08008C58: c01c     adds     r0, r0, #3 */
    r0 = r0 + 3;

    L_08008C5A:
    /* 0x08008C5A: e085     strh     r0, [r4, #0x2e] */
    *(uint16_t *)(r4 + 46) = r0;   /* store */

    L_08008C5C:
    /* 0x08008C5C: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x08008C5E: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08008C60: ebd3     blo      #0x8008c3a */
    if (r0 < r1) goto L_08008C3A;   /* unsigned */

    L_08008C62:
    /* 0x08008C62: e185     strh     r1, [r4, #0x2e] */
    *(uint16_t *)(r4 + 46) = r1;   /* store */
    /* 0x08008C64: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
