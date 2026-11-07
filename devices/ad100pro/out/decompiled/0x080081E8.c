/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080081E8
 * Name      : FUN_080081E8
 * End       : 0x0800821C
 * Size      : 52 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=6)
 * Class     : function
 * Callers   : 6  -> 0x0800967C FUN_0800967C, 0x080096A8 FUN_080096A8, 0x08009708 FUN_08009708, 0x08009768 FUN_08009768, 0x08009D84 FUN_08009D84, 0x08009DA4 FUN_08009DA4
 * Callees   : 2  -> 0x0800821C FUN_0800821C, 0x0800BABC FUN_0800BABC
 * MMIO      : RCC.+0x19, RCC.APB2ENR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080081E8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080081E8: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x080081EA: 0400     movs     r4, r0 */
    r4 = r0;
    /* 0x080081EC: 05d0     beq      #0x80081fa */
    if (/* beq */) goto L_080081FA;
    /* 0x080081EE: 2546     mov      r5, r4 */
    r5 = r4;
    /* 0x080081F0: 2035     adds     r5, #0x20 */
    r5 = r5 + 0x20;
    /* 0x080081F2: 687e     ldrb     r0, [r5, #0x19] */
    r0 = *(uint8_t *)(r5 + 25);  /* MMIO: RCC.+0x19(r) */
    /* 0x080081F4: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080081F6: 02d0     beq      #0x80081fe */
    if (r0 == 0) goto L_080081FE;
    /* 0x080081F8: 06e0     b        #0x8008208 */
    goto L_08008208;

    L_080081FA:
    /* 0x080081FA: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x080081FC: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_080081FE:
    /* 0x080081FE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08008200: 2876     strb     r0, [r5, #0x18] */
    *(uint8_t *)(r5 + 24) = r0;   /* store */  /* MMIO: RCC.APB2ENR(w) */
    /* 0x08008202: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08008204: 00f00af8 bl       #0x800821c */
    FUN_0800821C();   /* BL -> 0x0800821C FUN_0800821C */

    L_08008208:
    /* 0x08008208: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x0800820A: 6876     strb     r0, [r5, #0x19] */
    *(uint8_t *)(r5 + 25) = r0;   /* store */  /* MMIO: RCC.+0x19(w) */
    /* 0x0800820C: 211d     adds     r1, r4, #4 */
    r1 = r4 + 4;
    /* 0x0800820E: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x08008210: 03f054fc bl       #0x800babc */
    FUN_0800BABC();   /* BL -> 0x0800BABC FUN_0800BABC */
    /* 0x08008214: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08008216: 6876     strb     r0, [r5, #0x19] */
    *(uint8_t *)(r5 + 25) = r0;   /* store */  /* MMIO: RCC.+0x19(w) */
    /* 0x08008218: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800821A: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
