/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006DC8
 * Name      : FUN_08006DC8
 * End       : 0x08006DDE
 * Size      : 22 bytes, 11 instructions decoded in body (functions.json insns=11, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x080098F8 FUN_080098F8
 * Callees   : 0  -> -
 * MMIO      : ADC.+0x300, RCC.CFGR, RCC.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006DC8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006DC8: 30b5     push     {r4, r5, lr} */
    push({r4, r5, lr});
    /* 0x08006DCA: 0268     ldr      r2, [r0] */
    r2 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x08006DCC: 1468     ldr      r4, [r2] */
    r4 = *(uint32_t *)(r2);  /* MMIO: ADC.+0x300(r) */
    /* 0x08006DCE: 0123     movs     r3, #1 */
    r3 = 1;
    /* 0x08006DD0: 1d46     mov      r5, r3 */
    r5 = r3;
    /* 0x08006DD2: 8d40     lsls     r5, r1 */
    r5 = r5 << r1;
    /* 0x08006DD4: ac43     bics     r4, r5 */
    r4 = r4 & ~r5;
    /* 0x08006DD6: 1460     str      r4, [r2] */
    *(uint32_t *)(r2) = r4;   /* store */  /* MMIO: ADC.+0x300(w) */
    /* 0x08006DD8: 0371     strb     r3, [r0, #4] */
    *(uint8_t *)(r0 + 4) = r3;   /* store */  /* MMIO: RCC.CFGR(w) */
    /* 0x08006DDA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006DDC: 30bd     pop      {r4, r5, pc} */
    pop({r4, r5, pc});   /* return */
}
