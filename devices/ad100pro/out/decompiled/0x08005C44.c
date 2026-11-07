/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005C44
 * Name      : FUN_08005C44
 * End       : 0x08005CAA
 * Size      : 102 bytes, 48 instructions decoded in body (functions.json insns=48, blocks=10)
 * Class     : function
 * Callers   : 4  -> 0x080065CC FUN_080065CC, 0x08006600 FUN_08006600, 0x0800C49C FUN_0800C49C, 0x0800C4BC FUN_0800C4BC
 * Callees   : 3  -> 0x08006770 FUN_08006770, 0x080067C0 FUN_080067C0, 0x0800AF88 FUN_0800AF88
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005C44(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005C44: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08005C46: 1948     ldr      r0, [pc, #0x64]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x08005CAC;   /* = RAM 0x20000175 */
    /* 0x08005C48: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08005C4A: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x08005C4C: 0ed0     beq      #0x8005c6c */
    if (/* beq */) goto L_08005C6C;
    /* 0x08005C4E: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08005C50: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x08005C52: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08005C54: 26d0     beq      #0x8005ca4 */
    if (r0 == 2) goto L_08005CA4;
    /* 0x08005C56: 00f08bfd bl       #0x8006770 */
    FUN_08006770();   /* BL -> 0x08006770 FUN_08006770 */

    L_08005C5A:
    /* 0x08005C5A: 1549     ldr      r1, [pc, #0x54]  ; =RAM_200001B6 */
    r1 = *(uint32_t *)0x08005CB0;   /* = RAM 0x200001B6 */
    /* 0x08005C5C: ff22     movs     r2, #0xff */
    r2 = 0xff;
    /* 0x08005C5E: f032     adds     r2, #0xf0 */
    r2 = r2 + 0xf0;
    /* 0x08005C60: 0988     ldrh     r1, [r1] */
    r1 = *(uint16_t *)(r1);
    /* 0x08005C62: 05f091f9 bl       #0x800af88 */
    FUN_0800AF88();   /* BL -> 0x0800AF88 FUN_0800AF88 */
    /* 0x08005C66: 1349     ldr      r1, [pc, #0x4c]  ; =RAM_200001C4 */
    r1 = *(uint32_t *)0x08005CB4;   /* = RAM 0x200001C4 */
    /* 0x08005C68: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */
    /* 0x08005C6A: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_08005C6C:
    /* 0x08005C6C: 1349     ldr      r1, [pc, #0x4c]  ; =RAM_2000017D */
    r1 = *(uint32_t *)0x08005CBC;   /* = RAM 0x2000017D */
    /* 0x08005C6E: 1248     ldr      r0, [pc, #0x48]  ; =RAM_200001BD */
    r0 = *(uint32_t *)0x08005CB8;   /* = RAM 0x200001BD */
    /* 0x08005C70: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08005C72: 1822     movs     r2, #0x18 */
    r2 = 0x18;
    /* 0x08005C74: 405c     ldrb     r0, [r0, r1] */
    r0 = *(uint8_t *)(r0 + r1);
    /* 0x08005C76: 1249     ldr      r1, [pc, #0x48]  ; =RAM_200001B1 */
    r1 = *(uint32_t *)0x08005CC0;   /* = RAM 0x200001B1 */
    /* 0x08005C78: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08005C7A: 511a     subs     r1, r2, r1 */
    r1 = r2 - r1;
    /* 0x08005C7C: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x08005C7E: 8a00     lsls     r2, r1, #2 */
    r2 = r1 << 2;
    /* 0x08005C80: 8918     adds     r1, r1, r2 */
    r1 = r1 + r2;
    /* 0x08005C82: 8908     lsrs     r1, r1, #2 */
    r1 = r1 >> 2;
    /* 0x08005C84: 0818     adds     r0, r1, r0 */
    r0 = r1 + r0;
    /* 0x08005C86: 1d28     cmp      r0, #0x1d */
    /* flags = cmp(r0, #0x1d) */
    /* 0x08005C88: 00d2     bhs      #0x8005c8c */
    if (r0 >= 0x1d) goto L_08005C8C;   /* unsigned */
    /* 0x08005C8A: 1d20     movs     r0, #0x1d */
    r0 = 0x1d;

    L_08005C8C:
    /* 0x08005C8C: 1d38     subs     r0, #0x1d */
    r0 = r0 - 0x1d;
    /* 0x08005C8E: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x08005C90: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x08005C92: 00d9     bls      #0x8005c96 */
    if (r0 <= 0x64) goto L_08005C96;   /* unsigned */
    /* 0x08005C94: 6420     movs     r0, #0x64 */
    r0 = 0x64;

    L_08005C96:
    /* 0x08005C96: 0b49     ldr      r1, [pc, #0x2c]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x08005CC4;   /* = RAM 0x20000134 */
    /* 0x08005C98: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08005C9A: 8870     strb     r0, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r0;   /* store */
    /* 0x08005C9C: 0a49     ldr      r1, [pc, #0x28]  ; =flash_0800D35A */
    r1 = *(uint32_t *)0x08005CC8;   /* = flash data/const 0x0800D35A */
    /* 0x08005C9E: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x08005CA0: 085a     ldrh     r0, [r1, r0] */
    r0 = *(uint16_t *)(r1 + r0);
    /* 0x08005CA2: dae7     b        #0x8005c5a */
    goto L_08005C5A;   /* loop */

    L_08005CA4:
    /* 0x08005CA4: 00f08cfd bl       #0x80067c0 */
    FUN_080067C0();   /* BL -> 0x080067C0 FUN_080067C0 */
    /* 0x08005CA8: d7e7     b        #0x8005c5a */
    goto L_08005C5A;   /* loop */
}
