/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C230
 * Name      : FUN_0800C230
 * End       : 0x0800C2AA
 * Size      : 122 bytes, 53 instructions decoded in body (functions.json insns=53, blocks=10)
 * Class     : function
 * Callers   : 2  -> 0x0800A934 FUN_0800A934, 0x0800A9DE FUN_0800A9DE
 * Callees   : 7  -> 0x080047CC FUN_080047CC, 0x0800741C FUN_0800741C, 0x08007426 FUN_08007426, 0x080074F0 FUN_080074F0, 0x0800CA38 FUN_0800CA38, 0x0800D088 FUN_0800D088, 0x0800D0C4 FUN_0800D0C4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C230(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C230: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x0800C232: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800C234: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x0800C236: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x0800C238: 2021     movs     r1, #0x20 */
    r1 = 0x20;
    /* 0x0800C23A: 1c48     ldr      r0, [pc, #0x70]  ; =GPIOC.MODER */
    r0 = *(uint32_t *)0x0800C2AC;   /* = MMIO GPIOC+0x00 */
    /* 0x0800C23C: fbf7f3f8 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800C240: 1b4c     ldr      r4, [pc, #0x6c]  ; =RAM_200001E0 */
    r4 = *(uint32_t *)0x0800C2B0;   /* = RAM 0x200001E0 */
    /* 0x0800C242: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x0800C244: 2571     strb     r5, [r4, #4] */
    *(uint8_t *)(r4 + 4) = r5;   /* store */
    /* 0x0800C246: 2583     strh     r5, [r4, #0x18] */
    *(uint16_t *)(r4 + 24) = r5;   /* store */
    /* 0x0800C248: 00f01eff bl       #0x800d088 */
    FUN_0800D088();   /* BL -> 0x0800D088 FUN_0800D088 */
    /* 0x0800C24C: 0126     movs     r6, #1 */
    r6 = 1;
    /* 0x0800C24E: 4b27     movs     r7, #0x4b */
    r7 = 0x4b;
    /* 0x0800C250: e670     strb     r6, [r4, #3] */
    *(uint8_t *)(r4 + 3) = r6;   /* store */
    /* 0x0800C252: ff00     lsls     r7, r7, #3 */
    r7 = r7 << 3;

    L_0800C254:
    /* 0x0800C254: 7d20     movs     r0, #0x7d */
    r0 = 0x7d;
    /* 0x0800C256: 0001     lsls     r0, r0, #4 */
    r0 = r0 << 4;
    /* 0x0800C258: 00f0eefb bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800C25C: 1548     ldr      r0, [pc, #0x54]  ; =RAM_20000384 */
    r0 = *(uint32_t *)0x0800C2B4;   /* = RAM 0x20000384 */
    /* 0x0800C25E: fbf747f9 bl       #0x80074f0 */
    FUN_080074F0();   /* BL -> 0x080074F0 FUN_080074F0 */
    /* 0x0800C262: f8f7b3fa bl       #0x80047cc */
    FUN_080047CC();   /* BL -> 0x080047CC FUN_080047CC */
    /* 0x0800C266: 00f02dff bl       #0x800d0c4 */
    FUN_0800D0C4();   /* BL -> 0x0800D0C4 FUN_0800D0C4 */
    /* 0x0800C26A: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800C26C: 0f48     ldr      r0, [pc, #0x3c]  ; =GPIOC.MODER */
    r0 = *(uint32_t *)0x0800C2AC;   /* = MMIO GPIOC+0x00 */
    /* 0x0800C26E: fbf7d5f8 bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x0800C272: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800C274: 00d1     bne      #0x800c278 */
    if (r0 != 0) goto L_0800C278;
    /* 0x0800C276: 2583     strh     r5, [r4, #0x18] */
    *(uint16_t *)(r4 + 24) = r5;   /* store */

    L_0800C278:
    /* 0x0800C278: 208b     ldrh     r0, [r4, #0x18] */
    r0 = *(uint16_t *)(r4 + 24);
    /* 0x0800C27A: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800C27C: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x0800C27E: 2083     strh     r0, [r4, #0x18] */
    *(uint16_t *)(r4 + 24) = r0;   /* store */
    /* 0x0800C280: b842     cmp      r0, r7 */
    /* flags = cmp(r0, r7) */
    /* 0x0800C282: 04d3     blo      #0x800c28e */
    if (r0 < r7) goto L_0800C28E;   /* unsigned */
    /* 0x0800C284: 2571     strb     r5, [r4, #4] */
    *(uint8_t *)(r4 + 4) = r5;   /* store */
    /* 0x0800C286: e078     ldrb     r0, [r4, #3] */
    r0 = *(uint8_t *)(r4 + 3);
    /* 0x0800C288: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x0800C28A: 03d8     bhi      #0x800c294 */
    if (r0 > 2) goto L_0800C294;   /* unsigned */
    /* 0x0800C28C: 0be0     b        #0x800c2a6 */
    goto L_0800C2A6;

    L_0800C28E:
    /* 0x0800C28E: e078     ldrb     r0, [r4, #3] */
    r0 = *(uint8_t *)(r4 + 3);
    /* 0x0800C290: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x0800C292: 05d9     bls      #0x800c2a0 */
    if (r0 <= 2) goto L_0800C2A0;   /* unsigned */

    L_0800C294:
    /* 0x0800C294: 2671     strb     r6, [r4, #4] */
    *(uint8_t *)(r4 + 4) = r6;   /* store */
    /* 0x0800C296: 0420     movs     r0, #4 */
    r0 = 4;
    /* 0x0800C298: e070     strb     r0, [r4, #3] */
    *(uint8_t *)(r4 + 3) = r0;   /* store */
    /* 0x0800C29A: 00f013ff bl       #0x800d0c4 */
    FUN_0800D0C4();   /* BL -> 0x0800D0C4 FUN_0800D0C4 */
    /* 0x0800C29E: 02e0     b        #0x800c2a6 */
    goto L_0800C2A6;

    L_0800C2A0:
    /* 0x0800C2A0: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800C2A2: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800C2A4: d6d1     bne      #0x800c254 */
    if (r0 != 0) goto L_0800C254;

    L_0800C2A6:
    /* 0x0800C2A6: e570     strb     r5, [r4, #3] */
    *(uint8_t *)(r4 + 3) = r5;   /* store */
    /* 0x0800C2A8: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
