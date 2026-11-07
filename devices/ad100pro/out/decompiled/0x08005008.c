/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005008
 * Name      : FUN_08005008
 * End       : 0x08005050
 * Size      : 72 bytes, 36 instructions decoded in body (functions.json insns=36, blocks=3)
 * Class     : function
 * Callers   : 1  -> 0x08004C50 FUN_08004C50
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005008(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005008: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x0800500A: 114a     ldr      r2, [pc, #0x44]  ; =RAM_200004CC */
    r2 = *(uint32_t *)0x08005050;   /* = RAM 0x200004CC */
    /* 0x0800500C: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x0800500E: c001     lsls     r0, r0, #7 */
    r0 = r0 << 7;
    /* 0x08005010: 8418     adds     r4, r0, r2 */
    r4 = r0 + r2;

    L_08005012:
    /* 0x08005012: 9e00     lsls     r6, r3, #2 */
    r6 = r3 << 2;
    /* 0x08005014: 0f4d     ldr      r5, [pc, #0x3c]  ; =flash_0800F042 */
    r5 = *(uint32_t *)0x08005054;   /* = flash data/const 0x0800F042 */
    /* 0x08005016: c818     adds     r0, r1, r3 */
    r0 = r1 + r3;
    /* 0x08005018: 225c     ldrb     r2, [r4, r0] */
    r2 = *(uint8_t *)(r4 + r0);
    /* 0x0800501A: af5d     ldrb     r7, [r5, r6] */
    r7 = *(uint8_t *)(r5 + r6);
    /* 0x0800501C: 7519     adds     r5, r6, r5 */
    r5 = r6 + r5;
    /* 0x0800501E: 3a43     orrs     r2, r7 */
    r2 = r2 | r7;
    /* 0x08005020: 2254     strb     r2, [r4, r0] */
    *(uint8_t *)(r4 + r0) = r2;   /* store */
    /* 0x08005022: 2218     adds     r2, r4, r0 */
    r2 = r4 + r0;
    /* 0x08005024: 1046     mov      r0, r2 */
    r0 = r2;
    /* 0x08005026: 8030     adds     r0, #0x80 */
    r0 = r0 + 0x80;
    /* 0x08005028: 0778     ldrb     r7, [r0] */
    r7 = *(uint8_t *)(r0);
    /* 0x0800502A: 6e78     ldrb     r6, [r5, #1] */
    r6 = *(uint8_t *)(r5 + 1);
    /* 0x0800502C: ff32     adds     r2, #0xff */
    r2 = r2 + 0xff;
    /* 0x0800502E: 3743     orrs     r7, r6 */
    r7 = r7 | r6;
    /* 0x08005030: 0770     strb     r7, [r0] */
    *(uint8_t *)(r0) = r7;   /* store */
    /* 0x08005032: 8030     adds     r0, #0x80 */
    r0 = r0 + 0x80;
    /* 0x08005034: 0678     ldrb     r6, [r0] */
    r6 = *(uint8_t *)(r0);
    /* 0x08005036: af78     ldrb     r7, [r5, #2] */
    r7 = *(uint8_t *)(r5 + 2);
    /* 0x08005038: 8132     adds     r2, #0x81 */
    r2 = r2 + 0x81;
    /* 0x0800503A: 3e43     orrs     r6, r7 */
    r6 = r6 | r7;
    /* 0x0800503C: 0670     strb     r6, [r0] */
    *(uint8_t *)(r0) = r6;   /* store */
    /* 0x0800503E: 1078     ldrb     r0, [r2] */
    r0 = *(uint8_t *)(r2);
    /* 0x08005040: ed78     ldrb     r5, [r5, #3] */
    r5 = *(uint8_t *)(r5 + 3);
    /* 0x08005042: 5b1c     adds     r3, r3, #1 */
    r3 = r3 + 1;
    /* 0x08005044: 2843     orrs     r0, r5 */
    r0 = r0 | r5;
    /* 0x08005046: dbb2     uxtb     r3, r3 */
    r3 = uxtb(r3);
    /* 0x08005048: 1070     strb     r0, [r2] */
    *(uint8_t *)(r2) = r0;   /* store */
    /* 0x0800504A: 402b     cmp      r3, #0x40 */
    /* flags = cmp(r3, #0x40) */
    /* 0x0800504C: e1d3     blo      #0x8005012 */
    if (r3 < 0x40) goto L_08005012;   /* unsigned */
    /* 0x0800504E: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
