/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006D58
 * Name      : FUN_08006D58
 * End       : 0x08006D74
 * Size      : 28 bytes, 14 instructions decoded in body (functions.json insns=14, blocks=4)
 * Class     : function
 * Callers   : 1  -> 0x080098F8 FUN_080098F8
 * Callees   : 0  -> -
 * MMIO      : RCC.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006D58(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006D58: 18b5     push     {r3, r4, lr} */
    push({r3, r4, lr});
    /* 0x08006D5A: 0468     ldr      r4, [r0] */
    r4 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x08006D5C: 0094     str      r4, [sp] */
    *(uint32_t *)(sp) = r4;   /* store */
    /* 0x08006D5E: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x08006D60: 8018     adds     r0, r0, r2 */
    r0 = r0 + r2;
    /* 0x08006D62: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x08006D64: 04d0     beq      #0x8006d70 */
    if (r1 == 0) goto L_08006D70;
    /* 0x08006D66: 1430     adds     r0, #0x14 */
    r0 = r0 + 0x14;

    L_08006D68:
    /* 0x08006D68: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08006D6A: 0360     str      r3, [r0] */
    *(uint32_t *)(r0) = r3;   /* store */  /* MMIO: RCC.CR(w) */
    /* 0x08006D6C: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006D6E: 18bd     pop      {r3, r4, pc} */
    pop({r3, r4, pc});   /* return */

    L_08006D70:
    /* 0x08006D70: 0830     adds     r0, #8 */
    r0 = r0 + 8;
    /* 0x08006D72: f9e7     b        #0x8006d68 */
    goto L_08006D68;   /* loop */
}
