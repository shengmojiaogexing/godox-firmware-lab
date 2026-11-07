/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CDC8
 * Name      : FUN_0800CDC8
 * End       : 0x0800CDF4
 * Size      : 44 bytes, 18 instructions decoded in body (functions.json insns=18, blocks=4)
 * Class     : function
 * Callers   : 2  -> 0x0800CC98 FUN_0800CC98, 0x0800CCA6 FUN_0800CCA6
 * Callees   : 4  -> 0x0800CD40 FUN_0800CD40, 0x0800CD7C FUN_0800CD7C, 0x0800CE38 FUN_0800CE38, 0x0800CE58 FUN_0800CE58
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CDC8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CDC8: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x0800CDCA: 1546     mov      r5, r2 */
    r5 = r2;
    /* 0x0800CDCC: 0f46     mov      r7, r1 */
    r7 = r1;
    /* 0x0800CDCE: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x0800CDD0: 00f032f8 bl       #0x800ce38 */
    FUN_0800CE38();   /* BL -> 0x0800CE38 FUN_0800CE38 */
    /* 0x0800CDD4: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800CDD6: fff7d1ff bl       #0x800cd7c */
    FUN_0800CD7C();   /* BL -> 0x0800CD7C FUN_0800CD7C */
    /* 0x0800CDDA: 0646     mov      r6, r0 */
    r6 = r0;
    /* 0x0800CDDC: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x0800CDDE: 03e0     b        #0x800cde8 */
    goto L_0800CDE8;

    L_0800CDE0:
    /* 0x0800CDE0: fff7aeff bl       #0x800cd40 */
    FUN_0800CD40();   /* BL -> 0x0800CD40 FUN_0800CD40 */
    /* 0x0800CDE4: 3855     strb     r0, [r7, r4] */
    *(uint8_t *)(r7 + r4) = r0;   /* store */
    /* 0x0800CDE6: 641c     adds     r4, r4, #1 */
    r4 = r4 + 1;

    L_0800CDE8:
    /* 0x0800CDE8: ac42     cmp      r4, r5 */
    /* flags = cmp(r4, r5) */
    /* 0x0800CDEA: f9d3     blo      #0x800cde0 */
    if (r4 < r5) goto L_0800CDE0;   /* unsigned */
    /* 0x0800CDEC: 00f034f8 bl       #0x800ce58 */
    FUN_0800CE58();   /* BL -> 0x0800CE58 FUN_0800CE58 */
    /* 0x0800CDF0: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x0800CDF2: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
