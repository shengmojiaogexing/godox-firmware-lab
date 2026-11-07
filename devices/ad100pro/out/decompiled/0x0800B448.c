/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B448
 * Name      : FUN_0800B448
 * End       : 0x0800B4D2
 * Size      : 138 bytes, 68 instructions decoded in body (functions.json insns=68, blocks=17)
 * Class     : function
 * Callers   : 1  -> 0x080066C0 FUN_080066C0
 * Callees   : 1  -> 0x08006730 FUN_08006730
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B448(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B448: 30b5     push     {r4, r5, lr} */
    push({r4, r5, lr});
    /* 0x0800B44A: 2248     ldr      r0, [pc, #0x88]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800B4D4;   /* = RAM 0x20000175 */
    /* 0x0800B44C: 224d     ldr      r5, [pc, #0x88]  ; =RAM_200001D8 */
    r5 = *(uint32_t *)0x0800B4D8;   /* = RAM 0x200001D8 */
    /* 0x0800B44E: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800B450: 224c     ldr      r4, [pc, #0x88]  ; =RAM_200001C8 */
    r4 = *(uint32_t *)0x0800B4DC;   /* = RAM 0x200001C8 */
    /* 0x0800B452: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x0800B454: 08d0     beq      #0x800b468 */
    if (/* beq */) goto L_0800B468;
    /* 0x0800B456: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x0800B458: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x0800B45A: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800B45C: 25d0     beq      #0x800b4aa */
    if (r0 == 1) goto L_0800B4AA;
    /* 0x0800B45E: 2820     movs     r0, #0x28 */
    r0 = 0x28;
    /* 0x0800B460: 2880     strh     r0, [r5] */
    *(uint16_t *)(r5) = r0;   /* store */
    /* 0x0800B462: 1420     movs     r0, #0x14 */
    r0 = 0x14;
    /* 0x0800B464: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */
    /* 0x0800B466: 27e0     b        #0x800b4b8 */
    goto L_0800B4B8;

    L_0800B468:
    /* 0x0800B468: 1e49     ldr      r1, [pc, #0x78]  ; =RAM_2000017D */
    r1 = *(uint32_t *)0x0800B4E4;   /* = RAM 0x2000017D */
    /* 0x0800B46A: 1d48     ldr      r0, [pc, #0x74]  ; =RAM_200001BD */
    r0 = *(uint32_t *)0x0800B4E0;   /* = RAM 0x200001BD */
    /* 0x0800B46C: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800B46E: 1822     movs     r2, #0x18 */
    r2 = 0x18;
    /* 0x0800B470: 405c     ldrb     r0, [r0, r1] */
    r0 = *(uint8_t *)(r0 + r1);
    /* 0x0800B472: 1d49     ldr      r1, [pc, #0x74]  ; =RAM_200001B1 */
    r1 = *(uint32_t *)0x0800B4E8;   /* = RAM 0x200001B1 */
    /* 0x0800B474: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800B476: 511a     subs     r1, r2, r1 */
    r1 = r2 - r1;
    /* 0x0800B478: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x0800B47A: 8a00     lsls     r2, r1, #2 */
    r2 = r1 << 2;
    /* 0x0800B47C: 8918     adds     r1, r1, r2 */
    r1 = r1 + r2;
    /* 0x0800B47E: 8908     lsrs     r1, r1, #2 */
    r1 = r1 >> 2;
    /* 0x0800B480: 0818     adds     r0, r1, r0 */
    r0 = r1 + r0;
    /* 0x0800B482: 1e28     cmp      r0, #0x1e */
    /* flags = cmp(r0, #0x1e) */
    /* 0x0800B484: 00d2     bhs      #0x800b488 */
    if (r0 >= 0x1e) goto L_0800B488;   /* unsigned */
    /* 0x0800B486: 1e20     movs     r0, #0x1e */
    r0 = 0x1e;

    L_0800B488:
    /* 0x0800B488: 1e38     subs     r0, #0x1e */
    r0 = r0 - 0x1e;
    /* 0x0800B48A: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x0800B48C: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x0800B48E: 00d9     bls      #0x800b492 */
    if (r0 <= 0x64) goto L_0800B492;   /* unsigned */
    /* 0x0800B490: 6420     movs     r0, #0x64 */
    r0 = 0x64;

    L_0800B492:
    /* 0x0800B492: 1649     ldr      r1, [pc, #0x58]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x0800B4EC;   /* = RAM 0x20000134 */
    /* 0x0800B494: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800B496: 8870     strb     r0, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r0;   /* store */
    /* 0x0800B498: 1549     ldr      r1, [pc, #0x54]  ; =flash_0800D4C6 */
    r1 = *(uint32_t *)0x0800B4F0;   /* = flash data/const 0x0800D4C6 */
    /* 0x0800B49A: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x0800B49C: 085a     ldrh     r0, [r1, r0] */
    r0 = *(uint16_t *)(r1 + r0);
    /* 0x0800B49E: 8105     lsls     r1, r0, #0x16 */
    r1 = r0 << 0x16;
    /* 0x0800B4A0: 890d     lsrs     r1, r1, #0x16 */
    r1 = r1 >> 0x16;
    /* 0x0800B4A2: 2980     strh     r1, [r5] */
    *(uint16_t *)(r5) = r1;   /* store */
    /* 0x0800B4A4: 800a     lsrs     r0, r0, #0xa */
    r0 = r0 >> 0xa;
    /* 0x0800B4A6: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */
    /* 0x0800B4A8: 01e0     b        #0x800b4ae */
    goto L_0800B4AE;

    L_0800B4AA:
    /* 0x0800B4AA: fbf741f9 bl       #0x8006730 */
    FUN_08006730();   /* BL -> 0x08006730 FUN_08006730 */

    L_0800B4AE:
    /* 0x0800B4AE: 2888     ldrh     r0, [r5] */
    r0 = *(uint16_t *)(r5);
    /* 0x0800B4B0: 0c28     cmp      r0, #0xc */
    /* flags = cmp(r0, #0xc) */
    /* 0x0800B4B2: 01d2     bhs      #0x800b4b8 */
    if (r0 >= 0xc) goto L_0800B4B8;   /* unsigned */
    /* 0x0800B4B4: 0c20     movs     r0, #0xc */
    r0 = 0xc;
    /* 0x0800B4B6: 2880     strh     r0, [r5] */
    *(uint16_t *)(r5) = r0;   /* store */

    L_0800B4B8:
    /* 0x0800B4B8: 0e49     ldr      r1, [pc, #0x38]  ; =RAM_200001C6 */
    r1 = *(uint32_t *)0x0800B4F4;   /* = RAM 0x200001C6 */
    /* 0x0800B4BA: 2888     ldrh     r0, [r5] */
    r0 = *(uint16_t *)(r5);
    /* 0x0800B4BC: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */
    /* 0x0800B4BE: 2088     ldrh     r0, [r4] */
    r0 = *(uint16_t *)(r4);
    /* 0x0800B4C0: 3228     cmp      r0, #0x32 */
    /* flags = cmp(r0, #0x32) */
    /* 0x0800B4C2: 01d9     bls      #0x800b4c8 */
    if (r0 <= 0x32) goto L_0800B4C8;   /* unsigned */
    /* 0x0800B4C4: 3220     movs     r0, #0x32 */
    r0 = 0x32;
    /* 0x0800B4C6: 02e0     b        #0x800b4ce */
    goto L_0800B4CE;

    L_0800B4C8:
    /* 0x0800B4C8: 0f28     cmp      r0, #0xf */
    /* flags = cmp(r0, #0xf) */
    /* 0x0800B4CA: 01d2     bhs      #0x800b4d0 */
    if (r0 >= 0xf) goto L_0800B4D0;   /* unsigned */
    /* 0x0800B4CC: 0f20     movs     r0, #0xf */
    r0 = 0xf;

    L_0800B4CE:
    /* 0x0800B4CE: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */

    L_0800B4D0:
    /* 0x0800B4D0: 30bd     pop      {r4, r5, pc} */
    pop({r4, r5, pc});   /* return */
}
