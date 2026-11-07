/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A818
 * Name      : FUN_0800A818
 * End       : 0x0800A8BA
 * Size      : 162 bytes, 68 instructions decoded in body (functions.json insns=68, blocks=8)
 * Class     : function
 * Callers   : 4  -> 0x0800AFC4 FUN_0800AFC4, 0x0800B03C FUN_0800B03C, 0x0800B0BE FUN_0800B0BE, 0x0800B1F4 FUN_0800B1F4
 * Callees   : 2  -> 0x08007426 FUN_08007426, 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A818(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A818: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800A81A: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800A81C: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800A820: 2648     ldr      r0, [pc, #0x98]  ; =RAM_200001B8 */
    r0 = *(uint32_t *)0x0800A8BC;   /* = RAM 0x200001B8 */
    /* 0x0800A822: 0925     movs     r5, #9 */
    r5 = 9;
    /* 0x0800A824: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800A826: ed06     lsls     r5, r5, #0x1b */
    r5 = r5 << 0x1b;
    /* 0x0800A828: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800A82A: 35d1     bne      #0x800a898 */
    if (r0 != 1) goto L_0800A898;
    /* 0x0800A82C: 2448     ldr      r0, [pc, #0x90]  ; =RAM_200001B2 */
    r0 = *(uint32_t *)0x0800A8C0;   /* = RAM 0x200001B2 */
    /* 0x0800A82E: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x0800A830: 0529     cmp      r1, #5 */
    /* flags = cmp(r1, #5) */
    /* 0x0800A832: 07d0     beq      #0x800a844 */
    if (r1 == 5) goto L_0800A844;
    /* 0x0800A834: 0829     cmp      r1, #8 */
    /* flags = cmp(r1, #8) */
    /* 0x0800A836: 05d0     beq      #0x800a844 */
    if (r1 == 8) goto L_0800A844;
    /* 0x0800A838: 1829     cmp      r1, #0x18 */
    /* flags = cmp(r1, #0x18) */
    /* 0x0800A83A: 2dd1     bne      #0x800a898 */
    if (r1 != 0x18) goto L_0800A898;
    /* 0x0800A83C: 2148     ldr      r0, [pc, #0x84]  ; =RAM_2000017C */
    r0 = *(uint32_t *)0x0800A8C4;   /* = RAM 0x2000017C */
    /* 0x0800A83E: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800A840: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800A842: 29d1     bne      #0x800a898 */
    if (r0 != 1) goto L_0800A898;

    L_0800A844:
    /* 0x0800A844: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800A846: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x0800A848: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800A84A: fcf7ecfd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A84E: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800A850: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800A852: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800A854: fcf7e7fd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A858: 3220     movs     r0, #0x32 */
    r0 = 0x32;
    /* 0x0800A85A: 02f0edf8 bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800A85E: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800A860: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800A862: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800A864: fcf7dffd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A868: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x0800A86A: 02f0e5f8 bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800A86E: 0024     movs     r4, #0 */
    r4 = 0;

    L_0800A870:
    /* 0x0800A870: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800A872: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800A874: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800A876: fcf7d6fd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A87A: 0720     movs     r0, #7 */
    r0 = 7;
    /* 0x0800A87C: 02f0dcf8 bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800A880: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800A882: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800A884: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800A886: fcf7cefd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A88A: 1720     movs     r0, #0x17 */
    r0 = 0x17;
    /* 0x0800A88C: 02f0d4f8 bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800A890: 641c     adds     r4, r4, #1 */
    r4 = r4 + 1;
    /* 0x0800A892: e4b2     uxtb     r4, r4 */
    r4 = uxtb(r4);
    /* 0x0800A894: 2a2c     cmp      r4, #0x2a */
    /* flags = cmp(r4, #0x2a) */
    /* 0x0800A896: ebd3     blo      #0x800a870 */
    if (r4 < 0x2a) goto L_0800A870;   /* unsigned */

    L_0800A898:
    /* 0x0800A898: 6420     movs     r0, #0x64 */
    r0 = 0x64;
    /* 0x0800A89A: 02f0cdf8 bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800A89E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800A8A0: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800A8A4: 0849     ldr      r1, [pc, #0x20]  ; =RAM_200001BC */
    r1 = *(uint32_t *)0x0800A8C8;   /* = RAM 0x200001BC */
    /* 0x0800A8A6: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800A8A8: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x0800A8AA: 0849     ldr      r1, [pc, #0x20]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x0800A8CC;   /* = RAM 0x20000134 */
    /* 0x0800A8AC: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800A8AE: 4872     strb     r0, [r1, #9] */
    *(uint8_t *)(r1 + 9) = r0;   /* store */
    /* 0x0800A8B0: 0146     mov      r1, r0 */
    r1 = r0;
    /* 0x0800A8B2: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800A8B4: fcf7b7fd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A8B8: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
