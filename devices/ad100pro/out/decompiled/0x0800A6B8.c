/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A6B8
 * Name      : FUN_0800A6B8
 * End       : 0x0800A6C6
 * Size      : 14 bytes, 6 instructions decoded in body (functions.json insns=6, blocks=1)
 * Class     : function
 * Callers   : 2  -> 0x0800AB7C FUN_0800AB7C, 0x0800ABB8 FUN_0800ABB8
 * Callees   : 1  -> 0x08007426 FUN_08007426
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A6B8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A6B8: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800A6BA: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800A6BC: 1102     lsls     r1, r2, #8 */
    r1 = r2 << 8;
    /* 0x0800A6BE: 0248     ldr      r0, [pc, #8]  ; =GPIOC.MODER */
    r0 = *(uint32_t *)0x0800A6C8;   /* = MMIO GPIOC+0x00 */
    /* 0x0800A6C0: fcf7b1fe bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A6C4: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
