/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009E30
 * Name      : FUN_08009E30
 * End       : 0x08009EB8
 * Size      : 136 bytes, 64 instructions decoded in body (functions.json insns=64, blocks=14)
 * Class     : function
 * Callers   : 1  -> 0x080065CC FUN_080065CC
 * Callees   : 4  -> 0x080030F0 FUN_080030F0, 0x08006770 FUN_08006770, 0x080067C0 FUN_080067C0, 0x0800AF88 FUN_0800AF88
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009E30(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009E30: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08009E32: 2148     ldr      r0, [pc, #0x84]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x08009EB8;   /* = RAM 0x20000175 */
    /* 0x08009E34: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08009E36: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x08009E38: 0ed0     beq      #0x8009e58 */
    if (/* beq */) goto L_08009E58;
    /* 0x08009E3A: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08009E3C: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x08009E3E: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08009E40: 37d0     beq      #0x8009eb2 */
    if (r0 == 2) goto L_08009EB2;
    /* 0x08009E42: fcf795fc bl       #0x8006770 */
    FUN_08006770();   /* BL -> 0x08006770 FUN_08006770 */

    L_08009E46:
    /* 0x08009E46: 1d49     ldr      r1, [pc, #0x74]  ; =RAM_200001B6 */
    r1 = *(uint32_t *)0x08009EBC;   /* = RAM 0x200001B6 */
    /* 0x08009E48: ff22     movs     r2, #0xff */
    r2 = 0xff;
    /* 0x08009E4A: f032     adds     r2, #0xf0 */
    r2 = r2 + 0xf0;
    /* 0x08009E4C: 0988     ldrh     r1, [r1] */
    r1 = *(uint16_t *)(r1);
    /* 0x08009E4E: 01f09bf8 bl       #0x800af88 */
    FUN_0800AF88();   /* BL -> 0x0800AF88 FUN_0800AF88 */
    /* 0x08009E52: 1b49     ldr      r1, [pc, #0x6c]  ; =RAM_200001C4 */
    r1 = *(uint32_t *)0x08009EC0;   /* = RAM 0x200001C4 */
    /* 0x08009E54: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */
    /* 0x08009E56: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_08009E58:
    /* 0x08009E58: 1a48     ldr      r0, [pc, #0x68]  ; =RAM_200001B2 */
    r0 = *(uint32_t *)0x08009EC4;   /* = RAM 0x200001B2 */
    /* 0x08009E5A: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08009E5C: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x08009E5E: 0fd0     beq      #0x8009e80 */
    if (r0 == 0xa) goto L_08009E80;
    /* 0x08009E60: 0020     movs     r0, #0 */
    r0 = 0;

    L_08009E62:
    /* 0x08009E62: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x08009E64: 4843     muls     r0, r1, r0 */
    r0 = r1 * r0;
    /* 0x08009E66: 0c21     movs     r1, #0xc */
    r1 = 0xc;
    /* 0x08009E68: f9f742f9 bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x08009E6C: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x08009E6E: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x08009E70: 00d9     bls      #0x8009e74 */
    if (r0 <= 0x64) goto L_08009E74;   /* unsigned */
    /* 0x08009E72: 6420     movs     r0, #0x64 */
    r0 = 0x64;

    L_08009E74:
    /* 0x08009E74: 1449     ldr      r1, [pc, #0x50]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x08009EC8;   /* = RAM 0x20000134 */
    /* 0x08009E76: 8870     strb     r0, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r0;   /* store */
    /* 0x08009E78: 1449     ldr      r1, [pc, #0x50]  ; =flash_0800D35A */
    r1 = *(uint32_t *)0x08009ECC;   /* = flash data/const 0x0800D35A */
    /* 0x08009E7A: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x08009E7C: 085a     ldrh     r0, [r1, r0] */
    r0 = *(uint16_t *)(r1 + r0);
    /* 0x08009E7E: e2e7     b        #0x8009e46 */
    goto L_08009E46;   /* loop */

    L_08009E80:
    /* 0x08009E80: 1449     ldr      r1, [pc, #0x50]  ; =RAM_2000017D */
    r1 = *(uint32_t *)0x08009ED4;   /* = RAM 0x2000017D */
    /* 0x08009E82: 1348     ldr      r0, [pc, #0x4c]  ; =RAM_200001BD */
    r0 = *(uint32_t *)0x08009ED0;   /* = RAM 0x200001BD */
    /* 0x08009E84: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08009E86: 1822     movs     r2, #0x18 */
    r2 = 0x18;
    /* 0x08009E88: 405c     ldrb     r0, [r0, r1] */
    r0 = *(uint8_t *)(r0 + r1);
    /* 0x08009E8A: 1349     ldr      r1, [pc, #0x4c]  ; =RAM_200001DF */
    r1 = *(uint32_t *)0x08009ED8;   /* = RAM 0x200001DF */
    /* 0x08009E8C: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08009E8E: 4018     adds     r0, r0, r1 */
    r0 = r0 + r1;
    /* 0x08009E90: 1249     ldr      r1, [pc, #0x48]  ; =RAM_200001B1 */
    r1 = *(uint32_t *)0x08009EDC;   /* = RAM 0x200001B1 */
    /* 0x08009E92: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08009E94: 511a     subs     r1, r2, r1 */
    r1 = r2 - r1;
    /* 0x08009E96: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x08009E98: 4a00     lsls     r2, r1, #1 */
    r2 = r1 << 1;
    /* 0x08009E9A: 8918     adds     r1, r1, r2 */
    r1 = r1 + r2;
    /* 0x08009E9C: 4908     lsrs     r1, r1, #1 */
    r1 = r1 >> 1;
    /* 0x08009E9E: 0818     adds     r0, r1, r0 */
    r0 = r1 + r0;
    /* 0x08009EA0: 6628     cmp      r0, #0x66 */
    /* flags = cmp(r0, #0x66) */
    /* 0x08009EA2: 00d2     bhs      #0x8009ea6 */
    if (r0 >= 0x66) goto L_08009EA6;   /* unsigned */
    /* 0x08009EA4: 6620     movs     r0, #0x66 */
    r0 = 0x66;

    L_08009EA6:
    /* 0x08009EA6: 6638     subs     r0, #0x66 */
    r0 = r0 - 0x66;
    /* 0x08009EA8: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x08009EAA: 7828     cmp      r0, #0x78 */
    /* flags = cmp(r0, #0x78) */
    /* 0x08009EAC: d9d9     bls      #0x8009e62 */
    if (r0 <= 0x78) goto L_08009E62;   /* unsigned */
    /* 0x08009EAE: 7820     movs     r0, #0x78 */
    r0 = 0x78;
    /* 0x08009EB0: d7e7     b        #0x8009e62 */
    goto L_08009E62;   /* loop */

    L_08009EB2:
    /* 0x08009EB2: fcf785fc bl       #0x80067c0 */
    FUN_080067C0();   /* BL -> 0x080067C0 FUN_080067C0 */
    /* 0x08009EB6: c6e7     b        #0x8009e46 */
    goto L_08009E46;   /* loop */
}
