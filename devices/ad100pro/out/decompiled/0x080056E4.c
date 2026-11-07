/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080056E4
 * Name      : FUN_080056E4
 * End       : 0x080057D2
 * Size      : 238 bytes, 108 instructions decoded in body (functions.json insns=108, blocks=21)
 * Class     : function
 * Callers   : 2  -> 0x080056D0 FUN_080056D0, 0x080056D8 FUN_080056D8
 * Callees   : 3  -> 0x080030F0 FUN_080030F0, 0x08004E4C FUN_08004E4C, 0x08004E98 FUN_08004E98
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080056E4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080056E4: f3b5     push     {r0, r1, r4, r5, r6, r7, lr} */
    push({r0, r1, r4, r5, r6, r7, lr});
    /* 0x080056E6: 3b4a     ldr      r2, [pc, #0xec]  ; =RAM_20000165 */
    r2 = *(uint32_t *)0x080057D4;   /* = RAM 0x20000165 */
    /* 0x080056E8: 0125     movs     r5, #1 */
    r5 = 1;
    /* 0x080056EA: 1078     ldrb     r0, [r2] */
    r0 = *(uint8_t *)(r2);
    /* 0x080056EC: 83b0     sub      sp, #0xc */
    sp = sp - 0xc;
    /* 0x080056EE: 0c46     mov      r4, r1 */
    r4 = r1;
    /* 0x080056F0: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080056F2: 04d0     beq      #0x80056fe */
    if (r0 == 0) goto L_080056FE;
    /* 0x080056F4: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x080056F6: 03d3     blo      #0x8005700 */
    if (r0 < 0x64) goto L_08005700;   /* unsigned */
    /* 0x080056F8: 6438     subs     r0, #0x64 */
    r0 = r0 - 0x64;
    /* 0x080056FA: c6b2     uxtb     r6, r0 */
    r6 = uxtb(r0);
    /* 0x080056FC: 01e0     b        #0x8005702 */
    goto L_08005702;

    L_080056FE:
    /* 0x080056FE: 1570     strb     r5, [r2] */
    *(uint8_t *)(r2) = r5;   /* store */

    L_08005700:
    /* 0x08005700: 1678     ldrb     r6, [r2] */
    r6 = *(uint8_t *)(r2);

    L_08005702:
    /* 0x08005702: 6421     movs     r1, #0x64 */
    r1 = 0x64;
    /* 0x08005704: 1078     ldrb     r0, [r2] */
    r0 = *(uint8_t *)(r2);
    /* 0x08005706: fdf7f3fc bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x0800570A: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x0800570C: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x0800570E: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x08005710: fdf7eefc bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x08005714: 304e     ldr      r6, [pc, #0xc0]  ; =RAM_20000177 */
    r6 = *(uint32_t *)0x080057D8;   /* = RAM 0x20000177 */
    /* 0x08005716: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x08005718: 0191     str      r1, [sp, #4] */
    *(uint32_t *)(sp + 4) = r1;   /* store */
    /* 0x0800571A: 3078     ldrb     r0, [r6] */
    r0 = *(uint8_t *)(r6);
    /* 0x0800571C: 2f4f     ldr      r7, [pc, #0xbc]  ; =RAM_20000228 */
    r7 = *(uint32_t *)0x080057DC;   /* = RAM 0x20000228 */
    /* 0x0800571E: 0428     cmp      r0, #4 */
    /* flags = cmp(r0, #4) */
    /* 0x08005720: 02d1     bne      #0x8005728 */
    if (r0 != 4) goto L_08005728;
    /* 0x08005722: 3879     ldrb     r0, [r7, #4] */
    r0 = *(uint8_t *)(r7 + 4);
    /* 0x08005724: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08005726: 0ed1     bne      #0x8005746 */
    if (r0 != 1) goto L_08005746;

    L_08005728:
    /* 0x08005728: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800572A: 1043     orrs     r0, r2 */
    r0 = r0 | r2;
    /* 0x0800572C: 04d0     beq      #0x8005738 */
    if (r0 == 1) goto L_08005738;
    /* 0x0800572E: 0323     movs     r3, #3 */
    r3 = 3;
    /* 0x08005730: 2146     mov      r1, r4 */
    r1 = r4;
    /* 0x08005732: 0398     ldr      r0, [sp, #0xc] */
    r0 = *(uint32_t *)(sp + 12);
    /* 0x08005734: fff7b0fb bl       #0x8004e98 */
    FUN_08004E98();   /* BL -> 0x08004E98 FUN_08004E98 */

    L_08005738:
    /* 0x08005738: a01d     adds     r0, r4, #6 */
    r0 = r4 + 6;
    /* 0x0800573A: c1b2     uxtb     r1, r0 */
    r1 = uxtb(r0);
    /* 0x0800573C: 0323     movs     r3, #3 */
    r3 = 3;
    /* 0x0800573E: 019a     ldr      r2, [sp, #4] */
    r2 = *(uint32_t *)(sp + 4);
    /* 0x08005740: 0398     ldr      r0, [sp, #0xc] */
    r0 = *(uint32_t *)(sp + 12);
    /* 0x08005742: fff7a9fb bl       #0x8004e98 */
    FUN_08004E98();   /* BL -> 0x08004E98 FUN_08004E98 */

    L_08005746:
    /* 0x08005746: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08005748: 0e30     adds     r0, #0xe */
    r0 = r0 + 0xe;
    /* 0x0800574A: c1b2     uxtb     r1, r0 */
    r1 = uxtb(r0);
    /* 0x0800574C: 0223     movs     r3, #2 */
    r3 = 2;
    /* 0x0800574E: 4222     movs     r2, #0x42 */
    r2 = 0x42;
    /* 0x08005750: 0398     ldr      r0, [sp, #0xc] */
    r0 = *(uint32_t *)(sp + 12);
    /* 0x08005752: fff77bfb bl       #0x8004e4c */
    FUN_08004E4C();   /* BL -> 0x08004E4C FUN_08004E4C */
    /* 0x08005756: 2248     ldr      r0, [pc, #0x88]  ; =RAM_20000166 */
    r0 = *(uint32_t *)0x080057E0;   /* = RAM 0x20000166 */
    /* 0x08005758: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x0800575A: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x0800575C: 04d0     beq      #0x8005768 */
    if (r1 == 0) goto L_08005768;
    /* 0x0800575E: 6429     cmp      r1, #0x64 */
    /* flags = cmp(r1, #0x64) */
    /* 0x08005760: 03d3     blo      #0x800576a */
    if (r1 < 0x64) goto L_0800576A;   /* unsigned */
    /* 0x08005762: 6439     subs     r1, #0x64 */
    r1 = r1 - 0x64;
    /* 0x08005764: cdb2     uxtb     r5, r1 */
    r5 = uxtb(r1);
    /* 0x08005766: 01e0     b        #0x800576c */
    goto L_0800576C;

    L_08005768:
    /* 0x08005768: 0570     strb     r5, [r0] */
    *(uint8_t *)(r0) = r5;   /* store */

    L_0800576A:
    /* 0x0800576A: 0578     ldrb     r5, [r0] */
    r5 = *(uint8_t *)(r0);

    L_0800576C:
    /* 0x0800576C: 6421     movs     r1, #0x64 */
    r1 = 0x64;
    /* 0x0800576E: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08005770: fdf7befc bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x08005774: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08005776: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x08005778: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800577A: fdf7b9fc bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x0800577E: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x08005780: 1734     adds     r4, #0x17 */
    r4 = r4 + 0x17;
    /* 0x08005782: 3078     ldrb     r0, [r6] */
    r0 = *(uint8_t *)(r6);
    /* 0x08005784: 0d46     mov      r5, r1 */
    r5 = r1;
    /* 0x08005786: e4b2     uxtb     r4, r4 */
    r4 = uxtb(r4);
    /* 0x08005788: 0328     cmp      r0, #3 */
    /* flags = cmp(r0, #3) */
    /* 0x0800578A: 02d1     bne      #0x8005792 */
    if (r0 != 3) goto L_08005792;
    /* 0x0800578C: 3879     ldrb     r0, [r7, #4] */
    r0 = *(uint8_t *)(r7 + 4);
    /* 0x0800578E: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08005790: 0ed1     bne      #0x80057b0 */
    if (r0 != 1) goto L_080057B0;

    L_08005792:
    /* 0x08005792: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x08005794: 1043     orrs     r0, r2 */
    r0 = r0 | r2;
    /* 0x08005796: 04d0     beq      #0x80057a2 */
    if (r0 == 1) goto L_080057A2;
    /* 0x08005798: 0323     movs     r3, #3 */
    r3 = 3;
    /* 0x0800579A: 2146     mov      r1, r4 */
    r1 = r4;
    /* 0x0800579C: 0398     ldr      r0, [sp, #0xc] */
    r0 = *(uint32_t *)(sp + 12);
    /* 0x0800579E: fff77bfb bl       #0x8004e98 */
    FUN_08004E98();   /* BL -> 0x08004E98 FUN_08004E98 */

    L_080057A2:
    /* 0x080057A2: a01d     adds     r0, r4, #6 */
    r0 = r4 + 6;
    /* 0x080057A4: c1b2     uxtb     r1, r0 */
    r1 = uxtb(r0);
    /* 0x080057A6: 0323     movs     r3, #3 */
    r3 = 3;
    /* 0x080057A8: 2a46     mov      r2, r5 */
    r2 = r5;
    /* 0x080057AA: 0398     ldr      r0, [sp, #0xc] */
    r0 = *(uint32_t *)(sp + 12);
    /* 0x080057AC: fff774fb bl       #0x8004e98 */
    FUN_08004E98();   /* BL -> 0x08004E98 FUN_08004E98 */

    L_080057B0:
    /* 0x080057B0: 0e34     adds     r4, #0xe */
    r4 = r4 + 0xe;
    /* 0x080057B2: e4b2     uxtb     r4, r4 */
    r4 = uxtb(r4);
    /* 0x080057B4: 0223     movs     r3, #2 */
    r3 = 2;
    /* 0x080057B6: 1122     movs     r2, #0x11 */
    r2 = 0x11;
    /* 0x080057B8: 2146     mov      r1, r4 */
    r1 = r4;
    /* 0x080057BA: 0398     ldr      r0, [sp, #0xc] */
    r0 = *(uint32_t *)(sp + 12);
    /* 0x080057BC: fff746fb bl       #0x8004e4c */
    FUN_08004E4C();   /* BL -> 0x08004E4C FUN_08004E4C */
    /* 0x080057C0: a41d     adds     r4, r4, #6 */
    r4 = r4 + 6;
    /* 0x080057C2: e1b2     uxtb     r1, r4 */
    r1 = uxtb(r4);
    /* 0x080057C4: 0223     movs     r3, #2 */
    r3 = 2;
    /* 0x080057C6: 3d22     movs     r2, #0x3d */
    r2 = 0x3d;
    /* 0x080057C8: 0398     ldr      r0, [sp, #0xc] */
    r0 = *(uint32_t *)(sp + 12);
    /* 0x080057CA: fff73ffb bl       #0x8004e4c */
    FUN_08004E4C();   /* BL -> 0x08004E4C FUN_08004E4C */
    /* 0x080057CE: 05b0     add      sp, #0x14 */
    sp = sp + 0x14;
    /* 0x080057D0: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
