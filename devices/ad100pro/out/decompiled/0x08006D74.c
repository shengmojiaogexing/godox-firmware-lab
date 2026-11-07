/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006D74
 * Name      : FUN_08006D74
 * End       : 0x08006DC8
 * Size      : 84 bytes, 42 instructions decoded in body (functions.json insns=42, blocks=9)
 * Class     : function
 * Callers   : 1  -> 0x080098F8 FUN_080098F8
 * Callees   : 0  -> -
 * MMIO      : RCC.+0x5, RCC.CFGR, RCC.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006D74(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006D74: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08006D76: 4279     ldrb     r2, [r0, #5] */
    r2 = *(uint8_t *)(r0 + 5);  /* MMIO: RCC.+0x5(r) */
    /* 0x08006D78: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x08006D7A: 12d0     beq      #0x8006da2 */
    if (r2 == 1) goto L_08006DA2;
    /* 0x08006D7C: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08006D7E: 4271     strb     r2, [r0, #5] */
    *(uint8_t *)(r0 + 5) = r2;   /* store */  /* MMIO: RCC.+0x5(w) */
    /* 0x08006D80: 0223     movs     r3, #2 */
    r3 = 2;
    /* 0x08006D82: 0371     strb     r3, [r0, #4] */
    *(uint8_t *)(r0 + 4) = r3;   /* store */  /* MMIO: RCC.CFGR(w) */
    /* 0x08006D84: 0468     ldr      r4, [r0] */
    r4 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x08006D86: 2668     ldr      r6, [r4] */
    r6 = *(uint32_t *)(r4);
    /* 0x08006D88: 1546     mov      r5, r2 */
    r5 = r2;
    /* 0x08006D8A: 8d40     lsls     r5, r1 */
    r5 = r5 << r1;
    /* 0x08006D8C: 2e43     orrs     r6, r5 */
    r6 = r6 | r5;
    /* 0x08006D8E: 2660     str      r6, [r4] */
    *(uint32_t *)(r4) = r6;   /* store */
    /* 0x08006D90: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x08006D92: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x08006D94: 07d0     beq      #0x8006da6 */
    if (r1 == 0) goto L_08006DA6;
    /* 0x08006D96: 0c68     ldr      r4, [r1] */
    r4 = *(uint32_t *)(r1);
    /* 0x08006D98: a402     lsls     r4, r4, #0xa */
    r4 = r4 << 0xa;
    /* 0x08006D9A: 240f     lsrs     r4, r4, #0x1c */
    r4 = r4 >> 0x1c;
    /* 0x08006D9C: 0f2c     cmp      r4, #0xf */
    /* flags = cmp(r4, #0xf) */
    /* 0x08006D9E: 0bd0     beq      #0x8006db8 */
    if (r4 == 0xf) goto L_08006DB8;
    /* 0x08006DA0: 0de0     b        #0x8006dbe */
    goto L_08006DBE;

    L_08006DA2:
    /* 0x08006DA2: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08006DA4: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08006DA6:
    /* 0x08006DA6: 0b68     ldr      r3, [r1] */
    r3 = *(uint32_t *)(r1);
    /* 0x08006DA8: 9b06     lsls     r3, r3, #0x1a */
    r3 = r3 << 0x1a;
    /* 0x08006DAA: 1b0f     lsrs     r3, r3, #0x1c */
    r3 = r3 >> 0x1c;
    /* 0x08006DAC: 0f2b     cmp      r3, #0xf */
    /* flags = cmp(r3, #0xf) */
    /* 0x08006DAE: 06d1     bne      #0x8006dbe */
    if (r3 != 0xf) goto L_08006DBE;
    /* 0x08006DB0: 4b68     ldr      r3, [r1, #4] */
    r3 = *(uint32_t *)(r1 + 4);
    /* 0x08006DB2: 1343     orrs     r3, r2 */
    r3 = r3 | r2;
    /* 0x08006DB4: 4b60     str      r3, [r1, #4] */
    *(uint32_t *)(r1 + 4) = r3;   /* store */
    /* 0x08006DB6: 02e0     b        #0x8006dbe */
    goto L_08006DBE;

    L_08006DB8:
    /* 0x08006DB8: 4c68     ldr      r4, [r1, #4] */
    r4 = *(uint32_t *)(r1 + 4);
    /* 0x08006DBA: 1c43     orrs     r4, r3 */
    r4 = r4 | r3;
    /* 0x08006DBC: 4c60     str      r4, [r1, #4] */
    *(uint32_t *)(r1 + 4) = r4;   /* store */

    L_08006DBE:
    /* 0x08006DBE: 0271     strb     r2, [r0, #4] */
    *(uint8_t *)(r0 + 4) = r2;   /* store */  /* MMIO: RCC.CFGR(w) */
    /* 0x08006DC0: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x08006DC2: 4171     strb     r1, [r0, #5] */
    *(uint8_t *)(r0 + 5) = r1;   /* store */  /* MMIO: RCC.+0x5(w) */
    /* 0x08006DC4: 0846     mov      r0, r1 */
    r0 = r1;
    /* 0x08006DC6: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
