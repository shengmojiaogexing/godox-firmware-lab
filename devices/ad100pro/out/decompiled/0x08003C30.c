/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08003C30
 * Name      : FUN_08003C30
 * End       : 0x08003C4A
 * Size      : 26 bytes, 13 instructions decoded in body (functions.json insns=13, blocks=3)
 * Class     : function
 * Callers   : 5  -> 0x080038C0 FUN_080038C0, 0x080045E4 FUN_080045E4, 0x08005BC8 FUN_08005BC8, 0x0800642C FUN_0800642C, 0x08006450 FUN_08006450
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08003C30(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08003C30: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08003C32: 064b     ldr      r3, [pc, #0x18]  ; =RAM_200001E0 */
    r3 = *(uint32_t *)0x08003C4C;   /* = RAM 0x200001E0 */
    /* 0x08003C34: 9c7a     ldrb     r4, [r3, #0xa] */
    r4 = *(uint8_t *)(r3 + 10);
    /* 0x08003C36: 002c     cmp      r4, #0 */
    /* flags = cmp(r4, #0) */
    /* 0x08003C38: 06d1     bne      #0x8003c48 */
    if (r4 != 0) goto L_08003C48;
    /* 0x08003C3A: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x08003C3C: 9c72     strb     r4, [r3, #0xa] */
    *(uint8_t *)(r3 + 10) = r4;   /* store */
    /* 0x08003C3E: 5872     strb     r0, [r3, #9] */
    *(uint8_t *)(r3 + 9) = r0;   /* store */
    /* 0x08003C40: 1972     strb     r1, [r3, #8] */
    *(uint8_t *)(r3 + 8) = r1;   /* store */
    /* 0x08003C42: 5a84     strh     r2, [r3, #0x22] */
    *(uint16_t *)(r3 + 34) = r2;   /* store */
    /* 0x08003C44: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003C46: 9884     strh     r0, [r3, #0x24] */
    *(uint16_t *)(r3 + 36) = r0;   /* store */

    L_08003C48:
    /* 0x08003C48: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
