/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800ACBE
 * Name      : FUN_0800ACBE
 * End       : 0x0800ACF6
 * Size      : 56 bytes, 26 instructions decoded in body (functions.json insns=26, blocks=6)
 * Class     : function
 * Callers   : 6  -> 0x08007E70 FUN_08007E70, 0x08007E8A FUN_08007E8A, 0x08007F68 FUN_08007F68, 0x08007FC6 FUN_08007FC6, 0x08008030 FUN_08008030, 0x0800807A FUN_0800807A
 * Callees   : 1  -> 0x08007434 delay_or_timing
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800ACBE(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800ACBE: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800ACC0: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x0800ACC2: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);
    /* 0x0800ACC4: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);
    /* 0x0800ACC6: 4906     lsls     r1, r1, #0x19 */
    r1 = r1 << 0x19;
    /* 0x0800ACC8: 13d4     bmi      #0x800acf2 */
    if (/* bmi */) goto L_0800ACF2;   /* result negative */
    /* 0x0800ACCA: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800ACCC: c943     mvns     r1, r1 */
    r1 = ~r1;
    /* 0x0800ACCE: c160     str      r1, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r1;   /* store */
    /* 0x0800ACD0: fcf7b0fb bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x0800ACD4: 7d26     movs     r6, #0x7d */
    r6 = 0x7d;
    /* 0x0800ACD6: 0546     mov      r5, r0 */
    r5 = r0;
    /* 0x0800ACD8: f600     lsls     r6, r6, #3 */
    r6 = r6 << 3;
    /* 0x0800ACDA: 06e0     b        #0x800acea */
    goto L_0800ACEA;

    L_0800ACDC:
    /* 0x0800ACDC: fcf7aafb bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x0800ACE0: 401b     subs     r0, r0, r5 */
    r0 = r0 - r5;
    /* 0x0800ACE2: b042     cmp      r0, r6 */
    /* flags = cmp(r0, r6) */
    /* 0x0800ACE4: 01d9     bls      #0x800acea */
    if (r0 <= r6) goto L_0800ACEA;   /* unsigned */
    /* 0x0800ACE6: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x0800ACE8: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_0800ACEA:
    /* 0x0800ACEA: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x0800ACEC: c068     ldr      r0, [r0, #0xc] */
    r0 = *(uint32_t *)(r0 + 12);
    /* 0x0800ACEE: 4006     lsls     r0, r0, #0x19 */
    r0 = r0 << 0x19;
    /* 0x0800ACF0: f4d5     bpl      #0x800acdc */
    if (r0 >= r6) goto L_0800ACDC;   /* result non-negative */

    L_0800ACF2:
    /* 0x0800ACF2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800ACF4: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
