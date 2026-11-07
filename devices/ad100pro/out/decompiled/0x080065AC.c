/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080065AC
 * Name      : FUN_080065AC
 * End       : 0x080065C8
 * Size      : 28 bytes, 13 instructions decoded in body (functions.json insns=13, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x0800AB7C FUN_0800AB7C
 * Callees   : 1  -> 0x080072C4 FUN_080072C4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080065AC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080065AC: 00b5     push     {lr} */
    push({lr});
    /* 0x080065AE: 85b0     sub      sp, #0x14 */
    sp = sp - 0x14;
    /* 0x080065B0: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x080065B2: c003     lsls     r0, r0, #0xf */
    r0 = r0 << 0xf;
    /* 0x080065B4: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x080065B6: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080065B8: 0190     str      r0, [sp, #4] */
    *(uint32_t *)(sp + 4) = r0;   /* store */
    /* 0x080065BA: 0290     str      r0, [sp, #8] */
    *(uint32_t *)(sp + 8) = r0;   /* store */
    /* 0x080065BC: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x080065BE: 0248     ldr      r0, [pc, #8]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x080065C8;   /* = MMIO GPIOB+0x00 */
    /* 0x080065C0: 00f080fe bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x080065C4: 05b0     add      sp, #0x14 */
    sp = sp + 0x14;
    /* 0x080065C6: 00bd     pop      {pc} */
    pop({pc});   /* return */
}
