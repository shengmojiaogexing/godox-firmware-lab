/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080095F8
 * Name      : FUN_080095F8
 * End       : 0x08009672
 * Size      : 122 bytes, 53 instructions decoded in body (functions.json insns=53, blocks=9)
 * Class     : function
 * Callers   : 1  -> 0x0800CF74 main
 * Callees   : 5  -> 0x08005C40 FUN_08005C40, 0x08007D58 FUN_08007D58, 0x08007E70 FUN_08007E70, 0x08007F68 FUN_08007F68, 0x08008030 FUN_08008030
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080095F8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080095F8: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x080095FA: 1e49     ldr      r1, [pc, #0x78]  ; =RTC.TR */
    r1 = *(uint32_t *)0x08009674;   /* = MMIO RTC+0x00 */
    /* 0x080095FC: 1e48     ldr      r0, [pc, #0x78]  ; =RAM_20000274 */
    r0 = *(uint32_t *)0x08009678;   /* = RAM 0x20000274 */
    /* 0x080095FE: 86b0     sub      sp, #0x18 */
    sp = sp - 0x18;
    /* 0x08009600: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x08009602: 12c0     stm      r0!, {r1, r4} */
    /* stm r0!, {r1, r4} */
    /* 0x08009604: 7f21     movs     r1, #0x7f */
    r1 = 0x7f;
    /* 0x08009606: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */
    /* 0x08009608: 8460     str      r4, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r4;   /* store */
    /* 0x0800960A: c460     str      r4, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r4;   /* store */
    /* 0x0800960C: ff21     movs     r1, #0xff */
    r1 = 0xff;
    /* 0x0800960E: 0461     str      r4, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r4;   /* store */
    /* 0x08009610: 4160     str      r1, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r1;   /* store */
    /* 0x08009612: 0838     subs     r0, #8 */
    r0 = r0 - 8;
    /* 0x08009614: fef72cfc bl       #0x8007e70 */
    FUN_08007E70();   /* BL -> 0x08007E70 FUN_08007E70 */
    /* 0x08009618: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800961A: 01d0     beq      #0x8009620 */
    if (r0 == 0) goto L_08009620;
    /* 0x0800961C: fcf710fb bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_08009620:
    /* 0x08009620: 6846     mov      r0, sp */
    r0 = sp;
    /* 0x08009622: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */
    /* 0x08009624: 4470     strb     r4, [r0, #1] */
    *(uint8_t *)(r0 + 1) = r4;   /* store */
    /* 0x08009626: 8470     strb     r4, [r0, #2] */
    *(uint8_t *)(r0 + 2) = r4;   /* store */
    /* 0x08009628: 0394     str      r4, [sp, #0xc] */
    *(uint32_t *)(sp + 12) = r4;   /* store */
    /* 0x0800962A: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800962C: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x0800962E: 1248     ldr      r0, [pc, #0x48]  ; =RAM_20000274 */
    r0 = *(uint32_t *)0x08009678;   /* = RAM 0x20000274 */
    /* 0x08009630: 0494     str      r4, [sp, #0x10] */
    *(uint32_t *)(sp + 16) = r4;   /* store */
    /* 0x08009632: fef7fdfc bl       #0x8008030 */
    FUN_08008030();   /* BL -> 0x08008030 FUN_08008030 */
    /* 0x08009636: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009638: 01d0     beq      #0x800963e */
    if (r0 == 0) goto L_0800963E;
    /* 0x0800963A: fcf701fb bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_0800963E:
    /* 0x0800963E: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08009640: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08009642: 0875     strb     r0, [r1, #0x14] */
    *(uint8_t *)(r1 + 20) = r0;   /* store */  /* MMIO: RTC.WUTR(w) */
    /* 0x08009644: 4875     strb     r0, [r1, #0x15] */
    *(uint8_t *)(r1 + 21) = r0;   /* store */  /* MMIO: RTC.+0x15(w) */
    /* 0x08009646: 8875     strb     r0, [r1, #0x16] */
    *(uint8_t *)(r1 + 22) = r0;   /* store */  /* MMIO: RTC.+0x16(w) */
    /* 0x08009648: cc75     strb     r4, [r1, #0x17] */
    *(uint8_t *)(r1 + 23) = r4;   /* store */  /* MMIO: RTC.+0x17(w) */
    /* 0x0800964A: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x0800964C: 05a9     add      r1, sp, #0x14 */
    r1 = sp + 0x14;
    /* 0x0800964E: 0a48     ldr      r0, [pc, #0x28]  ; =RAM_20000274 */
    r0 = *(uint32_t *)0x08009678;   /* = RAM 0x20000274 */
    /* 0x08009650: fef78afc bl       #0x8007f68 */
    FUN_08007F68();   /* BL -> 0x08007F68 FUN_08007F68 */
    /* 0x08009654: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009656: 01d0     beq      #0x800965c */
    if (r0 == 0) goto L_0800965C;
    /* 0x08009658: fcf7f2fa bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_0800965C:
    /* 0x0800965C: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800965E: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x08009660: 0548     ldr      r0, [pc, #0x14]  ; =RAM_20000274 */
    r0 = *(uint32_t *)0x08009678;   /* = RAM 0x20000274 */
    /* 0x08009662: fef779fb bl       #0x8007d58 */
    FUN_08007D58();   /* BL -> 0x08007D58 FUN_08007D58 */
    /* 0x08009666: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009668: 01d0     beq      #0x800966e */
    if (r0 == 0) goto L_0800966E;
    /* 0x0800966A: fcf7e9fa bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_0800966E:
    /* 0x0800966E: 06b0     add      sp, #0x18 */
    sp = sp + 0x18;
    /* 0x08009670: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
