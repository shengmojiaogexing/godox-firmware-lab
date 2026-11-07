/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800D134
 * Name      : FUN_0800D134
 * End       : 0x0800D15E
 * Size      : 42 bytes, 17 instructions decoded in body (functions.json insns=17, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x0800D088 FUN_0800D088
 * Callees   : 2  -> 0x08007426 FUN_08007426, 0x08009038 FUN_08009038
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800D134(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800D134: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800D136: ff24     movs     r4, #0xff */
    r4 = 0xff;
    /* 0x0800D138: 094d     ldr      r5, [pc, #0x24]  ; =GPIOB.MODER */
    r5 = *(uint32_t *)0x0800D160;   /* = MMIO GPIOB+0x00 */
    /* 0x0800D13A: 0134     adds     r4, #1 */
    r4 = r4 + 1;
    /* 0x0800D13C: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800D13E: 2146     mov      r1, r4 */
    r1 = r4;
    /* 0x0800D140: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800D142: faf770f9 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800D146: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x0800D148: fbf776ff bl       #0x8009038 */
    FUN_08009038();   /* BL -> 0x08009038 FUN_08009038 */
    /* 0x0800D14C: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800D14E: 2146     mov      r1, r4 */
    r1 = r4;
    /* 0x0800D150: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800D152: faf768f9 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800D156: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x0800D158: fbf76eff bl       #0x8009038 */
    FUN_08009038();   /* BL -> 0x08009038 FUN_08009038 */
    /* 0x0800D15C: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
