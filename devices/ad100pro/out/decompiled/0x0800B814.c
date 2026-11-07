/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B814
 * Name      : FUN_0800B814
 * End       : 0x0800B860
 * Size      : 76 bytes, 36 instructions decoded in body (functions.json insns=36, blocks=7)
 * Class     : function
 * Callers   : 1  -> 0x0800B80C FUN_0800B80C
 * Callees   : 1  -> 0x0800A934 FUN_0800A934
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B814(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B814: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800B816: 1248     ldr      r0, [pc, #0x48]  ; =RAM_20000480 */
    r0 = *(uint32_t *)0x0800B860;   /* = RAM 0x20000480 */
    /* 0x0800B818: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x0800B81A: 8178     ldrb     r1, [r0, #2] */
    r1 = *(uint8_t *)(r0 + 2);
    /* 0x0800B81C: 114c     ldr      r4, [pc, #0x44]  ; =RAM_200001D5 */
    r4 = *(uint32_t *)0x0800B864;   /* = RAM 0x200001D5 */
    /* 0x0800B81E: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x0800B820: 15d0     beq      #0x800b84e */
    if (r1 == 0) goto L_0800B84E;
    /* 0x0800B822: 1148     ldr      r0, [pc, #0x44]  ; =RAM_200001D2 */
    r0 = *(uint32_t *)0x0800B868;   /* = RAM 0x200001D2 */
    /* 0x0800B824: 114b     ldr      r3, [pc, #0x44]  ; =0x00002EE0 */
    r3 = *(uint32_t *)0x0800B86C;   /* = 0x00002EE0 */
    /* 0x0800B826: 0288     ldrh     r2, [r0] */
    r2 = *(uint16_t *)(r0);
    /* 0x0800B828: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x0800B82A: 92b2     uxth     r2, r2 */
    r2 = uxth(r2);
    /* 0x0800B82C: 0280     strh     r2, [r0] */
    *(uint16_t *)(r0) = r2;   /* store */
    /* 0x0800B82E: 9a42     cmp      r2, r3 */
    /* flags = cmp(r2, r3) */
    /* 0x0800B830: 0dd3     blo      #0x800b84e */
    if (r2 < r3) goto L_0800B84E;   /* unsigned */
    /* 0x0800B832: 0580     strh     r5, [r0] */
    *(uint16_t *)(r0) = r5;   /* store */
    /* 0x0800B834: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800B836: 1e22     movs     r2, #0x1e */
    r2 = 0x1e;
    /* 0x0800B838: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800B83A: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800B83C: 5143     muls     r1, r2, r1 */
    r1 = r2 * r1;
    /* 0x0800B83E: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x0800B840: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800B842: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x0800B844: 03d3     blo      #0x800b84e */
    if (r0 < r1) goto L_0800B84E;   /* unsigned */
    /* 0x0800B846: 2570     strb     r5, [r4] */
    *(uint8_t *)(r4) = r5;   /* store */
    /* 0x0800B848: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800B84A: fff773f8 bl       #0x800a934 */
    FUN_0800A934();   /* BL -> 0x0800A934 FUN_0800A934 */

    L_0800B84E:
    /* 0x0800B84E: 0848     ldr      r0, [pc, #0x20]  ; =RAM_200001D4 */
    r0 = *(uint32_t *)0x0800B870;   /* = RAM 0x200001D4 */
    /* 0x0800B850: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800B852: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800B854: 03d1     bne      #0x800b85e */
    if (r0 != 1) goto L_0800B85E;
    /* 0x0800B856: 2570     strb     r5, [r4] */
    *(uint8_t *)(r4) = r5;   /* store */
    /* 0x0800B858: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800B85A: fff76bf8 bl       #0x800a934 */
    FUN_0800A934();   /* BL -> 0x0800A934 FUN_0800A934 */

    L_0800B85E:
    /* 0x0800B85E: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
