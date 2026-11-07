/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009EE0
 * Name      : FUN_08009EE0
 * End       : 0x08009F92
 * Size      : 178 bytes, 87 instructions decoded in body (functions.json insns=87, blocks=21)
 * Class     : function
 * Callers   : 1  -> 0x080066C0 FUN_080066C0
 * Callees   : 2  -> 0x080030F0 FUN_080030F0, 0x08006730 FUN_08006730
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009EE0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009EE0: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08009EE2: 2c48     ldr      r0, [pc, #0xb0]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x08009F94;   /* = RAM 0x20000175 */
    /* 0x08009EE4: 2c4d     ldr      r5, [pc, #0xb0]  ; =RAM_200001D8 */
    r5 = *(uint32_t *)0x08009F98;   /* = RAM 0x200001D8 */
    /* 0x08009EE6: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08009EE8: 2c4c     ldr      r4, [pc, #0xb0]  ; =RAM_200001C8 */
    r4 = *(uint32_t *)0x08009F9C;   /* = RAM 0x200001C8 */
    /* 0x08009EEA: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x08009EEC: 08d0     beq      #0x8009f00 */
    if (/* beq */) goto L_08009F00;
    /* 0x08009EEE: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08009EF0: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x08009EF2: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08009EF4: 39d0     beq      #0x8009f6a */
    if (r0 == 1) goto L_08009F6A;
    /* 0x08009EF6: 2820     movs     r0, #0x28 */
    r0 = 0x28;
    /* 0x08009EF8: 2880     strh     r0, [r5] */
    *(uint16_t *)(r5) = r0;   /* store */
    /* 0x08009EFA: 1420     movs     r0, #0x14 */
    r0 = 0x14;
    /* 0x08009EFC: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */
    /* 0x08009EFE: 3be0     b        #0x8009f78 */
    goto L_08009F78;

    L_08009F00:
    /* 0x08009F00: 2748     ldr      r0, [pc, #0x9c]  ; =RAM_200001B2 */
    r0 = *(uint32_t *)0x08009FA0;   /* = RAM 0x200001B2 */
    /* 0x08009F02: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08009F04: 1a28     cmp      r0, #0x1a */
    /* flags = cmp(r0, #0x1a) */
    /* 0x08009F06: 15d0     beq      #0x8009f34 */
    if (r0 == 0x1a) goto L_08009F34;
    /* 0x08009F08: 0020     movs     r0, #0 */
    r0 = 0;

    L_08009F0A:
    /* 0x08009F0A: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x08009F0C: 4843     muls     r0, r1, r0 */
    r0 = r1 * r0;
    /* 0x08009F0E: 0c21     movs     r1, #0xc */
    r1 = 0xc;
    /* 0x08009F10: f9f7eef8 bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x08009F14: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x08009F16: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x08009F18: 00d9     bls      #0x8009f1c */
    if (r0 <= 0x64) goto L_08009F1C;   /* unsigned */
    /* 0x08009F1A: 6420     movs     r0, #0x64 */
    r0 = 0x64;

    L_08009F1C:
    /* 0x08009F1C: 2149     ldr      r1, [pc, #0x84]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x08009FA4;   /* = RAM 0x20000134 */
    /* 0x08009F1E: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08009F20: 8870     strb     r0, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r0;   /* store */
    /* 0x08009F22: 2149     ldr      r1, [pc, #0x84]  ; =flash_0800D4C6 */
    r1 = *(uint32_t *)0x08009FA8;   /* = flash data/const 0x0800D4C6 */
    /* 0x08009F24: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x08009F26: 085a     ldrh     r0, [r1, r0] */
    r0 = *(uint16_t *)(r1 + r0);
    /* 0x08009F28: 8105     lsls     r1, r0, #0x16 */
    r1 = r0 << 0x16;
    /* 0x08009F2A: 890d     lsrs     r1, r1, #0x16 */
    r1 = r1 >> 0x16;
    /* 0x08009F2C: 2980     strh     r1, [r5] */
    *(uint16_t *)(r5) = r1;   /* store */
    /* 0x08009F2E: 800a     lsrs     r0, r0, #0xa */
    r0 = r0 >> 0xa;
    /* 0x08009F30: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */
    /* 0x08009F32: 1ce0     b        #0x8009f6e */
    goto L_08009F6E;

    L_08009F34:
    /* 0x08009F34: 1e49     ldr      r1, [pc, #0x78]  ; =RAM_2000017D */
    r1 = *(uint32_t *)0x08009FB0;   /* = RAM 0x2000017D */
    /* 0x08009F36: 1d48     ldr      r0, [pc, #0x74]  ; =RAM_200001BD */
    r0 = *(uint32_t *)0x08009FAC;   /* = RAM 0x200001BD */
    /* 0x08009F38: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08009F3A: 1822     movs     r2, #0x18 */
    r2 = 0x18;
    /* 0x08009F3C: 405c     ldrb     r0, [r0, r1] */
    r0 = *(uint8_t *)(r0 + r1);
    /* 0x08009F3E: 1d49     ldr      r1, [pc, #0x74]  ; =RAM_200001DF */
    r1 = *(uint32_t *)0x08009FB4;   /* = RAM 0x200001DF */
    /* 0x08009F40: 8030     adds     r0, #0x80 */
    r0 = r0 + 0x80;
    /* 0x08009F42: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08009F44: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08009F46: 4018     adds     r0, r0, r1 */
    r0 = r0 + r1;
    /* 0x08009F48: 1b49     ldr      r1, [pc, #0x6c]  ; =RAM_200001B1 */
    r1 = *(uint32_t *)0x08009FB8;   /* = RAM 0x200001B1 */
    /* 0x08009F4A: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08009F4C: 511a     subs     r1, r2, r1 */
    r1 = r2 - r1;
    /* 0x08009F4E: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x08009F50: 4a00     lsls     r2, r1, #1 */
    r2 = r1 << 1;
    /* 0x08009F52: 8918     adds     r1, r1, r2 */
    r1 = r1 + r2;
    /* 0x08009F54: 4908     lsrs     r1, r1, #1 */
    r1 = r1 >> 1;
    /* 0x08009F56: 0818     adds     r0, r1, r0 */
    r0 = r1 + r0;
    /* 0x08009F58: 7028     cmp      r0, #0x70 */
    /* flags = cmp(r0, #0x70) */
    /* 0x08009F5A: 00d2     bhs      #0x8009f5e */
    if (r0 >= 0x70) goto L_08009F5E;   /* unsigned */
    /* 0x08009F5C: 7020     movs     r0, #0x70 */
    r0 = 0x70;

    L_08009F5E:
    /* 0x08009F5E: 7038     subs     r0, #0x70 */
    r0 = r0 - 0x70;
    /* 0x08009F60: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x08009F62: 7828     cmp      r0, #0x78 */
    /* flags = cmp(r0, #0x78) */
    /* 0x08009F64: d1d9     bls      #0x8009f0a */
    if (r0 <= 0x78) goto L_08009F0A;   /* unsigned */
    /* 0x08009F66: 7820     movs     r0, #0x78 */
    r0 = 0x78;
    /* 0x08009F68: cfe7     b        #0x8009f0a */
    goto L_08009F0A;   /* loop */

    L_08009F6A:
    /* 0x08009F6A: fcf7e1fb bl       #0x8006730 */
    FUN_08006730();   /* BL -> 0x08006730 FUN_08006730 */

    L_08009F6E:
    /* 0x08009F6E: 2888     ldrh     r0, [r5] */
    r0 = *(uint16_t *)(r5);
    /* 0x08009F70: 0c28     cmp      r0, #0xc */
    /* flags = cmp(r0, #0xc) */
    /* 0x08009F72: 01d2     bhs      #0x8009f78 */
    if (r0 >= 0xc) goto L_08009F78;   /* unsigned */
    /* 0x08009F74: 0c20     movs     r0, #0xc */
    r0 = 0xc;
    /* 0x08009F76: 2880     strh     r0, [r5] */
    *(uint16_t *)(r5) = r0;   /* store */

    L_08009F78:
    /* 0x08009F78: 1049     ldr      r1, [pc, #0x40]  ; =RAM_200001C6 */
    r1 = *(uint32_t *)0x08009FBC;   /* = RAM 0x200001C6 */
    /* 0x08009F7A: 2888     ldrh     r0, [r5] */
    r0 = *(uint16_t *)(r5);
    /* 0x08009F7C: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */
    /* 0x08009F7E: 2088     ldrh     r0, [r4] */
    r0 = *(uint16_t *)(r4);
    /* 0x08009F80: 3228     cmp      r0, #0x32 */
    /* flags = cmp(r0, #0x32) */
    /* 0x08009F82: 01d9     bls      #0x8009f88 */
    if (r0 <= 0x32) goto L_08009F88;   /* unsigned */
    /* 0x08009F84: 3220     movs     r0, #0x32 */
    r0 = 0x32;
    /* 0x08009F86: 02e0     b        #0x8009f8e */
    goto L_08009F8E;

    L_08009F88:
    /* 0x08009F88: 0f28     cmp      r0, #0xf */
    /* flags = cmp(r0, #0xf) */
    /* 0x08009F8A: 01d2     bhs      #0x8009f90 */
    if (r0 >= 0xf) goto L_08009F90;   /* unsigned */
    /* 0x08009F8C: 0f20     movs     r0, #0xf */
    r0 = 0xf;

    L_08009F8E:
    /* 0x08009F8E: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */

    L_08009F90:
    /* 0x08009F90: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
