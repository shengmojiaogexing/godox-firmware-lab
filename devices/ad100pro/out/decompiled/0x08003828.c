/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08003828
 * Name      : FUN_08003828
 * End       : 0x080038A0
 * Size      : 120 bytes, 57 instructions decoded in body (functions.json insns=57, blocks=12)
 * Class     : function
 * Callers   : 1  -> 0x08006BA4 FUN_08006BA4
 * Callees   : 2  -> 0x080030F0 FUN_080030F0, 0x08007434 delay_or_timing
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08003828(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08003828: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x0800382A: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x0800382C: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800382E: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08003830: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x08003832: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);
    /* 0x08003834: 8907     lsls     r1, r1, #0x1e */
    r1 = r1 << 0x1e;
    /* 0x08003836: 890f     lsrs     r1, r1, #0x1e */
    r1 = r1 >> 0x1e;
    /* 0x08003838: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x0800383A: 05d1     bne      #0x8003848 */
    if (r1 != 1) goto L_08003848;
    /* 0x0800383C: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);
    /* 0x0800383E: c907     lsls     r1, r1, #0x1f */
    r1 = r1 << 0x1f;
    /* 0x08003840: 2cd1     bne      #0x800389c */
    if (r1 != 1) goto L_0800389C;
    /* 0x08003842: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);
    /* 0x08003844: 0904     lsls     r1, r1, #0x10 */
    r1 = r1 << 0x10;
    /* 0x08003846: 29d4     bmi      #0x800389c */
    if (r1 < 1) goto L_0800389C;   /* result negative */

    L_08003848:
    /* 0x08003848: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);
    /* 0x0800384A: 154a     ldr      r2, [pc, #0x54]  ; =0x80000017 */
    r2 = *(uint32_t *)0x080038A0;   /* = 0x80000017 */
    /* 0x0800384C: 1026     movs     r6, #0x10 */
    r6 = 0x10;
    /* 0x0800384E: 1140     ands     r1, r2 */
    r1 = r1 & r2;
    /* 0x08003850: 0125     movs     r5, #1 */
    r5 = 1;
    /* 0x08003852: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x08003854: 15d1     bne      #0x8003882 */
    if (r1 != 0) goto L_08003882;
    /* 0x08003856: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);
    /* 0x08003858: 2943     orrs     r1, r5 */
    r1 = r1 | r5;
    /* 0x0800385A: 8160     str      r1, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r1;   /* store */
    /* 0x0800385C: 1248     ldr      r0, [pc, #0x48]  ; =RAM_20000100 */
    r0 = *(uint32_t *)0x080038A8;   /* = RAM 0x20000100 */
    /* 0x0800385E: 1149     ldr      r1, [pc, #0x44]  ; =0x000F4240 */
    r1 = *(uint32_t *)0x080038A4;   /* = 0x000F4240 */
    /* 0x08003860: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);
    /* 0x08003862: fff745fc bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x08003866: 00e0     b        #0x800386a */
    goto L_0800386A;

    L_08003868:
    /* 0x08003868: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;

    L_0800386A:
    /* 0x0800386A: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x0800386C: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800386E: fbd1     bne      #0x8003868 */
    if (r0 != 0) goto L_08003868;
    /* 0x08003870: 03f0e0fd bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08003874: 0746     mov      r7, r0 */
    r7 = r0;
    /* 0x08003876: 0ce0     b        #0x8003892 */
    goto L_08003892;

    L_08003878:
    /* 0x08003878: 03f0dcfd bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x0800387C: c01b     subs     r0, r0, r7 */
    r0 = r0 - r7;
    /* 0x0800387E: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08003880: 07d9     bls      #0x8003892 */
    if (r0 <= 2) goto L_08003892;   /* unsigned */

    L_08003882:
    /* 0x08003882: 606c     ldr      r0, [r4, #0x44] */
    r0 = *(uint32_t *)(r4 + 68);
    /* 0x08003884: 3043     orrs     r0, r6 */
    r0 = r0 | r6;
    /* 0x08003886: 6064     str      r0, [r4, #0x44] */
    *(uint32_t *)(r4 + 68) = r0;   /* store */
    /* 0x08003888: a06c     ldr      r0, [r4, #0x48] */
    r0 = *(uint32_t *)(r4 + 72);
    /* 0x0800388A: 2843     orrs     r0, r5 */
    r0 = r0 | r5;
    /* 0x0800388C: a064     str      r0, [r4, #0x48] */
    *(uint32_t *)(r4 + 72) = r0;   /* store */
    /* 0x0800388E: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08003890: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_08003892:
    /* 0x08003892: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x08003894: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);
    /* 0x08003896: c043     mvns     r0, r0 */
    r0 = ~r0;
    /* 0x08003898: c007     lsls     r0, r0, #0x1f */
    r0 = r0 << 0x1f;
    /* 0x0800389A: edd1     bne      #0x8003878 */
    if (r0 != 2) goto L_08003878;

    L_0800389C:
    /* 0x0800389C: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800389E: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
