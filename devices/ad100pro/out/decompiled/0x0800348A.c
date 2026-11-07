/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800348A
 * Name      : FUN_0800348A
 * End       : 0x080034A4
 * Size      : 26 bytes, 13 instructions decoded in body (functions.json insns=13, blocks=4)
 * Class     : function
 * Callers   : 3  -> 0x0800322E FUN_0800322E, 0x080034A4 FUN_080034A4, 0x080034EC FUN_080034EC
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800348A(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800348A: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800348C: 002b     cmp      r3, #0 */
    /* flags = cmp(r3, #0) */
    /* 0x0800348E: 08da     bge      #0x80034a2 */
    if (r3 >= 0) goto L_080034A2;   /* signed */
    /* 0x08003490: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x08003492: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08003494: 6141     adcs     r1, r4 */
    r1 = r1 + r4;
    /* 0x08003496: 9218     adds     r2, r2, r2 */
    r2 = r2 + r2;
    /* 0x08003498: 5b41     adcs     r3, r3 */
    r3 = r3 + r3;
    /* 0x0800349A: 1a43     orrs     r2, r3 */
    r2 = r2 | r3;
    /* 0x0800349C: 01d1     bne      #0x80034a2 */
    if (r3 != 0) goto L_080034A2;
    /* 0x0800349E: 4008     lsrs     r0, r0, #1 */
    r0 = r0 >> 1;
    /* 0x080034A0: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;

    L_080034A2:
    /* 0x080034A2: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
