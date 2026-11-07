/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080030F0
 * Name      : FUN_080030F0
 * End       : 0x0800311C
 * Size      : 44 bytes, 22 instructions decoded in body (functions.json insns=22, blocks=5)
 * Class     : hub
 * Callers   : 45  -> 0x0800311C FUN_0800311C, 0x08003828 FUN_08003828, 0x08003F0C FUN_08003F0C, 0x08003F7A FUN_08003F7A, 0x0800419C FUN_0800419C, 0x08004230 FUN_08004230, 0x0800423E FUN_0800423E, 0x08004244 FUN_08004244, ... +37
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080030F0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080030F0: 30b5     push     {r4, r5, lr} */
    push({r4, r5, lr});
    /* 0x080030F2: 0b46     mov      r3, r1 */
    r3 = r1;
    /* 0x080030F4: 0146     mov      r1, r0 */
    r1 = r0;
    /* 0x080030F6: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080030F8: 2022     movs     r2, #0x20 */
    r2 = 0x20;
    /* 0x080030FA: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x080030FC: 09e0     b        #0x8003112 */
    goto L_08003112;

    L_080030FE:
    /* 0x080030FE: 0d46     mov      r5, r1 */
    r5 = r1;
    /* 0x08003100: d540     lsrs     r5, r2 */
    r5 = r5 >> r2;
    /* 0x08003102: 9d42     cmp      r5, r3 */
    /* flags = cmp(r5, r3) */
    /* 0x08003104: 05d3     blo      #0x8003112 */
    if (r5 < r3) goto L_08003112;   /* unsigned */
    /* 0x08003106: 1d46     mov      r5, r3 */
    r5 = r3;
    /* 0x08003108: 9540     lsls     r5, r2 */
    r5 = r5 << r2;
    /* 0x0800310A: 491b     subs     r1, r1, r5 */
    r1 = r1 - r5;
    /* 0x0800310C: 2546     mov      r5, r4 */
    r5 = r4;
    /* 0x0800310E: 9540     lsls     r5, r2 */
    r5 = r5 << r2;
    /* 0x08003110: 4019     adds     r0, r0, r5 */
    r0 = r0 + r5;

    L_08003112:
    /* 0x08003112: 1546     mov      r5, r2 */
    r5 = r2;
    /* 0x08003114: 521e     subs     r2, r2, #1 */
    r2 = r2 - 1;
    /* 0x08003116: 002d     cmp      r5, #0 */
    /* flags = cmp(r5, #0) */
    /* 0x08003118: f1dc     bgt      #0x80030fe */
    if (r5 > 0) goto L_080030FE;   /* signed */
    /* 0x0800311A: 30bd     pop      {r4, r5, pc} */
    pop({r4, r5, pc});   /* return */
}
