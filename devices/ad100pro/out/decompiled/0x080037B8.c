/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080037B8
 * Name      : FUN_080037B8
 * End       : 0x080037C2
 * Size      : 10 bytes, 4 instructions decoded in body (functions.json insns=4, blocks=1)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x080069A6 FUN_080069A6
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080037B8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080037B8: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x080037BA: 406a     ldr      r0, [r0, #0x24] */
    r0 = *(uint32_t *)(r0 + 36);
    /* 0x080037BC: 03f0f3f8 bl       #0x80069a6 */
    FUN_080069A6();   /* BL -> 0x080069A6 FUN_080069A6 */
    /* 0x080037C0: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
