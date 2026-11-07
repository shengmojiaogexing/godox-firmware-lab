/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C92C
 * Name      : FUN_0800C92C
 * End       : 0x0800C950
 * Size      : 36 bytes, 18 instructions decoded in body (functions.json insns=18, blocks=5)
 * Class     : function
 * Callers   : 2  -> 0x080097F8 FUN_080097F8, 0x0800980E FUN_0800980E
 * Callees   : 0  -> -
 * MMIO      : TIM1.CR1
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C92C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C92C: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800C92E: 084c     ldr      r4, [pc, #0x20]  ; =RAM_20000454 */
    r4 = *(uint32_t *)0x0800C950;   /* = RAM 0x20000454 */
    /* 0x0800C930: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C932: 2346     mov      r3, r4 */
    r3 = r4;
    /* 0x0800C934: 0146     mov      r1, r0 */
    r1 = r0;
    /* 0x0800C936: 203b     subs     r3, #0x20 */
    r3 = r3 - 0x20;

    L_0800C938:
    /* 0x0800C938: 8a00     lsls     r2, r1, #2 */
    r2 = r1 << 2;
    /* 0x0800C93A: a558     ldr      r5, [r4, r2] */
    r5 = *(uint32_t *)(r4 + r2);
    /* 0x0800C93C: 9e58     ldr      r6, [r3, r2] */
    r6 = *(uint32_t *)(r3 + r2);  /* MMIO: TIM1.CR1(r) */
    /* 0x0800C93E: b542     cmp      r5, r6 */
    /* flags = cmp(r5, r6) */
    /* 0x0800C940: 00d0     beq      #0x800c944 */
    if (r5 == r6) goto L_0800C944;
    /* 0x0800C942: 0120     movs     r0, #1 */
    r0 = 1;

    L_0800C944:
    /* 0x0800C944: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x0800C946: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x0800C948: 9d50     str      r5, [r3, r2] */
    *(uint32_t *)(r3 + r2) = r5;   /* store */  /* MMIO: TIM1.CR1(w) */
    /* 0x0800C94A: 0829     cmp      r1, #8 */
    /* flags = cmp(r1, #8) */
    /* 0x0800C94C: f4d3     blo      #0x800c938 */
    if (r1 < 8) goto L_0800C938;   /* unsigned */
    /* 0x0800C94E: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
