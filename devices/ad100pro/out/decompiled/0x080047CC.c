/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080047CC
 * Name      : FUN_080047CC
 * End       : 0x08004876
 * Size      : 170 bytes, 77 instructions decoded in body (functions.json insns=77, blocks=27)
 * Class     : function
 * Callers   : 4  -> 0x0800C230 FUN_0800C230, 0x0800C24A FUN_0800C24A, 0x0800CF74 main, 0x0800D014 FUN_0800D014
 * Callees   : 5  -> 0x08004880 FUN_08004880, 0x080064E8 FUN_080064E8, 0x0800741C FUN_0800741C, 0x08008A5C FUN_08008A5C, 0x08009BF8 FUN_08009BF8
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080047CC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080047CC: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x080047CE: 2a4c     ldr      r4, [pc, #0xa8]  ; =GPIOC.MODER */
    r4 = *(uint32_t *)0x08004878;   /* = MMIO GPIOC+0x00 */
    /* 0x080047D0: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x080047D2: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x080047D4: 8903     lsls     r1, r1, #0xe */
    r1 = r1 << 0xe;
    /* 0x080047D6: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080047D8: 02f020fe bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x080047DC: 0226     movs     r6, #2 */
    r6 = 2;
    /* 0x080047DE: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080047E0: 00d1     bne      #0x80047e4 */
    if (r0 != 0) goto L_080047E4;
    /* 0x080047E2: 0225     movs     r5, #2 */
    r5 = 2;

    L_080047E4:
    /* 0x080047E4: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x080047E6: c903     lsls     r1, r1, #0xf */
    r1 = r1 << 0xf;
    /* 0x080047E8: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080047EA: 02f017fe bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x080047EE: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x080047F0: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080047F2: 00d1     bne      #0x80047f6 */
    if (r0 != 0) goto L_080047F6;
    /* 0x080047F4: 0d43     orrs     r5, r1 */
    r5 = r5 | r1;

    L_080047F6:
    /* 0x080047F6: 0027     movs     r7, #0 */
    r7 = 0;
    /* 0x080047F8: 204c     ldr      r4, [pc, #0x80]  ; =RAM_20000184 */
    r4 = *(uint32_t *)0x0800487C;   /* = RAM 0x20000184 */
    /* 0x080047FA: 002d     cmp      r5, #0 */
    /* flags = cmp(r5, #0) */
    /* 0x080047FC: 06d0     beq      #0x800480c */
    if (r5 == 0) goto L_0800480C;
    /* 0x080047FE: 012d     cmp      r5, #1 */
    /* flags = cmp(r5, #1) */
    /* 0x08004800: 21d0     beq      #0x8004846 */
    if (r5 == 1) goto L_08004846;
    /* 0x08004802: 022d     cmp      r5, #2 */
    /* flags = cmp(r5, #2) */
    /* 0x08004804: 28d0     beq      #0x8004858 */
    if (r5 == 2) goto L_08004858;
    /* 0x08004806: 032d     cmp      r5, #3 */
    /* flags = cmp(r5, #3) */
    /* 0x08004808: 30d1     bne      #0x800486c */
    if (r5 != 3) goto L_0800486C;
    /* 0x0800480A: 0de0     b        #0x8004828 */
    goto L_08004828;

    L_0800480C:
    /* 0x0800480C: 6079     ldrb     r0, [r4, #5] */
    r0 = *(uint8_t *)(r4 + 5);
    /* 0x0800480E: 0d28     cmp      r0, #0xd */
    /* flags = cmp(r0, #0xd) */
    /* 0x08004810: 02d0     beq      #0x8004818 */
    if (r0 == 0xd) goto L_08004818;
    /* 0x08004812: 0e28     cmp      r0, #0xe */
    /* flags = cmp(r0, #0xe) */
    /* 0x08004814: 03d0     beq      #0x800481e */
    if (r0 == 0xe) goto L_0800481E;
    /* 0x08004816: 04e0     b        #0x8004822 */
    goto L_08004822;

    L_08004818:
    /* 0x08004818: 04f020f9 bl       #0x8008a5c */
    FUN_08008A5C();   /* BL -> 0x08008A5C FUN_08008A5C */
    /* 0x0800481C: 01e0     b        #0x8004822 */
    goto L_08004822;

    L_0800481E:
    /* 0x0800481E: 00f02ff8 bl       #0x8004880 */
    FUN_08004880();   /* BL -> 0x08004880 FUN_08004880 */

    L_08004822:
    /* 0x08004822: 6771     strb     r7, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r7;   /* store */
    /* 0x08004824: 2771     strb     r7, [r4, #4] */
    *(uint8_t *)(r4 + 4) = r7;   /* store */
    /* 0x08004826: 21e0     b        #0x800486c */
    goto L_0800486C;

    L_08004828:
    /* 0x08004828: 6079     ldrb     r0, [r4, #5] */
    r0 = *(uint8_t *)(r4 + 5);
    /* 0x0800482A: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x0800482C: 02d0     beq      #0x8004834 */
    if (r0 == 2) goto L_08004834;
    /* 0x0800482E: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08004830: 03d0     beq      #0x800483a */
    if (r0 == 1) goto L_0800483A;
    /* 0x08004832: 04e0     b        #0x800483e */
    goto L_0800483E;

    L_08004834:
    /* 0x08004834: 04f012f9 bl       #0x8008a5c */
    FUN_08008A5C();   /* BL -> 0x08008A5C FUN_08008A5C */
    /* 0x08004838: 01e0     b        #0x800483e */
    goto L_0800483E;

    L_0800483A:
    /* 0x0800483A: 00f021f8 bl       #0x8004880 */
    FUN_08004880();   /* BL -> 0x08004880 FUN_08004880 */

    L_0800483E:
    /* 0x0800483E: 6771     strb     r7, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r7;   /* store */
    /* 0x08004840: 0c20     movs     r0, #0xc */
    r0 = 0xc;
    /* 0x08004842: 2071     strb     r0, [r4, #4] */
    *(uint8_t *)(r4 + 4) = r0;   /* store */
    /* 0x08004844: 12e0     b        #0x800486c */
    goto L_0800486C;

    L_08004846:
    /* 0x08004846: 2079     ldrb     r0, [r4, #4] */
    r0 = *(uint8_t *)(r4 + 4);
    /* 0x08004848: 8207     lsls     r2, r0, #0x1e */
    r2 = r0 << 0x1e;
    /* 0x0800484A: 920f     lsrs     r2, r2, #0x1e */
    r2 = r2 >> 0x1e;
    /* 0x0800484C: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800484E: 0dd0     beq      #0x800486c */
    if (r2 == 1) goto L_0800486C;
    /* 0x08004850: 8008     lsrs     r0, r0, #2 */
    r0 = r0 >> 2;
    /* 0x08004852: 8000     lsls     r0, r0, #2 */
    r0 = r0 << 2;
    /* 0x08004854: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08004856: 07e0     b        #0x8004868 */
    goto L_08004868;

    L_08004858:
    /* 0x08004858: 2079     ldrb     r0, [r4, #4] */
    r0 = *(uint8_t *)(r4 + 4);
    /* 0x0800485A: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x0800485C: 890f     lsrs     r1, r1, #0x1e */
    r1 = r1 >> 0x1e;
    /* 0x0800485E: 0229     cmp      r1, #2 */
    /* flags = cmp(r1, #2) */
    /* 0x08004860: 04d0     beq      #0x800486c */
    if (r1 == 2) goto L_0800486C;
    /* 0x08004862: 8008     lsrs     r0, r0, #2 */
    r0 = r0 >> 2;
    /* 0x08004864: 8000     lsls     r0, r0, #2 */
    r0 = r0 << 2;
    /* 0x08004866: 3043     orrs     r0, r6 */
    r0 = r0 | r6;

    L_08004868:
    /* 0x08004868: 2071     strb     r0, [r4, #4] */
    *(uint8_t *)(r4 + 4) = r0;   /* store */
    /* 0x0800486A: 6071     strb     r0, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r0;   /* store */

    L_0800486C:
    /* 0x0800486C: 05f0c4f9 bl       #0x8009bf8 */
    FUN_08009BF8();   /* BL -> 0x08009BF8 FUN_08009BF8 */
    /* 0x08004870: 01f03afe bl       #0x80064e8 */
    FUN_080064E8();   /* BL -> 0x080064E8 FUN_080064E8 */
    /* 0x08004874: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
