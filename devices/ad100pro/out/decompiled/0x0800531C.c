/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800531C
 * Name      : FUN_0800531C
 * End       : 0x0800534C
 * Size      : 48 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=3)
 * Class     : function
 * Callers   : 1  -> 0x08004F94 FUN_08004F94
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800531C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800531C: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x0800531E: 0b4c     ldr      r4, [pc, #0x2c]  ; =RAM_200004CC */
    r4 = *(uint32_t *)0x0800534C;   /* = RAM 0x200004CC */
    /* 0x08005320: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08005322: 0b4b     ldr      r3, [pc, #0x2c]  ; =flash_0800EC6A */
    r3 = *(uint32_t *)0x08005350;   /* = flash data/const 0x0800EC6A */
    /* 0x08005324: c001     lsls     r0, r0, #7 */
    r0 = r0 << 7;
    /* 0x08005326: 0019     adds     r0, r0, r4 */
    r0 = r0 + r4;

    L_08005328:
    /* 0x08005328: 5500     lsls     r5, r2, #1 */
    r5 = r2 << 1;
    /* 0x0800532A: 8c18     adds     r4, r1, r2 */
    r4 = r1 + r2;
    /* 0x0800532C: 065d     ldrb     r6, [r0, r4] */
    r6 = *(uint8_t *)(r0 + r4);
    /* 0x0800532E: 5f5d     ldrb     r7, [r3, r5] */
    r7 = *(uint8_t *)(r3 + r5);
    /* 0x08005330: ed18     adds     r5, r5, r3 */
    r5 = r5 + r3;
    /* 0x08005332: 3e43     orrs     r6, r7 */
    r6 = r6 | r7;
    /* 0x08005334: 0655     strb     r6, [r0, r4] */
    *(uint8_t *)(r0 + r4) = r6;   /* store */
    /* 0x08005336: 0419     adds     r4, r0, r4 */
    r4 = r0 + r4;
    /* 0x08005338: 8034     adds     r4, #0x80 */
    r4 = r4 + 0x80;
    /* 0x0800533A: 2678     ldrb     r6, [r4] */
    r6 = *(uint8_t *)(r4);
    /* 0x0800533C: 6d78     ldrb     r5, [r5, #1] */
    r5 = *(uint8_t *)(r5 + 1);
    /* 0x0800533E: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x08005340: 2e43     orrs     r6, r5 */
    r6 = r6 | r5;
    /* 0x08005342: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);
    /* 0x08005344: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */
    /* 0x08005346: 1c2a     cmp      r2, #0x1c */
    /* flags = cmp(r2, #0x1c) */
    /* 0x08005348: eed3     blo      #0x8005328 */
    if (r2 < 0x1c) goto L_08005328;   /* unsigned */
    /* 0x0800534A: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
