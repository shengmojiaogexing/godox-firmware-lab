/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08003D2C
 * Name      : FUN_08003D2C
 * End       : 0x08003DF2
 * Size      : 198 bytes, 98 instructions decoded in body (functions.json insns=98, blocks=29)
 * Class     : function
 * Callers   : 2  -> 0x080066C0 FUN_080066C0, 0x08006712 FUN_08006712
 * Callees   : 1  -> 0x08006730 FUN_08006730
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08003D2C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08003D2C: 30b5     push     {r4, r5, lr} */
    push({r4, r5, lr});
    /* 0x08003D2E: 3148     ldr      r0, [pc, #0xc4]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x08003DF4;   /* = RAM 0x20000175 */
    /* 0x08003D30: 314d     ldr      r5, [pc, #0xc4]  ; =RAM_200001D8 */
    r5 = *(uint32_t *)0x08003DF8;   /* = RAM 0x200001D8 */
    /* 0x08003D32: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08003D34: 314c     ldr      r4, [pc, #0xc4]  ; =RAM_200001C8 */
    r4 = *(uint32_t *)0x08003DFC;   /* = RAM 0x200001C8 */
    /* 0x08003D36: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x08003D38: 0cd0     beq      #0x8003d54 */
    if (/* beq */) goto L_08003D54;
    /* 0x08003D3A: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08003D3C: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x08003D3E: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08003D40: 3dd0     beq      #0x8003dbe */
    if (r0 == 1) goto L_08003DBE;
    /* 0x08003D42: 2820     movs     r0, #0x28 */
    r0 = 0x28;
    /* 0x08003D44: 2880     strh     r0, [r5] */
    *(uint16_t *)(r5) = r0;   /* store */
    /* 0x08003D46: 1420     movs     r0, #0x14 */
    r0 = 0x14;

    L_08003D48:
    /* 0x08003D48: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */

    L_08003D4A:
    /* 0x08003D4A: 2d48     ldr      r0, [pc, #0xb4]  ; =RAM_20000167 */
    r0 = *(uint32_t *)0x08003E00;   /* = RAM 0x20000167 */
    /* 0x08003D4C: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08003D4E: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08003D50: 38d0     beq      #0x8003dc4 */
    if (r0 == 1) goto L_08003DC4;
    /* 0x08003D52: 3de0     b        #0x8003dd0 */
    goto L_08003DD0;

    L_08003D54:
    /* 0x08003D54: 2b48     ldr      r0, [pc, #0xac]  ; =RAM_200001B2 */
    r0 = *(uint32_t *)0x08003E04;   /* = RAM 0x200001B2 */
    /* 0x08003D56: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x08003D58: 2b48     ldr      r0, [pc, #0xac]  ; =RAM_200001BD */
    r0 = *(uint32_t *)0x08003E08;   /* = RAM 0x200001BD */
    /* 0x08003D5A: 1a29     cmp      r1, #0x1a */
    /* flags = cmp(r1, #0x1a) */
    /* 0x08003D5C: 07d0     beq      #0x8003d6e */
    if (r1 == 0x1a) goto L_08003D6E;
    /* 0x08003D5E: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);

    L_08003D60:
    /* 0x08003D60: 7028     cmp      r0, #0x70 */
    /* flags = cmp(r0, #0x70) */
    /* 0x08003D62: 13d2     bhs      #0x8003d8c */
    if (r0 >= 0x70) goto L_08003D8C;   /* unsigned */
    /* 0x08003D64: 7020     movs     r0, #0x70 */
    r0 = 0x70;
    /* 0x08003D66: 294a     ldr      r2, [pc, #0xa4]  ; =RAM_200001D6 */
    r2 = *(uint32_t *)0x08003E0C;   /* = RAM 0x200001D6 */
    /* 0x08003D68: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x08003D6A: 1170     strb     r1, [r2] */
    *(uint8_t *)(r2) = r1;   /* store */
    /* 0x08003D6C: 11e0     b        #0x8003d92 */
    goto L_08003D92;

    L_08003D6E:
    /* 0x08003D6E: 2849     ldr      r1, [pc, #0xa0]  ; =RAM_2000017D */
    r1 = *(uint32_t *)0x08003E10;   /* = RAM 0x2000017D */
    /* 0x08003D70: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08003D72: 405c     ldrb     r0, [r0, r1] */
    r0 = *(uint8_t *)(r0 + r1);
    /* 0x08003D74: cf28     cmp      r0, #0xcf */
    /* flags = cmp(r0, #0xcf) */
    /* 0x08003D76: 00d9     bls      #0x8003d7a */
    if (r0 <= 0xcf) goto L_08003D7A;   /* unsigned */
    /* 0x08003D78: cf20     movs     r0, #0xcf */
    r0 = 0xcf;

    L_08003D7A:
    /* 0x08003D7A: 2649     ldr      r1, [pc, #0x98]  ; =RAM_200001B1 */
    r1 = *(uint32_t *)0x08003E14;   /* = RAM 0x200001B1 */
    /* 0x08003D7C: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08003D7E: 401a     subs     r0, r0, r1 */
    r0 = r0 - r1;
    /* 0x08003D80: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08003D82: c028     cmp      r0, #0xc0 */
    /* flags = cmp(r0, #0xc0) */
    /* 0x08003D84: ecd2     bhs      #0x8003d60 */
    if (r0 >= 0xc0) goto L_08003D60;   /* unsigned */
    /* 0x08003D86: 2330     adds     r0, #0x23 */
    r0 = r0 + 0x23;
    /* 0x08003D88: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08003D8A: e9e7     b        #0x8003d60 */
    goto L_08003D60;   /* loop */

    L_08003D8C:
    /* 0x08003D8C: c028     cmp      r0, #0xc0 */
    /* flags = cmp(r0, #0xc0) */
    /* 0x08003D8E: 00d9     bls      #0x8003d92 */
    if (r0 <= 0xc0) goto L_08003D92;   /* unsigned */
    /* 0x08003D90: c020     movs     r0, #0xc0 */
    r0 = 0xc0;

    L_08003D92:
    /* 0x08003D92: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x08003D94: 7038     subs     r0, #0x70 */
    r0 = r0 - 0x70;
    /* 0x08003D96: 4843     muls     r0, r1, r0 */
    r0 = r1 * r0;
    /* 0x08003D98: c117     asrs     r1, r0, #0x1f */
    r1 = r0 >> 0x1f;
    /* 0x08003D9A: 490f     lsrs     r1, r1, #0x1d */
    r1 = r1 >> 0x1d;
    /* 0x08003D9C: 0818     adds     r0, r1, r0 */
    r0 = r1 + r0;
    /* 0x08003D9E: 4003     lsls     r0, r0, #0xd */
    r0 = r0 << 0xd;
    /* 0x08003DA0: 000c     lsrs     r0, r0, #0x10 */
    r0 = r0 >> 0x10;
    /* 0x08003DA2: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x08003DA4: 00d9     bls      #0x8003da8 */
    if (r0 <= 0x64) goto L_08003DA8;   /* unsigned */
    /* 0x08003DA6: 6420     movs     r0, #0x64 */
    r0 = 0x64;

    L_08003DA8:
    /* 0x08003DA8: 1b49     ldr      r1, [pc, #0x6c]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x08003E18;   /* = RAM 0x20000134 */
    /* 0x08003DAA: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08003DAC: 8870     strb     r0, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r0;   /* store */
    /* 0x08003DAE: 1b49     ldr      r1, [pc, #0x6c]  ; =flash_0800D4C6 */
    r1 = *(uint32_t *)0x08003E1C;   /* = flash data/const 0x0800D4C6 */
    /* 0x08003DB0: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x08003DB2: 085a     ldrh     r0, [r1, r0] */
    r0 = *(uint16_t *)(r1 + r0);
    /* 0x08003DB4: 8105     lsls     r1, r0, #0x16 */
    r1 = r0 << 0x16;
    /* 0x08003DB6: 890d     lsrs     r1, r1, #0x16 */
    r1 = r1 >> 0x16;
    /* 0x08003DB8: 2980     strh     r1, [r5] */
    *(uint16_t *)(r5) = r1;   /* store */
    /* 0x08003DBA: 800a     lsrs     r0, r0, #0xa */
    r0 = r0 >> 0xa;
    /* 0x08003DBC: c4e7     b        #0x8003d48 */
    goto L_08003D48;   /* loop */

    L_08003DBE:
    /* 0x08003DBE: 02f0b7fc bl       #0x8006730 */
    FUN_08006730();   /* BL -> 0x08006730 FUN_08006730 */
    /* 0x08003DC2: c2e7     b        #0x8003d4a */
    goto L_08003D4A;   /* loop */

    L_08003DC4:
    /* 0x08003DC4: 1648     ldr      r0, [pc, #0x58]  ; =RAM_20000192 */
    r0 = *(uint32_t *)0x08003E20;   /* = RAM 0x20000192 */
    /* 0x08003DC6: 0088     ldrh     r0, [r0] */
    r0 = *(uint16_t *)(r0);
    /* 0x08003DC8: 2880     strh     r0, [r5] */
    *(uint16_t *)(r5) = r0;   /* store */
    /* 0x08003DCA: 1648     ldr      r0, [pc, #0x58]  ; =RAM_2000018A */
    r0 = *(uint32_t *)0x08003E24;   /* = RAM 0x2000018A */
    /* 0x08003DCC: 0088     ldrh     r0, [r0] */
    r0 = *(uint16_t *)(r0);
    /* 0x08003DCE: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */

    L_08003DD0:
    /* 0x08003DD0: 2888     ldrh     r0, [r5] */
    r0 = *(uint16_t *)(r5);
    /* 0x08003DD2: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x08003DD4: 01d2     bhs      #0x8003dda */
    if (r0 >= 0xa) goto L_08003DDA;   /* unsigned */
    /* 0x08003DD6: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x08003DD8: 2880     strh     r0, [r5] */
    *(uint16_t *)(r5) = r0;   /* store */

    L_08003DDA:
    /* 0x08003DDA: 1349     ldr      r1, [pc, #0x4c]  ; =RAM_200001C6 */
    r1 = *(uint32_t *)0x08003E28;   /* = RAM 0x200001C6 */
    /* 0x08003DDC: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */
    /* 0x08003DDE: 2088     ldrh     r0, [r4] */
    r0 = *(uint16_t *)(r4);
    /* 0x08003DE0: 3228     cmp      r0, #0x32 */
    /* flags = cmp(r0, #0x32) */
    /* 0x08003DE2: 01d9     bls      #0x8003de8 */
    if (r0 <= 0x32) goto L_08003DE8;   /* unsigned */
    /* 0x08003DE4: 3220     movs     r0, #0x32 */
    r0 = 0x32;
    /* 0x08003DE6: 02e0     b        #0x8003dee */
    goto L_08003DEE;

    L_08003DE8:
    /* 0x08003DE8: 0f28     cmp      r0, #0xf */
    /* flags = cmp(r0, #0xf) */
    /* 0x08003DEA: 01d2     bhs      #0x8003df0 */
    if (r0 >= 0xf) goto L_08003DF0;   /* unsigned */
    /* 0x08003DEC: 0f20     movs     r0, #0xf */
    r0 = 0xf;

    L_08003DEE:
    /* 0x08003DEE: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */

    L_08003DF0:
    /* 0x08003DF0: 30bd     pop      {r4, r5, pc} */
    pop({r4, r5, pc});   /* return */
}
