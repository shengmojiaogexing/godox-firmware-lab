/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800AA74
 * Name      : FUN_0800AA74
 * End       : 0x0800AB5C
 * Size      : 232 bytes, 101 instructions decoded in body (functions.json insns=101, blocks=32)
 * Class     : hub
 * Callers   : 1  -> 0x08005B0C IRQ7_Handler
 * Callees   : 9  -> 0x08006584 FUN_08006584, 0x08009BCC FUN_08009BCC, 0x0800AFC4 FUN_0800AFC4, 0x0800B344 FUN_0800B344, 0x0800B7F0 FUN_0800B7F0, 0x0800C6F4 FUN_0800C6F4, 0x0800CC98 FUN_0800CC98, 0x0800CD32 FUN_0800CD32, ... +1
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800AA74(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800AA74: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800AA76: 3948     ldr      r0, [pc, #0xe4]  ; =RAM_20000185 */
    r0 = *(uint32_t *)0x0800AB5C;   /* = RAM 0x20000185 */
    /* 0x0800AA78: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x0800AA7A: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800AA7C: 384d     ldr      r5, [pc, #0xe0]  ; =RAM_200001B2 */
    r5 = *(uint32_t *)0x0800AB60;   /* = RAM 0x200001B2 */
    /* 0x0800AA7E: 394e     ldr      r6, [pc, #0xe4]  ; =RAM_20000214 */
    r6 = *(uint32_t *)0x0800AB64;   /* = RAM 0x20000214 */
    /* 0x0800AA80: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x0800AA82: 05d0     beq      #0x800aa90 */
    if (r0 == 2) goto L_0800AA90;
    /* 0x0800AA84: 0428     cmp      r0, #4 */
    /* flags = cmp(r0, #4) */
    /* 0x0800AA86: 03d0     beq      #0x800aa90 */
    if (r0 == 4) goto L_0800AA90;
    /* 0x0800AA88: 0528     cmp      r0, #5 */
    /* flags = cmp(r0, #5) */
    /* 0x0800AA8A: 01d0     beq      #0x800aa90 */
    if (r0 == 5) goto L_0800AA90;
    /* 0x0800AA8C: 0728     cmp      r0, #7 */
    /* flags = cmp(r0, #7) */
    /* 0x0800AA8E: 02d1     bne      #0x800aa96 */
    if (r0 != 7) goto L_0800AA96;

    L_0800AA90:
    /* 0x0800AA90: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);
    /* 0x0800AA92: 1a28     cmp      r0, #0x1a */
    /* flags = cmp(r0, #0x1a) */
    /* 0x0800AA94: 09d0     beq      #0x800aaaa */
    if (r0 == 0x1a) goto L_0800AAAA;

    L_0800AA96:
    /* 0x0800AA96: 0421     movs     r1, #4 */
    r1 = 4;
    /* 0x0800AA98: 3248     ldr      r0, [pc, #0xc8]  ; =RAM_20000214 */
    r0 = *(uint32_t *)0x0800AB64;   /* = RAM 0x20000214 */
    /* 0x0800AA9A: 02f0fdf8 bl       #0x800cc98 */
    FUN_0800CC98();   /* BL -> 0x0800CC98 FUN_0800CC98 */

    L_0800AA9E:
    /* 0x0800AA9E: 3078     ldrb     r0, [r6] */
    r0 = *(uint8_t *)(r6);
    /* 0x0800AAA0: d528     cmp      r0, #0xd5 */
    /* flags = cmp(r0, #0xd5) */
    /* 0x0800AAA2: 11d0     beq      #0x800aac8 */
    if (r0 == 0xd5) goto L_0800AAC8;
    /* 0x0800AAA4: a928     cmp      r0, #0xa9 */
    /* flags = cmp(r0, #0xa9) */
    /* 0x0800AAA6: 53d0     beq      #0x800ab50 */
    if (r0 == 0xa9) goto L_0800AB50;
    /* 0x0800AAA8: 4be0     b        #0x800ab42 */
    goto L_0800AB42;

    L_0800AAAA:
    /* 0x0800AAAA: 01f023fe bl       #0x800c6f4 */
    FUN_0800C6F4();   /* BL -> 0x0800C6F4 FUN_0800C6F4 */
    /* 0x0800AAAE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800AAB0: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800AAB4: 2c48     ldr      r0, [pc, #0xb0]  ; =RAM_200001DE */
    r0 = *(uint32_t *)0x0800AB68;   /* = RAM 0x200001DE */
    /* 0x0800AAB6: 3470     strb     r4, [r6] */
    *(uint8_t *)(r6) = r4;   /* store */
    /* 0x0800AAB8: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */
    /* 0x0800AABA: 0421     movs     r1, #4 */
    r1 = 4;
    /* 0x0800AABC: 0620     movs     r0, #6 */
    r0 = 6;
    /* 0x0800AABE: 02f038f9 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800AAC2: fbf75ffd bl       #0x8006584 */
    FUN_08006584();   /* BL -> 0x08006584 FUN_08006584 */
    /* 0x0800AAC6: eae7     b        #0x800aa9e */
    goto L_0800AA9E;   /* loop */

    L_0800AAC8:
    /* 0x0800AAC8: 7078     ldrb     r0, [r6, #1] */
    r0 = *(uint8_t *)(r6 + 1);
    /* 0x0800AACA: 2849     ldr      r1, [pc, #0xa0]  ; =RAM_20000480 */
    r1 = *(uint32_t *)0x0800AB6C;   /* = RAM 0x20000480 */
    /* 0x0800AACC: 8028     cmp      r0, #0x80 */
    /* flags = cmp(r0, #0x80) */
    /* 0x0800AACE: 30d0     beq      #0x800ab32 */
    if (r0 == 0x80) goto L_0800AB32;
    /* 0x0800AAD0: 09dc     bgt      #0x800aae6 */
    if (r0 > 0x80) goto L_0800AAE6;   /* signed */
    /* 0x0800AAD2: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x0800AAD4: 0928     cmp      r0, #9 */
    /* flags = cmp(r0, #9) */
    /* 0x0800AAD6: 0fd0     beq      #0x800aaf8 */
    if (r0 == 9) goto L_0800AAF8;
    /* 0x0800AAD8: 1928     cmp      r0, #0x19 */
    /* flags = cmp(r0, #0x19) */
    /* 0x0800AADA: 20d0     beq      #0x800ab1e */
    if (r0 == 0x19) goto L_0800AB1E;
    /* 0x0800AADC: 2128     cmp      r0, #0x21 */
    /* flags = cmp(r0, #0x21) */
    /* 0x0800AADE: 25d0     beq      #0x800ab2c */
    if (r0 == 0x21) goto L_0800AB2C;
    /* 0x0800AAE0: 2928     cmp      r0, #0x29 */
    /* flags = cmp(r0, #0x29) */
    /* 0x0800AAE2: 2cd1     bne      #0x800ab3e */
    if (r0 != 0x29) goto L_0800AB3E;
    /* 0x0800AAE4: 29e0     b        #0x800ab3a */
    goto L_0800AB3A;

    L_0800AAE6:
    /* 0x0800AAE6: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800AAE8: 8128     cmp      r0, #0x81 */
    /* flags = cmp(r0, #0x81) */
    /* 0x0800AAEA: 24d0     beq      #0x800ab36 */
    if (r0 == 0x81) goto L_0800AB36;
    /* 0x0800AAEC: 2049     ldr      r1, [pc, #0x80]  ; =RAM_2000016A */
    r1 = *(uint32_t *)0x0800AB70;   /* = RAM 0x2000016A */
    /* 0x0800AAEE: 8828     cmp      r0, #0x88 */
    /* flags = cmp(r0, #0x88) */
    /* 0x0800AAF0: 1fd0     beq      #0x800ab32 */
    if (r0 == 0x88) goto L_0800AB32;
    /* 0x0800AAF2: 8928     cmp      r0, #0x89 */
    /* flags = cmp(r0, #0x89) */
    /* 0x0800AAF4: 23d1     bne      #0x800ab3e */
    if (r0 != 0x89) goto L_0800AB3E;
    /* 0x0800AAF6: 1ee0     b        #0x800ab36 */
    goto L_0800AB36;

    L_0800AAF8:
    /* 0x0800AAF8: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800AAFA: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800AAFE: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);
    /* 0x0800AB00: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x0800AB02: 2870     strb     r0, [r5] */
    *(uint8_t *)(r5) = r0;   /* store */
    /* 0x0800AB04: 1b48     ldr      r0, [pc, #0x6c]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800AB74;   /* = RAM 0x20000175 */
    /* 0x0800AB06: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800AB08: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x0800AB0A: 02d1     bne      #0x800ab12 */
    if (r0 != 0x89) goto L_0800AB12;
    /* 0x0800AB0C: 1a49     ldr      r1, [pc, #0x68]  ; =RAM_200001C4 */
    r1 = *(uint32_t *)0x0800AB78;   /* = RAM 0x200001C4 */
    /* 0x0800AB0E: 6420     movs     r0, #0x64 */
    r0 = 0x64;
    /* 0x0800AB10: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */

    L_0800AB12:
    /* 0x0800AB12: 01f0effd bl       #0x800c6f4 */
    FUN_0800C6F4();   /* BL -> 0x0800C6F4 FUN_0800C6F4 */
    /* 0x0800AB16: 2c70     strb     r4, [r5] */
    *(uint8_t *)(r5) = r4;   /* store */
    /* 0x0800AB18: 84f31088 msr      primask, r4 */
    /* TODO: msr primask, r4 */
    /* 0x0800AB1C: 0fe0     b        #0x800ab3e */
    goto L_0800AB3E;

    L_0800AB1E:
    /* 0x0800AB1E: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800AB20: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800AB24: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);
    /* 0x0800AB26: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x0800AB28: 2870     strb     r0, [r5] */
    *(uint8_t *)(r5) = r0;   /* store */
    /* 0x0800AB2A: f2e7     b        #0x800ab12 */
    goto L_0800AB12;   /* loop */

    L_0800AB2C:
    /* 0x0800AB2C: fff74ef8 bl       #0x8009bcc */
    FUN_08009BCC();   /* BL -> 0x08009BCC FUN_08009BCC */
    /* 0x0800AB30: 05e0     b        #0x800ab3e */
    goto L_0800AB3E;

    L_0800AB32:
    /* 0x0800AB32: 0c70     strb     r4, [r1] */
    *(uint8_t *)(r1) = r4;   /* store */
    /* 0x0800AB34: 03e0     b        #0x800ab3e */
    goto L_0800AB3E;

    L_0800AB36:
    /* 0x0800AB36: 0a70     strb     r2, [r1] */
    *(uint8_t *)(r1) = r2;   /* store */
    /* 0x0800AB38: 01e0     b        #0x800ab3e */
    goto L_0800AB3E;

    L_0800AB3A:
    /* 0x0800AB3A: 00f003fc bl       #0x800b344 */
    FUN_0800B344();   /* BL -> 0x0800B344 FUN_0800B344 */

    L_0800AB3E:
    /* 0x0800AB3E: 00f057fe bl       #0x800b7f0 */
    FUN_0800B7F0();   /* BL -> 0x0800B7F0 FUN_0800B7F0 */

    L_0800AB42:
    /* 0x0800AB42: 3a20     movs     r0, #0x3a */
    r0 = 0x3a;
    /* 0x0800AB44: 02f056f9 bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800AB48: 3420     movs     r0, #0x34 */
    r0 = 0x34;
    /* 0x0800AB4A: 02f053f9 bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800AB4E: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_0800AB50:
    /* 0x0800AB50: f278     ldrb     r2, [r6, #3] */
    r2 = *(uint8_t *)(r6 + 3);
    /* 0x0800AB52: b178     ldrb     r1, [r6, #2] */
    r1 = *(uint8_t *)(r6 + 2);
    /* 0x0800AB54: 7078     ldrb     r0, [r6, #1] */
    r0 = *(uint8_t *)(r6 + 1);
    /* 0x0800AB56: 00f035fa bl       #0x800afc4 */
    FUN_0800AFC4();   /* BL -> 0x0800AFC4 FUN_0800AFC4 */
    /* 0x0800AB5A: f0e7     b        #0x800ab3e */
    goto L_0800AB3E;   /* loop */
}
