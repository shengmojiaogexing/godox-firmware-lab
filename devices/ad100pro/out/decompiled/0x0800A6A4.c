/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A6A4
 * Name      : FUN_0800A6A4
 * End       : 0x0800A6B4
 * Size      : 16 bytes, 7 instructions decoded in body (functions.json insns=7, blocks=1)
 * Class     : function
 * Callers   : 4  -> 0x0800AB7C FUN_0800AB7C, 0x0800AB9E FUN_0800AB9E, 0x0800C204 FUN_0800C204, 0x0800C21E FUN_0800C21E
 * Callees   : 1  -> 0x08007426 FUN_08007426
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A6A4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A6A4: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800A6A6: ff21     movs     r1, #0xff */
    r1 = 0xff;
    /* 0x0800A6A8: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800A6AA: 0131     adds     r1, #1 */
    r1 = r1 + 1;
    /* 0x0800A6AC: 0148     ldr      r0, [pc, #4]  ; =GPIOC.MODER */
    r0 = *(uint32_t *)0x0800A6B4;   /* = MMIO GPIOC+0x00 */
    /* 0x0800A6AE: fcf7bafe bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A6B2: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
