/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A6E0
 * Name      : FUN_0800A6E0
 * End       : 0x0800A746
 * Size      : 102 bytes, 48 instructions decoded in body (functions.json insns=48, blocks=10)
 * Class     : function
 * Callers   : 4  -> 0x080065CC FUN_080065CC, 0x08006614 FUN_08006614, 0x0800C49C FUN_0800C49C, 0x0800C4D0 FUN_0800C4D0
 * Callees   : 3  -> 0x08006770 FUN_08006770, 0x080067C0 FUN_080067C0, 0x0800AF88 FUN_0800AF88
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A6E0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A6E0: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800A6E2: 1948     ldr      r0, [pc, #0x64]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800A748;   /* = RAM 0x20000175 */
    /* 0x0800A6E4: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800A6E6: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x0800A6E8: 0ed0     beq      #0x800a708 */
    if (/* beq */) goto L_0800A708;
    /* 0x0800A6EA: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x0800A6EC: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x0800A6EE: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x0800A6F0: 26d0     beq      #0x800a740 */
    if (r0 == 2) goto L_0800A740;
    /* 0x0800A6F2: fcf73df8 bl       #0x8006770 */
    FUN_08006770();   /* BL -> 0x08006770 FUN_08006770 */

    L_0800A6F6:
    /* 0x0800A6F6: 1549     ldr      r1, [pc, #0x54]  ; =RAM_200001B6 */
    r1 = *(uint32_t *)0x0800A74C;   /* = RAM 0x200001B6 */
    /* 0x0800A6F8: ff22     movs     r2, #0xff */
    r2 = 0xff;
    /* 0x0800A6FA: f032     adds     r2, #0xf0 */
    r2 = r2 + 0xf0;
    /* 0x0800A6FC: 0988     ldrh     r1, [r1] */
    r1 = *(uint16_t *)(r1);
    /* 0x0800A6FE: 00f043fc bl       #0x800af88 */
    FUN_0800AF88();   /* BL -> 0x0800AF88 FUN_0800AF88 */
    /* 0x0800A702: 1349     ldr      r1, [pc, #0x4c]  ; =RAM_200001C4 */
    r1 = *(uint32_t *)0x0800A750;   /* = RAM 0x200001C4 */
    /* 0x0800A704: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */
    /* 0x0800A706: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800A708:
    /* 0x0800A708: 1349     ldr      r1, [pc, #0x4c]  ; =RAM_2000017D */
    r1 = *(uint32_t *)0x0800A758;   /* = RAM 0x2000017D */
    /* 0x0800A70A: 1248     ldr      r0, [pc, #0x48]  ; =RAM_200001BD */
    r0 = *(uint32_t *)0x0800A754;   /* = RAM 0x200001BD */
    /* 0x0800A70C: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800A70E: 1822     movs     r2, #0x18 */
    r2 = 0x18;
    /* 0x0800A710: 405c     ldrb     r0, [r0, r1] */
    r0 = *(uint8_t *)(r0 + r1);
    /* 0x0800A712: 1249     ldr      r1, [pc, #0x48]  ; =RAM_200001B1 */
    r1 = *(uint32_t *)0x0800A75C;   /* = RAM 0x200001B1 */
    /* 0x0800A714: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800A716: 511a     subs     r1, r2, r1 */
    r1 = r2 - r1;
    /* 0x0800A718: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x0800A71A: 8a00     lsls     r2, r1, #2 */
    r2 = r1 << 2;
    /* 0x0800A71C: 8918     adds     r1, r1, r2 */
    r1 = r1 + r2;
    /* 0x0800A71E: 8908     lsrs     r1, r1, #2 */
    r1 = r1 >> 2;
    /* 0x0800A720: 0818     adds     r0, r1, r0 */
    r0 = r1 + r0;
    /* 0x0800A722: 1e28     cmp      r0, #0x1e */
    /* flags = cmp(r0, #0x1e) */
    /* 0x0800A724: 00d2     bhs      #0x800a728 */
    if (r0 >= 0x1e) goto L_0800A728;   /* unsigned */
    /* 0x0800A726: 1e20     movs     r0, #0x1e */
    r0 = 0x1e;

    L_0800A728:
    /* 0x0800A728: 1e38     subs     r0, #0x1e */
    r0 = r0 - 0x1e;
    /* 0x0800A72A: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x0800A72C: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x0800A72E: 00d9     bls      #0x800a732 */
    if (r0 <= 0x64) goto L_0800A732;   /* unsigned */
    /* 0x0800A730: 6420     movs     r0, #0x64 */
    r0 = 0x64;

    L_0800A732:
    /* 0x0800A732: 0b49     ldr      r1, [pc, #0x2c]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x0800A760;   /* = RAM 0x20000134 */
    /* 0x0800A734: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A736: 8870     strb     r0, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r0;   /* store */
    /* 0x0800A738: 0a49     ldr      r1, [pc, #0x28]  ; =flash_0800D35A */
    r1 = *(uint32_t *)0x0800A764;   /* = flash data/const 0x0800D35A */
    /* 0x0800A73A: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x0800A73C: 085a     ldrh     r0, [r1, r0] */
    r0 = *(uint16_t *)(r1 + r0);
    /* 0x0800A73E: dae7     b        #0x800a6f6 */
    goto L_0800A6F6;   /* loop */

    L_0800A740:
    /* 0x0800A740: fcf73ef8 bl       #0x80067c0 */
    FUN_080067C0();   /* BL -> 0x080067C0 FUN_080067C0 */
    /* 0x0800A744: d7e7     b        #0x800a6f6 */
    goto L_0800A6F6;   /* loop */
}
