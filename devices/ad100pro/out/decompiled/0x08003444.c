/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08003444
 * Name      : FUN_08003444
 * End       : 0x08003464
 * Size      : 32 bytes, 16 instructions decoded in body (functions.json insns=16, blocks=3)
 * Class     : function
 * Callers   : 3  -> 0x08003168 lookup_or_pad, 0x080034A4 FUN_080034A4, 0x080034EC FUN_080034EC
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08003444(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08003444: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08003446: 202a     cmp      r2, #0x20 */
    /* flags = cmp(r2, #0x20) */
    /* 0x08003448: 04db     blt      #0x8003454 */
    if (r2 < 0x20) goto L_08003454;   /* signed */
    /* 0x0800344A: 0146     mov      r1, r0 */
    r1 = r0;
    /* 0x0800344C: 203a     subs     r2, #0x20 */
    r2 = r2 - 0x20;
    /* 0x0800344E: 9140     lsls     r1, r2 */
    r1 = r1 << r2;
    /* 0x08003450: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003452: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_08003454:
    /* 0x08003454: 9140     lsls     r1, r2 */
    r1 = r1 << r2;
    /* 0x08003456: 2023     movs     r3, #0x20 */
    r3 = 0x20;
    /* 0x08003458: 9c1a     subs     r4, r3, r2 */
    r4 = r3 - r2;
    /* 0x0800345A: 0346     mov      r3, r0 */
    r3 = r0;
    /* 0x0800345C: e340     lsrs     r3, r4 */
    r3 = r3 >> r4;
    /* 0x0800345E: 1943     orrs     r1, r3 */
    r1 = r1 | r3;
    /* 0x08003460: 9040     lsls     r0, r2 */
    r0 = r0 << r2;
    /* 0x08003462: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
