/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080054E4
 * Name      : FUN_080054E4
 * End       : 0x08005562
 * Size      : 126 bytes, 55 instructions decoded in body (functions.json insns=55, blocks=7)
 * Class     : function
 * Callers   : 2  -> 0x0800C2B8 FUN_0800C2B8, 0x0800C358 FUN_0800C358
 * Callees   : 5  -> 0x080030F0 FUN_080030F0, 0x08004E98 FUN_08004E98, 0x0800557C FUN_0800557C, 0x080058D0 FUN_080058D0, 0x0800596C FUN_0800596C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080054E4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080054E4: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x080054E6: 1f4c     ldr      r4, [pc, #0x7c]  ; =RAM_20000175 */
    r4 = *(uint32_t *)0x08005564;   /* = RAM 0x20000175 */
    /* 0x080054E8: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x080054EA: 00f0f1f9 bl       #0x80058d0 */
    FUN_080058D0();   /* BL -> 0x080058D0 FUN_080058D0 */
    /* 0x080054EE: 1e48     ldr      r0, [pc, #0x78]  ; =RAM_20000167 */
    r0 = *(uint32_t *)0x08005568;   /* = RAM 0x20000167 */
    /* 0x080054F0: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x080054F2: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x080054F4: 07d0     beq      #0x8005506 */
    if (r0 == 1) goto L_08005506;
    /* 0x080054F6: 2821     movs     r1, #0x28 */
    r1 = 0x28;
    /* 0x080054F8: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x080054FA: 00f03ff8 bl       #0x800557c */
    FUN_0800557C();   /* BL -> 0x0800557C FUN_0800557C */

    L_080054FE:
    /* 0x080054FE: 1b49     ldr      r1, [pc, #0x6c]  ; =RAM_20000480 */
    r1 = *(uint32_t *)0x0800556C;   /* = RAM 0x20000480 */
    /* 0x08005500: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005502: 8871     strb     r0, [r1, #6] */
    *(uint8_t *)(r1 + 6) = r0;   /* store */
    /* 0x08005504: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08005506:
    /* 0x08005506: 1a48     ldr      r0, [pc, #0x68]  ; =RAM_20000192 */
    r0 = *(uint32_t *)0x08005570;   /* = RAM 0x20000192 */
    /* 0x08005508: 5a21     movs     r1, #0x5a */
    r1 = 0x5a;
    /* 0x0800550A: 0288     ldrh     r2, [r0] */
    r2 = *(uint16_t *)(r0);
    /* 0x0800550C: 0420     movs     r0, #4 */
    r0 = 4;
    /* 0x0800550E: 00f02dfa bl       #0x800596c */
    FUN_0800596C();   /* BL -> 0x0800596C FUN_0800596C */
    /* 0x08005512: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x08005514: 1749     ldr      r1, [pc, #0x5c]  ; =RAM_2000018C */
    r1 = *(uint32_t *)0x08005574;   /* = RAM 0x2000018C */
    /* 0x08005516: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x08005518: 05d5     bpl      #0x8005526 */
    if (r0 >= 1) goto L_08005526;   /* result non-negative */
    /* 0x0800551A: 1748     ldr      r0, [pc, #0x5c]  ; =RAM_2000018A */
    r0 = *(uint32_t *)0x08005578;   /* = RAM 0x2000018A */
    /* 0x0800551C: ff22     movs     r2, #0xff */
    r2 = 0xff;
    /* 0x0800551E: 0088     ldrh     r0, [r0] */
    r0 = *(uint16_t *)(r0);
    /* 0x08005520: 4b32     adds     r2, #0x4b */
    r2 = r2 + 0x4b;
    /* 0x08005522: 0a80     strh     r2, [r1] */
    *(uint16_t *)(r1) = r2;   /* store */
    /* 0x08005524: 00e0     b        #0x8005528 */
    goto L_08005528;

    L_08005526:
    /* 0x08005526: 0888     ldrh     r0, [r1] */
    r0 = *(uint16_t *)(r1);

    L_08005528:
    /* 0x08005528: 6421     movs     r1, #0x64 */
    r1 = 0x64;
    /* 0x0800552A: fdf7e1fd bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x0800552E: c6b2     uxtb     r6, r0 */
    r6 = uxtb(r0);
    /* 0x08005530: 0846     mov      r0, r1 */
    r0 = r1;
    /* 0x08005532: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x08005534: fdf7dcfd bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x08005538: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x0800553A: 0d46     mov      r5, r1 */
    r5 = r1;
    /* 0x0800553C: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x0800553E: 3246     mov      r2, r6 */
    r2 = r6;
    /* 0x08005540: 3221     movs     r1, #0x32 */
    r1 = 0x32;
    /* 0x08005542: 0420     movs     r0, #4 */
    r0 = 4;
    /* 0x08005544: fff7a8fc bl       #0x8004e98 */
    FUN_08004E98();   /* BL -> 0x08004E98 FUN_08004E98 */
    /* 0x08005548: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x0800554A: 2246     mov      r2, r4 */
    r2 = r4;
    /* 0x0800554C: 3921     movs     r1, #0x39 */
    r1 = 0x39;
    /* 0x0800554E: 0420     movs     r0, #4 */
    r0 = 4;
    /* 0x08005550: fff7a2fc bl       #0x8004e98 */
    FUN_08004E98();   /* BL -> 0x08004E98 FUN_08004E98 */
    /* 0x08005554: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x08005556: 2a46     mov      r2, r5 */
    r2 = r5;
    /* 0x08005558: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x0800555A: 0420     movs     r0, #4 */
    r0 = 4;
    /* 0x0800555C: fff79cfc bl       #0x8004e98 */
    FUN_08004E98();   /* BL -> 0x08004E98 FUN_08004E98 */
    /* 0x08005560: cde7     b        #0x80054fe */
    goto L_080054FE;   /* loop */
}
