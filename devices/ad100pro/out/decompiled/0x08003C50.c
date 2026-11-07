/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08003C50
 * Name      : FUN_08003C50
 * End       : 0x08003CF6
 * Size      : 166 bytes, 80 instructions decoded in body (functions.json insns=80, blocks=22)
 * Class     : function
 * Callers   : 2  -> 0x080065CC FUN_080065CC, 0x08006614 FUN_08006614
 * Callees   : 3  -> 0x08006770 FUN_08006770, 0x080067C0 FUN_080067C0, 0x0800AF88 FUN_0800AF88
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08003C50(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08003C50: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08003C52: 2949     ldr      r1, [pc, #0xa4]  ; =RAM_20000175 */
    r1 = *(uint32_t *)0x08003CF8;   /* = RAM 0x20000175 */
    /* 0x08003C54: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003C56: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08003C58: 8a07     lsls     r2, r1, #0x1e */
    r2 = r1 << 0x1e;
    /* 0x08003C5A: 15d0     beq      #0x8003c88 */
    if (/* beq */) goto L_08003C88;
    /* 0x08003C5C: 8807     lsls     r0, r1, #0x1e */
    r0 = r1 << 0x1e;
    /* 0x08003C5E: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x08003C60: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08003C62: 45d0     beq      #0x8003cf0 */
    if (r0 == 2) goto L_08003CF0;
    /* 0x08003C64: 02f084fd bl       #0x8006770 */
    FUN_08006770();   /* BL -> 0x08006770 FUN_08006770 */

    L_08003C68:
    /* 0x08003C68: 2449     ldr      r1, [pc, #0x90]  ; =RAM_200001B6 */
    r1 = *(uint32_t *)0x08003CFC;   /* = RAM 0x200001B6 */
    /* 0x08003C6A: ff22     movs     r2, #0xff */
    r2 = 0xff;
    /* 0x08003C6C: f032     adds     r2, #0xf0 */
    r2 = r2 + 0xf0;
    /* 0x08003C6E: 0988     ldrh     r1, [r1] */
    r1 = *(uint16_t *)(r1);
    /* 0x08003C70: 07f08af9 bl       #0x800af88 */
    FUN_0800AF88();   /* BL -> 0x0800AF88 FUN_0800AF88 */
    /* 0x08003C74: 2249     ldr      r1, [pc, #0x88]  ; =RAM_200001C4 */
    r1 = *(uint32_t *)0x08003D00;   /* = RAM 0x200001C4 */
    /* 0x08003C76: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */
    /* 0x08003C78: 2248     ldr      r0, [pc, #0x88]  ; =RAM_20000167 */
    r0 = *(uint32_t *)0x08003D04;   /* = RAM 0x20000167 */
    /* 0x08003C7A: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08003C7C: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08003C7E: 02d1     bne      #0x8003c86 */
    if (r0 != 1) goto L_08003C86;
    /* 0x08003C80: 2148     ldr      r0, [pc, #0x84]  ; =RAM_20000192 */
    r0 = *(uint32_t *)0x08003D08;   /* = RAM 0x20000192 */
    /* 0x08003C82: 0088     ldrh     r0, [r0] */
    r0 = *(uint16_t *)(r0);
    /* 0x08003C84: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */

    L_08003C86:
    /* 0x08003C86: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_08003C88:
    /* 0x08003C88: 2049     ldr      r1, [pc, #0x80]  ; =RAM_200001BC */
    r1 = *(uint32_t *)0x08003D0C;   /* = RAM 0x200001BC */
    /* 0x08003C8A: 0b78     ldrb     r3, [r1] */
    r3 = *(uint8_t *)(r1);
    /* 0x08003C8C: 2049     ldr      r1, [pc, #0x80]  ; =RAM_200001B2 */
    r1 = *(uint32_t *)0x08003D10;   /* = RAM 0x200001B2 */
    /* 0x08003C8E: 0a2b     cmp      r3, #0xa */
    /* flags = cmp(r3, #0xa) */
    /* 0x08003C90: 0a78     ldrb     r2, [r1] */
    r2 = *(uint8_t *)(r1);
    /* 0x08003C92: 2049     ldr      r1, [pc, #0x80]  ; =RAM_200001BD */
    r1 = *(uint32_t *)0x08003D14;   /* = RAM 0x200001BD */
    /* 0x08003C94: 02d0     beq      #0x8003c9c */
    if (r3 == 0xa) goto L_08003C9C;
    /* 0x08003C96: 0a2a     cmp      r2, #0xa */
    /* flags = cmp(r2, #0xa) */
    /* 0x08003C98: 0ad0     beq      #0x8003cb0 */
    if (r2 == 0xa) goto L_08003CB0;
    /* 0x08003C9A: e5e7     b        #0x8003c68 */
    goto L_08003C68;   /* loop */

    L_08003C9C:
    /* 0x08003C9C: 0a2a     cmp      r2, #0xa */
    /* flags = cmp(r2, #0xa) */
    /* 0x08003C9E: 07d0     beq      #0x8003cb0 */
    if (r2 == 0xa) goto L_08003CB0;
    /* 0x08003CA0: 0878     ldrb     r0, [r1] */
    r0 = *(uint8_t *)(r1);

    L_08003CA2:
    /* 0x08003CA2: 7028     cmp      r0, #0x70 */
    /* flags = cmp(r0, #0x70) */
    /* 0x08003CA4: 10d2     bhs      #0x8003cc8 */
    if (r0 >= 0x70) goto L_08003CC8;   /* unsigned */
    /* 0x08003CA6: 7020     movs     r0, #0x70 */
    r0 = 0x70;
    /* 0x08003CA8: 1b4a     ldr      r2, [pc, #0x6c]  ; =RAM_200001D6 */
    r2 = *(uint32_t *)0x08003D18;   /* = RAM 0x200001D6 */
    /* 0x08003CAA: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x08003CAC: 1170     strb     r1, [r2] */
    *(uint8_t *)(r2) = r1;   /* store */
    /* 0x08003CAE: 0ee0     b        #0x8003cce */
    goto L_08003CCE;

    L_08003CB0:
    /* 0x08003CB0: 1a48     ldr      r0, [pc, #0x68]  ; =RAM_2000017D */
    r0 = *(uint32_t *)0x08003D1C;   /* = RAM 0x2000017D */
    /* 0x08003CB2: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08003CB4: 085c     ldrb     r0, [r1, r0] */
    r0 = *(uint8_t *)(r1 + r0);
    /* 0x08003CB6: cf28     cmp      r0, #0xcf */
    /* flags = cmp(r0, #0xcf) */
    /* 0x08003CB8: 00d9     bls      #0x8003cbc */
    if (r0 <= 0xcf) goto L_08003CBC;   /* unsigned */
    /* 0x08003CBA: cf20     movs     r0, #0xcf */
    r0 = 0xcf;

    L_08003CBC:
    /* 0x08003CBC: 1849     ldr      r1, [pc, #0x60]  ; =RAM_200001B1 */
    r1 = *(uint32_t *)0x08003D20;   /* = RAM 0x200001B1 */
    /* 0x08003CBE: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08003CC0: 401a     subs     r0, r0, r1 */
    r0 = r0 - r1;
    /* 0x08003CC2: 0830     adds     r0, #8 */
    r0 = r0 + 8;
    /* 0x08003CC4: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08003CC6: ece7     b        #0x8003ca2 */
    goto L_08003CA2;   /* loop */

    L_08003CC8:
    /* 0x08003CC8: c028     cmp      r0, #0xc0 */
    /* flags = cmp(r0, #0xc0) */
    /* 0x08003CCA: 00d9     bls      #0x8003cce */
    if (r0 <= 0xc0) goto L_08003CCE;   /* unsigned */
    /* 0x08003CCC: c020     movs     r0, #0xc0 */
    r0 = 0xc0;

    L_08003CCE:
    /* 0x08003CCE: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x08003CD0: 7038     subs     r0, #0x70 */
    r0 = r0 - 0x70;
    /* 0x08003CD2: 4843     muls     r0, r1, r0 */
    r0 = r1 * r0;
    /* 0x08003CD4: c117     asrs     r1, r0, #0x1f */
    r1 = r0 >> 0x1f;
    /* 0x08003CD6: 490f     lsrs     r1, r1, #0x1d */
    r1 = r1 >> 0x1d;
    /* 0x08003CD8: 0818     adds     r0, r1, r0 */
    r0 = r1 + r0;
    /* 0x08003CDA: 4003     lsls     r0, r0, #0xd */
    r0 = r0 << 0xd;
    /* 0x08003CDC: 000c     lsrs     r0, r0, #0x10 */
    r0 = r0 >> 0x10;
    /* 0x08003CDE: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x08003CE0: 00d9     bls      #0x8003ce4 */
    if (r0 <= 0x64) goto L_08003CE4;   /* unsigned */
    /* 0x08003CE2: 6420     movs     r0, #0x64 */
    r0 = 0x64;

    L_08003CE4:
    /* 0x08003CE4: 0f49     ldr      r1, [pc, #0x3c]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x08003D24;   /* = RAM 0x20000134 */
    /* 0x08003CE6: 8870     strb     r0, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r0;   /* store */
    /* 0x08003CE8: 0f49     ldr      r1, [pc, #0x3c]  ; =flash_0800D35A */
    r1 = *(uint32_t *)0x08003D28;   /* = flash data/const 0x0800D35A */
    /* 0x08003CEA: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x08003CEC: 085a     ldrh     r0, [r1, r0] */
    r0 = *(uint16_t *)(r1 + r0);
    /* 0x08003CEE: bbe7     b        #0x8003c68 */
    goto L_08003C68;   /* loop */

    L_08003CF0:
    /* 0x08003CF0: 02f066fd bl       #0x80067c0 */
    FUN_080067C0();   /* BL -> 0x080067C0 FUN_080067C0 */
    /* 0x08003CF4: b8e7     b        #0x8003c68 */
    goto L_08003C68;   /* loop */
}
