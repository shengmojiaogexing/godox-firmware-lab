/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800854C
 * Name      : FUN_0800854C
 * End       : 0x0800857A
 * Size      : 46 bytes, 22 instructions decoded in body (functions.json insns=22, blocks=3)
 * Class     : function
 * Callers   : 2  -> 0x08009768 FUN_08009768, 0x0800A8D0 FUN_0800A8D0
 * Callees   : 1  -> 0x080072C4 FUN_080072C4
 * MMIO      : RCC.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800854C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800854C: 00b5     push     {lr} */
    push({lr});
    /* 0x0800854E: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x08008550: 0a49     ldr      r1, [pc, #0x28]  ; =TIM3.CR1 */
    r1 = *(uint32_t *)0x0800857C;   /* = MMIO TIM3+0x00 */
    /* 0x08008552: 85b0     sub      sp, #0x14 */
    sp = sp - 0x14;
    /* 0x08008554: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08008556: 0ed1     bne      #0x8008576 */
    if (r0 != r1) goto L_08008576;
    /* 0x08008558: 8020     movs     r0, #0x80 */
    r0 = 0x80;
    /* 0x0800855A: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x0800855C: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x0800855E: 0190     str      r0, [sp, #4] */
    *(uint32_t *)(sp + 4) = r0;   /* store */
    /* 0x08008560: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08008562: 0290     str      r0, [sp, #8] */
    *(uint32_t *)(sp + 8) = r0;   /* store */
    /* 0x08008564: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x08008566: 0390     str      r0, [sp, #0xc] */
    *(uint32_t *)(sp + 12) = r0;   /* store */
    /* 0x08008568: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800856A: 0490     str      r0, [sp, #0x10] */
    *(uint32_t *)(sp + 16) = r0;   /* store */
    /* 0x0800856C: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x0800856E: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08008570: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x08008572: fef7a7fe bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */

    L_08008576:
    /* 0x08008576: 05b0     add      sp, #0x14 */
    sp = sp + 0x14;
    /* 0x08008578: 00bd     pop      {pc} */
    pop({pc});   /* return */
}
