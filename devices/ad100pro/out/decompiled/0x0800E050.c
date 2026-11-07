/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800E050
 * Name      : FUN_0800E050
 * End       : 0x0800E0DE
 * Size      : 142 bytes, 60 instructions decoded in body (functions.json insns=60, blocks=2)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800E050(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800E050: 03fe00c0 cdp2     p0, #0, c12, c3, c0, #0 */
    /* TODO: cdp2 p0, #0, c12, c3, c0, #0 */
    /* 0x0800E054: 0060     str      r0, [r0] */
    *(uint32_t *)(r0) = r0;   /* store */
    /* 0x0800E056: 0030     adds     r0, #0 */
    r0 = r0 + 0;
    /* 0x0800E058: 03fe03fe cdp2     p14, #0, c15, c3, c3, #0 */
    /* TODO: cdp2 p14, #0, c15, c3, c3, #0 */
    /* 0x0800E05C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E05E: 0601     lsls     r6, r0, #4 */
    r6 = r0 << 4;
    /* 0x0800E060: fc03     lsls     r4, r7, #0xf */
    r4 = r7 << 0xf;
    /* 0x0800E062: fe02     lsls     r6, r7, #0xb */
    r6 = r7 << 0xb;
    /* 0x0800E064: 0202     lsls     r2, r0, #8 */
    r2 = r0 << 8;
    /* 0x0800E066: 0203     lsls     r2, r0, #0xc */
    r2 = r0 << 0xc;
    /* 0x0800E068: fe01     lsls     r6, r7, #7 */
    r6 = r7 << 7;
    /* 0x0800E06A: fc00     lsls     r4, r7, #3 */
    r4 = r7 << 3;
    /* 0x0800E06C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E06E: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800E070: 03fe03fe cdp2     p14, #0, c15, c3, c3, #0 */
    /* TODO: cdp2 p14, #0, c15, c3, c3, #0 */
    /* 0x0800E074: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x0800E076: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x0800E078: 03e0     b        #0x800e082 */
    goto L_0800E082;

    L_0800E082:
    /* 0x0800E082: fc43     mvns     r4, r7 */
    r4 = ~r7;
    /* 0x0800E084: fe22     movs     r2, #0xfe */
    r2 = 0xfe;
    /* 0x0800E086: 0212     asrs     r2, r0, #8 */
    r2 = r0 >> 8;
    /* 0x0800E088: 030b     lsrs     r3, r0, #0xc */
    r3 = r0 >> 0xc;
    /* 0x0800E08A: ff05     lsls     r7, r7, #0x17 */
    r7 = r7 << 0x17;
    /* 0x0800E08C: fc02     lsls     r4, r7, #0xb */
    r4 = r7 << 0xb;
    /* 0x0800E08E: 0001     lsls     r0, r0, #4 */
    r0 = r0 << 4;
    /* 0x0800E090: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800E092: 03fe03fe cdp2     p14, #0, c15, c3, c3, #0 */
    /* TODO: cdp2 p14, #0, c15, c3, c3, #0 */
    /* 0x0800E096: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x0800E098: 0230     adds     r0, #2 */
    r0 = r0 + 2;
    /* 0x0800E09A: 03fe01ce cdp2     p14, #0, c12, c3, c1, #0 */
    /* TODO: cdp2 p14, #0, c12, c3, c1, #0 */
    /* 0x0800E09E: 0200     movs     r2, r0 */
    r2 = r0;
    /* 0x0800E0A0: 0100     movs     r1, r0 */
    r1 = r0;
    /* 0x0800E0A2: 0601     lsls     r6, r0, #4 */
    r6 = r0 << 4;
    /* 0x0800E0A4: 8403     lsls     r4, r0, #0xe */
    r4 = r0 << 0xe;
    /* 0x0800E0A6: c602     lsls     r6, r0, #0xb */
    r6 = r0 << 0xb;
    /* 0x0800E0A8: 6202     lsls     r2, r4, #9 */
    r2 = r4 << 9;
    /* 0x0800E0AA: 3203     lsls     r2, r6, #0xc */
    r2 = r6 << 0xc;
    /* 0x0800E0AC: 1e01     lsls     r6, r3, #4 */
    r6 = r3 << 4;
    /* 0x0800E0AE: 0c00     movs     r4, r1 */
    r4 = r1;
    /* 0x0800E0B0: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E0B2: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800E0B4: 0200     movs     r2, r0 */
    r2 = r0;
    /* 0x0800E0B6: 0200     movs     r2, r0 */
    r2 = r0;
    /* 0x0800E0B8: 03fe03fe cdp2     p14, #0, c15, c3, c3, #0 */
    /* TODO: cdp2 p14, #0, c15, c3, c3, #0 */
    /* 0x0800E0BC: 0200     movs     r2, r0 */
    r2 = r0;
    /* 0x0800E0BE: 0200     movs     r2, r0 */
    r2 = r0;
    /* 0x0800E0C0: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E0C2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E0C4: 0603     lsls     r6, r0, #0xc */
    r6 = r0 << 0xc;
    /* 0x0800E0C6: fc03     lsls     r4, r7, #0xf */
    r4 = r7 << 0xf;
    /* 0x0800E0C8: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800E0CA: 0200     movs     r2, r0 */
    r2 = r0;
    /* 0x0800E0CC: 0203     lsls     r2, r0, #0xc */
    r2 = r0 << 0xc;
    /* 0x0800E0CE: fe03     lsls     r6, r7, #0xf */
    r6 = r7 << 0xf;
    /* 0x0800E0D0: fc00     lsls     r4, r7, #3 */
    r4 = r7 << 3;
    /* 0x0800E0D2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E0D4: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800E0D6: 03f803fc strb     pc, [r3, #-0x3] */
    *(uint8_t *)(r3 - 3) = pc;   /* store */
    /* 0x0800E0DA: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800E0DC: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
}
