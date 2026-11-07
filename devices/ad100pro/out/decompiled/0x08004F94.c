/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004F94
 * Name      : FUN_08004F94
 * End       : 0x08004FC4
 * Size      : 48 bytes, 21 instructions decoded in body (functions.json insns=21, blocks=7)
 * Class     : function
 * Callers   : 2  -> 0x0800C2B8 FUN_0800C2B8, 0x0800C380 FUN_0800C380
 * Callees   : 3  -> 0x080052AC FUN_080052AC, 0x080052E4 FUN_080052E4, 0x0800531C FUN_0800531C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004F94(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004F94: 00b5     push     {lr} */
    push({lr});
    /* 0x08004F96: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x08004F98: 0ad0     beq      #0x8004fb0 */
    if (/* beq */) goto L_08004FB0;
    /* 0x08004F9A: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08004F9C: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x08004F9E: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08004FA0: 0bd0     beq      #0x8004fba */
    if (r0 == 2) goto L_08004FBA;
    /* 0x08004FA2: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08004FA4: 03d1     bne      #0x8004fae */
    if (r0 != 1) goto L_08004FAE;
    /* 0x08004FA6: 3a21     movs     r1, #0x3a */
    r1 = 0x3a;
    /* 0x08004FA8: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08004FAA: 00f07ff9 bl       #0x80052ac */
    FUN_080052AC();   /* BL -> 0x080052AC FUN_080052AC */

    L_08004FAE:
    /* 0x08004FAE: 00bd     pop      {pc} */
    pop({pc});   /* return */

    L_08004FB0:
    /* 0x08004FB0: 3221     movs     r1, #0x32 */
    r1 = 0x32;
    /* 0x08004FB2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08004FB4: 00f0b2f9 bl       #0x800531c */
    FUN_0800531C();   /* BL -> 0x0800531C FUN_0800531C */
    /* 0x08004FB8: 00bd     pop      {pc} */
    pop({pc});   /* return */

    L_08004FBA:
    /* 0x08004FBA: 2e21     movs     r1, #0x2e */
    r1 = 0x2e;
    /* 0x08004FBC: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08004FBE: 00f091f9 bl       #0x80052e4 */
    FUN_080052E4();   /* BL -> 0x080052E4 FUN_080052E4 */
    /* 0x08004FC2: 00bd     pop      {pc} */
    pop({pc});   /* return */
}
