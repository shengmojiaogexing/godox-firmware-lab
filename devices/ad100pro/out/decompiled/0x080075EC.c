/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080075EC
 * Name      : FUN_080075EC
 * End       : 0x0800760E
 * Size      : 34 bytes, 13 instructions decoded in body (functions.json insns=13, blocks=1)
 * Class     : function
 * Callers   : 3  -> 0x080075CC FUN_080075CC, 0x08007798 FUN_08007798, 0x0800786C FUN_0800786C
 * Callees   : 4  -> 0x080030F0 FUN_080030F0, 0x08007670 FUN_08007670, 0x080078EC FUN_080078EC, 0x08008164 FUN_08008164
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080075EC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080075EC: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x080075EE: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x080075F0: 00f07cf9 bl       #0x80078ec */
    FUN_080078EC();   /* BL -> 0x080078EC FUN_080078EC */
    /* 0x080075F4: 7d21     movs     r1, #0x7d */
    r1 = 0x7d;
    /* 0x080075F6: c900     lsls     r1, r1, #3 */
    r1 = r1 << 3;
    /* 0x080075F8: fbf77afd bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x080075FC: 00f0b2fd bl       #0x8008164 */
    FUN_08008164();   /* BL -> 0x08008164 FUN_08008164 */
    /* 0x08007600: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08007602: 2146     mov      r1, r4 */
    r1 = r4;
    /* 0x08007604: 501e     subs     r0, r2, #1 */
    r0 = r2 - 1;
    /* 0x08007606: 00f033f8 bl       #0x8007670 */
    FUN_08007670();   /* BL -> 0x08007670 FUN_08007670 */
    /* 0x0800760A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800760C: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
