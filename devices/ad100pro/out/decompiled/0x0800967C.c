/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800967C
 * Name      : FUN_0800967C
 * End       : 0x080096A0
 * Size      : 36 bytes, 16 instructions decoded in body (functions.json insns=16, blocks=3)
 * Class     : function
 * Callers   : 1  -> 0x0800CF74 main
 * Callees   : 2  -> 0x08005C40 FUN_08005C40, 0x080081E8 FUN_080081E8
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800967C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800967C: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800967E: 0948     ldr      r0, [pc, #0x24]  ; =RAM_20000348 */
    r0 = *(uint32_t *)0x080096A4;   /* = RAM 0x20000348 */
    /* 0x08009680: 0749     ldr      r1, [pc, #0x1c]  ; =TIM14.CR1 */
    r1 = *(uint32_t *)0x080096A0;   /* = MMIO TIM14+0x00 */
    /* 0x08009682: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */
    /* 0x08009684: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08009686: 2f21     movs     r1, #0x2f */
    r1 = 0x2f;
    /* 0x08009688: 8260     str      r2, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r2;   /* store */
    /* 0x0800968A: 4160     str      r1, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r1;   /* store */
    /* 0x0800968C: c721     movs     r1, #0xc7 */
    r1 = 0xc7;
    /* 0x0800968E: 0261     str      r2, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r2;   /* store */
    /* 0x08009690: c160     str      r1, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r1;   /* store */
    /* 0x08009692: fef7a9fd bl       #0x80081e8 */
    FUN_080081E8();   /* BL -> 0x080081E8 FUN_080081E8 */
    /* 0x08009696: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009698: 01d0     beq      #0x800969e */
    if (r0 == 0) goto L_0800969E;
    /* 0x0800969A: fcf7d1fa bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_0800969E:
    /* 0x0800969E: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
