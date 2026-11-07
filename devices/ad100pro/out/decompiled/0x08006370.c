/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006370
 * Name      : FUN_08006370
 * End       : 0x08006408
 * Size      : 152 bytes, 73 instructions decoded in body (functions.json insns=73, blocks=18)
 * Class     : function
 * Callers   : 1  -> 0x0800642C FUN_0800642C
 * Callees   : 1  -> 0x08007426 FUN_08007426
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006370(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006370: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08006372: 254c     ldr      r4, [pc, #0x94]  ; =RAM_2000014A */
    r4 = *(uint32_t *)0x08006408;   /* = RAM 0x2000014A */
    /* 0x08006374: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x08006376: e17b     ldrb     r1, [r4, #0xf] */
    r1 = *(uint8_t *)(r4 + 15);
    /* 0x08006378: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x0800637A: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x0800637C: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x0800637E: 1ad1     bne      #0x80063b6 */
    if (r1 != 1) goto L_080063B6;
    /* 0x08006380: a17b     ldrb     r1, [r4, #0xe] */
    r1 = *(uint8_t *)(r4 + 14);
    /* 0x08006382: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x08006384: 17d1     bne      #0x80063b6 */
    if (r1 != 0) goto L_080063B6;
    /* 0x08006386: 2149     ldr      r1, [pc, #0x84]  ; =RAM_200001B9 */
    r1 = *(uint32_t *)0x0800640C;   /* = RAM 0x200001B9 */
    /* 0x08006388: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800638A: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x0800638C: 13d1     bne      #0x80063b6 */
    if (r1 != 0) goto L_080063B6;
    /* 0x0800638E: 2049     ldr      r1, [pc, #0x80]  ; =RAM_200001CF */
    r1 = *(uint32_t *)0x08006410;   /* = RAM 0x200001CF */
    /* 0x08006390: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08006392: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x08006394: 0fd1     bne      #0x80063b6 */
    if (r1 != 0) goto L_080063B6;
    /* 0x08006396: 1f49     ldr      r1, [pc, #0x7c]  ; =RAM_200001D0 */
    r1 = *(uint32_t *)0x08006414;   /* = RAM 0x200001D0 */
    /* 0x08006398: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800639A: 7b29     cmp      r1, #0x7b */
    /* flags = cmp(r1, #0x7b) */
    /* 0x0800639C: 0bd1     bne      #0x80063b6 */
    if (r1 != 0x7b) goto L_080063B6;
    /* 0x0800639E: 1e4e     ldr      r6, [pc, #0x78]  ; =RAM_200001CD */
    r6 = *(uint32_t *)0x08006418;   /* = RAM 0x200001CD */
    /* 0x080063A0: 3178     ldrb     r1, [r6] */
    r1 = *(uint8_t *)(r6);
    /* 0x080063A2: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x080063A4: 07d1     bne      #0x80063b6 */
    if (r1 != 0) goto L_080063B6;
    /* 0x080063A6: 1d49     ldr      r1, [pc, #0x74]  ; =RAM_2000011E */
    r1 = *(uint32_t *)0x0800641C;   /* = RAM 0x2000011E */
    /* 0x080063A8: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x080063AA: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x080063AC: 03d1     bne      #0x80063b6 */
    if (r1 != 0) goto L_080063B6;
    /* 0x080063AE: 1c49     ldr      r1, [pc, #0x70]  ; =RAM_20000138 */
    r1 = *(uint32_t *)0x08006420;   /* = RAM 0x20000138 */
    /* 0x080063B0: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x080063B2: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x080063B4: 05d0     beq      #0x80063c2 */
    if (r1 == 0) goto L_080063C2;

    L_080063B6:
    /* 0x080063B6: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x080063B8: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x080063BA: 01f034f8 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x080063BE: a570     strb     r5, [r4, #2] */
    *(uint8_t *)(r4 + 2) = r5;   /* store */
    /* 0x080063C0: 20e0     b        #0x8006404 */
    goto L_08006404;

    L_080063C2:
    /* 0x080063C2: 1849     ldr      r1, [pc, #0x60]  ; =RAM_20000235 */
    r1 = *(uint32_t *)0x08006424;   /* = RAM 0x20000235 */
    /* 0x080063C4: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x080063C6: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x080063C8: 11d0     beq      #0x80063ee */
    if (r1 == 1) goto L_080063EE;
    /* 0x080063CA: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x080063CC: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x080063CE: 01f02af8 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x080063D2: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x080063D4: a070     strb     r0, [r4, #2] */
    *(uint8_t *)(r4 + 2) = r0;   /* store */
    /* 0x080063D6: e18a     ldrh     r1, [r4, #0x16] */
    r1 = *(uint16_t *)(r4 + 22);
    /* 0x080063D8: 7d22     movs     r2, #0x7d */
    r2 = 0x7d;
    /* 0x080063DA: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x080063DC: 89b2     uxth     r1, r1 */
    r1 = uxth(r1);
    /* 0x080063DE: 1201     lsls     r2, r2, #4 */
    r2 = r2 << 4;
    /* 0x080063E0: e182     strh     r1, [r4, #0x16] */
    *(uint16_t *)(r4 + 22) = r1;   /* store */
    /* 0x080063E2: 9142     cmp      r1, r2 */
    /* flags = cmp(r1, r2) */
    /* 0x080063E4: 0ad9     bls      #0x80063fc */
    if (r1 <= r2) goto L_080063FC;   /* unsigned */
    /* 0x080063E6: 3178     ldrb     r1, [r6] */
    r1 = *(uint8_t *)(r6);
    /* 0x080063E8: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x080063EA: 06d0     beq      #0x80063fa */
    if (r1 == 0) goto L_080063FA;
    /* 0x080063EC: 06e0     b        #0x80063fc */
    goto L_080063FC;

    L_080063EE:
    /* 0x080063EE: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x080063F0: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x080063F2: 01f018f8 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x080063F6: a570     strb     r5, [r4, #2] */
    *(uint8_t *)(r4 + 2) = r5;   /* store */

    L_080063F8:
    /* 0x080063F8: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_080063FA:
    /* 0x080063FA: 3070     strb     r0, [r6] */
    *(uint8_t *)(r6) = r0;   /* store */

    L_080063FC:
    /* 0x080063FC: 0a48     ldr      r0, [pc, #0x28]  ; =RAM_200001B8 */
    r0 = *(uint32_t *)0x08006428;   /* = RAM 0x200001B8 */
    /* 0x080063FE: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08006400: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08006402: f9d1     bne      #0x80063f8 */
    if (r0 != 1) goto L_080063F8;

    L_08006404:
    /* 0x08006404: e582     strh     r5, [r4, #0x16] */
    *(uint16_t *)(r4 + 22) = r5;   /* store */
    /* 0x08006406: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
