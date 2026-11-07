/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CE58
 * Name      : FUN_0800CE58
 * End       : 0x0800CE66
 * Size      : 14 bytes, 6 instructions decoded in body (functions.json insns=6, blocks=1)
 * Class     : function
 * Callers   : 4  -> 0x0800CDC8 FUN_0800CDC8, 0x0800CDE2 FUN_0800CDE2, 0x0800CDF4 FUN_0800CDF4, 0x0800CE0C FUN_0800CE0C
 * Callees   : 1  -> 0x08007426 FUN_08007426
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CE58(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CE58: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800CE5A: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800CE5C: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x0800CE5E: 0248     ldr      r0, [pc, #8]  ; =GPIOC.MODER */
    r0 = *(uint32_t *)0x0800CE68;   /* = MMIO GPIOC+0x00 */
    /* 0x0800CE60: faf7e1fa bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800CE64: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
