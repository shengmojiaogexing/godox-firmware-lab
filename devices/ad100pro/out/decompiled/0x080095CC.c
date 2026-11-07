/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080095CC
 * Name      : FUN_080095CC
 * End       : 0x080095EC
 * Size      : 32 bytes, 14 instructions decoded in body (functions.json insns=14, blocks=3)
 * Class     : function
 * Callers   : 1  -> 0x0800CF74 main
 * Callees   : 2  -> 0x08005C40 FUN_08005C40, 0x08007440 FUN_08007440
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080095CC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080095CC: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x080095CE: 0848     ldr      r0, [pc, #0x20]  ; =RAM_20000384 */
    r0 = *(uint32_t *)0x080095F0;   /* = RAM 0x20000384 */
    /* 0x080095D0: 0649     ldr      r1, [pc, #0x18]  ; =IWDG.KR */
    r1 = *(uint32_t *)0x080095EC;   /* = MMIO IWDG+0x00 */
    /* 0x080095D2: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */
    /* 0x080095D4: 0621     movs     r1, #6 */
    r1 = 6;
    /* 0x080095D6: 4160     str      r1, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r1;   /* store */
    /* 0x080095D8: 0649     ldr      r1, [pc, #0x18]  ; =0x00000FFF */
    r1 = *(uint32_t *)0x080095F4;   /* = 0x00000FFF */
    /* 0x080095DA: c160     str      r1, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r1;   /* store */
    /* 0x080095DC: 8160     str      r1, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r1;   /* store */
    /* 0x080095DE: fdf72fff bl       #0x8007440 */
    FUN_08007440();   /* BL -> 0x08007440 FUN_08007440 */
    /* 0x080095E2: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080095E4: 01d0     beq      #0x80095ea */
    if (r0 == 0) goto L_080095EA;
    /* 0x080095E6: fcf72bfb bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_080095EA:
    /* 0x080095EA: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
