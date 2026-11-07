/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005090
 * Name      : FUN_08005090
 * End       : 0x080050C0
 * Size      : 48 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=3)
 * Class     : function
 * Callers   : 3  -> 0x08003F0C FUN_08003F0C, 0x0800419C FUN_0800419C, 0x08004CD4 FUN_08004CD4
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005090(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005090: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x08005092: 0b4c     ldr      r4, [pc, #0x2c]  ; =RAM_200004CC */
    r4 = *(uint32_t *)0x080050C0;   /* = RAM 0x200004CC */
    /* 0x08005094: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08005096: 0b4b     ldr      r3, [pc, #0x2c]  ; =flash_0800ED02 */
    r3 = *(uint32_t *)0x080050C4;   /* = flash data/const 0x0800ED02 */
    /* 0x08005098: c001     lsls     r0, r0, #7 */
    r0 = r0 << 7;
    /* 0x0800509A: 0019     adds     r0, r0, r4 */
    r0 = r0 + r4;

    L_0800509C:
    /* 0x0800509C: 5500     lsls     r5, r2, #1 */
    r5 = r2 << 1;
    /* 0x0800509E: 8c18     adds     r4, r1, r2 */
    r4 = r1 + r2;
    /* 0x080050A0: 065d     ldrb     r6, [r0, r4] */
    r6 = *(uint8_t *)(r0 + r4);
    /* 0x080050A2: 5f5d     ldrb     r7, [r3, r5] */
    r7 = *(uint8_t *)(r3 + r5);
    /* 0x080050A4: ed18     adds     r5, r5, r3 */
    r5 = r5 + r3;
    /* 0x080050A6: 3e43     orrs     r6, r7 */
    r6 = r6 | r7;
    /* 0x080050A8: 0655     strb     r6, [r0, r4] */
    *(uint8_t *)(r0 + r4) = r6;   /* store */
    /* 0x080050AA: 0419     adds     r4, r0, r4 */
    r4 = r0 + r4;
    /* 0x080050AC: 8034     adds     r4, #0x80 */
    r4 = r4 + 0x80;
    /* 0x080050AE: 2678     ldrb     r6, [r4] */
    r6 = *(uint8_t *)(r4);
    /* 0x080050B0: 6d78     ldrb     r5, [r5, #1] */
    r5 = *(uint8_t *)(r5 + 1);
    /* 0x080050B2: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x080050B4: 2e43     orrs     r6, r5 */
    r6 = r6 | r5;
    /* 0x080050B6: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);
    /* 0x080050B8: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */
    /* 0x080050BA: 102a     cmp      r2, #0x10 */
    /* flags = cmp(r2, #0x10) */
    /* 0x080050BC: eed3     blo      #0x800509c */
    if (r2 < 0x10) goto L_0800509C;   /* unsigned */
    /* 0x080050BE: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
