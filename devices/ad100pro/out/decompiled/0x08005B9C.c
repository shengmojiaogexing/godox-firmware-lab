/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005B9C
 * Name      : FUN_08005B9C
 * End       : 0x08005BB8
 * Size      : 28 bytes, 14 instructions decoded in body (functions.json insns=14, blocks=3)
 * Class     : function
 * Callers   : 2  -> 0x0800A034 FUN_0800A034, 0x0800A088 FUN_0800A088
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005B9C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005B9C: 0649     ldr      r1, [pc, #0x18]  ; =RAM_200001BA */
    r1 = *(uint32_t *)0x08005BB8;   /* = RAM 0x200001BA */
    /* 0x08005B9E: 0878     ldrb     r0, [r1] */
    r0 = *(uint8_t *)(r1);
    /* 0x08005BA0: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08005BA2: 08d1     bne      #0x8005bb6 */
    if (r0 != 0) goto L_08005BB6;
    /* 0x08005BA4: 0548     ldr      r0, [pc, #0x14]  ; =RAM_20000134 */
    r0 = *(uint32_t *)0x08005BBC;   /* = RAM 0x20000134 */
    /* 0x08005BA6: 064b     ldr      r3, [pc, #0x18]  ; =RAM_200001B9 */
    r3 = *(uint32_t *)0x08005BC0;   /* = RAM 0x200001B9 */
    /* 0x08005BA8: 4278     ldrb     r2, [r0, #1] */
    r2 = *(uint8_t *)(r0 + 1);
    /* 0x08005BAA: 0270     strb     r2, [r0] */
    *(uint8_t *)(r0) = r2;   /* store */
    /* 0x08005BAC: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08005BAE: 1a70     strb     r2, [r3] */
    *(uint8_t *)(r3) = r2;   /* store */
    /* 0x08005BB0: 044b     ldr      r3, [pc, #0x10]  ; =0x00000BB8 */
    r3 = *(uint32_t *)0x08005BC4;   /* = 0x00000BB8 */
    /* 0x08005BB2: c381     strh     r3, [r0, #0xe] */
    *(uint16_t *)(r0 + 14) = r3;   /* store */
    /* 0x08005BB4: 0a70     strb     r2, [r1] */
    *(uint8_t *)(r1) = r2;   /* store */

    L_08005BB6:
    /* 0x08005BB6: 7047     bx       lr */
    return;
}
