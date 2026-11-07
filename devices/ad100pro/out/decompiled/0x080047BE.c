/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080047BE
 * Name      : FUN_080047BE
 * End       : 0x080047CC
 * Size      : 14 bytes, 6 instructions decoded in body (functions.json insns=6, blocks=1)
 * Class     : hub
 * Callers   : 28  -> 0x08004670 FUN_08004670, 0x0800467C FUN_0800467C, 0x0800C954 FUN_0800C954, 0x0800C964 FUN_0800C964, 0x0800C96C FUN_0800C96C, 0x0800C974 FUN_0800C974, 0x0800C97C FUN_0800C97C, 0x0800C98C FUN_0800C98C, ... +20
 * Callees   : 1  -> 0x0800A480 FUN_0800A480
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080047BE(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080047BE: 0a46     mov      r2, r1 */
    r2 = r1;
    /* 0x080047C0: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x080047C2: 0146     mov      r1, r0 */
    r1 = r0;
    /* 0x080047C4: 1046     mov      r0, r2 */
    r0 = r2;
    /* 0x080047C6: 05f05bfe bl       #0x800a480 */
    FUN_0800A480();   /* BL -> 0x0800A480 FUN_0800A480 */
    /* 0x080047CA: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
