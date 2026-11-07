/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080036E0
 * Name      : FUN_080036E0
 * End       : 0x08003732
 * Size      : 82 bytes, 39 instructions decoded in body (functions.json insns=39, blocks=9)
 * Class     : function
 * Callers   : 1  -> 0x08006C44 FUN_08006C44
 * Callees   : 1  -> 0x08007434 delay_or_timing
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080036E0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080036E0: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x080036E2: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x080036E4: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);
    /* 0x080036E6: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);
    /* 0x080036E8: 4907     lsls     r1, r1, #0x1d */
    r1 = r1 << 0x1d;
    /* 0x080036EA: 20d5     bpl      #0x800372e */
    if (/* bpl */) goto L_0800372E;   /* result non-negative */
    /* 0x080036EC: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);
    /* 0x080036EE: 1026     movs     r6, #0x10 */
    r6 = 0x10;
    /* 0x080036F0: 4907     lsls     r1, r1, #0x1d */
    r1 = r1 << 0x1d;
    /* 0x080036F2: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x080036F4: 05da     bge      #0x8003702 */
    if (r1 >= 0) goto L_08003702;   /* signed */
    /* 0x080036F6: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);
    /* 0x080036F8: 8907     lsls     r1, r1, #0x1e */
    r1 = r1 << 0x1e;
    /* 0x080036FA: 02d4     bmi      #0x8003702 */
    if (r1 < 0) goto L_08003702;   /* result negative */
    /* 0x080036FC: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);
    /* 0x080036FE: 3143     orrs     r1, r6 */
    r1 = r1 | r6;
    /* 0x08003700: 8160     str      r1, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r1;   /* store */

    L_08003702:
    /* 0x08003702: 03f097fe bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08003706: 0546     mov      r5, r0 */
    r5 = r0;
    /* 0x08003708: 0de0     b        #0x8003726 */
    goto L_08003726;

    L_0800370A:
    /* 0x0800370A: 03f093fe bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x0800370E: 401b     subs     r0, r0, r5 */
    r0 = r0 - r5;
    /* 0x08003710: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08003712: 08d9     bls      #0x8003726 */
    if (r0 <= 2) goto L_08003726;   /* unsigned */
    /* 0x08003714: 606c     ldr      r0, [r4, #0x44] */
    r0 = *(uint32_t *)(r4 + 68);
    /* 0x08003716: 3043     orrs     r0, r6 */
    r0 = r0 | r6;
    /* 0x08003718: 6064     str      r0, [r4, #0x44] */
    *(uint32_t *)(r4 + 68) = r0;   /* store */
    /* 0x0800371A: a06c     ldr      r0, [r4, #0x48] */
    r0 = *(uint32_t *)(r4 + 72);
    /* 0x0800371C: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800371E: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08003720: a064     str      r0, [r4, #0x48] */
    *(uint32_t *)(r4 + 72) = r0;   /* store */
    /* 0x08003722: 0846     mov      r0, r1 */
    r0 = r1;
    /* 0x08003724: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08003726:
    /* 0x08003726: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x08003728: 8068     ldr      r0, [r0, #8] */
    r0 = *(uint32_t *)(r0 + 8);
    /* 0x0800372A: 4007     lsls     r0, r0, #0x1d */
    r0 = r0 << 0x1d;
    /* 0x0800372C: edd4     bmi      #0x800370a */
    if (r0 < 2) goto L_0800370A;   /* result negative */

    L_0800372E:
    /* 0x0800372E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003730: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
