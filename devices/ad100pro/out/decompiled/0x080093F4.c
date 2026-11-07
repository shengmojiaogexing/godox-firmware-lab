/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080093F4
 * Name      : FUN_080093F4
 * End       : 0x08009418
 * Size      : 36 bytes, 16 instructions decoded in body (functions.json insns=16, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x0800CF74 main
 * Callees   : 2  -> 0x0800765C FUN_0800765C, 0x08007670 FUN_08007670
 * MMIO      : RCC.AHBENR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080093F4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080093F4: 08b5     push     {r3, lr} */
    push({r3, lr});
    /* 0x080093F6: 0848     ldr      r0, [pc, #0x20]  ; =RCC.CR */
    r0 = *(uint32_t *)0x08009418;   /* = MMIO RCC+0x00 */
    /* 0x080093F8: 4169     ldr      r1, [r0, #0x14] */
    r1 = *(uint32_t *)(r0 + 20);  /* MMIO: RCC.AHBENR(r) */
    /* 0x080093FA: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x080093FC: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x080093FE: 4161     str      r1, [r0, #0x14] */
    *(uint32_t *)(r0 + 20) = r1;   /* store */  /* MMIO: RCC.AHBENR(w) */
    /* 0x08009400: 4069     ldr      r0, [r0, #0x14] */
    r0 = *(uint32_t *)(r0 + 20);  /* MMIO: RCC.AHBENR(r) */
    /* 0x08009402: 1040     ands     r0, r2 */
    r0 = r0 & r2;
    /* 0x08009404: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08009406: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08009408: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x0800940A: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x0800940C: fef730f9 bl       #0x8007670 */
    FUN_08007670();   /* BL -> 0x08007670 FUN_08007670 */
    /* 0x08009410: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x08009412: fef723f9 bl       #0x800765c */
    FUN_0800765C();   /* BL -> 0x0800765C FUN_0800765C */
    /* 0x08009416: 08bd     pop      {r3, pc} */
    pop({r3, pc});   /* return */
}
