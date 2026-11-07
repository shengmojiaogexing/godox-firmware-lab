/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800655C
 * Name      : FUN_0800655C
 * End       : 0x0800657A
 * Size      : 30 bytes, 14 instructions decoded in body (functions.json insns=14, blocks=1)
 * Class     : function
 * Callers   : 4  -> 0x0800AFC4 FUN_0800AFC4, 0x0800B03C FUN_0800B03C, 0x0800B0BE FUN_0800B0BE, 0x0800B188 FUN_0800B188
 * Callees   : 1  -> 0x080072C4 FUN_080072C4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800655C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800655C: 00b5     push     {lr} */
    push({lr});
    /* 0x0800655E: 85b0     sub      sp, #0x14 */
    sp = sp - 0x14;
    /* 0x08006560: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08006562: c003     lsls     r0, r0, #0xf */
    r0 = r0 << 0xf;
    /* 0x08006564: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08006566: 0548     ldr      r0, [pc, #0x14]  ; =0x10110000 */
    r0 = *(uint32_t *)0x0800657C;   /* = 0x10110000 */
    /* 0x08006568: 0190     str      r0, [sp, #4] */
    *(uint32_t *)(sp + 4) = r0;   /* store */
    /* 0x0800656A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800656C: 0290     str      r0, [sp, #8] */
    *(uint32_t *)(sp + 8) = r0;   /* store */
    /* 0x0800656E: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08006570: 0348     ldr      r0, [pc, #0xc]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x08006580;   /* = MMIO GPIOB+0x00 */
    /* 0x08006572: 00f0a7fe bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08006576: 05b0     add      sp, #0x14 */
    sp = sp + 0x14;
    /* 0x08006578: 00bd     pop      {pc} */
    pop({pc});   /* return */
}
