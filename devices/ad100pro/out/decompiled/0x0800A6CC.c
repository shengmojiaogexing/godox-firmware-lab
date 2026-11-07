/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A6CC
 * Name      : FUN_0800A6CC
 * End       : 0x0800A6DC
 * Size      : 16 bytes, 7 instructions decoded in body (functions.json insns=7, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x0800C204 FUN_0800C204
 * Callees   : 1  -> 0x08007426 FUN_08007426
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A6CC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A6CC: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800A6CE: ff21     movs     r1, #0xff */
    r1 = 0xff;
    /* 0x0800A6D0: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800A6D2: 0131     adds     r1, #1 */
    r1 = r1 + 1;
    /* 0x0800A6D4: 0148     ldr      r0, [pc, #4]  ; =GPIOC.MODER */
    r0 = *(uint32_t *)0x0800A6DC;   /* = MMIO GPIOC+0x00 */
    /* 0x0800A6D6: fcf7a6fe bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A6DA: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
