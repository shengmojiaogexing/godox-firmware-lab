/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007F10
 * Name      : FUN_08007F10
 * End       : 0x08007F2E
 * Size      : 30 bytes, 14 instructions decoded in body (functions.json insns=14, blocks=3)
 * Class     : function
 * Callers   : 2  -> 0x0800CF74 main, 0x0800CFD6 FUN_0800CFD6
 * Callees   : 1  -> 0x08007648 FUN_08007648
 * MMIO      : EXTI.IMR, RCC.BDCR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007F10(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007F10: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08007F12: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);  /* MMIO: EXTI.IMR(r) */
    /* 0x08007F14: 0649     ldr      r1, [pc, #0x18]  ; =RTC.TR */
    r1 = *(uint32_t *)0x08007F30;   /* = MMIO RTC+0x00 */
    /* 0x08007F16: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08007F18: 08d1     bne      #0x8007f2c */
    if (r0 != r1) goto L_08007F2C;
    /* 0x08007F1A: 0648     ldr      r0, [pc, #0x18]  ; =RCC.CR */
    r0 = *(uint32_t *)0x08007F34;   /* = MMIO RCC+0x00 */
    /* 0x08007F1C: 016a     ldr      r1, [r0, #0x20] */
    r1 = *(uint32_t *)(r0 + 32);  /* MMIO: RCC.BDCR(r) */
    /* 0x08007F1E: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08007F20: d203     lsls     r2, r2, #0xf */
    r2 = r2 << 0xf;
    /* 0x08007F22: 9143     bics     r1, r2 */
    r1 = r1 & ~r2;
    /* 0x08007F24: 0162     str      r1, [r0, #0x20] */
    *(uint32_t *)(r0 + 32) = r1;   /* store */  /* MMIO: RCC.BDCR(w) */
    /* 0x08007F26: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08007F28: fff78efb bl       #0x8007648 */
    FUN_08007648();   /* BL -> 0x08007648 FUN_08007648 */

    L_08007F2C:
    /* 0x08007F2C: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
