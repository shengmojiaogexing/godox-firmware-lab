/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800D0E4
 * Name      : FUN_0800D0E4
 * End       : 0x0800D0FE
 * Size      : 26 bytes, 12 instructions decoded in body (functions.json insns=12, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x0800B7B0 FUN_0800B7B0
 * Callees   : 1  -> 0x080072C4 FUN_080072C4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800D0E4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800D0E4: 00b5     push     {lr} */
    push({lr});
    /* 0x0800D0E6: 85b0     sub      sp, #0x14 */
    sp = sp - 0x14;
    /* 0x0800D0E8: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800D0EA: 0548     ldr      r0, [pc, #0x14]  ; =0x10210000 */
    r0 = *(uint32_t *)0x0800D100;   /* = 0x10210000 */
    /* 0x0800D0EC: 0091     str      r1, [sp] */
    *(uint32_t *)(sp) = r1;   /* store */
    /* 0x0800D0EE: 0291     str      r1, [sp, #8] */
    *(uint32_t *)(sp + 8) = r1;   /* store */
    /* 0x0800D0F0: 0190     str      r0, [sp, #4] */
    *(uint32_t *)(sp + 4) = r0;   /* store */
    /* 0x0800D0F2: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x0800D0F4: 0348     ldr      r0, [pc, #0xc]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x0800D104;   /* = MMIO GPIOB+0x00 */
    /* 0x0800D0F6: faf7e5f8 bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x0800D0FA: 05b0     add      sp, #0x14 */
    sp = sp + 0x14;
    /* 0x0800D0FC: 00bd     pop      {pc} */
    pop({pc});   /* return */
}
