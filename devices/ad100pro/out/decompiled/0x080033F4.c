/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080033F4
 * Name      : FUN_080033F4
 * End       : 0x0800341A
 * Size      : 38 bytes, 19 instructions decoded in body (functions.json insns=19, blocks=7)
 * Class     : hub
 * Callers   : 11  -> 0x080038C0 FUN_080038C0, 0x080039C4 FUN_080039C4, 0x08003AC8 FUN_08003AC8, 0x08003B22 FUN_08003B22, 0x0800A034 FUN_0800A034, 0x0800A0A0 FUN_0800A0A0, 0x0800A0C2 FUN_0800A0C2, 0x0800A0D2 FUN_0800A0D2, ... +3
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080033F4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080033F4: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x080033F6: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x080033F8: 04db     blt      #0x8003404 */
    if (r1 < 0) goto L_08003404;   /* signed */
    /* 0x080033FA: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x080033FC: e407     lsls     r4, r4, #0x1f */
    r4 = r4 << 0x1f;
    /* 0x080033FE: 4042     rsbs     r0, r0, #0 */
    r0 = 0 - r0;
    /* 0x08003400: 8c41     sbcs     r4, r1 */
    r4 = r4 - r1;
    /* 0x08003402: 2146     mov      r1, r4 */
    r1 = r4;

    L_08003404:
    /* 0x08003404: 002b     cmp      r3, #0 */
    /* flags = cmp(r3, #0) */
    /* 0x08003406: 04db     blt      #0x8003412 */
    if (r3 < 0) goto L_08003412;   /* signed */
    /* 0x08003408: 1c46     mov      r4, r3 */
    r4 = r3;
    /* 0x0800340A: 0123     movs     r3, #1 */
    r3 = 1;
    /* 0x0800340C: db07     lsls     r3, r3, #0x1f */
    r3 = r3 << 0x1f;
    /* 0x0800340E: 5242     rsbs     r2, r2, #0 */
    r2 = 0 - r2;
    /* 0x08003410: a341     sbcs     r3, r4 */
    r3 = r3 - r4;

    L_08003412:
    /* 0x08003412: 8b42     cmp      r3, r1 */
    /* flags = cmp(r3, r1) */
    /* 0x08003414: 00d1     bne      #0x8003418 */
    if (r3 != r1) goto L_08003418;
    /* 0x08003416: 8242     cmp      r2, r0 */
    /* flags = cmp(r2, r0) */

    L_08003418:
    /* 0x08003418: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
