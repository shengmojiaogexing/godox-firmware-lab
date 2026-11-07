/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008948
 * Name      : FUN_08008948
 * End       : 0x08008960
 * Size      : 24 bytes, 12 instructions decoded in body (functions.json insns=12, blocks=1)
 * Class     : function
 * Callers   : 3  -> 0x0800896C FUN_0800896C, 0x0800A034 FUN_0800A034, 0x0800A0B2 FUN_0800A0B2
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08008948(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08008948: 0549     ldr      r1, [pc, #0x14]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x08008960;   /* = RAM 0x20000134 */
    /* 0x0800894A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800894C: 4881     strh     r0, [r1, #0xa] */
    *(uint16_t *)(r1 + 10) = r0;   /* store */
    /* 0x0800894E: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08008950: c881     strh     r0, [r1, #0xe] */
    *(uint16_t *)(r1 + 14) = r0;   /* store */
    /* 0x08008952: 044a     ldr      r2, [pc, #0x10]  ; =RAM_200001B9 */
    r2 = *(uint32_t *)0x08008964;   /* = RAM 0x200001B9 */
    /* 0x08008954: 8881     strh     r0, [r1, #0xc] */
    *(uint16_t *)(r1 + 12) = r0;   /* store */
    /* 0x08008956: 1070     strb     r0, [r2] */
    *(uint8_t *)(r2) = r0;   /* store */
    /* 0x08008958: 034a     ldr      r2, [pc, #0xc]  ; =RAM_200001BA */
    r2 = *(uint32_t *)0x08008968;   /* = RAM 0x200001BA */
    /* 0x0800895A: 1070     strb     r0, [r2] */
    *(uint8_t *)(r2) = r0;   /* store */
    /* 0x0800895C: 4870     strb     r0, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r0;   /* store */
    /* 0x0800895E: 7047     bx       lr */
    return;
}
