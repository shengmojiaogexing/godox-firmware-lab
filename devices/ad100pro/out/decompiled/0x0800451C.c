/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800451C
 * Name      : FUN_0800451C
 * End       : 0x08004562
 * Size      : 70 bytes, 35 instructions decoded in body (functions.json insns=35, blocks=13)
 * Class     : function
 * Callers   : 2  -> 0x08008D00 dispatcher_large, 0x08008EC6 FUN_08008EC6
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800451C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800451C: 1148     ldr      r0, [pc, #0x44]  ; =RAM_20000164 */
    r0 = *(uint32_t *)0x08004564;   /* = RAM 0x20000164 */
    /* 0x0800451E: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x08004520: 427d     ldrb     r2, [r0, #0x15] */
    r2 = *(uint8_t *)(r0 + 21);
    /* 0x08004522: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x08004524: 0ed0     beq      #0x8004544 */
    if (r2 == 1) goto L_08004544;
    /* 0x08004526: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x08004528: 2032     adds     r2, #0x20 */
    r2 = r2 + 0x20;
    /* 0x0800452A: 9378     ldrb     r3, [r2, #2] */
    r3 = *(uint8_t *)(r2 + 2);
    /* 0x0800452C: 012b     cmp      r3, #1 */
    /* flags = cmp(r3, #1) */
    /* 0x0800452E: 0bd0     beq      #0x8004548 */
    if (r3 == 1) goto L_08004548;
    /* 0x08004530: 0278     ldrb     r2, [r0] */
    r2 = *(uint8_t *)(r0);
    /* 0x08004532: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x08004534: 02d0     beq      #0x800453c */
    if (r2 == 1) goto L_0800453C;
    /* 0x08004536: 827c     ldrb     r2, [r0, #0x12] */
    r2 = *(uint8_t *)(r0 + 18);
    /* 0x08004538: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x0800453A: 07d0     beq      #0x800454c */
    if (r2 == 0) goto L_0800454C;

    L_0800453C:
    /* 0x0800453C: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x0800453E: 8174     strb     r1, [r0, #0x12] */
    *(uint8_t *)(r0 + 18) = r1;   /* store */
    /* 0x08004540: 4163     str      r1, [r0, #0x34] */
    *(uint32_t *)(r0 + 52) = r1;   /* store */
    /* 0x08004542: 7047     bx       lr */
    return;

    L_08004544:
    /* 0x08004544: 4175     strb     r1, [r0, #0x15] */
    *(uint8_t *)(r0 + 21) = r1;   /* store */

    L_08004546:
    /* 0x08004546: 7047     bx       lr */
    return;

    L_08004548:
    /* 0x08004548: 9170     strb     r1, [r2, #2] */
    *(uint8_t *)(r2 + 2) = r1;   /* store */
    /* 0x0800454A: 08e0     b        #0x800455e */
    goto L_0800455E;

    L_0800454C:
    /* 0x0800454C: 017e     ldrb     r1, [r0, #0x18] */
    r1 = *(uint8_t *)(r0 + 24);
    /* 0x0800454E: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x08004550: f9d0     beq      #0x8004546 */
    if (r1 == 0) goto L_08004546;
    /* 0x08004552: c17c     ldrb     r1, [r0, #0x13] */
    r1 = *(uint8_t *)(r0 + 19);
    /* 0x08004554: 0a29     cmp      r1, #0xa */
    /* flags = cmp(r1, #0xa) */
    /* 0x08004556: 01d0     beq      #0x800455c */
    if (r1 == 0xa) goto L_0800455C;
    /* 0x08004558: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x0800455A: 00e0     b        #0x800455e */
    goto L_0800455E;

    L_0800455C:
    /* 0x0800455C: 0021     movs     r1, #0 */
    r1 = 0;

    L_0800455E:
    /* 0x0800455E: c174     strb     r1, [r0, #0x13] */
    *(uint8_t *)(r0 + 19) = r1;   /* store */
    /* 0x08004560: 7047     bx       lr */
    return;
}
