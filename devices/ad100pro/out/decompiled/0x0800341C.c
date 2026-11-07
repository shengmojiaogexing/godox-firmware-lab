/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800341C
 * Name      : FUN_0800341C
 * End       : 0x08003442
 * Size      : 38 bytes, 19 instructions decoded in body (functions.json insns=19, blocks=7)
 * Class     : hub
 * Callers   : 8  -> 0x080038C0 FUN_080038C0, 0x080039B8 FUN_080039B8, 0x080039D4 FUN_080039D4, 0x08003AC8 FUN_08003AC8, 0x08003B2E FUN_08003B2E, 0x0800610C FUN_0800610C, 0x080061EA FUN_080061EA, 0x08006294 FUN_08006294
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800341C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800341C: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800341E: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x08003420: 04db     blt      #0x800342c */
    if (r1 < 0) goto L_0800342C;   /* signed */
    /* 0x08003422: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x08003424: e407     lsls     r4, r4, #0x1f */
    r4 = r4 << 0x1f;
    /* 0x08003426: 4042     rsbs     r0, r0, #0 */
    r0 = 0 - r0;
    /* 0x08003428: 8c41     sbcs     r4, r1 */
    r4 = r4 - r1;
    /* 0x0800342A: 2146     mov      r1, r4 */
    r1 = r4;

    L_0800342C:
    /* 0x0800342C: 002b     cmp      r3, #0 */
    /* flags = cmp(r3, #0) */
    /* 0x0800342E: 04db     blt      #0x800343a */
    if (r3 < 0) goto L_0800343A;   /* signed */
    /* 0x08003430: 1c46     mov      r4, r3 */
    r4 = r3;
    /* 0x08003432: 0123     movs     r3, #1 */
    r3 = 1;
    /* 0x08003434: db07     lsls     r3, r3, #0x1f */
    r3 = r3 << 0x1f;
    /* 0x08003436: 5242     rsbs     r2, r2, #0 */
    r2 = 0 - r2;
    /* 0x08003438: a341     sbcs     r3, r4 */
    r3 = r3 - r4;

    L_0800343A:
    /* 0x0800343A: 9942     cmp      r1, r3 */
    /* flags = cmp(r1, r3) */
    /* 0x0800343C: 00d1     bne      #0x8003440 */
    if (r1 != r3) goto L_08003440;
    /* 0x0800343E: 9042     cmp      r0, r2 */
    /* flags = cmp(r0, r2) */

    L_08003440:
    /* 0x08003440: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
