/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009BF8
 * Name      : FUN_08009BF8
 * End       : 0x08009CAC
 * Size      : 180 bytes, 90 instructions decoded in body (functions.json insns=90, blocks=40)
 * Class     : function
 * Callers   : 2  -> 0x080047CC FUN_080047CC, 0x08004836 FUN_08004836
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009BF8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009BF8: 2c49     ldr      r1, [pc, #0xb0]  ; =RAM_20000164 */
    r1 = *(uint32_t *)0x08009CAC;   /* = RAM 0x20000164 */
    /* 0x08009BFA: 8878     ldrb     r0, [r1, #2] */
    r0 = *(uint8_t *)(r1 + 2);
    /* 0x08009BFC: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08009BFE: 0bd0     beq      #0x8009c18 */
    if (r0 == 1) goto L_08009C18;
    /* 0x08009C00: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08009C02: 0bd0     beq      #0x8009c1c */
    if (r0 == 2) goto L_08009C1C;
    /* 0x08009C04: 0328     cmp      r0, #3 */
    /* flags = cmp(r0, #3) */
    /* 0x08009C06: 0bd0     beq      #0x8009c20 */
    if (r0 == 3) goto L_08009C20;
    /* 0x08009C08: 0428     cmp      r0, #4 */
    /* flags = cmp(r0, #4) */
    /* 0x08009C0A: 0bd0     beq      #0x8009c24 */
    if (r0 == 4) goto L_08009C24;
    /* 0x08009C0C: 0528     cmp      r0, #5 */
    /* flags = cmp(r0, #5) */
    /* 0x08009C0E: 0bd0     beq      #0x8009c28 */
    if (r0 == 5) goto L_08009C28;
    /* 0x08009C10: 0728     cmp      r0, #7 */
    /* flags = cmp(r0, #7) */
    /* 0x08009C12: 0bd8     bhi      #0x8009c2c */
    if (r0 > 7) goto L_08009C2C;   /* unsigned */
    /* 0x08009C14: 0520     movs     r0, #5 */
    r0 = 5;
    /* 0x08009C16: 22e0     b        #0x8009c5e */
    goto L_08009C5E;

    L_08009C18:
    /* 0x08009C18: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08009C1A: 20e0     b        #0x8009c5e */
    goto L_08009C5E;

    L_08009C1C:
    /* 0x08009C1C: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08009C1E: 1ee0     b        #0x8009c5e */
    goto L_08009C5E;

    L_08009C20:
    /* 0x08009C20: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08009C22: 1ce0     b        #0x8009c5e */
    goto L_08009C5E;

    L_08009C24:
    /* 0x08009C24: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x08009C26: 1ae0     b        #0x8009c5e */
    goto L_08009C5E;

    L_08009C28:
    /* 0x08009C28: 0420     movs     r0, #4 */
    r0 = 4;
    /* 0x08009C2A: 18e0     b        #0x8009c5e */
    goto L_08009C5E;

    L_08009C2C:
    /* 0x08009C2C: 0928     cmp      r0, #9 */
    /* flags = cmp(r0, #9) */
    /* 0x08009C2E: 01d8     bhi      #0x8009c34 */
    if (r0 > 9) goto L_08009C34;   /* unsigned */
    /* 0x08009C30: 0620     movs     r0, #6 */
    r0 = 6;
    /* 0x08009C32: 14e0     b        #0x8009c5e */
    goto L_08009C5E;

    L_08009C34:
    /* 0x08009C34: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x08009C36: 05d0     beq      #0x8009c44 */
    if (r0 == 0xa) goto L_08009C44;
    /* 0x08009C38: 0b28     cmp      r0, #0xb */
    /* flags = cmp(r0, #0xb) */
    /* 0x08009C3A: 05d0     beq      #0x8009c48 */
    if (r0 == 0xb) goto L_08009C48;
    /* 0x08009C3C: 0e28     cmp      r0, #0xe */
    /* flags = cmp(r0, #0xe) */
    /* 0x08009C3E: 05d8     bhi      #0x8009c4c */
    if (r0 > 0xe) goto L_08009C4C;   /* unsigned */
    /* 0x08009C40: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x08009C42: 0ce0     b        #0x8009c5e */
    goto L_08009C5E;

    L_08009C44:
    /* 0x08009C44: 0720     movs     r0, #7 */
    r0 = 7;
    /* 0x08009C46: 0ae0     b        #0x8009c5e */
    goto L_08009C5E;

    L_08009C48:
    /* 0x08009C48: 0820     movs     r0, #8 */
    r0 = 8;
    /* 0x08009C4A: 08e0     b        #0x8009c5e */
    goto L_08009C5E;

    L_08009C4C:
    /* 0x08009C4C: 1328     cmp      r0, #0x13 */
    /* flags = cmp(r0, #0x13) */
    /* 0x08009C4E: 01d8     bhi      #0x8009c54 */
    if (r0 > 0x13) goto L_08009C54;   /* unsigned */
    /* 0x08009C50: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x08009C52: 04e0     b        #0x8009c5e */
    goto L_08009C5E;

    L_08009C54:
    /* 0x08009C54: 3b28     cmp      r0, #0x3b */
    /* flags = cmp(r0, #0x3b) */
    /* 0x08009C56: 01d8     bhi      #0x8009c5c */
    if (r0 > 0x3b) goto L_08009C5C;   /* unsigned */
    /* 0x08009C58: 0b20     movs     r0, #0xb */
    r0 = 0xb;
    /* 0x08009C5A: 00e0     b        #0x8009c5e */
    goto L_08009C5E;

    L_08009C5C:
    /* 0x08009C5C: 0c20     movs     r0, #0xc */
    r0 = 0xc;

    L_08009C5E:
    /* 0x08009C5E: 134a     ldr      r2, [pc, #0x4c]  ; =RAM_20000164 */
    r2 = *(uint32_t *)0x08009CAC;   /* = RAM 0x20000164 */
    /* 0x08009C60: 4b7e     ldrb     r3, [r1, #0x19] */
    r3 = *(uint8_t *)(r1 + 25);
    /* 0x08009C62: 3e32     adds     r2, #0x3e */
    r2 = r2 + 0x3e;
    /* 0x08009C64: d25c     ldrb     r2, [r2, r3] */
    r2 = *(uint8_t *)(r2 + r3);
    /* 0x08009C66: 462a     cmp      r2, #0x46 */
    /* flags = cmp(r2, #0x46) */
    /* 0x08009C68: 01d3     blo      #0x8009c6e */
    if (r2 < 0x46) goto L_08009C6E;   /* unsigned */
    /* 0x08009C6A: 114a     ldr      r2, [pc, #0x44]  ; =func_0800DA78 */
    r2 = *(uint32_t *)0x08009CB0;   /* = flash data/const 0x0800DA79 */
    /* 0x08009C6C: 03e0     b        #0x8009c76 */
    goto L_08009C76;

    L_08009C6E:
    /* 0x08009C6E: 3c2a     cmp      r2, #0x3c */
    /* flags = cmp(r2, #0x3c) */
    /* 0x08009C70: 03d3     blo      #0x8009c7a */
    if (r2 < 0x3c) goto L_08009C7A;   /* unsigned */
    /* 0x08009C72: 0f4a     ldr      r2, [pc, #0x3c]  ; =func_0800DA78 */
    r2 = *(uint32_t *)0x08009CB0;   /* = flash data/const 0x0800DA79 */
    /* 0x08009C74: 0d3a     subs     r2, #0xd */
    r2 = r2 - 0xd;

    L_08009C76:
    /* 0x08009C76: 105c     ldrb     r0, [r2, r0] */
    r0 = *(uint8_t *)(r2 + r0);
    /* 0x08009C78: 11e0     b        #0x8009c9e */
    goto L_08009C9E;

    L_08009C7A:
    /* 0x08009C7A: 322a     cmp      r2, #0x32 */
    /* flags = cmp(r2, #0x32) */
    /* 0x08009C7C: 02d3     blo      #0x8009c84 */
    if (r2 < 0x32) goto L_08009C84;   /* unsigned */
    /* 0x08009C7E: 0c4a     ldr      r2, [pc, #0x30]  ; =func_0800DA78 */
    r2 = *(uint32_t *)0x08009CB0;   /* = flash data/const 0x0800DA79 */
    /* 0x08009C80: 1a3a     subs     r2, #0x1a */
    r2 = r2 - 0x1a;
    /* 0x08009C82: f8e7     b        #0x8009c76 */
    goto L_08009C76;   /* loop */

    L_08009C84:
    /* 0x08009C84: 282a     cmp      r2, #0x28 */
    /* flags = cmp(r2, #0x28) */
    /* 0x08009C86: 02d3     blo      #0x8009c8e */
    if (r2 < 0x28) goto L_08009C8E;   /* unsigned */
    /* 0x08009C88: 094a     ldr      r2, [pc, #0x24]  ; =func_0800DA78 */
    r2 = *(uint32_t *)0x08009CB0;   /* = flash data/const 0x0800DA79 */
    /* 0x08009C8A: 273a     subs     r2, #0x27 */
    r2 = r2 - 0x27;
    /* 0x08009C8C: f3e7     b        #0x8009c76 */
    goto L_08009C76;   /* loop */

    L_08009C8E:
    /* 0x08009C8E: 1e2a     cmp      r2, #0x1e */
    /* flags = cmp(r2, #0x1e) */
    /* 0x08009C90: 02d3     blo      #0x8009c98 */
    if (r2 < 0x1e) goto L_08009C98;   /* unsigned */
    /* 0x08009C92: 074a     ldr      r2, [pc, #0x1c]  ; =func_0800DA78 */
    r2 = *(uint32_t *)0x08009CB0;   /* = flash data/const 0x0800DA79 */
    /* 0x08009C94: 343a     subs     r2, #0x34 */
    r2 = r2 - 0x34;
    /* 0x08009C96: eee7     b        #0x8009c76 */
    goto L_08009C76;   /* loop */

    L_08009C98:
    /* 0x08009C98: 054b     ldr      r3, [pc, #0x14]  ; =func_0800DA78 */
    r3 = *(uint32_t *)0x08009CB0;   /* = flash data/const 0x0800DA79 */
    /* 0x08009C9A: 413b     subs     r3, #0x41 */
    r3 = r3 - 0x41;
    /* 0x08009C9C: 185c     ldrb     r0, [r3, r0] */
    r0 = *(uint8_t *)(r3 + r0);

    L_08009C9E:
    /* 0x08009C9E: 0875     strb     r0, [r1, #0x14] */
    *(uint8_t *)(r1 + 20) = r0;   /* store */
    /* 0x08009CA0: 4a78     ldrb     r2, [r1, #1] */
    r2 = *(uint8_t *)(r1 + 1);
    /* 0x08009CA2: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08009CA4: 8242     cmp      r2, r0 */
    /* flags = cmp(r2, r0) */
    /* 0x08009CA6: 00d9     bls      #0x8009caa */
    if (r2 <= r0) goto L_08009CAA;   /* unsigned */
    /* 0x08009CA8: 4870     strb     r0, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r0;   /* store */

    L_08009CAA:
    /* 0x08009CAA: 7047     bx       lr */
    return;
}
