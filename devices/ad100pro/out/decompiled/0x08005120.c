/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005120
 * Name      : FUN_08005120
 * End       : 0x08005150
 * Size      : 48 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=3)
 * Class     : function
 * Callers   : 2  -> 0x0800C2B8 FUN_0800C2B8, 0x0800C3A0 FUN_0800C3A0
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005120(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005120: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x08005122: 0b4c     ldr      r4, [pc, #0x2c]  ; =RAM_200004CC */
    r4 = *(uint32_t *)0x08005150;   /* = RAM 0x200004CC */
    /* 0x08005124: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08005126: 0b4b     ldr      r3, [pc, #0x2c]  ; =flash_0800EC22 */
    r3 = *(uint32_t *)0x08005154;   /* = flash data/const 0x0800EC22 */
    /* 0x08005128: c001     lsls     r0, r0, #7 */
    r0 = r0 << 7;
    /* 0x0800512A: 0019     adds     r0, r0, r4 */
    r0 = r0 + r4;

    L_0800512C:
    /* 0x0800512C: 5500     lsls     r5, r2, #1 */
    r5 = r2 << 1;
    /* 0x0800512E: 8c18     adds     r4, r1, r2 */
    r4 = r1 + r2;
    /* 0x08005130: 065d     ldrb     r6, [r0, r4] */
    r6 = *(uint8_t *)(r0 + r4);
    /* 0x08005132: 5f5d     ldrb     r7, [r3, r5] */
    r7 = *(uint8_t *)(r3 + r5);
    /* 0x08005134: ed18     adds     r5, r5, r3 */
    r5 = r5 + r3;
    /* 0x08005136: 3e43     orrs     r6, r7 */
    r6 = r6 | r7;
    /* 0x08005138: 0655     strb     r6, [r0, r4] */
    *(uint8_t *)(r0 + r4) = r6;   /* store */
    /* 0x0800513A: 0419     adds     r4, r0, r4 */
    r4 = r0 + r4;
    /* 0x0800513C: 8034     adds     r4, #0x80 */
    r4 = r4 + 0x80;
    /* 0x0800513E: 2678     ldrb     r6, [r4] */
    r6 = *(uint8_t *)(r4);
    /* 0x08005140: 6d78     ldrb     r5, [r5, #1] */
    r5 = *(uint8_t *)(r5 + 1);
    /* 0x08005142: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x08005144: 2e43     orrs     r6, r5 */
    r6 = r6 | r5;
    /* 0x08005146: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);
    /* 0x08005148: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */
    /* 0x0800514A: 052a     cmp      r2, #5 */
    /* flags = cmp(r2, #5) */
    /* 0x0800514C: eed3     blo      #0x800512c */
    if (r2 < 5) goto L_0800512C;   /* unsigned */
    /* 0x0800514E: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
