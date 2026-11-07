/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005200
 * Name      : FUN_08005200
 * End       : 0x08005234
 * Size      : 52 bytes, 26 instructions decoded in body (functions.json insns=26, blocks=3)
 * Class     : function
 * Callers   : 1  -> 0x080053BC FUN_080053BC
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005200(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005200: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x08005202: 0d4d     ldr      r5, [pc, #0x34]  ; =flash_0800ED62 */
    r5 = *(uint32_t *)0x08005238;   /* = flash data/const 0x0800ED62 */
    /* 0x08005204: 0b4c     ldr      r4, [pc, #0x2c]  ; =RAM_200004CC */
    r4 = *(uint32_t *)0x08005234;   /* = RAM 0x200004CC */
    /* 0x08005206: 5201     lsls     r2, r2, #5 */
    r2 = r2 << 5;
    /* 0x08005208: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x0800520A: 5219     adds     r2, r2, r5 */
    r2 = r2 + r5;
    /* 0x0800520C: c001     lsls     r0, r0, #7 */
    r0 = r0 << 7;
    /* 0x0800520E: 0019     adds     r0, r0, r4 */
    r0 = r0 + r4;

    L_08005210:
    /* 0x08005210: 5d00     lsls     r5, r3, #1 */
    r5 = r3 << 1;
    /* 0x08005212: cc18     adds     r4, r1, r3 */
    r4 = r1 + r3;
    /* 0x08005214: 065d     ldrb     r6, [r0, r4] */
    r6 = *(uint8_t *)(r0 + r4);
    /* 0x08005216: 575d     ldrb     r7, [r2, r5] */
    r7 = *(uint8_t *)(r2 + r5);
    /* 0x08005218: 5519     adds     r5, r2, r5 */
    r5 = r2 + r5;
    /* 0x0800521A: 3e43     orrs     r6, r7 */
    r6 = r6 | r7;
    /* 0x0800521C: 0655     strb     r6, [r0, r4] */
    *(uint8_t *)(r0 + r4) = r6;   /* store */
    /* 0x0800521E: 0419     adds     r4, r0, r4 */
    r4 = r0 + r4;
    /* 0x08005220: 8034     adds     r4, #0x80 */
    r4 = r4 + 0x80;
    /* 0x08005222: 2678     ldrb     r6, [r4] */
    r6 = *(uint8_t *)(r4);
    /* 0x08005224: 6d78     ldrb     r5, [r5, #1] */
    r5 = *(uint8_t *)(r5 + 1);
    /* 0x08005226: 5b1c     adds     r3, r3, #1 */
    r3 = r3 + 1;
    /* 0x08005228: 2e43     orrs     r6, r5 */
    r6 = r6 | r5;
    /* 0x0800522A: dbb2     uxtb     r3, r3 */
    r3 = uxtb(r3);
    /* 0x0800522C: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */
    /* 0x0800522E: 102b     cmp      r3, #0x10 */
    /* flags = cmp(r3, #0x10) */
    /* 0x08005230: eed3     blo      #0x8005210 */
    if (r3 < 0x10) goto L_08005210;   /* unsigned */
    /* 0x08005232: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
