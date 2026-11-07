/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A768
 * Name      : FUN_0800A768
 * End       : 0x0800A7F2
 * Size      : 138 bytes, 68 instructions decoded in body (functions.json insns=68, blocks=17)
 * Class     : function
 * Callers   : 2  -> 0x080066C0 FUN_080066C0, 0x080066F4 FUN_080066F4
 * Callees   : 1  -> 0x08006730 FUN_08006730
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A768(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A768: 30b5     push     {r4, r5, lr} */
    push({r4, r5, lr});
    /* 0x0800A76A: 2248     ldr      r0, [pc, #0x88]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800A7F4;   /* = RAM 0x20000175 */
    /* 0x0800A76C: 224d     ldr      r5, [pc, #0x88]  ; =RAM_200001D8 */
    r5 = *(uint32_t *)0x0800A7F8;   /* = RAM 0x200001D8 */
    /* 0x0800A76E: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800A770: 224c     ldr      r4, [pc, #0x88]  ; =RAM_200001C8 */
    r4 = *(uint32_t *)0x0800A7FC;   /* = RAM 0x200001C8 */
    /* 0x0800A772: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x0800A774: 08d0     beq      #0x800a788 */
    if (/* beq */) goto L_0800A788;
    /* 0x0800A776: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x0800A778: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x0800A77A: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800A77C: 25d0     beq      #0x800a7ca */
    if (r0 == 1) goto L_0800A7CA;
    /* 0x0800A77E: 2820     movs     r0, #0x28 */
    r0 = 0x28;
    /* 0x0800A780: 2880     strh     r0, [r5] */
    *(uint16_t *)(r5) = r0;   /* store */
    /* 0x0800A782: 1420     movs     r0, #0x14 */
    r0 = 0x14;
    /* 0x0800A784: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */
    /* 0x0800A786: 27e0     b        #0x800a7d8 */
    goto L_0800A7D8;

    L_0800A788:
    /* 0x0800A788: 1e49     ldr      r1, [pc, #0x78]  ; =RAM_2000017D */
    r1 = *(uint32_t *)0x0800A804;   /* = RAM 0x2000017D */
    /* 0x0800A78A: 1d48     ldr      r0, [pc, #0x74]  ; =RAM_200001BD */
    r0 = *(uint32_t *)0x0800A800;   /* = RAM 0x200001BD */
    /* 0x0800A78C: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800A78E: 1822     movs     r2, #0x18 */
    r2 = 0x18;
    /* 0x0800A790: 405c     ldrb     r0, [r0, r1] */
    r0 = *(uint8_t *)(r0 + r1);
    /* 0x0800A792: 1d49     ldr      r1, [pc, #0x74]  ; =RAM_200001B1 */
    r1 = *(uint32_t *)0x0800A808;   /* = RAM 0x200001B1 */
    /* 0x0800A794: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800A796: 511a     subs     r1, r2, r1 */
    r1 = r2 - r1;
    /* 0x0800A798: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x0800A79A: 8a00     lsls     r2, r1, #2 */
    r2 = r1 << 2;
    /* 0x0800A79C: 8918     adds     r1, r1, r2 */
    r1 = r1 + r2;
    /* 0x0800A79E: 8908     lsrs     r1, r1, #2 */
    r1 = r1 >> 2;
    /* 0x0800A7A0: 0818     adds     r0, r1, r0 */
    r0 = r1 + r0;
    /* 0x0800A7A2: 1528     cmp      r0, #0x15 */
    /* flags = cmp(r0, #0x15) */
    /* 0x0800A7A4: 00d2     bhs      #0x800a7a8 */
    if (r0 >= 0x15) goto L_0800A7A8;   /* unsigned */
    /* 0x0800A7A6: 1520     movs     r0, #0x15 */
    r0 = 0x15;

    L_0800A7A8:
    /* 0x0800A7A8: 1538     subs     r0, #0x15 */
    r0 = r0 - 0x15;
    /* 0x0800A7AA: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x0800A7AC: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x0800A7AE: 00d9     bls      #0x800a7b2 */
    if (r0 <= 0x64) goto L_0800A7B2;   /* unsigned */
    /* 0x0800A7B0: 6420     movs     r0, #0x64 */
    r0 = 0x64;

    L_0800A7B2:
    /* 0x0800A7B2: 1649     ldr      r1, [pc, #0x58]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x0800A80C;   /* = RAM 0x20000134 */
    /* 0x0800A7B4: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A7B6: 8870     strb     r0, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r0;   /* store */
    /* 0x0800A7B8: 1549     ldr      r1, [pc, #0x54]  ; =flash_0800D4C6 */
    r1 = *(uint32_t *)0x0800A810;   /* = flash data/const 0x0800D4C6 */
    /* 0x0800A7BA: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x0800A7BC: 085a     ldrh     r0, [r1, r0] */
    r0 = *(uint16_t *)(r1 + r0);
    /* 0x0800A7BE: 8105     lsls     r1, r0, #0x16 */
    r1 = r0 << 0x16;
    /* 0x0800A7C0: 890d     lsrs     r1, r1, #0x16 */
    r1 = r1 >> 0x16;
    /* 0x0800A7C2: 2980     strh     r1, [r5] */
    *(uint16_t *)(r5) = r1;   /* store */
    /* 0x0800A7C4: 800a     lsrs     r0, r0, #0xa */
    r0 = r0 >> 0xa;
    /* 0x0800A7C6: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */
    /* 0x0800A7C8: 01e0     b        #0x800a7ce */
    goto L_0800A7CE;

    L_0800A7CA:
    /* 0x0800A7CA: fbf7b1ff bl       #0x8006730 */
    FUN_08006730();   /* BL -> 0x08006730 FUN_08006730 */

    L_0800A7CE:
    /* 0x0800A7CE: 2888     ldrh     r0, [r5] */
    r0 = *(uint16_t *)(r5);
    /* 0x0800A7D0: 0c28     cmp      r0, #0xc */
    /* flags = cmp(r0, #0xc) */
    /* 0x0800A7D2: 01d2     bhs      #0x800a7d8 */
    if (r0 >= 0xc) goto L_0800A7D8;   /* unsigned */
    /* 0x0800A7D4: 0c20     movs     r0, #0xc */
    r0 = 0xc;
    /* 0x0800A7D6: 2880     strh     r0, [r5] */
    *(uint16_t *)(r5) = r0;   /* store */

    L_0800A7D8:
    /* 0x0800A7D8: 0e49     ldr      r1, [pc, #0x38]  ; =RAM_200001C6 */
    r1 = *(uint32_t *)0x0800A814;   /* = RAM 0x200001C6 */
    /* 0x0800A7DA: 2888     ldrh     r0, [r5] */
    r0 = *(uint16_t *)(r5);
    /* 0x0800A7DC: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */
    /* 0x0800A7DE: 2088     ldrh     r0, [r4] */
    r0 = *(uint16_t *)(r4);
    /* 0x0800A7E0: 3228     cmp      r0, #0x32 */
    /* flags = cmp(r0, #0x32) */
    /* 0x0800A7E2: 01d9     bls      #0x800a7e8 */
    if (r0 <= 0x32) goto L_0800A7E8;   /* unsigned */
    /* 0x0800A7E4: 3220     movs     r0, #0x32 */
    r0 = 0x32;
    /* 0x0800A7E6: 02e0     b        #0x800a7ee */
    goto L_0800A7EE;

    L_0800A7E8:
    /* 0x0800A7E8: 0f28     cmp      r0, #0xf */
    /* flags = cmp(r0, #0xf) */
    /* 0x0800A7EA: 01d2     bhs      #0x800a7f0 */
    if (r0 >= 0xf) goto L_0800A7F0;   /* unsigned */
    /* 0x0800A7EC: 0f20     movs     r0, #0xf */
    r0 = 0xf;

    L_0800A7EE:
    /* 0x0800A7EE: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */

    L_0800A7F0:
    /* 0x0800A7F0: 30bd     pop      {r4, r5, pc} */
    pop({r4, r5, pc});   /* return */
}
