/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009060
 * Name      : FUN_08009060
 * End       : 0x080090C6
 * Size      : 102 bytes, 48 instructions decoded in body (functions.json insns=48, blocks=10)
 * Class     : function
 * Callers   : 3  -> 0x080065CC FUN_080065CC, 0x08006628 FUN_08006628, 0x0800C49C FUN_0800C49C
 * Callees   : 3  -> 0x08006770 FUN_08006770, 0x080067C0 FUN_080067C0, 0x0800AF88 FUN_0800AF88
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009060(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009060: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08009062: 1948     ldr      r0, [pc, #0x64]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x080090C8;   /* = RAM 0x20000175 */
    /* 0x08009064: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08009066: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x08009068: 0ed0     beq      #0x8009088 */
    if (/* beq */) goto L_08009088;
    /* 0x0800906A: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x0800906C: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x0800906E: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08009070: 26d0     beq      #0x80090c0 */
    if (r0 == 2) goto L_080090C0;
    /* 0x08009072: fdf77dfb bl       #0x8006770 */
    FUN_08006770();   /* BL -> 0x08006770 FUN_08006770 */

    L_08009076:
    /* 0x08009076: 1549     ldr      r1, [pc, #0x54]  ; =RAM_200001B6 */
    r1 = *(uint32_t *)0x080090CC;   /* = RAM 0x200001B6 */
    /* 0x08009078: ff22     movs     r2, #0xff */
    r2 = 0xff;
    /* 0x0800907A: f032     adds     r2, #0xf0 */
    r2 = r2 + 0xf0;
    /* 0x0800907C: 0988     ldrh     r1, [r1] */
    r1 = *(uint16_t *)(r1);
    /* 0x0800907E: 01f083ff bl       #0x800af88 */
    FUN_0800AF88();   /* BL -> 0x0800AF88 FUN_0800AF88 */
    /* 0x08009082: 1349     ldr      r1, [pc, #0x4c]  ; =RAM_200001C4 */
    r1 = *(uint32_t *)0x080090D0;   /* = RAM 0x200001C4 */
    /* 0x08009084: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */
    /* 0x08009086: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_08009088:
    /* 0x08009088: 1349     ldr      r1, [pc, #0x4c]  ; =RAM_2000017D */
    r1 = *(uint32_t *)0x080090D8;   /* = RAM 0x2000017D */
    /* 0x0800908A: 1248     ldr      r0, [pc, #0x48]  ; =RAM_200001BD */
    r0 = *(uint32_t *)0x080090D4;   /* = RAM 0x200001BD */
    /* 0x0800908C: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800908E: 1822     movs     r2, #0x18 */
    r2 = 0x18;
    /* 0x08009090: 405c     ldrb     r0, [r0, r1] */
    r0 = *(uint8_t *)(r0 + r1);
    /* 0x08009092: 1249     ldr      r1, [pc, #0x48]  ; =RAM_200001B1 */
    r1 = *(uint32_t *)0x080090DC;   /* = RAM 0x200001B1 */
    /* 0x08009094: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08009096: 511a     subs     r1, r2, r1 */
    r1 = r2 - r1;
    /* 0x08009098: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x0800909A: 8a00     lsls     r2, r1, #2 */
    r2 = r1 << 2;
    /* 0x0800909C: 8918     adds     r1, r1, r2 */
    r1 = r1 + r2;
    /* 0x0800909E: 8908     lsrs     r1, r1, #2 */
    r1 = r1 >> 2;
    /* 0x080090A0: 0818     adds     r0, r1, r0 */
    r0 = r1 + r0;
    /* 0x080090A2: 1b28     cmp      r0, #0x1b */
    /* flags = cmp(r0, #0x1b) */
    /* 0x080090A4: 00d2     bhs      #0x80090a8 */
    if (r0 >= 0x1b) goto L_080090A8;   /* unsigned */
    /* 0x080090A6: 1b20     movs     r0, #0x1b */
    r0 = 0x1b;

    L_080090A8:
    /* 0x080090A8: 1b38     subs     r0, #0x1b */
    r0 = r0 - 0x1b;
    /* 0x080090AA: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x080090AC: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x080090AE: 00d9     bls      #0x80090b2 */
    if (r0 <= 0x64) goto L_080090B2;   /* unsigned */
    /* 0x080090B0: 6420     movs     r0, #0x64 */
    r0 = 0x64;

    L_080090B2:
    /* 0x080090B2: 0b49     ldr      r1, [pc, #0x2c]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x080090E0;   /* = RAM 0x20000134 */
    /* 0x080090B4: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x080090B6: 8870     strb     r0, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r0;   /* store */
    /* 0x080090B8: 0a49     ldr      r1, [pc, #0x28]  ; =flash_0800D35A */
    r1 = *(uint32_t *)0x080090E4;   /* = flash data/const 0x0800D35A */
    /* 0x080090BA: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x080090BC: 085a     ldrh     r0, [r1, r0] */
    r0 = *(uint16_t *)(r1 + r0);
    /* 0x080090BE: dae7     b        #0x8009076 */
    goto L_08009076;   /* loop */

    L_080090C0:
    /* 0x080090C0: fdf77efb bl       #0x80067c0 */
    FUN_080067C0();   /* BL -> 0x080067C0 FUN_080067C0 */
    /* 0x080090C4: d7e7     b        #0x8009076 */
    goto L_08009076;   /* loop */
}
