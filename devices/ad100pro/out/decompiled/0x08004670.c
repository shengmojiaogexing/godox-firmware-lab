/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004670
 * Name      : FUN_08004670
 * End       : 0x0800468E
 * Size      : 30 bytes, 13 instructions decoded in body (functions.json insns=13, blocks=1)
 * Class     : hub
 * Callers   : 9  -> 0x0800A30C FUN_0800A30C, 0x0800A32C FUN_0800A32C, 0x0800A356 FUN_0800A356, 0x0800A370 FUN_0800A370, 0x0800A394 FUN_0800A394, 0x0800A3C8 FUN_0800A3C8, 0x0800A3EE FUN_0800A3EE, 0x0800A41C FUN_0800A41C, ... +1
 * Callees   : 1  -> 0x080047BE FUN_080047BE
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004670(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004670: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08004672: 0109     lsrs     r1, r0, #4 */
    r1 = r0 >> 4;
    /* 0x08004674: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x08004676: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08004678: 1031     adds     r1, #0x10 */
    r1 = r1 + 0x10;
    /* 0x0800467A: 00f0a0f8 bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800467E: 2107     lsls     r1, r4, #0x1c */
    r1 = r4 << 0x1c;
    /* 0x08004680: 090f     lsrs     r1, r1, #0x1c */
    r1 = r1 >> 0x1c;
    /* 0x08004682: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08004684: 0143     orrs     r1, r0 */
    r1 = r1 | r0;
    /* 0x08004686: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08004688: 00f099f8 bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800468C: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
