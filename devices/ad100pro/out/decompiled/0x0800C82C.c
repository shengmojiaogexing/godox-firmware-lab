/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C82C
 * Name      : FUN_0800C82C
 * End       : 0x0800C872
 * Size      : 70 bytes, 32 instructions decoded in body (functions.json insns=32, blocks=5)
 * Class     : function
 * Callers   : 7  -> 0x0800BDC4 FUN_0800BDC4, 0x0800C024 FUN_0800C024, 0x0800D098 FUN_0800D098, 0x0800D0C4 FUN_0800D0C4, 0x0800D0CE FUN_0800D0CE, 0x0800D0D4 FUN_0800D0D4, 0x0800D0DA FUN_0800D0DA
 * Callees   : 2  -> 0x08004670 FUN_08004670, 0x0800A480 FUN_0800A480
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C82C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C82C: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x0800C82E: 4006     lsls     r0, r0, #0x19 */
    r0 = r0 << 0x19;
    /* 0x0800C830: 060e     lsrs     r6, r0, #0x18 */
    r6 = r0 >> 0x18;
    /* 0x0800C832: 0024     movs     r4, #0 */
    r4 = 0;

    L_0800C834:
    /* 0x0800C834: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C836: f7f71bff bl       #0x8004670 */
    FUN_08004670();   /* BL -> 0x08004670 FUN_08004670 */
    /* 0x0800C83A: a019     adds     r0, r4, r6 */
    r0 = r4 + r6;
    /* 0x0800C83C: b030     adds     r0, #0xb0 */
    r0 = r0 + 0xb0;
    /* 0x0800C83E: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800C840: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800C842: fdf71dfe bl       #0x800a480 */
    FUN_0800A480();   /* BL -> 0x0800A480 FUN_0800A480 */
    /* 0x0800C846: 0720     movs     r0, #7 */
    r0 = 7;
    /* 0x0800C848: 001b     subs     r0, r0, r4 */
    r0 = r0 - r4;
    /* 0x0800C84A: 801b     subs     r0, r0, r6 */
    r0 = r0 - r6;
    /* 0x0800C84C: c101     lsls     r1, r0, #7 */
    r1 = r0 << 7;
    /* 0x0800C84E: 0948     ldr      r0, [pc, #0x24]  ; =RAM_200004CC */
    r0 = *(uint32_t *)0x0800C874;   /* = RAM 0x200004CC */
    /* 0x0800C850: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x0800C852: 0f18     adds     r7, r1, r0 */
    r7 = r1 + r0;

    L_0800C854:
    /* 0x0800C854: 7f20     movs     r0, #0x7f */
    r0 = 0x7f;
    /* 0x0800C856: 401b     subs     r0, r0, r5 */
    r0 = r0 - r5;
    /* 0x0800C858: 385c     ldrb     r0, [r7, r0] */
    r0 = *(uint8_t *)(r7 + r0);
    /* 0x0800C85A: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800C85C: fdf710fe bl       #0x800a480 */
    FUN_0800A480();   /* BL -> 0x0800A480 FUN_0800A480 */
    /* 0x0800C860: 6d1c     adds     r5, r5, #1 */
    r5 = r5 + 1;
    /* 0x0800C862: edb2     uxtb     r5, r5 */
    r5 = uxtb(r5);
    /* 0x0800C864: 802d     cmp      r5, #0x80 */
    /* flags = cmp(r5, #0x80) */
    /* 0x0800C866: f5d3     blo      #0x800c854 */
    if (r5 < 0x80) goto L_0800C854;   /* unsigned */
    /* 0x0800C868: 641c     adds     r4, r4, #1 */
    r4 = r4 + 1;
    /* 0x0800C86A: e4b2     uxtb     r4, r4 */
    r4 = uxtb(r4);
    /* 0x0800C86C: 022c     cmp      r4, #2 */
    /* flags = cmp(r4, #2) */
    /* 0x0800C86E: e1d3     blo      #0x800c834 */
    if (r4 < 2) goto L_0800C834;   /* unsigned */
    /* 0x0800C870: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
