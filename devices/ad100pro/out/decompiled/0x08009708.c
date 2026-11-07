/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009708
 * Name      : FUN_08009708
 * End       : 0x0800975C
 * Size      : 84 bytes, 36 instructions decoded in body (functions.json insns=36, blocks=7)
 * Class     : function
 * Callers   : 1  -> 0x0800CF74 main
 * Callees   : 4  -> 0x08005C40 FUN_08005C40, 0x080081A0 FUN_080081A0, 0x080081E8 FUN_080081E8, 0x080082F8 FUN_080082F8
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009708(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009708: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800970A: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800970C: 1348     ldr      r0, [pc, #0x4c]  ; =RAM_200002D0 */
    r0 = *(uint32_t *)0x0800975C;   /* = RAM 0x200002D0 */
    /* 0x0800970E: 86b0     sub      sp, #0x18 */
    sp = sp - 0x18;
    /* 0x08009710: 8907     lsls     r1, r1, #0x1e */
    r1 = r1 << 0x1e;
    /* 0x08009712: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */
    /* 0x08009714: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x08009716: 1249     ldr      r1, [pc, #0x48]  ; =0x000003BF */
    r1 = *(uint32_t *)0x08009760;   /* = 0x000003BF */
    /* 0x08009718: 8460     str      r4, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r4;   /* store */
    /* 0x0800971A: 4160     str      r1, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r1;   /* store */
    /* 0x0800971C: 1149     ldr      r1, [pc, #0x44]  ; =0x0000C350 */
    r1 = *(uint32_t *)0x08009764;   /* = 0x0000C350 */
    /* 0x0800971E: 0461     str      r4, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r4;   /* store */
    /* 0x08009720: c160     str      r1, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r1;   /* store */
    /* 0x08009722: fef761fd bl       #0x80081e8 */
    FUN_080081E8();   /* BL -> 0x080081E8 FUN_080081E8 */
    /* 0x08009726: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009728: 01d0     beq      #0x800972e */
    if (r0 == 0) goto L_0800972E;
    /* 0x0800972A: fcf789fa bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_0800972E:
    /* 0x0800972E: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08009730: 0003     lsls     r0, r0, #0xc */
    r0 = r0 << 0xc;
    /* 0x08009732: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08009734: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08009736: 0948     ldr      r0, [pc, #0x24]  ; =RAM_200002D0 */
    r0 = *(uint32_t *)0x0800975C;   /* = RAM 0x200002D0 */
    /* 0x08009738: fef7defd bl       #0x80082f8 */
    FUN_080082F8();   /* BL -> 0x080082F8 FUN_080082F8 */
    /* 0x0800973C: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800973E: 01d0     beq      #0x8009744 */
    if (r0 == 0) goto L_08009744;
    /* 0x08009740: fcf77efa bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_08009744:
    /* 0x08009744: 0494     str      r4, [sp, #0x10] */
    *(uint32_t *)(sp + 16) = r4;   /* store */
    /* 0x08009746: 04a9     add      r1, sp, #0x10 */
    r1 = sp + 0x10;
    /* 0x08009748: 0448     ldr      r0, [pc, #0x10]  ; =RAM_200002D0 */
    r0 = *(uint32_t *)0x0800975C;   /* = RAM 0x200002D0 */
    /* 0x0800974A: 0594     str      r4, [sp, #0x14] */
    *(uint32_t *)(sp + 20) = r4;   /* store */
    /* 0x0800974C: fef728fd bl       #0x80081a0 */
    FUN_080081A0();   /* BL -> 0x080081A0 FUN_080081A0 */
    /* 0x08009750: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009752: 01d0     beq      #0x8009758 */
    if (r0 == 0) goto L_08009758;
    /* 0x08009754: fcf774fa bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_08009758:
    /* 0x08009758: 06b0     add      sp, #0x18 */
    sp = sp + 0x18;
    /* 0x0800975A: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
