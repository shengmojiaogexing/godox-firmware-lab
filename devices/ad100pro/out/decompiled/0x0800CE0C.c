/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CE0C
 * Name      : FUN_0800CE0C
 * End       : 0x0800CE38
 * Size      : 44 bytes, 18 instructions decoded in body (functions.json insns=18, blocks=4)
 * Class     : function
 * Callers   : 4  -> 0x0800CAE8 FUN_0800CAE8, 0x0800CC30 FUN_0800CC30, 0x0800CD24 FUN_0800CD24, 0x0800CD32 FUN_0800CD32
 * Callees   : 3  -> 0x0800CD7C FUN_0800CD7C, 0x0800CE38 FUN_0800CE38, 0x0800CE58 FUN_0800CE58
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CE0C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CE0C: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x0800CE0E: 1546     mov      r5, r2 */
    r5 = r2;
    /* 0x0800CE10: 0f46     mov      r7, r1 */
    r7 = r1;
    /* 0x0800CE12: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x0800CE14: 00f010f8 bl       #0x800ce38 */
    FUN_0800CE38();   /* BL -> 0x0800CE38 FUN_0800CE38 */
    /* 0x0800CE18: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800CE1A: fff7afff bl       #0x800cd7c */
    FUN_0800CD7C();   /* BL -> 0x0800CD7C FUN_0800CD7C */
    /* 0x0800CE1E: 0646     mov      r6, r0 */
    r6 = r0;
    /* 0x0800CE20: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x0800CE22: 03e0     b        #0x800ce2c */
    goto L_0800CE2C;

    L_0800CE24:
    /* 0x0800CE24: 385d     ldrb     r0, [r7, r4] */
    r0 = *(uint8_t *)(r7 + r4);
    /* 0x0800CE26: fff7a9ff bl       #0x800cd7c */
    FUN_0800CD7C();   /* BL -> 0x0800CD7C FUN_0800CD7C */
    /* 0x0800CE2A: 641c     adds     r4, r4, #1 */
    r4 = r4 + 1;

    L_0800CE2C:
    /* 0x0800CE2C: ac42     cmp      r4, r5 */
    /* flags = cmp(r4, r5) */
    /* 0x0800CE2E: f9d3     blo      #0x800ce24 */
    if (r4 < r5) goto L_0800CE24;   /* unsigned */
    /* 0x0800CE30: 00f012f8 bl       #0x800ce58 */
    FUN_0800CE58();   /* BL -> 0x0800CE58 FUN_0800CE58 */
    /* 0x0800CE34: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x0800CE36: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
