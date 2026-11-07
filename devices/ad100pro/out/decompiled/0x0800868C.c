/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800868C
 * Name      : FUN_0800868C
 * End       : 0x080086CA
 * Size      : 62 bytes, 30 instructions decoded in body (functions.json insns=30, blocks=6)
 * Class     : function
 * Callers   : 2  -> 0x0800A8D0 FUN_0800A8D0, 0x0800A8EA FUN_0800A8EA
 * Callees   : 1  -> 0x0800BB48 FUN_0800BB48
 * MMIO      : RCC.CR, SYS.+0x0, TIM17.CR1, TIM3.BDTR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800868C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800868C: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800868E: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x08008690: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x08008692: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08008694: 03f058fa bl       #0x800bb48 */
    FUN_0800BB48();   /* BL -> 0x0800BB48 FUN_0800BB48 */
    /* 0x08008698: 0c48     ldr      r0, [pc, #0x30]  ; =TIM1.CR1 */
    r0 = *(uint32_t *)0x080086CC;   /* = MMIO TIM1+0x00 */
    /* 0x0800869A: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x0800869C: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x0800869E: 08d0     beq      #0x80086b2 */
    if (r1 == r0) goto L_080086B2;
    /* 0x080086A0: 0b48     ldr      r0, [pc, #0x2c]  ; =TIM15.CR1 */
    r0 = *(uint32_t *)0x080086D0;   /* = MMIO TIM15+0x00 */
    /* 0x080086A2: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x080086A4: 05d0     beq      #0x80086b2 */
    if (r1 == r0) goto L_080086B2;
    /* 0x080086A6: 0b48     ldr      r0, [pc, #0x2c]  ; =TIM16.CR1 */
    r0 = *(uint32_t *)0x080086D4;   /* = MMIO TIM16+0x00 */
    /* 0x080086A8: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x080086AA: 02d0     beq      #0x80086b2 */
    if (r1 == r0) goto L_080086B2;
    /* 0x080086AC: 0a48     ldr      r0, [pc, #0x28]  ; =TIM17.CR1 */
    r0 = *(uint32_t *)0x080086D8;   /* = MMIO TIM17+0x00 */
    /* 0x080086AE: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x080086B0: 04d1     bne      #0x80086bc */
    if (r1 != r0) goto L_080086BC;

    L_080086B2:
    /* 0x080086B2: 486c     ldr      r0, [r1, #0x44] */
    r0 = *(uint32_t *)(r1 + 68);  /* MMIO: TIM3.BDTR(r) */
    /* 0x080086B4: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x080086B6: d203     lsls     r2, r2, #0xf */
    r2 = r2 << 0xf;
    /* 0x080086B8: 1043     orrs     r0, r2 */
    r0 = r0 | r2;
    /* 0x080086BA: 4864     str      r0, [r1, #0x44] */
    *(uint32_t *)(r1 + 68) = r0;   /* store */  /* MMIO: TIM3.BDTR(w) */

    L_080086BC:
    /* 0x080086BC: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x080086BE: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: TIM17.CR1(r) */
    /* 0x080086C0: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x080086C2: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x080086C4: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: TIM17.CR1(w) */
    /* 0x080086C6: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080086C8: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
