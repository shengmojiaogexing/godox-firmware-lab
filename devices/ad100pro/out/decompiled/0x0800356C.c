/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800356C
 * Name      : FUN_0800356C
 * End       : 0x080035A6
 * Size      : 58 bytes, 29 instructions decoded in body (functions.json insns=29, blocks=12)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800356C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800356C: 30b5     push     {r4, r5, lr} */
    push({r4, r5, lr});
    /* 0x0800356E: 8c18     adds     r4, r1, r2 */
    r4 = r1 + r2;

    L_08003570:
    /* 0x08003570: 0278     ldrb     r2, [r0] */
    r2 = *(uint8_t *)(r0);
    /* 0x08003572: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08003574: 1307     lsls     r3, r2, #0x1c */
    r3 = r2 << 0x1c;
    /* 0x08003576: 1b0f     lsrs     r3, r3, #0x1c */
    r3 = r3 >> 0x1c;
    /* 0x08003578: 01d1     bne      #0x800357e */
    if (/* bne */) goto L_0800357E;
    /* 0x0800357A: 0378     ldrb     r3, [r0] */
    r3 = *(uint8_t *)(r0);
    /* 0x0800357C: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;

    L_0800357E:
    /* 0x0800357E: 1209     lsrs     r2, r2, #4 */
    r2 = r2 >> 4;
    /* 0x08003580: 06d1     bne      #0x8003590 */
    if (/* bne */) goto L_08003590;
    /* 0x08003582: 0278     ldrb     r2, [r0] */
    r2 = *(uint8_t *)(r0);
    /* 0x08003584: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08003586: 03e0     b        #0x8003590 */
    goto L_08003590;

    L_08003588:
    /* 0x08003588: 0578     ldrb     r5, [r0] */
    r5 = *(uint8_t *)(r0);
    /* 0x0800358A: 0d70     strb     r5, [r1] */
    *(uint8_t *)(r1) = r5;   /* store */
    /* 0x0800358C: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800358E: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;

    L_08003590:
    /* 0x08003590: 5b1e     subs     r3, r3, #1 */
    r3 = r3 - 1;
    /* 0x08003592: f9d1     bne      #0x8003588 */
    if (/* bne */) goto L_08003588;
    /* 0x08003594: 01e0     b        #0x800359a */
    goto L_0800359A;

    L_08003596:
    /* 0x08003596: 0b70     strb     r3, [r1] */
    *(uint8_t *)(r1) = r3;   /* store */
    /* 0x08003598: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;

    L_0800359A:
    /* 0x0800359A: 521e     subs     r2, r2, #1 */
    r2 = r2 - 1;
    /* 0x0800359C: fbd1     bne      #0x8003596 */
    if (/* bne */) goto L_08003596;
    /* 0x0800359E: a142     cmp      r1, r4 */
    /* flags = cmp(r1, r4) */
    /* 0x080035A0: e6d3     blo      #0x8003570 */
    if (r1 < r4) goto L_08003570;   /* unsigned */
    /* 0x080035A2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080035A4: 30bd     pop      {r4, r5, pc} */
    pop({r4, r5, pc});   /* return */
}
