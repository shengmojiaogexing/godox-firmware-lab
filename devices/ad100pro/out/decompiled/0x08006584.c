/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006584
 * Name      : FUN_08006584
 * End       : 0x080065A2
 * Size      : 30 bytes, 14 instructions decoded in body (functions.json insns=14, blocks=1)
 * Class     : function
 * Callers   : 5  -> 0x0800AA74 FUN_0800AA74, 0x0800AAB2 FUN_0800AAB2, 0x0800AB7C FUN_0800AB7C, 0x0800ABCE FUN_0800ABCE, 0x0800C7D4 FUN_0800C7D4
 * Callees   : 1  -> 0x080072C4 FUN_080072C4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006584(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006584: 00b5     push     {lr} */
    push({lr});
    /* 0x08006586: 85b0     sub      sp, #0x14 */
    sp = sp - 0x14;
    /* 0x08006588: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800658A: c003     lsls     r0, r0, #0xf */
    r0 = r0 << 0xf;
    /* 0x0800658C: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x0800658E: 0548     ldr      r0, [pc, #0x14]  ; =0x10210000 */
    r0 = *(uint32_t *)0x080065A4;   /* = 0x10210000 */
    /* 0x08006590: 0190     str      r0, [sp, #4] */
    *(uint32_t *)(sp + 4) = r0;   /* store */
    /* 0x08006592: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006594: 0290     str      r0, [sp, #8] */
    *(uint32_t *)(sp + 8) = r0;   /* store */
    /* 0x08006596: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08006598: 0348     ldr      r0, [pc, #0xc]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x080065A8;   /* = MMIO GPIOB+0x00 */
    /* 0x0800659A: 00f093fe bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x0800659E: 05b0     add      sp, #0x14 */
    sp = sp + 0x14;
    /* 0x080065A0: 00bd     pop      {pc} */
    pop({pc});   /* return */
}
