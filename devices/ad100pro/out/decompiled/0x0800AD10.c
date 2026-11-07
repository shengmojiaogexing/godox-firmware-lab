/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800AD10
 * Name      : FUN_0800AD10
 * End       : 0x0800AD34
 * Size      : 36 bytes, 17 instructions decoded in body (functions.json insns=17, blocks=4)
 * Class     : function
 * Callers   : 2  -> 0x0800CF74 main, 0x0800CFDE FUN_0800CFDE
 * Callees   : 1  -> 0x08005EB0 FUN_08005EB0
 * MMIO      : RCC.CSR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800AD10(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800AD10: 00b5     push     {lr} */
    push({lr});
    /* 0x0800AD12: 084a     ldr      r2, [pc, #0x20]  ; =RAM_200001D0 */
    r2 = *(uint32_t *)0x0800AD34;   /* = RAM 0x200001D0 */
    /* 0x0800AD14: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800AD16: 1070     strb     r0, [r2] */
    *(uint8_t *)(r2) = r0;   /* store */
    /* 0x0800AD18: 0748     ldr      r0, [pc, #0x1c]  ; =RCC.CR */
    r0 = *(uint32_t *)0x0800AD38;   /* = MMIO RCC+0x00 */
    /* 0x0800AD1A: 406a     ldr      r0, [r0, #0x24] */
    r0 = *(uint32_t *)(r0 + 36);  /* MMIO: RCC.CSR(r) */
    /* 0x0800AD1C: 7b23     movs     r3, #0x7b */
    r3 = 0x7b;
    /* 0x0800AD1E: 4001     lsls     r0, r0, #5 */
    r0 = r0 << 5;
    /* 0x0800AD20: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800AD22: 05da     bge      #0x800ad30 */
    if (r0 >= 0) goto L_0800AD30;   /* signed */
    /* 0x0800AD24: 0548     ldr      r0, [pc, #0x14]  ; =0x0801F800 */
    r0 = *(uint32_t *)0x0800AD3C;   /* = 0x0801F800 */
    /* 0x0800AD26: fbf7c3f8 bl       #0x8005eb0 */
    FUN_08005EB0();   /* BL -> 0x08005EB0 FUN_08005EB0 */
    /* 0x0800AD2A: 0549     ldr      r1, [pc, #0x14]  ; =0x00001018 */
    r1 = *(uint32_t *)0x0800AD40;   /* = 0x00001018 */
    /* 0x0800AD2C: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x0800AD2E: 00d1     bne      #0x800ad32 */
    if (r0 != r1) goto L_0800AD32;

    L_0800AD30:
    /* 0x0800AD30: 1370     strb     r3, [r2] */
    *(uint8_t *)(r2) = r3;   /* store */

    L_0800AD32:
    /* 0x0800AD32: 00bd     pop      {pc} */
    pop({pc});   /* return */
}
