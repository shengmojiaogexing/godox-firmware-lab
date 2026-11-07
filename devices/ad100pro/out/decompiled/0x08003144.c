/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08003144
 * Name      : FUN_08003144
 * End       : 0x08003166
 * Size      : 34 bytes, 17 instructions decoded in body (functions.json insns=17, blocks=3)
 * Class     : function
 * Callers   : 4  -> 0x080033B8 FUN_080033B8, 0x080034A4 FUN_080034A4, 0x080034C8 FUN_080034C8, 0x08007208 FUN_08007208
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08003144(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08003144: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08003146: 202a     cmp      r2, #0x20 */
    /* flags = cmp(r2, #0x20) */
    /* 0x08003148: 04db     blt      #0x8003154 */
    if (r2 < 0x20) goto L_08003154;   /* signed */
    /* 0x0800314A: 0846     mov      r0, r1 */
    r0 = r1;
    /* 0x0800314C: 203a     subs     r2, #0x20 */
    r2 = r2 - 0x20;
    /* 0x0800314E: d040     lsrs     r0, r2 */
    r0 = r0 >> r2;
    /* 0x08003150: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x08003152: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_08003154:
    /* 0x08003154: 0b46     mov      r3, r1 */
    r3 = r1;
    /* 0x08003156: d340     lsrs     r3, r2 */
    r3 = r3 >> r2;
    /* 0x08003158: d040     lsrs     r0, r2 */
    r0 = r0 >> r2;
    /* 0x0800315A: 2024     movs     r4, #0x20 */
    r4 = 0x20;
    /* 0x0800315C: a21a     subs     r2, r4, r2 */
    r2 = r4 - r2;
    /* 0x0800315E: 9140     lsls     r1, r2 */
    r1 = r1 << r2;
    /* 0x08003160: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08003162: 1946     mov      r1, r3 */
    r1 = r3;
    /* 0x08003164: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
