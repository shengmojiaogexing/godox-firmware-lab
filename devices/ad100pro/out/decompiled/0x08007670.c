/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007670
 * Name      : FUN_08007670
 * End       : 0x08007678
 * Size      : 8 bytes, 3 instructions decoded in body (functions.json insns=3, blocks=1)
 * Class     : hub
 * Callers   : 9  -> 0x080075EC FUN_080075EC, 0x080075FE FUN_080075FE, 0x08007610 FUN_08007610, 0x08007F38 FUN_08007F38, 0x0800821C FUN_0800821C, 0x080093F4 FUN_080093F4, 0x0800941C FUN_0800941C, 0x08009576 FUN_08009576, ... +1
 * Callees   : 1  -> 0x0800A2C8 FUN_0800A2C8
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007670(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007670: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08007672: 02f029fe bl       #0x800a2c8 */
    FUN_0800A2C8();   /* BL -> 0x0800A2C8 FUN_0800A2C8 */
    /* 0x08007676: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
