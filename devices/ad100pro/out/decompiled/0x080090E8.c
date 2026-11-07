/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080090E8
 * Name      : FUN_080090E8
 * End       : 0x08009172
 * Size      : 138 bytes, 68 instructions decoded in body (functions.json insns=68, blocks=17)
 * Class     : function
 * Callers   : 2  -> 0x080066C0 FUN_080066C0, 0x0800671C FUN_0800671C
 * Callees   : 1  -> 0x08006730 FUN_08006730
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080090E8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080090E8: 30b5     push     {r4, r5, lr} */
    push({r4, r5, lr});
    /* 0x080090EA: 2248     ldr      r0, [pc, #0x88]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x08009174;   /* = RAM 0x20000175 */
    /* 0x080090EC: 224d     ldr      r5, [pc, #0x88]  ; =RAM_200001D8 */
    r5 = *(uint32_t *)0x08009178;   /* = RAM 0x200001D8 */
    /* 0x080090EE: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x080090F0: 224c     ldr      r4, [pc, #0x88]  ; =RAM_200001C8 */
    r4 = *(uint32_t *)0x0800917C;   /* = RAM 0x200001C8 */
    /* 0x080090F2: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x080090F4: 08d0     beq      #0x8009108 */
    if (/* beq */) goto L_08009108;
    /* 0x080090F6: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x080090F8: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x080090FA: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x080090FC: 25d0     beq      #0x800914a */
    if (r0 == 1) goto L_0800914A;
    /* 0x080090FE: 2820     movs     r0, #0x28 */
    r0 = 0x28;
    /* 0x08009100: 2880     strh     r0, [r5] */
    *(uint16_t *)(r5) = r0;   /* store */
    /* 0x08009102: 1420     movs     r0, #0x14 */
    r0 = 0x14;
    /* 0x08009104: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */
    /* 0x08009106: 27e0     b        #0x8009158 */
    goto L_08009158;

    L_08009108:
    /* 0x08009108: 1e49     ldr      r1, [pc, #0x78]  ; =RAM_2000017D */
    r1 = *(uint32_t *)0x08009184;   /* = RAM 0x2000017D */
    /* 0x0800910A: 1d48     ldr      r0, [pc, #0x74]  ; =RAM_200001BD */
    r0 = *(uint32_t *)0x08009180;   /* = RAM 0x200001BD */
    /* 0x0800910C: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800910E: 1822     movs     r2, #0x18 */
    r2 = 0x18;
    /* 0x08009110: 405c     ldrb     r0, [r0, r1] */
    r0 = *(uint8_t *)(r0 + r1);
    /* 0x08009112: 1d49     ldr      r1, [pc, #0x74]  ; =RAM_200001B1 */
    r1 = *(uint32_t *)0x08009188;   /* = RAM 0x200001B1 */
    /* 0x08009114: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08009116: 511a     subs     r1, r2, r1 */
    r1 = r2 - r1;
    /* 0x08009118: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x0800911A: 8a00     lsls     r2, r1, #2 */
    r2 = r1 << 2;
    /* 0x0800911C: 8918     adds     r1, r1, r2 */
    r1 = r1 + r2;
    /* 0x0800911E: 8908     lsrs     r1, r1, #2 */
    r1 = r1 >> 2;
    /* 0x08009120: 0818     adds     r0, r1, r0 */
    r0 = r1 + r0;
    /* 0x08009122: 0c28     cmp      r0, #0xc */
    /* flags = cmp(r0, #0xc) */
    /* 0x08009124: 00d2     bhs      #0x8009128 */
    if (r0 >= 0xc) goto L_08009128;   /* unsigned */
    /* 0x08009126: 0c20     movs     r0, #0xc */
    r0 = 0xc;

    L_08009128:
    /* 0x08009128: 0c38     subs     r0, #0xc */
    r0 = r0 - 0xc;
    /* 0x0800912A: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x0800912C: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x0800912E: 00d9     bls      #0x8009132 */
    if (r0 <= 0x64) goto L_08009132;   /* unsigned */
    /* 0x08009130: 6420     movs     r0, #0x64 */
    r0 = 0x64;

    L_08009132:
    /* 0x08009132: 1649     ldr      r1, [pc, #0x58]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x0800918C;   /* = RAM 0x20000134 */
    /* 0x08009134: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08009136: 8870     strb     r0, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r0;   /* store */
    /* 0x08009138: 1549     ldr      r1, [pc, #0x54]  ; =flash_0800D4C6 */
    r1 = *(uint32_t *)0x08009190;   /* = flash data/const 0x0800D4C6 */
    /* 0x0800913A: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x0800913C: 085a     ldrh     r0, [r1, r0] */
    r0 = *(uint16_t *)(r1 + r0);
    /* 0x0800913E: 8105     lsls     r1, r0, #0x16 */
    r1 = r0 << 0x16;
    /* 0x08009140: 890d     lsrs     r1, r1, #0x16 */
    r1 = r1 >> 0x16;
    /* 0x08009142: 2980     strh     r1, [r5] */
    *(uint16_t *)(r5) = r1;   /* store */
    /* 0x08009144: 800a     lsrs     r0, r0, #0xa */
    r0 = r0 >> 0xa;
    /* 0x08009146: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */
    /* 0x08009148: 01e0     b        #0x800914e */
    goto L_0800914E;

    L_0800914A:
    /* 0x0800914A: fdf7f1fa bl       #0x8006730 */
    FUN_08006730();   /* BL -> 0x08006730 FUN_08006730 */

    L_0800914E:
    /* 0x0800914E: 2888     ldrh     r0, [r5] */
    r0 = *(uint16_t *)(r5);
    /* 0x08009150: 0c28     cmp      r0, #0xc */
    /* flags = cmp(r0, #0xc) */
    /* 0x08009152: 01d2     bhs      #0x8009158 */
    if (r0 >= 0xc) goto L_08009158;   /* unsigned */
    /* 0x08009154: 0c20     movs     r0, #0xc */
    r0 = 0xc;
    /* 0x08009156: 2880     strh     r0, [r5] */
    *(uint16_t *)(r5) = r0;   /* store */

    L_08009158:
    /* 0x08009158: 0e49     ldr      r1, [pc, #0x38]  ; =RAM_200001C6 */
    r1 = *(uint32_t *)0x08009194;   /* = RAM 0x200001C6 */
    /* 0x0800915A: 2888     ldrh     r0, [r5] */
    r0 = *(uint16_t *)(r5);
    /* 0x0800915C: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */
    /* 0x0800915E: 2088     ldrh     r0, [r4] */
    r0 = *(uint16_t *)(r4);
    /* 0x08009160: 3228     cmp      r0, #0x32 */
    /* flags = cmp(r0, #0x32) */
    /* 0x08009162: 01d9     bls      #0x8009168 */
    if (r0 <= 0x32) goto L_08009168;   /* unsigned */
    /* 0x08009164: 3220     movs     r0, #0x32 */
    r0 = 0x32;
    /* 0x08009166: 02e0     b        #0x800916e */
    goto L_0800916E;

    L_08009168:
    /* 0x08009168: 0f28     cmp      r0, #0xf */
    /* flags = cmp(r0, #0xf) */
    /* 0x0800916A: 01d2     bhs      #0x8009170 */
    if (r0 >= 0xf) goto L_08009170;   /* unsigned */
    /* 0x0800916C: 0f20     movs     r0, #0xf */
    r0 = 0xf;

    L_0800916E:
    /* 0x0800916E: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */

    L_08009170:
    /* 0x08009170: 30bd     pop      {r4, r5, pc} */
    pop({r4, r5, pc});   /* return */
}
