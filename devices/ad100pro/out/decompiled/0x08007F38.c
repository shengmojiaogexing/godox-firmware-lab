/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007F38
 * Name      : FUN_08007F38
 * End       : 0x08007F60
 * Size      : 40 bytes, 18 instructions decoded in body (functions.json insns=18, blocks=3)
 * Class     : function
 * Callers   : 1  -> 0x08007E70 FUN_08007E70
 * Callees   : 2  -> 0x0800765C FUN_0800765C, 0x08007670 FUN_08007670
 * MMIO      : RCC.BDCR, RCC.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007F38(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007F38: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08007F3A: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x08007F3C: 0849     ldr      r1, [pc, #0x20]  ; =RTC.TR */
    r1 = *(uint32_t *)0x08007F60;   /* = MMIO RTC+0x00 */
    /* 0x08007F3E: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08007F40: 0dd1     bne      #0x8007f5e */
    if (r0 != r1) goto L_08007F5E;
    /* 0x08007F42: 0848     ldr      r0, [pc, #0x20]  ; =RCC.CR */
    r0 = *(uint32_t *)0x08007F64;   /* = MMIO RCC+0x00 */
    /* 0x08007F44: 016a     ldr      r1, [r0, #0x20] */
    r1 = *(uint32_t *)(r0 + 32);  /* MMIO: RCC.BDCR(r) */
    /* 0x08007F46: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08007F48: d203     lsls     r2, r2, #0xf */
    r2 = r2 << 0xf;
    /* 0x08007F4A: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x08007F4C: 0162     str      r1, [r0, #0x20] */
    *(uint32_t *)(r0 + 32) = r1;   /* store */  /* MMIO: RCC.BDCR(w) */
    /* 0x08007F4E: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08007F50: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x08007F52: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08007F54: fff78cfb bl       #0x8007670 */
    FUN_08007670();   /* BL -> 0x08007670 FUN_08007670 */
    /* 0x08007F58: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08007F5A: fff77ffb bl       #0x800765c */
    FUN_0800765C();   /* BL -> 0x0800765C FUN_0800765C */

    L_08007F5E:
    /* 0x08007F5E: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
