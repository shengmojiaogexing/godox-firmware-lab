/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008114
 * Name      : FUN_08008114
 * End       : 0x08008148
 * Size      : 52 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=5)
 * Class     : function
 * Callers   : 4  -> 0x08007F68 FUN_08007F68, 0x08007FE0 FUN_08007FE0, 0x08008030 FUN_08008030, 0x0800807A FUN_0800807A
 * Callees   : 1  -> 0x08007434 delay_or_timing
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08008114(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08008114: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08008116: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x08008118: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);
    /* 0x0800811A: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);
    /* 0x0800811C: a022     movs     r2, #0xa0 */
    r2 = 0xa0;
    /* 0x0800811E: 9143     bics     r1, r2 */
    r1 = r1 & ~r2;
    /* 0x08008120: c160     str      r1, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r1;   /* store */
    /* 0x08008122: fff787f9 bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08008126: 7d26     movs     r6, #0x7d */
    r6 = 0x7d;
    /* 0x08008128: 0546     mov      r5, r0 */
    r5 = r0;
    /* 0x0800812A: f600     lsls     r6, r6, #3 */
    r6 = r6 << 3;
    /* 0x0800812C: 06e0     b        #0x800813c */
    goto L_0800813C;

    L_0800812E:
    /* 0x0800812E: fff781f9 bl       #0x8007434 */
    delay_or_timing();   /* BL -> 0x08007434 delay_or_timing */
    /* 0x08008132: 401b     subs     r0, r0, r5 */
    r0 = r0 - r5;
    /* 0x08008134: b042     cmp      r0, r6 */
    /* flags = cmp(r0, r6) */
    /* 0x08008136: 01d9     bls      #0x800813c */
    if (r0 <= r6) goto L_0800813C;   /* unsigned */
    /* 0x08008138: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x0800813A: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_0800813C:
    /* 0x0800813C: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x0800813E: c068     ldr      r0, [r0, #0xc] */
    r0 = *(uint32_t *)(r0 + 12);
    /* 0x08008140: 8006     lsls     r0, r0, #0x1a */
    r0 = r0 << 0x1a;
    /* 0x08008142: f4d5     bpl      #0x800812e */
    if (r0 >= r6) goto L_0800812E;   /* result non-negative */
    /* 0x08008144: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08008146: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
