/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006CEC
 * Name      : FUN_08006CEC
 * End       : 0x08006D16
 * Size      : 42 bytes, 20 instructions decoded in body (functions.json insns=20, blocks=6)
 * Class     : function
 * Callers   : 1  -> 0x080093A8 FUN_080093A8
 * Callees   : 1  -> 0x08006D18 FUN_08006D18
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006CEC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006CEC: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08006CEE: 0400     movs     r4, r0 */
    r4 = r0;
    /* 0x08006CF0: 04d0     beq      #0x8006cfc */
    if (/* beq */) goto L_08006CFC;
    /* 0x08006CF2: 2079     ldrb     r0, [r4, #4] */
    r0 = *(uint8_t *)(r4 + 4);
    /* 0x08006CF4: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x08006CF6: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08006CF8: 02d0     beq      #0x8006d00 */
    if (r0 == 0) goto L_08006D00;
    /* 0x08006CFA: 05e0     b        #0x8006d08 */
    goto L_08006D08;

    L_08006CFC:
    /* 0x08006CFC: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08006CFE: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08006D00:
    /* 0x08006D00: 6571     strb     r5, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r5;   /* store */
    /* 0x08006D02: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08006D04: 00f008f8 bl       #0x8006d18 */
    FUN_08006D18();   /* BL -> 0x08006D18 FUN_08006D18 */

    L_08006D08:
    /* 0x08006D08: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08006D0A: 2071     strb     r0, [r4, #4] */
    *(uint8_t *)(r4 + 4) = r0;   /* store */
    /* 0x08006D0C: 2561     str      r5, [r4, #0x10] */
    *(uint32_t *)(r4 + 16) = r5;   /* store */
    /* 0x08006D0E: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08006D10: 2071     strb     r0, [r4, #4] */
    *(uint8_t *)(r4 + 4) = r0;   /* store */
    /* 0x08006D12: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006D14: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
