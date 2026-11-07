/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800E0F0
 * Name      : FUN_0800E0F0
 * End       : 0x0800E22A
 * Size      : 314 bytes, 145 instructions decoded in body (functions.json insns=145, blocks=2)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800E0F0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800E0F0: 0e03     lsls     r6, r1, #0xc */
    r6 = r1 << 0xc;
    /* 0x0800E0F2: fe03     lsls     r6, r7, #0xf */
    r6 = r7 << 0xf;
    /* 0x0800E0F4: f000     lsls     r0, r6, #3 */
    r0 = r6 << 3;
    /* 0x0800E0F6: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800E0F8: 030e     lsrs     r3, r0, #0x18 */
    r3 = r0 >> 0x18;
    /* 0x0800E0FA: 039e     ldr      r6, [sp, #0xc] */
    r6 = *(uint32_t *)(sp + 12);
    /* 0x0800E0FC: 00e0     b        #0x800e100 */
    goto L_0800E100;

    L_0800E100:
    /* 0x0800E100: 039e     ldr      r6, [sp, #0xc] */
    r6 = *(uint32_t *)(sp + 12);
    /* 0x0800E102: 030e     lsrs     r3, r0, #0x18 */
    r3 = r0 >> 0x18;
    /* 0x0800E104: 0200     movs     r2, r0 */
    r2 = r0;
    /* 0x0800E106: 0100     movs     r1, r0 */
    r1 = r0;
    /* 0x0800E108: 0603     lsls     r6, r0, #0xc */
    r6 = r0 << 0xc;
    /* 0x0800E10A: c003     lsls     r0, r0, #0xf */
    r0 = r0 << 0xf;
    /* 0x0800E10C: e000     lsls     r0, r4, #3 */
    r0 = r4 << 3;
    /* 0x0800E10E: 3e00     movs     r6, r7 */
    r6 = r7;
    /* 0x0800E110: 3e03     lsls     r6, r7, #0xc */
    r6 = r7 << 0xc;
    /* 0x0800E112: e003     lsls     r0, r4, #0xf */
    r0 = r4 << 0xf;
    /* 0x0800E114: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x0800E116: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E118: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800E11A: 020e     lsrs     r2, r0, #0x18 */
    r2 = r0 >> 0x18;
    /* 0x0800E11C: 021e     subs     r2, r0, #0 */
    r2 = r0 - 0;
    /* 0x0800E11E: 0232     adds     r2, #2 */
    r2 = r2 + 2;
    /* 0x0800E120: 0262     str      r2, [r0, #0x20] */
    *(uint32_t *)(r0 + 32) = r2;   /* store */
    /* 0x0800E122: 03c2     stm      r2!, {r0, r1} */
    /* stm r2!, {r0, r1} */
    /* 0x0800E124: 0382     strh     r3, [r0, #0x10] */
    *(uint16_t *)(r0 + 16) = r3;   /* store */
    /* 0x0800E126: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E128: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E12A: 0600     movs     r6, r0 */
    r6 = r0;
    /* 0x0800E12C: 0c00     movs     r4, r1 */
    r4 = r1;
    /* 0x0800E12E: 9e00     lsls     r6, r3, #2 */
    r6 = r3 << 2;
    /* 0x0800E130: 9200     lsls     r2, r2, #2 */
    r2 = r2 << 2;
    /* 0x0800E132: 9200     lsls     r2, r2, #2 */
    r2 = r2 << 2;
    /* 0x0800E134: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800E136: 7e00     lsls     r6, r7, #1 */
    r6 = r7 << 1;
    /* 0x0800E138: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E13A: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800E13C: 03fe03fe cdp2     p14, #0, c15, c3, c3, #0 */
    /* TODO: cdp2 p14, #0, c15, c3, c3, #0 */
    /* 0x0800E140: 0082     strh     r0, [r0, #0x10] */
    *(uint16_t *)(r0 + 16) = r0;   /* store */
    /* 0x0800E142: 0082     strh     r0, [r0, #0x10] */
    *(uint16_t *)(r0 + 16) = r0;   /* store */
    /* 0x0800E144: 00fe007c cdp2     p12, #0, c7, c0, c0, #0 */
    /* TODO: cdp2 p12, #0, c7, c0, c0, #0 */
    /* 0x0800E148: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E14A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E14C: 0600     movs     r6, r0 */
    r6 = r0;
    /* 0x0800E14E: 7c00     lsls     r4, r7, #1 */
    r4 = r7 << 1;
    /* 0x0800E150: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800E152: 8200     lsls     r2, r0, #2 */
    r2 = r0 << 2;
    /* 0x0800E154: 8200     lsls     r2, r0, #2 */
    r2 = r0 << 2;
    /* 0x0800E156: c600     lsls     r6, r0, #3 */
    r6 = r0 << 3;
    /* 0x0800E158: 4400     lsls     r4, r0, #1 */
    r4 = r0 << 1;
    /* 0x0800E15A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E15C: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800E15E: 807c     ldrb     r0, [r0, #0x12] */
    r0 = *(uint8_t *)(r0 + 18);
    /* 0x0800E160: 40fe2082 cdp2     p2, #4, c8, c0, c0, #1 */
    /* TODO: cdp2 p2, #4, c8, c0, c0, #1 */
    /* 0x0800E164: 1082     strh     r0, [r2, #0x10] */
    *(uint16_t *)(r2 + 16) = r0;   /* store */
    /* 0x0800E166: 0bfe07fe cdp2     p14, #0, c15, c11, c7, #0 */
    /* TODO: cdp2 p14, #0, c15, c11, c7, #0 */
    /* 0x0800E16A: 0200     movs     r2, r0 */
    r2 = r0;
    /* 0x0800E16C: 0100     movs     r1, r0 */
    r1 = r0;
    /* 0x0800E16E: 0600     movs     r6, r0 */
    r6 = r0;
    /* 0x0800E170: 7c00     lsls     r4, r7, #1 */
    r4 = r7 << 1;
    /* 0x0800E172: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800E174: 9200     lsls     r2, r2, #2 */
    r2 = r2 << 2;
    /* 0x0800E176: 9200     lsls     r2, r2, #2 */
    r2 = r2 << 2;
    /* 0x0800E178: f200     lsls     r2, r6, #3 */
    r2 = r6 << 3;
    /* 0x0800E17A: 7000     lsls     r0, r6, #1 */
    r0 = r6 << 1;
    /* 0x0800E17C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E17E: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800E180: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800E182: 01fe03fe cdp2     p14, #0, c15, c1, c3, #0 */
    /* TODO: cdp2 p14, #0, c15, c1, c3, #0 */
    /* 0x0800E186: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x0800E188: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x0800E18A: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x0800E18C: 0200     movs     r2, r0 */
    r2 = r0;
    /* 0x0800E18E: 0100     movs     r1, r0 */
    r1 = r0;
    /* 0x0800E190: 0680     strh     r6, [r0] */
    *(uint16_t *)(r0) = r6;   /* store */
    /* 0x0800E192: 7c40     eors     r4, r7 */
    r4 = r4 ^ r7;
    /* 0x0800E194: fe20     movs     r0, #0xfe */
    r0 = 0xfe;
    /* 0x0800E196: 8210     asrs     r2, r0, #2 */
    r2 = r0 >> 2;
    /* 0x0800E198: 8208     lsrs     r2, r0, #2 */
    r2 = r0 >> 2;
    /* 0x0800E19A: ff04     lsls     r7, r7, #0x13 */
    r7 = r7 << 0x13;
    /* 0x0800E19C: ff02     lsls     r7, r7, #0xb */
    r7 = r7 << 0xb;
    /* 0x0800E19E: 0001     lsls     r0, r0, #4 */
    r0 = r0 << 4;
    /* 0x0800E1A0: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800E1A2: 03fe03fe cdp2     p14, #0, c15, c3, c3, #0 */
    /* TODO: cdp2 p14, #0, c15, c3, c3, #0 */
    /* 0x0800E1A6: 0080     strh     r0, [r0] */
    *(uint16_t *)(r0) = r0;   /* store */
    /* 0x0800E1A8: 0080     strh     r0, [r0] */
    *(uint16_t *)(r0) = r0;   /* store */
    /* 0x0800E1AA: 00fe007e cdp2     p14, #0, c7, c0, c0, #0 */
    /* TODO: cdp2 p14, #0, c7, c0, c0, #0 */
    /* 0x0800E1AE: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E1B0: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E1B2: 0600     movs     r6, r0 */
    r6 = r0;
    /* 0x0800E1B4: 8200     lsls     r2, r0, #2 */
    r2 = r0 << 2;
    /* 0x0800E1B6: 8206     lsls     r2, r0, #0x1a */
    r2 = r0 << 0x1a;
    /* 0x0800E1B8: fe06     lsls     r6, r7, #0x1b */
    r6 = r7 << 0x1b;
    /* 0x0800E1BA: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800E1BC: 0200     movs     r2, r0 */
    r2 = r0;
    /* 0x0800E1BE: 0200     movs     r2, r0 */
    r2 = r0;
    /* 0x0800E1C0: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E1C2: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800E1C4: 8000     lsls     r0, r0, #2 */
    r0 = r0 << 2;
    /* 0x0800E1C6: 4080     strh     r0, [r0, #2] */
    *(uint16_t *)(r0 + 2) = r0;   /* store */
    /* 0x0800E1C8: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */
    /* 0x0800E1CA: 16ff0eff vpmax.f16 d15, d6, d14 */
    /* TODO: vpmax.f16 d15, d6, d14 */
    /* 0x0800E1CE: 0400     movs     r4, r0 */
    r4 = r0;
    /* 0x0800E1D0: 0200     movs     r2, r0 */
    r2 = r0;
    /* 0x0800E1D2: 0100     movs     r1, r0 */
    r1 = r0;
    /* 0x0800E1D4: 0603     lsls     r6, r0, #0xc */
    r6 = r0 << 0xc;
    /* 0x0800E1D6: fe03     lsls     r6, r7, #0xf */
    r6 = r7 << 0xf;
    /* 0x0800E1D8: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800E1DA: 1000     movs     r0, r2 */
    r0 = r2;
    /* 0x0800E1DC: 3800     movs     r0, r7 */
    r0 = r7;
    /* 0x0800E1DE: ee00     lsls     r6, r5, #3 */
    r6 = r5 << 3;
    /* 0x0800E1E0: c600     lsls     r6, r0, #3 */
    r6 = r0 << 3;
    /* 0x0800E1E2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E1E4: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800E1E6: 0202     lsls     r2, r0, #8 */
    r2 = r0 << 8;
    /* 0x0800E1E8: 0202     lsls     r2, r0, #8 */
    r2 = r0 << 8;
    /* 0x0800E1EA: 03fe03fe cdp2     p14, #0, c15, c3, c3, #0 */
    /* TODO: cdp2 p14, #0, c15, c3, c3, #0 */
    /* 0x0800E1EE: 0002     lsls     r0, r0, #8 */
    r0 = r0 << 8;
    /* 0x0800E1F0: 0002     lsls     r0, r0, #8 */
    r0 = r0 << 8;
    /* 0x0800E1F2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E1F4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E1F6: 0700     movs     r7, r0 */
    r7 = r0;
    /* 0x0800E1F8: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800E1FA: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800E1FC: 8000     lsls     r0, r0, #2 */
    r0 = r0 << 2;
    /* 0x0800E1FE: fc00     lsls     r4, r7, #3 */
    r4 = r7 << 3;
    /* 0x0800E200: 8000     lsls     r0, r0, #2 */
    r0 = r0 << 2;
    /* 0x0800E202: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800E204: 7e00     lsls     r6, r7, #1 */
    r6 = r7 << 1;
    /* 0x0800E206: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800E208: 00fe00fe cdp2     p14, #0, c15, c0, c0, #0 */
    /* TODO: cdp2 p14, #0, c15, c0, c0, #0 */
    /* 0x0800E20C: 0080     strh     r0, [r0] */
    *(uint16_t *)(r0) = r0;   /* store */
    /* 0x0800E20E: 0080     strh     r0, [r0] */
    *(uint16_t *)(r0) = r0;   /* store */
    /* 0x0800E210: 00fe007e cdp2     p14, #0, c7, c0, c0, #0 */
    /* TODO: cdp2 p14, #0, c7, c0, c0, #0 */
    /* 0x0800E214: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E216: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E218: 0600     movs     r6, r0 */
    r6 = r0;
    /* 0x0800E21A: 7c00     lsls     r4, r7, #1 */
    r4 = r7 << 1;
    /* 0x0800E21C: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800E21E: 8200     lsls     r2, r0, #2 */
    r2 = r0 << 2;
    /* 0x0800E220: 8200     lsls     r2, r0, #2 */
    r2 = r0 << 2;
    /* 0x0800E222: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800E224: 7c00     lsls     r4, r7, #1 */
    r4 = r7 << 1;
    /* 0x0800E226: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E228: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
}
