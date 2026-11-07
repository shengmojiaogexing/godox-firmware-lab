/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004C6C
 * Name      : FUN_08004C6C
 * End       : 0x08004CCA
 * Size      : 94 bytes, 42 instructions decoded in body (functions.json insns=42, blocks=7)
 * Class     : function
 * Callers   : 2  -> 0x080053E4 FUN_080053E4, 0x080053F0 FUN_080053F0
 * Callees   : 3  -> 0x080030F0 FUN_080030F0, 0x08004E1C FUN_08004E1C, 0x08004E98 FUN_08004E98
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004C6C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004C6C: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x08004C6E: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08004C70: 0d27     movs     r7, #0xd */
    r7 = 0xd;
    /* 0x08004C72: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08004C74: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x08004C76: fef73bfa bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x08004C7A: 0546     mov      r5, r0 */
    r5 = r0;
    /* 0x08004C7C: 0e46     mov      r6, r1 */
    r6 = r1;
    /* 0x08004C7E: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x08004C80: 5422     movs     r2, #0x54 */
    r2 = 0x54;
    /* 0x08004C82: 3946     mov      r1, r7 */
    r1 = r7;
    /* 0x08004C84: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08004C86: 00f0c9f8 bl       #0x8004e1c */
    FUN_08004E1C();   /* BL -> 0x08004E1C FUN_08004E1C */
    /* 0x08004C8A: 1221     movs     r1, #0x12 */
    r1 = 0x12;
    /* 0x08004C8C: 5522     movs     r2, #0x55 */
    r2 = 0x55;
    /* 0x08004C8E: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08004C90: 00f0c4f8 bl       #0x8004e1c */
    FUN_08004E1C();   /* BL -> 0x08004E1C FUN_08004E1C */
    /* 0x08004C94: 0d48     ldr      r0, [pc, #0x34]  ; =RAM_20000177 */
    r0 = *(uint32_t *)0x08004CCC;   /* = RAM 0x20000177 */
    /* 0x08004C96: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08004C98: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x08004C9A: 03d1     bne      #0x8004ca4 */
    if (r0 != 0xa) goto L_08004CA4;
    /* 0x08004C9C: 0c48     ldr      r0, [pc, #0x30]  ; =RAM_20000228 */
    r0 = *(uint32_t *)0x08004CD0;   /* = RAM 0x20000228 */
    /* 0x08004C9E: 0079     ldrb     r0, [r0, #4] */
    r0 = *(uint8_t *)(r0 + 4);
    /* 0x08004CA0: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08004CA2: 0dd1     bne      #0x8004cc0 */
    if (r0 != 1) goto L_08004CC0;

    L_08004CA4:
    /* 0x08004CA4: 1821     movs     r1, #0x18 */
    r1 = 0x18;
    /* 0x08004CA6: 002d     cmp      r5, #0 */
    /* flags = cmp(r5, #0) */
    /* 0x08004CA8: 0bd0     beq      #0x8004cc2 */
    if (r5 == 0) goto L_08004CC2;
    /* 0x08004CAA: 0223     movs     r3, #2 */
    r3 = 2;
    /* 0x08004CAC: 2a46     mov      r2, r5 */
    r2 = r5;
    /* 0x08004CAE: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08004CB0: 00f0f2f8 bl       #0x8004e98 */
    FUN_08004E98();   /* BL -> 0x08004E98 FUN_08004E98 */
    /* 0x08004CB4: 1e21     movs     r1, #0x1e */
    r1 = 0x1e;
    /* 0x08004CB6: 0223     movs     r3, #2 */
    r3 = 2;
    /* 0x08004CB8: 3246     mov      r2, r6 */
    r2 = r6;
    /* 0x08004CBA: 2046     mov      r0, r4 */
    r0 = r4;

    L_08004CBC:
    /* 0x08004CBC: 00f0ecf8 bl       #0x8004e98 */
    FUN_08004E98();   /* BL -> 0x08004E98 FUN_08004E98 */

    L_08004CC0:
    /* 0x08004CC0: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_08004CC2:
    /* 0x08004CC2: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08004CC4: 0223     movs     r3, #2 */
    r3 = 2;
    /* 0x08004CC6: 3246     mov      r2, r6 */
    r2 = r6;
    /* 0x08004CC8: f8e7     b        #0x8004cbc */
    goto L_08004CBC;   /* loop */
}
