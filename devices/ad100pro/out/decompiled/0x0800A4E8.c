/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A4E8
 * Name      : FUN_0800A4E8
 * End       : 0x0800A54E
 * Size      : 102 bytes, 48 instructions decoded in body (functions.json insns=48, blocks=10)
 * Class     : function
 * Callers   : 4  -> 0x080065CC FUN_080065CC, 0x08006600 FUN_08006600, 0x0800C49C FUN_0800C49C, 0x0800C4BC FUN_0800C4BC
 * Callees   : 3  -> 0x08006770 FUN_08006770, 0x080067C0 FUN_080067C0, 0x0800AF88 FUN_0800AF88
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A4E8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A4E8: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800A4EA: 1948     ldr      r0, [pc, #0x64]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800A550;   /* = RAM 0x20000175 */
    /* 0x0800A4EC: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800A4EE: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x0800A4F0: 0ed0     beq      #0x800a510 */
    if (/* beq */) goto L_0800A510;
    /* 0x0800A4F2: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x0800A4F4: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x0800A4F6: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x0800A4F8: 26d0     beq      #0x800a548 */
    if (r0 == 2) goto L_0800A548;
    /* 0x0800A4FA: fcf739f9 bl       #0x8006770 */
    FUN_08006770();   /* BL -> 0x08006770 FUN_08006770 */

    L_0800A4FE:
    /* 0x0800A4FE: 1549     ldr      r1, [pc, #0x54]  ; =RAM_200001B6 */
    r1 = *(uint32_t *)0x0800A554;   /* = RAM 0x200001B6 */
    /* 0x0800A500: ff22     movs     r2, #0xff */
    r2 = 0xff;
    /* 0x0800A502: f032     adds     r2, #0xf0 */
    r2 = r2 + 0xf0;
    /* 0x0800A504: 0988     ldrh     r1, [r1] */
    r1 = *(uint16_t *)(r1);
    /* 0x0800A506: 00f03ffd bl       #0x800af88 */
    FUN_0800AF88();   /* BL -> 0x0800AF88 FUN_0800AF88 */
    /* 0x0800A50A: 1349     ldr      r1, [pc, #0x4c]  ; =RAM_200001C4 */
    r1 = *(uint32_t *)0x0800A558;   /* = RAM 0x200001C4 */
    /* 0x0800A50C: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */
    /* 0x0800A50E: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800A510:
    /* 0x0800A510: 1349     ldr      r1, [pc, #0x4c]  ; =RAM_2000017D */
    r1 = *(uint32_t *)0x0800A560;   /* = RAM 0x2000017D */
    /* 0x0800A512: 1248     ldr      r0, [pc, #0x48]  ; =RAM_200001BD */
    r0 = *(uint32_t *)0x0800A55C;   /* = RAM 0x200001BD */
    /* 0x0800A514: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800A516: 1822     movs     r2, #0x18 */
    r2 = 0x18;
    /* 0x0800A518: 405c     ldrb     r0, [r0, r1] */
    r0 = *(uint8_t *)(r0 + r1);
    /* 0x0800A51A: 1249     ldr      r1, [pc, #0x48]  ; =RAM_200001B1 */
    r1 = *(uint32_t *)0x0800A564;   /* = RAM 0x200001B1 */
    /* 0x0800A51C: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800A51E: 511a     subs     r1, r2, r1 */
    r1 = r2 - r1;
    /* 0x0800A520: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x0800A522: 8a00     lsls     r2, r1, #2 */
    r2 = r1 << 2;
    /* 0x0800A524: 8918     adds     r1, r1, r2 */
    r1 = r1 + r2;
    /* 0x0800A526: 8908     lsrs     r1, r1, #2 */
    r1 = r1 >> 2;
    /* 0x0800A528: 0818     adds     r0, r1, r0 */
    r0 = r1 + r0;
    /* 0x0800A52A: 1528     cmp      r0, #0x15 */
    /* flags = cmp(r0, #0x15) */
    /* 0x0800A52C: 00d2     bhs      #0x800a530 */
    if (r0 >= 0x15) goto L_0800A530;   /* unsigned */
    /* 0x0800A52E: 1520     movs     r0, #0x15 */
    r0 = 0x15;

    L_0800A530:
    /* 0x0800A530: 1538     subs     r0, #0x15 */
    r0 = r0 - 0x15;
    /* 0x0800A532: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x0800A534: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x0800A536: 00d9     bls      #0x800a53a */
    if (r0 <= 0x64) goto L_0800A53A;   /* unsigned */
    /* 0x0800A538: 6420     movs     r0, #0x64 */
    r0 = 0x64;

    L_0800A53A:
    /* 0x0800A53A: 0b49     ldr      r1, [pc, #0x2c]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x0800A568;   /* = RAM 0x20000134 */
    /* 0x0800A53C: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A53E: 8870     strb     r0, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r0;   /* store */
    /* 0x0800A540: 0a49     ldr      r1, [pc, #0x28]  ; =flash_0800D35A */
    r1 = *(uint32_t *)0x0800A56C;   /* = flash data/const 0x0800D35A */
    /* 0x0800A542: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x0800A544: 085a     ldrh     r0, [r1, r0] */
    r0 = *(uint16_t *)(r1 + r0);
    /* 0x0800A546: dae7     b        #0x800a4fe */
    goto L_0800A4FE;   /* loop */

    L_0800A548:
    /* 0x0800A548: fcf73af9 bl       #0x80067c0 */
    FUN_080067C0();   /* BL -> 0x080067C0 FUN_080067C0 */
    /* 0x0800A54C: d7e7     b        #0x800a4fe */
    goto L_0800A4FE;   /* loop */
}
