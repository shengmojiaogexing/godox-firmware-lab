/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005EF4
 * Name      : FUN_08005EF4
 * End       : 0x08005F2A
 * Size      : 54 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=5)
 * Class     : function
 * Callers   : 1  -> 0x0800B588 FUN_0800B588
 * Callees   : 3  -> 0x080071F4 FUN_080071F4, 0x08007208 FUN_08007208, 0x08007284 FUN_08007284
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005EF4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005EF4: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08005EF6: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x08005EF8: 01f0c4f9 bl       #0x8007284 */
    FUN_08007284();   /* BL -> 0x08007284 FUN_08007284 */
    /* 0x08005EFC: 0b4d     ldr      r5, [pc, #0x2c]  ; =RAM_20000120 */
    r5 = *(uint32_t *)0x08005F2C;   /* = RAM 0x20000120 */
    /* 0x08005EFE: 0c4e     ldr      r6, [pc, #0x30]  ; =RAM_20000434 */
    r6 = *(uint32_t *)0x08005F30;   /* = RAM 0x20000434 */
    /* 0x08005F00: ac60     str      r4, [r5, #8] */
    *(uint32_t *)(r5 + 8) = r4;   /* store */
    /* 0x08005F02: 0024     movs     r4, #0 */
    r4 = 0;

    L_08005F04:
    /* 0x08005F04: a000     lsls     r0, r4, #2 */
    r0 = r4 << 2;
    /* 0x08005F06: 3258     ldr      r2, [r6, r0] */
    r2 = *(uint32_t *)(r6 + r0);
    /* 0x08005F08: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x08005F0A: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08005F0C: a968     ldr      r1, [r5, #8] */
    r1 = *(uint32_t *)(r5 + 8);
    /* 0x08005F0E: 01f07bf9 bl       #0x8007208 */
    FUN_08007208();   /* BL -> 0x08007208 FUN_08007208 */
    /* 0x08005F12: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08005F14: 02d1     bne      #0x8005f1c */
    if (r0 != 0) goto L_08005F1C;
    /* 0x08005F16: a868     ldr      r0, [r5, #8] */
    r0 = *(uint32_t *)(r5 + 8);
    /* 0x08005F18: 001d     adds     r0, r0, #4 */
    r0 = r0 + 4;
    /* 0x08005F1A: a860     str      r0, [r5, #8] */
    *(uint32_t *)(r5 + 8) = r0;   /* store */

    L_08005F1C:
    /* 0x08005F1C: 641c     adds     r4, r4, #1 */
    r4 = r4 + 1;
    /* 0x08005F1E: e4b2     uxtb     r4, r4 */
    r4 = uxtb(r4);
    /* 0x08005F20: 082c     cmp      r4, #8 */
    /* flags = cmp(r4, #8) */
    /* 0x08005F22: efd3     blo      #0x8005f04 */
    if (r4 < 8) goto L_08005F04;   /* unsigned */
    /* 0x08005F24: 01f066f9 bl       #0x80071f4 */
    FUN_080071F4();   /* BL -> 0x080071F4 FUN_080071F4 */
    /* 0x08005F28: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
