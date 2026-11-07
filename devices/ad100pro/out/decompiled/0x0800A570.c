/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A570
 * Name      : FUN_0800A570
 * End       : 0x0800A5FA
 * Size      : 138 bytes, 68 instructions decoded in body (functions.json insns=68, blocks=17)
 * Class     : function
 * Callers   : 2  -> 0x080066C0 FUN_080066C0, 0x080066F4 FUN_080066F4
 * Callees   : 1  -> 0x08006730 FUN_08006730
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A570(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A570: 30b5     push     {r4, r5, lr} */
    push({r4, r5, lr});
    /* 0x0800A572: 2248     ldr      r0, [pc, #0x88]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800A5FC;   /* = RAM 0x20000175 */
    /* 0x0800A574: 224d     ldr      r5, [pc, #0x88]  ; =RAM_200001D8 */
    r5 = *(uint32_t *)0x0800A600;   /* = RAM 0x200001D8 */
    /* 0x0800A576: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800A578: 224c     ldr      r4, [pc, #0x88]  ; =RAM_200001C8 */
    r4 = *(uint32_t *)0x0800A604;   /* = RAM 0x200001C8 */
    /* 0x0800A57A: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x0800A57C: 08d0     beq      #0x800a590 */
    if (/* beq */) goto L_0800A590;
    /* 0x0800A57E: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x0800A580: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x0800A582: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800A584: 25d0     beq      #0x800a5d2 */
    if (r0 == 1) goto L_0800A5D2;
    /* 0x0800A586: 2820     movs     r0, #0x28 */
    r0 = 0x28;
    /* 0x0800A588: 2880     strh     r0, [r5] */
    *(uint16_t *)(r5) = r0;   /* store */
    /* 0x0800A58A: 1420     movs     r0, #0x14 */
    r0 = 0x14;
    /* 0x0800A58C: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */
    /* 0x0800A58E: 27e0     b        #0x800a5e0 */
    goto L_0800A5E0;

    L_0800A590:
    /* 0x0800A590: 1e49     ldr      r1, [pc, #0x78]  ; =RAM_2000017D */
    r1 = *(uint32_t *)0x0800A60C;   /* = RAM 0x2000017D */
    /* 0x0800A592: 1d48     ldr      r0, [pc, #0x74]  ; =RAM_200001BD */
    r0 = *(uint32_t *)0x0800A608;   /* = RAM 0x200001BD */
    /* 0x0800A594: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800A596: 1822     movs     r2, #0x18 */
    r2 = 0x18;
    /* 0x0800A598: 405c     ldrb     r0, [r0, r1] */
    r0 = *(uint8_t *)(r0 + r1);
    /* 0x0800A59A: 1d49     ldr      r1, [pc, #0x74]  ; =RAM_200001B1 */
    r1 = *(uint32_t *)0x0800A610;   /* = RAM 0x200001B1 */
    /* 0x0800A59C: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800A59E: 511a     subs     r1, r2, r1 */
    r1 = r2 - r1;
    /* 0x0800A5A0: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x0800A5A2: 8a00     lsls     r2, r1, #2 */
    r2 = r1 << 2;
    /* 0x0800A5A4: 8918     adds     r1, r1, r2 */
    r1 = r1 + r2;
    /* 0x0800A5A6: 8908     lsrs     r1, r1, #2 */
    r1 = r1 >> 2;
    /* 0x0800A5A8: 0818     adds     r0, r1, r0 */
    r0 = r1 + r0;
    /* 0x0800A5AA: 2028     cmp      r0, #0x20 */
    /* flags = cmp(r0, #0x20) */
    /* 0x0800A5AC: 00d2     bhs      #0x800a5b0 */
    if (r0 >= 0x20) goto L_0800A5B0;   /* unsigned */
    /* 0x0800A5AE: 2020     movs     r0, #0x20 */
    r0 = 0x20;

    L_0800A5B0:
    /* 0x0800A5B0: 2038     subs     r0, #0x20 */
    r0 = r0 - 0x20;
    /* 0x0800A5B2: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x0800A5B4: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x0800A5B6: 00d9     bls      #0x800a5ba */
    if (r0 <= 0x64) goto L_0800A5BA;   /* unsigned */
    /* 0x0800A5B8: 6420     movs     r0, #0x64 */
    r0 = 0x64;

    L_0800A5BA:
    /* 0x0800A5BA: 1649     ldr      r1, [pc, #0x58]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x0800A614;   /* = RAM 0x20000134 */
    /* 0x0800A5BC: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A5BE: 8870     strb     r0, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r0;   /* store */
    /* 0x0800A5C0: 1549     ldr      r1, [pc, #0x54]  ; =flash_0800D4C6 */
    r1 = *(uint32_t *)0x0800A618;   /* = flash data/const 0x0800D4C6 */
    /* 0x0800A5C2: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x0800A5C4: 085a     ldrh     r0, [r1, r0] */
    r0 = *(uint16_t *)(r1 + r0);
    /* 0x0800A5C6: 8105     lsls     r1, r0, #0x16 */
    r1 = r0 << 0x16;
    /* 0x0800A5C8: 890d     lsrs     r1, r1, #0x16 */
    r1 = r1 >> 0x16;
    /* 0x0800A5CA: 2980     strh     r1, [r5] */
    *(uint16_t *)(r5) = r1;   /* store */
    /* 0x0800A5CC: 800a     lsrs     r0, r0, #0xa */
    r0 = r0 >> 0xa;
    /* 0x0800A5CE: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */
    /* 0x0800A5D0: 01e0     b        #0x800a5d6 */
    goto L_0800A5D6;

    L_0800A5D2:
    /* 0x0800A5D2: fcf7adf8 bl       #0x8006730 */
    FUN_08006730();   /* BL -> 0x08006730 FUN_08006730 */

    L_0800A5D6:
    /* 0x0800A5D6: 2888     ldrh     r0, [r5] */
    r0 = *(uint16_t *)(r5);
    /* 0x0800A5D8: 0c28     cmp      r0, #0xc */
    /* flags = cmp(r0, #0xc) */
    /* 0x0800A5DA: 01d2     bhs      #0x800a5e0 */
    if (r0 >= 0xc) goto L_0800A5E0;   /* unsigned */
    /* 0x0800A5DC: 0c20     movs     r0, #0xc */
    r0 = 0xc;
    /* 0x0800A5DE: 2880     strh     r0, [r5] */
    *(uint16_t *)(r5) = r0;   /* store */

    L_0800A5E0:
    /* 0x0800A5E0: 0e49     ldr      r1, [pc, #0x38]  ; =RAM_200001C6 */
    r1 = *(uint32_t *)0x0800A61C;   /* = RAM 0x200001C6 */
    /* 0x0800A5E2: 2888     ldrh     r0, [r5] */
    r0 = *(uint16_t *)(r5);
    /* 0x0800A5E4: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */
    /* 0x0800A5E6: 2088     ldrh     r0, [r4] */
    r0 = *(uint16_t *)(r4);
    /* 0x0800A5E8: 3228     cmp      r0, #0x32 */
    /* flags = cmp(r0, #0x32) */
    /* 0x0800A5EA: 01d9     bls      #0x800a5f0 */
    if (r0 <= 0x32) goto L_0800A5F0;   /* unsigned */
    /* 0x0800A5EC: 3220     movs     r0, #0x32 */
    r0 = 0x32;
    /* 0x0800A5EE: 02e0     b        #0x800a5f6 */
    goto L_0800A5F6;

    L_0800A5F0:
    /* 0x0800A5F0: 0f28     cmp      r0, #0xf */
    /* flags = cmp(r0, #0xf) */
    /* 0x0800A5F2: 01d2     bhs      #0x800a5f8 */
    if (r0 >= 0xf) goto L_0800A5F8;   /* unsigned */
    /* 0x0800A5F4: 0f20     movs     r0, #0xf */
    r0 = 0xf;

    L_0800A5F6:
    /* 0x0800A5F6: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */

    L_0800A5F8:
    /* 0x0800A5F8: 30bd     pop      {r4, r5, pc} */
    pop({r4, r5, pc});   /* return */
}
