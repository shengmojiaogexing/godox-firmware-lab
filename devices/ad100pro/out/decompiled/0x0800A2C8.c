/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A2C8
 * Name      : FUN_0800A2C8
 * End       : 0x0800A304
 * Size      : 60 bytes, 30 instructions decoded in body (functions.json insns=30, blocks=3)
 * Class     : function
 * Callers   : 2  -> 0x08007670 FUN_08007670, 0x08008164 FUN_08008164
 * Callees   : 0  -> -
 * MMIO      : SCB.SHPR2
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A2C8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A2C8: 8307     lsls     r3, r0, #0x1e */
    r3 = r0 << 0x1e;
    /* 0x0800A2CA: ff22     movs     r2, #0xff */
    r2 = 0xff;
    /* 0x0800A2CC: db0e     lsrs     r3, r3, #0x1b */
    r3 = r3 >> 0x1b;
    /* 0x0800A2CE: 9a40     lsls     r2, r3 */
    r2 = r2 << r3;
    /* 0x0800A2D0: 8907     lsls     r1, r1, #0x1e */
    r1 = r1 << 0x1e;
    /* 0x0800A2D2: 090e     lsrs     r1, r1, #0x18 */
    r1 = r1 >> 0x18;
    /* 0x0800A2D4: 9940     lsls     r1, r3 */
    r1 = r1 << r3;
    /* 0x0800A2D6: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800A2D8: 0bda     bge      #0x800a2f2 */
    if (r0 >= 0) goto L_0800A2F2;   /* signed */
    /* 0x0800A2DA: 0007     lsls     r0, r0, #0x1c */
    r0 = r0 << 0x1c;
    /* 0x0800A2DC: 000f     lsrs     r0, r0, #0x1c */
    r0 = r0 >> 0x1c;
    /* 0x0800A2DE: 0838     subs     r0, #8 */
    r0 = r0 - 8;
    /* 0x0800A2E0: 8308     lsrs     r3, r0, #2 */
    r3 = r0 >> 2;
    /* 0x0800A2E2: 0848     ldr      r0, [pc, #0x20]  ; =SCB.CPUID */
    r0 = *(uint32_t *)0x0800A304;   /* = MMIO SCB+0x00 */
    /* 0x0800A2E4: 9b00     lsls     r3, r3, #2 */
    r3 = r3 << 2;
    /* 0x0800A2E6: 1818     adds     r0, r3, r0 */
    r0 = r3 + r0;
    /* 0x0800A2E8: c369     ldr      r3, [r0, #0x1c] */
    r3 = *(uint32_t *)(r0 + 28);  /* MMIO: SCB.SHPR2(r) */
    /* 0x0800A2EA: 9343     bics     r3, r2 */
    r3 = r3 & ~r2;
    /* 0x0800A2EC: 0b43     orrs     r3, r1 */
    r3 = r3 | r1;
    /* 0x0800A2EE: c361     str      r3, [r0, #0x1c] */
    *(uint32_t *)(r0 + 28) = r3;   /* store */  /* MMIO: SCB.SHPR2(w) */
    /* 0x0800A2F0: 7047     bx       lr */
    return;

    L_0800A2F2:
    /* 0x0800A2F2: 8308     lsrs     r3, r0, #2 */
    r3 = r0 >> 2;
    /* 0x0800A2F4: 0448     ldr      r0, [pc, #0x10]  ; =NVIC_ICP.0x180 */
    r0 = *(uint32_t *)0x0800A308;   /* = MMIO SysTick+0x3F0 */
    /* 0x0800A2F6: 9b00     lsls     r3, r3, #2 */
    r3 = r3 << 2;
    /* 0x0800A2F8: 1b18     adds     r3, r3, r0 */
    r3 = r3 + r0;
    /* 0x0800A2FA: 1868     ldr      r0, [r3] */
    r0 = *(uint32_t *)(r3);
    /* 0x0800A2FC: 9043     bics     r0, r2 */
    r0 = r0 & ~r2;
    /* 0x0800A2FE: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x0800A300: 1860     str      r0, [r3] */
    *(uint32_t *)(r3) = r0;   /* store */
    /* 0x0800A302: 7047     bx       lr */
    return;
}
