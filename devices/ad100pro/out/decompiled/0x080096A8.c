/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080096A8
 * Name      : FUN_080096A8
 * End       : 0x080096FC
 * Size      : 84 bytes, 36 instructions decoded in body (functions.json insns=36, blocks=7)
 * Class     : function
 * Callers   : 1  -> 0x0800CF74 main
 * Callees   : 4  -> 0x08005C40 FUN_08005C40, 0x080081A0 FUN_080081A0, 0x080081E8 FUN_080081E8, 0x080082F8 FUN_080082F8
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080096A8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080096A8: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x080096AA: 1548     ldr      r0, [pc, #0x54]  ; =RAM_20000294 */
    r0 = *(uint32_t *)0x08009700;   /* = RAM 0x20000294 */
    /* 0x080096AC: 86b0     sub      sp, #0x18 */
    sp = sp - 0x18;
    /* 0x080096AE: 1349     ldr      r1, [pc, #0x4c]  ; =TIM1.CR1 */
    r1 = *(uint32_t *)0x080096FC;   /* = MMIO TIM1+0x00 */
    /* 0x080096B0: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */
    /* 0x080096B2: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x080096B4: 2f21     movs     r1, #0x2f */
    r1 = 0x2f;
    /* 0x080096B6: 8460     str      r4, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r4;   /* store */
    /* 0x080096B8: 4160     str      r1, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r1;   /* store */
    /* 0x080096BA: 0461     str      r4, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r4;   /* store */
    /* 0x080096BC: 1149     ldr      r1, [pc, #0x44]  ; =0x0000270F */
    r1 = *(uint32_t *)0x08009704;   /* = 0x0000270F */
    /* 0x080096BE: 4461     str      r4, [r0, #0x14] */
    *(uint32_t *)(r0 + 20) = r4;   /* store */
    /* 0x080096C0: c160     str      r1, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r1;   /* store */
    /* 0x080096C2: fef791fd bl       #0x80081e8 */
    FUN_080081E8();   /* BL -> 0x080081E8 FUN_080081E8 */
    /* 0x080096C6: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080096C8: 01d0     beq      #0x80096ce */
    if (r0 == 0) goto L_080096CE;
    /* 0x080096CA: fcf7b9fa bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_080096CE:
    /* 0x080096CE: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x080096D0: 0003     lsls     r0, r0, #0xc */
    r0 = r0 << 0xc;
    /* 0x080096D2: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x080096D4: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x080096D6: 0a48     ldr      r0, [pc, #0x28]  ; =RAM_20000294 */
    r0 = *(uint32_t *)0x08009700;   /* = RAM 0x20000294 */
    /* 0x080096D8: fef70efe bl       #0x80082f8 */
    FUN_080082F8();   /* BL -> 0x080082F8 FUN_080082F8 */
    /* 0x080096DC: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080096DE: 01d0     beq      #0x80096e4 */
    if (r0 == 0) goto L_080096E4;
    /* 0x080096E0: fcf7aefa bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_080096E4:
    /* 0x080096E4: 0494     str      r4, [sp, #0x10] */
    *(uint32_t *)(sp + 16) = r4;   /* store */
    /* 0x080096E6: 04a9     add      r1, sp, #0x10 */
    r1 = sp + 0x10;
    /* 0x080096E8: 0548     ldr      r0, [pc, #0x14]  ; =RAM_20000294 */
    r0 = *(uint32_t *)0x08009700;   /* = RAM 0x20000294 */
    /* 0x080096EA: 0594     str      r4, [sp, #0x14] */
    *(uint32_t *)(sp + 20) = r4;   /* store */
    /* 0x080096EC: fef758fd bl       #0x80081a0 */
    FUN_080081A0();   /* BL -> 0x080081A0 FUN_080081A0 */
    /* 0x080096F0: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080096F2: 01d0     beq      #0x80096f8 */
    if (r0 == 0) goto L_080096F8;
    /* 0x080096F4: fcf7a4fa bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_080096F8:
    /* 0x080096F8: 06b0     add      sp, #0x18 */
    sp = sp + 0x18;
    /* 0x080096FA: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
