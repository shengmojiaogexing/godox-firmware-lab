/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B3C0
 * Name      : FUN_0800B3C0
 * End       : 0x0800B426
 * Size      : 102 bytes, 48 instructions decoded in body (functions.json insns=48, blocks=10)
 * Class     : function
 * Callers   : 1  -> 0x080065CC FUN_080065CC
 * Callees   : 3  -> 0x08006770 FUN_08006770, 0x080067C0 FUN_080067C0, 0x0800AF88 FUN_0800AF88
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B3C0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B3C0: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800B3C2: 1948     ldr      r0, [pc, #0x64]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800B428;   /* = RAM 0x20000175 */
    /* 0x0800B3C4: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800B3C6: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x0800B3C8: 0ed0     beq      #0x800b3e8 */
    if (/* beq */) goto L_0800B3E8;
    /* 0x0800B3CA: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x0800B3CC: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x0800B3CE: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x0800B3D0: 26d0     beq      #0x800b420 */
    if (r0 == 2) goto L_0800B420;
    /* 0x0800B3D2: fbf7cdf9 bl       #0x8006770 */
    FUN_08006770();   /* BL -> 0x08006770 FUN_08006770 */

    L_0800B3D6:
    /* 0x0800B3D6: 1549     ldr      r1, [pc, #0x54]  ; =RAM_200001B6 */
    r1 = *(uint32_t *)0x0800B42C;   /* = RAM 0x200001B6 */
    /* 0x0800B3D8: ff22     movs     r2, #0xff */
    r2 = 0xff;
    /* 0x0800B3DA: f032     adds     r2, #0xf0 */
    r2 = r2 + 0xf0;
    /* 0x0800B3DC: 0988     ldrh     r1, [r1] */
    r1 = *(uint16_t *)(r1);
    /* 0x0800B3DE: fff7d3fd bl       #0x800af88 */
    FUN_0800AF88();   /* BL -> 0x0800AF88 FUN_0800AF88 */
    /* 0x0800B3E2: 1349     ldr      r1, [pc, #0x4c]  ; =RAM_200001C4 */
    r1 = *(uint32_t *)0x0800B430;   /* = RAM 0x200001C4 */
    /* 0x0800B3E4: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */
    /* 0x0800B3E6: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800B3E8:
    /* 0x0800B3E8: 1349     ldr      r1, [pc, #0x4c]  ; =RAM_2000017D */
    r1 = *(uint32_t *)0x0800B438;   /* = RAM 0x2000017D */
    /* 0x0800B3EA: 1248     ldr      r0, [pc, #0x48]  ; =RAM_200001BD */
    r0 = *(uint32_t *)0x0800B434;   /* = RAM 0x200001BD */
    /* 0x0800B3EC: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800B3EE: 1822     movs     r2, #0x18 */
    r2 = 0x18;
    /* 0x0800B3F0: 405c     ldrb     r0, [r0, r1] */
    r0 = *(uint8_t *)(r0 + r1);
    /* 0x0800B3F2: 1249     ldr      r1, [pc, #0x48]  ; =RAM_200001B1 */
    r1 = *(uint32_t *)0x0800B43C;   /* = RAM 0x200001B1 */
    /* 0x0800B3F4: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800B3F6: 511a     subs     r1, r2, r1 */
    r1 = r2 - r1;
    /* 0x0800B3F8: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x0800B3FA: 8a00     lsls     r2, r1, #2 */
    r2 = r1 << 2;
    /* 0x0800B3FC: 8918     adds     r1, r1, r2 */
    r1 = r1 + r2;
    /* 0x0800B3FE: 8908     lsrs     r1, r1, #2 */
    r1 = r1 >> 2;
    /* 0x0800B400: 0818     adds     r0, r1, r0 */
    r0 = r1 + r0;
    /* 0x0800B402: 1e28     cmp      r0, #0x1e */
    /* flags = cmp(r0, #0x1e) */
    /* 0x0800B404: 00d2     bhs      #0x800b408 */
    if (r0 >= 0x1e) goto L_0800B408;   /* unsigned */
    /* 0x0800B406: 1e20     movs     r0, #0x1e */
    r0 = 0x1e;

    L_0800B408:
    /* 0x0800B408: 1e38     subs     r0, #0x1e */
    r0 = r0 - 0x1e;
    /* 0x0800B40A: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x0800B40C: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x0800B40E: 00d9     bls      #0x800b412 */
    if (r0 <= 0x64) goto L_0800B412;   /* unsigned */
    /* 0x0800B410: 6420     movs     r0, #0x64 */
    r0 = 0x64;

    L_0800B412:
    /* 0x0800B412: 0b49     ldr      r1, [pc, #0x2c]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x0800B440;   /* = RAM 0x20000134 */
    /* 0x0800B414: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800B416: 8870     strb     r0, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r0;   /* store */
    /* 0x0800B418: 0a49     ldr      r1, [pc, #0x28]  ; =flash_0800D35A */
    r1 = *(uint32_t *)0x0800B444;   /* = flash data/const 0x0800D35A */
    /* 0x0800B41A: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x0800B41C: 085a     ldrh     r0, [r1, r0] */
    r0 = *(uint16_t *)(r1 + r0);
    /* 0x0800B41E: dae7     b        #0x800b3d6 */
    goto L_0800B3D6;   /* loop */

    L_0800B420:
    /* 0x0800B420: fbf7cef9 bl       #0x80067c0 */
    FUN_080067C0();   /* BL -> 0x080067C0 FUN_080067C0 */
    /* 0x0800B424: d7e7     b        #0x800b3d6 */
    goto L_0800B3D6;   /* loop */
}
