/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080057E4
 * Name      : FUN_080057E4
 * End       : 0x080058AC
 * Size      : 200 bytes, 91 instructions decoded in body (functions.json insns=47, blocks=7)
 * Class     : function
 * Callers   : 1  -> 0x080056D0 FUN_080056D0
 * Callees   : 4  -> 0x080030F0 FUN_080030F0, 0x08004D28 FUN_08004D28, 0x08004D74 FUN_08004D74, 0x0800C8F2 FUN_0800C8F2
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080057E4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080057E4: f3b5     push     {r0, r1, r4, r5, r6, r7, lr} */
    push({r0, r1, r4, r5, r6, r7, lr});
    /* 0x080057E6: 3748     ldr      r0, [pc, #0xdc]  ; =RAM_2000017C */
    r0 = *(uint32_t *)0x080058C4;   /* = RAM 0x2000017C */
    /* 0x080057E8: 81b0     sub      sp, #4 */
    sp = sp - 4;
    /* 0x080057EA: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x080057EC: 0f46     mov      r7, r1 */
    r7 = r1;
    /* 0x080057EE: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x080057F0: 13d0     beq      #0x800581a */
    if (r0 == 1) goto L_0800581A;
    /* 0x080057F2: 0020     movs     r0, #0 */
    r0 = 0;

    L_080057F4:
    /* 0x080057F4: 3449     ldr      r1, [pc, #0xd0]  ; =RAM_200001A2 */
    r1 = *(uint32_t *)0x080058C8;   /* = RAM 0x200001A2 */
    /* 0x080057F6: 085c     ldrb     r0, [r1, r0] */
    r0 = *(uint8_t *)(r1 + r0);
    /* 0x080057F8: ff28     cmp      r0, #0xff */
    /* flags = cmp(r0, #0xff) */
    /* 0x080057FA: 11d0     beq      #0x8005820 */
    if (r0 == 0xff) goto L_08005820;
    /* 0x080057FC: 5021     movs     r1, #0x50 */
    r1 = 0x50;
    /* 0x080057FE: 081a     subs     r0, r1, r0 */
    r0 = r1 - r0;
    /* 0x08005800: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08005802: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x08005804: fdf774fc bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x08005808: 0300     movs     r3, r0 */
    r3 = r0;
    /* 0x0800580A: 07f072f8 bl       #0x800c8f2 */
    FUN_0800C8F2();   /* BL -> 0x0800C8F2 FUN_0800C8F2 */
    /* 0x0800580E: 091b     subs     r1, r1, r4 */
    r1 = r1 - r4;
    /* 0x08005810: 1f23     movs     r3, #0x1f */
    r3 = 0x1f;
    /* 0x08005812: 264f     ldr      r7, [pc, #0x98]  ; =0x25062601 */
    r7 = *(uint32_t *)0x080058AC;   /* = 0x25062601 */
    /* 0x08005814: 5254     strb     r2, [r2, r1] */
    *(uint8_t *)(r2 + r1) = r2;   /* store */
    /* 0x08005816: 5658     ldr      r6, [r2, r1] */
    r6 = *(uint32_t *)(r2 + r1);
    /* 0x08005818: 2900     movs     r1, r5 */
    r1 = r5;

    L_0800581A:
    /* 0x0800581A: 2c48     ldr      r0, [pc, #0xb0]  ; =RAM_2000017D */
    r0 = *(uint32_t *)0x080058CC;   /* = RAM 0x2000017D */
    /* 0x0800581C: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800581E: e9e7     b        #0x80057f4 */
    goto L_080057F4;   /* loop */

    L_08005820:
    /* 0x08005820: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x08005822: 0d30     adds     r0, #0xd */
    r0 = r0 + 0xd;
    /* 0x08005824: c1b2     uxtb     r1, r0 */
    r1 = uxtb(r0);
    /* 0x08005826: 0a22     movs     r2, #0xa */
    r2 = 0xa;
    /* 0x08005828: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x0800582A: fff7a3fa bl       #0x8004d74 */
    FUN_08004D74();   /* BL -> 0x08004D74 FUN_08004D74 */
    /* 0x0800582E: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x08005830: 1b30     adds     r0, #0x1b */
    r0 = r0 + 0x1b;
    /* 0x08005832: c1b2     uxtb     r1, r0 */
    r1 = uxtb(r0);
    /* 0x08005834: 0b22     movs     r2, #0xb */
    r2 = 0xb;
    /* 0x08005836: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x08005838: fff79cfa bl       #0x8004d74 */
    FUN_08004D74();   /* BL -> 0x08004D74 FUN_08004D74 */
    /* 0x0800583C: 2637     adds     r7, #0x26 */
    r7 = r7 + 0x26;
    /* 0x0800583E: f9b2     uxtb     r1, r7 */
    r1 = uxtb(r7);
    /* 0x08005840: 0b22     movs     r2, #0xb */
    r2 = 0xb;
    /* 0x08005842: 2fe0     b        #0x80058a4 */
    goto L_080058A4;
    /* 0x08005844: 0226     movs     r6, #2 */
    r6 = 2;
    /* 0x08005846: 0525     movs     r5, #5 */
    r5 = 5;
    /* 0x08005848: 0624     movs     r4, #6 */
    r4 = 6;
    /* 0x0800584A: 09e0     b        #0x8005860 */
    goto L_08005860;
    /* 0x0800584C: 0126     movs     r6, #1 */
    r6 = 1;
    /* 0x0800584E: 0225     movs     r5, #2 */
    r5 = 2;
    /* 0x08005850: 0824     movs     r4, #8 */
    r4 = 8;
    /* 0x08005852: 05e0     b        #0x8005860 */
    goto L_08005860;
    /* 0x08005854: 0626     movs     r6, #6 */
    r6 = 6;
    /* 0x08005856: 0425     movs     r5, #4 */
    r5 = 4;
    /* 0x08005858: 01e0     b        #0x800585e */
    goto L_0800585E;
    /* 0x0800585A: 0326     movs     r6, #3 */
    r6 = 3;
    /* 0x0800585C: 0225     movs     r5, #2 */
    r5 = 2;

    L_0800585E:
    /* 0x0800585E: 0024     movs     r4, #0 */
    r4 = 0;

    L_08005860:
    /* 0x08005860: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08005862: 3946     mov      r1, r7 */
    r1 = r7;
    /* 0x08005864: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x08005866: fff75ffa bl       #0x8004d28 */
    FUN_08004D28();   /* BL -> 0x08004D28 FUN_08004D28 */
    /* 0x0800586A: b81d     adds     r0, r7, #6 */
    r0 = r7 + 6;
    /* 0x0800586C: c1b2     uxtb     r1, r0 */
    r1 = uxtb(r0);
    /* 0x0800586E: 0a22     movs     r2, #0xa */
    r2 = 0xa;
    /* 0x08005870: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x08005872: fff759fa bl       #0x8004d28 */
    FUN_08004D28();   /* BL -> 0x08004D28 FUN_08004D28 */
    /* 0x08005876: 002e     cmp      r6, #0 */
    /* flags = cmp(r6, #0) */
    /* 0x08005878: 06d0     beq      #0x8005888 */
    if (r6 == 0) goto L_08005888;
    /* 0x0800587A: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800587C: 0f30     adds     r0, #0xf */
    r0 = r0 + 0xf;
    /* 0x0800587E: c1b2     uxtb     r1, r0 */
    r1 = uxtb(r0);
    /* 0x08005880: 3246     mov      r2, r6 */
    r2 = r6;
    /* 0x08005882: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x08005884: fff776fa bl       #0x8004d74 */
    FUN_08004D74();   /* BL -> 0x08004D74 FUN_08004D74 */

    L_08005888:
    /* 0x08005888: 002d     cmp      r5, #0 */
    /* flags = cmp(r5, #0) */
    /* 0x0800588A: 06d0     beq      #0x800589a */
    if (r5 == 0) goto L_0800589A;
    /* 0x0800588C: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800588E: 1a30     adds     r0, #0x1a */
    r0 = r0 + 0x1a;
    /* 0x08005890: c1b2     uxtb     r1, r0 */
    r1 = uxtb(r0);
    /* 0x08005892: 2a46     mov      r2, r5 */
    r2 = r5;
    /* 0x08005894: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x08005896: fff76dfa bl       #0x8004d74 */
    FUN_08004D74();   /* BL -> 0x08004D74 FUN_08004D74 */

    L_0800589A:
    /* 0x0800589A: 002c     cmp      r4, #0 */
    /* flags = cmp(r4, #0) */
    /* 0x0800589C: 05d0     beq      #0x80058aa */
    if (r4 == 0) goto L_080058AA;
    /* 0x0800589E: 2537     adds     r7, #0x25 */
    r7 = r7 + 0x25;
    /* 0x080058A0: f9b2     uxtb     r1, r7 */
    r1 = uxtb(r7);
    /* 0x080058A2: 2246     mov      r2, r4 */
    r2 = r4;

    L_080058A4:
    /* 0x080058A4: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x080058A6: fff765fa bl       #0x8004d74 */
    FUN_08004D74();   /* BL -> 0x08004D74 FUN_08004D74 */

    L_080058AA:
    /* 0x080058AA: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */
}
