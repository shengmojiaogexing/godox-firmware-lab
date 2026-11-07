/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080067EC
 * Name      : FUN_080067EC
 * End       : 0x08006872
 * Size      : 134 bytes, 67 instructions decoded in body (functions.json insns=67, blocks=21)
 * Class     : function
 * Callers   : 2  -> 0x08008D00 dispatcher_large, 0x08008F3A FUN_08008F3A
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080067EC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080067EC: 2148     ldr      r0, [pc, #0x84]  ; =RAM_20000164 */
    r0 = *(uint32_t *)0x08006874;   /* = RAM 0x20000164 */
    /* 0x080067EE: c178     ldrb     r1, [r0, #3] */
    r1 = *(uint8_t *)(r0 + 3);
    /* 0x080067F0: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x080067F2: 12d0     beq      #0x800681a */
    if (r1 == 1) goto L_0800681A;
    /* 0x080067F4: 427d     ldrb     r2, [r0, #0x15] */
    r2 = *(uint8_t *)(r0 + 21);
    /* 0x080067F6: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x080067F8: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x080067FA: 29d0     beq      #0x8006850 */
    if (r2 == 1) goto L_08006850;
    /* 0x080067FC: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x080067FE: 2032     adds     r2, #0x20 */
    r2 = r2 + 0x20;
    /* 0x08006800: 9378     ldrb     r3, [r2, #2] */
    r3 = *(uint8_t *)(r2 + 2);
    /* 0x08006802: 012b     cmp      r3, #1 */
    /* flags = cmp(r3, #1) */
    /* 0x08006804: 26d0     beq      #0x8006854 */
    if (r3 == 1) goto L_08006854;
    /* 0x08006806: 0278     ldrb     r2, [r0] */
    r2 = *(uint8_t *)(r0);
    /* 0x08006808: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800680A: 02d0     beq      #0x8006812 */
    if (r2 == 1) goto L_08006812;
    /* 0x0800680C: 827c     ldrb     r2, [r0, #0x12] */
    r2 = *(uint8_t *)(r0 + 18);
    /* 0x0800680E: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x08006810: 22d0     beq      #0x8006858 */
    if (r2 == 0) goto L_08006858;

    L_08006812:
    /* 0x08006812: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x08006814: 8174     strb     r1, [r0, #0x12] */
    *(uint8_t *)(r0 + 18) = r1;   /* store */
    /* 0x08006816: 4163     str      r1, [r0, #0x34] */
    *(uint32_t *)(r0 + 52) = r1;   /* store */

    L_08006818:
    /* 0x08006818: 7047     bx       lr */
    return;

    L_0800681A:
    /* 0x0800681A: 417c     ldrb     r1, [r0, #0x11] */
    r1 = *(uint8_t *)(r0 + 17);
    /* 0x0800681C: c906     lsls     r1, r1, #0x1b */
    r1 = r1 << 0x1b;
    /* 0x0800681E: 08d5     bpl      #0x8006832 */
    if (r2 >= 0) goto L_08006832;   /* result non-negative */
    /* 0x08006820: c18c     ldrh     r1, [r0, #0x26] */
    r1 = *(uint16_t *)(r0 + 38);
    /* 0x08006822: 491e     subs     r1, r1, #1 */
    r1 = r1 - 1;
    /* 0x08006824: 89b2     uxth     r1, r1 */
    r1 = uxth(r1);
    /* 0x08006826: c184     strh     r1, [r0, #0x26] */
    *(uint16_t *)(r0 + 38) = r1;   /* store */
    /* 0x08006828: 0f29     cmp      r1, #0xf */
    /* flags = cmp(r1, #0xf) */
    /* 0x0800682A: f5d2     bhs      #0x8006818 */
    if (r1 >= 0xf) goto L_08006818;   /* unsigned */
    /* 0x0800682C: 0f21     movs     r1, #0xf */
    r1 = 0xf;
    /* 0x0800682E: c184     strh     r1, [r0, #0x26] */
    *(uint16_t *)(r0 + 38) = r1;   /* store */
    /* 0x08006830: 7047     bx       lr */
    return;

    L_08006832:
    /* 0x08006832: 018d     ldrh     r1, [r0, #0x28] */
    r1 = *(uint16_t *)(r0 + 40);
    /* 0x08006834: 491f     subs     r1, r1, #5 */
    r1 = r1 - 5;
    /* 0x08006836: 89b2     uxth     r1, r1 */
    r1 = uxth(r1);
    /* 0x08006838: 0185     strh     r1, [r0, #0x28] */
    *(uint16_t *)(r0 + 40) = r1;   /* store */
    /* 0x0800683A: 6429     cmp      r1, #0x64 */
    /* flags = cmp(r1, #0x64) */
    /* 0x0800683C: 02d2     bhs      #0x8006844 */
    if (r1 >= 0x64) goto L_08006844;   /* unsigned */
    /* 0x0800683E: 6421     movs     r1, #0x64 */
    r1 = 0x64;
    /* 0x08006840: 0185     strh     r1, [r0, #0x28] */
    *(uint16_t *)(r0 + 40) = r1;   /* store */

    L_08006842:
    /* 0x08006842: 7047     bx       lr */
    return;

    L_08006844:
    /* 0x08006844: ff22     movs     r2, #0xff */
    r2 = 0xff;
    /* 0x08006846: 4b32     adds     r2, #0x4b */
    r2 = r2 + 0x4b;
    /* 0x08006848: 9142     cmp      r1, r2 */
    /* flags = cmp(r1, r2) */
    /* 0x0800684A: fad9     bls      #0x8006842 */
    if (r1 <= r2) goto L_08006842;   /* unsigned */
    /* 0x0800684C: 0285     strh     r2, [r0, #0x28] */
    *(uint16_t *)(r0 + 40) = r2;   /* store */
    /* 0x0800684E: 7047     bx       lr */
    return;

    L_08006850:
    /* 0x08006850: 4175     strb     r1, [r0, #0x15] */
    *(uint8_t *)(r0 + 21) = r1;   /* store */
    /* 0x08006852: 7047     bx       lr */
    return;

    L_08006854:
    /* 0x08006854: 9170     strb     r1, [r2, #2] */
    *(uint8_t *)(r2 + 2) = r1;   /* store */
    /* 0x08006856: 0ae0     b        #0x800686e */
    goto L_0800686E;

    L_08006858:
    /* 0x08006858: 027e     ldrb     r2, [r0, #0x18] */
    r2 = *(uint8_t *)(r0 + 24);
    /* 0x0800685A: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x0800685C: 07d0     beq      #0x800686e */
    if (r2 == 0) goto L_0800686E;
    /* 0x0800685E: 427e     ldrb     r2, [r0, #0x19] */
    r2 = *(uint8_t *)(r0 + 25);
    /* 0x08006860: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x08006862: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);
    /* 0x08006864: 4276     strb     r2, [r0, #0x19] */
    *(uint8_t *)(r0 + 25) = r2;   /* store */
    /* 0x08006866: 052a     cmp      r2, #5 */
    /* flags = cmp(r2, #5) */
    /* 0x08006868: 01d9     bls      #0x800686e */
    if (r2 <= 5) goto L_0800686E;   /* unsigned */
    /* 0x0800686A: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800686C: 4276     strb     r2, [r0, #0x19] */
    *(uint8_t *)(r0 + 25) = r2;   /* store */

    L_0800686E:
    /* 0x0800686E: c174     strb     r1, [r0, #0x13] */
    *(uint8_t *)(r0 + 19) = r1;   /* store */
    /* 0x08006870: 7047     bx       lr */
    return;
}
