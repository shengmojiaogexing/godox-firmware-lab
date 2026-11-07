/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C8C4
 * Name      : FUN_0800C8C4
 * End       : 0x0800C8F2
 * Size      : 46 bytes, 23 instructions decoded in body (functions.json insns=23, blocks=11)
 * Class     : function
 * Callers   : 1  -> 0x080034A4 FUN_080034A4
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C8C4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C8C4: 2021     movs     r1, #0x20 */
    r1 = 0x20;
    /* 0x0800C8C6: 020c     lsrs     r2, r0, #0x10 */
    r2 = r0 >> 0x10;
    /* 0x0800C8C8: 01d0     beq      #0x800c8ce */
    if (/* beq */) goto L_0800C8CE;
    /* 0x0800C8CA: 1021     movs     r1, #0x10 */
    r1 = 0x10;
    /* 0x0800C8CC: 1046     mov      r0, r2 */
    r0 = r2;

    L_0800C8CE:
    /* 0x0800C8CE: 020a     lsrs     r2, r0, #8 */
    r2 = r0 >> 8;
    /* 0x0800C8D0: 01d0     beq      #0x800c8d6 */
    if (/* beq */) goto L_0800C8D6;
    /* 0x0800C8D2: 1046     mov      r0, r2 */
    r0 = r2;
    /* 0x0800C8D4: 0839     subs     r1, #8 */
    r1 = r1 - 8;

    L_0800C8D6:
    /* 0x0800C8D6: 0209     lsrs     r2, r0, #4 */
    r2 = r0 >> 4;
    /* 0x0800C8D8: 01d0     beq      #0x800c8de */
    if (/* beq */) goto L_0800C8DE;
    /* 0x0800C8DA: 1046     mov      r0, r2 */
    r0 = r2;
    /* 0x0800C8DC: 091f     subs     r1, r1, #4 */
    r1 = r1 - 4;

    L_0800C8DE:
    /* 0x0800C8DE: 8208     lsrs     r2, r0, #2 */
    r2 = r0 >> 2;
    /* 0x0800C8E0: 01d0     beq      #0x800c8e6 */
    if (/* beq */) goto L_0800C8E6;
    /* 0x0800C8E2: 1046     mov      r0, r2 */
    r0 = r2;
    /* 0x0800C8E4: 891e     subs     r1, r1, #2 */
    r1 = r1 - 2;

    L_0800C8E6:
    /* 0x0800C8E6: 4208     lsrs     r2, r0, #1 */
    r2 = r0 >> 1;
    /* 0x0800C8E8: 01d0     beq      #0x800c8ee */
    if (/* beq */) goto L_0800C8EE;
    /* 0x0800C8EA: 881e     subs     r0, r1, #2 */
    r0 = r1 - 2;
    /* 0x0800C8EC: 7047     bx       lr */
    return;

    L_0800C8EE:
    /* 0x0800C8EE: 081a     subs     r0, r1, r0 */
    r0 = r1 - r0;
    /* 0x0800C8F0: 7047     bx       lr */
    return;
}
