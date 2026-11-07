/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B5EC
 * Name      : FUN_0800B5EC
 * End       : 0x0800B60C
 * Size      : 32 bytes, 15 instructions decoded in body (functions.json insns=15, blocks=3)
 * Class     : function
 * Callers   : 3  -> 0x08009CB4 FUN_08009CB4, 0x08009CCA FUN_08009CCA, 0x0800C674 FUN_0800C674
 * Callees   : 1  -> 0x08009D84 FUN_08009D84
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B5EC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B5EC: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800B5EE: 0749     ldr      r1, [pc, #0x1c]  ; =RAM_2000017F */
    r1 = *(uint32_t *)0x0800B60C;   /* = RAM 0x2000017F */
    /* 0x0800B5F0: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800B5F2: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x0800B5F4: 0648     ldr      r0, [pc, #0x18]  ; =RAM_20000165 */
    r0 = *(uint32_t *)0x0800B610;   /* = RAM 0x20000165 */
    /* 0x0800B5F6: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800B5F8: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800B5FA: 06d0     beq      #0x800b60a */
    if (r0 == 1) goto L_0800B60A;
    /* 0x0800B5FC: 0549     ldr      r1, [pc, #0x14]  ; =RAM_200001AF */
    r1 = *(uint32_t *)0x0800B614;   /* = RAM 0x200001AF */
    /* 0x0800B5FE: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800B600: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x0800B602: 0549     ldr      r1, [pc, #0x14]  ; =RAM_200001AD */
    r1 = *(uint32_t *)0x0800B618;   /* = RAM 0x200001AD */
    /* 0x0800B604: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x0800B606: fef7bdfb bl       #0x8009d84 */
    FUN_08009D84();   /* BL -> 0x08009D84 FUN_08009D84 */

    L_0800B60A:
    /* 0x0800B60A: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
