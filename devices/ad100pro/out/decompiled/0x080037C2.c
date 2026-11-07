/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080037C2
 * Name      : FUN_080037C2
 * End       : 0x08003828
 * Size      : 102 bytes, 49 instructions decoded in body (functions.json insns=49, blocks=9)
 * Class     : function
 * Callers   : 2  -> 0x08006C44 FUN_08006C44, 0x08006C94 FUN_08006C94
 * Callees   : 1  -> 0x08007434 delay_or_timing
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080037C2(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080037C2: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x080037C4: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x080037C6: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);
    /* 0x080037C8: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);
    /* 0x080037CA: 8907     lsls     r1, r1, #0x1e */
    r1 = r1 << 0x1e;
    /* 0x080037CC: 890f     lsrs     r1, r1, #0x1e */
    r1 = r1 >> 0x1e;
    /* 0x080037CE: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x080037D0: 28d1     bne      #0x8003824 */
    if (r1 != 1) goto L_08003824;
    /* 0x080037D2: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);
    /* 0x080037D4: c907     lsls     r1, r1, #0x1f */
    r1 = r1 << 0x1f;
    /* 0x080037D6: 02d1     bne      #0x80037de */
    if (r1 != 1) goto L_080037DE;
    /* 0x080037D8: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);
    /* 0x080037DA: 0904     lsls     r1, r1, #0x10 */
    r1 = r1 << 0x10;
    /* 0x080037DC: 22d5     bpl      #0x8003824 */
    if (r1 >= 1) goto L_08003824;   /* result non-negative */

    L_080037DE:
    /* 0x080037DE: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);
    /* 0x080037E0: 0522     movs     r2, #5 */
    r2 = 5;
    /* 0x080037E2: 1140     ands     r1, r2 */
    r1 = r1 & r2;
    /* 0x080037E4: 1025     movs     r5, #0x10 */
    r5 = 0x10;
    /* 0x080037E6: 0126     movs     r6, #1 */
    r6 = 1;
    /* 0x080037E8: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x080037EA: 0fd1     bne      #0x800380c */
    if (r1 != 1) goto L_0800380C;
    /* 0x080037EC: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);
    /* 0x080037EE: 0222     movs     r2, #2 */
    r2 = 2;
    /* 0x080037F0: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x080037F2: 8160     str      r1, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r1;   /* store */
    /* 0x080037F4: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);
    /* 0x080037F6: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x080037F8: 0860     str      r0, [r1] */
    *(uint32_t *)(r1) = r0;   /* store */
    /* 0x080037FA: 03f01bfe bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x080037FE: 0746     mov      r7, r0 */
    r7 = r0;
    /* 0x08003800: 0ce0     b        #0x800381c */
    goto L_0800381C;

    L_08003802:
    /* 0x08003802: 03f017fe bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08003806: c01b     subs     r0, r0, r7 */
    r0 = r0 - r7;
    /* 0x08003808: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x0800380A: 07d9     bls      #0x800381c */
    if (r0 <= 2) goto L_0800381C;   /* unsigned */

    L_0800380C:
    /* 0x0800380C: 606c     ldr      r0, [r4, #0x44] */
    r0 = *(uint32_t *)(r4 + 68);
    /* 0x0800380E: 2843     orrs     r0, r5 */
    r0 = r0 | r5;
    /* 0x08003810: 6064     str      r0, [r4, #0x44] */
    *(uint32_t *)(r4 + 68) = r0;   /* store */
    /* 0x08003812: a06c     ldr      r0, [r4, #0x48] */
    r0 = *(uint32_t *)(r4 + 72);
    /* 0x08003814: 3043     orrs     r0, r6 */
    r0 = r0 | r6;
    /* 0x08003816: a064     str      r0, [r4, #0x48] */
    *(uint32_t *)(r4 + 72) = r0;   /* store */
    /* 0x08003818: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800381A: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_0800381C:
    /* 0x0800381C: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x0800381E: 8068     ldr      r0, [r0, #8] */
    r0 = *(uint32_t *)(r0 + 8);
    /* 0x08003820: c007     lsls     r0, r0, #0x1f */
    r0 = r0 << 0x1f;
    /* 0x08003822: eed1     bne      #0x8003802 */
    if (r0 != 2) goto L_08003802;

    L_08003824:
    /* 0x08003824: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003826: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
