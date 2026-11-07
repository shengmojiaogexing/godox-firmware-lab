/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006878
 * Name      : FUN_08006878
 * End       : 0x08006994
 * Size      : 284 bytes, 140 instructions decoded in body (functions.json insns=140, blocks=40)
 * Class     : function
 * Callers   : 5  -> 0x08009310 FUN_08009310, 0x08009344 FUN_08009344, 0x08009360 FUN_08009360, 0x08009374 FUN_08009374, 0x08009388 FUN_08009388
 * Callees   : 1  -> 0x080030F0 FUN_080030F0
 * MMIO      : ADC.+0x308
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006878(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006878: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x0800687A: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x0800687C: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x0800687E: 4034     adds     r4, #0x40 */
    r4 = r4 + 0x40;
    /* 0x08006880: 0095     str      r5, [sp] */
    *(uint32_t *)(sp) = r5;   /* store */
    /* 0x08006882: 2278     ldrb     r2, [r4] */
    r2 = *(uint8_t *)(r4);
    /* 0x08006884: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x08006886: 3bd0     beq      #0x8006900 */
    if (r2 == 1) goto L_08006900;
    /* 0x08006888: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800688A: 2270     strb     r2, [r4] */
    *(uint8_t *)(r4) = r2;   /* store */
    /* 0x0800688C: 0368     ldr      r3, [r0] */
    r3 = *(uint32_t *)(r0);
    /* 0x0800688E: 9e68     ldr      r6, [r3, #8] */
    r6 = *(uint32_t *)(r3 + 8);
    /* 0x08006890: 7607     lsls     r6, r6, #0x1d */
    r6 = r6 << 0x1d;
    /* 0x08006892: 76d4     bmi      #0x8006982 */
    if (r2 < 1) goto L_08006982;   /* result negative */
    /* 0x08006894: 3f4f     ldr      r7, [pc, #0xfc]  ; =0x00001001 */
    r7 = *(uint32_t *)0x08006994;   /* = 0x00001001 */
    /* 0x08006896: 4e68     ldr      r6, [r1, #4] */
    r6 = *(uint32_t *)(r1 + 4);
    /* 0x08006898: be42     cmp      r6, r7 */
    /* flags = cmp(r6, r7) */
    /* 0x0800689A: 54d0     beq      #0x8006946 */
    if (r6 == r7) goto L_08006946;
    /* 0x0800689C: 9e6a     ldr      r6, [r3, #0x28] */
    r6 = *(uint32_t *)(r3 + 40);
    /* 0x0800689E: 0f78     ldrb     r7, [r1] */
    r7 = *(uint8_t *)(r1);
    /* 0x080068A0: ba40     lsls     r2, r7 */
    r2 = r2 << r7;
    /* 0x080068A2: 1643     orrs     r6, r2 */
    r6 = r6 | r2;
    /* 0x080068A4: 9e62     str      r6, [r3, #0x28] */
    *(uint32_t *)(r3 + 40) = r6;   /* store */
    /* 0x080068A6: 0123     movs     r3, #1 */
    r3 = 1;
    /* 0x080068A8: 826b     ldr      r2, [r0, #0x38] */
    r2 = *(uint32_t *)(r0 + 56);
    /* 0x080068AA: 1b07     lsls     r3, r3, #0x1c */
    r3 = r3 << 0x1c;
    /* 0x080068AC: 9a42     cmp      r2, r3 */
    /* flags = cmp(r2, r3) */
    /* 0x080068AE: 1fd0     beq      #0x80068f0 */
    if (r2 == r3) goto L_080068F0;
    /* 0x080068B0: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x080068B2: 1dd0     beq      #0x80068f0 */
    if (r2 == 1) goto L_080068F0;
    /* 0x080068B4: 022a     cmp      r2, #2 */
    /* flags = cmp(r2, #2) */
    /* 0x080068B6: 1bd0     beq      #0x80068f0 */
    if (r2 == 2) goto L_080068F0;
    /* 0x080068B8: 032a     cmp      r2, #3 */
    /* flags = cmp(r2, #3) */
    /* 0x080068BA: 19d0     beq      #0x80068f0 */
    if (r2 == 3) goto L_080068F0;
    /* 0x080068BC: 042a     cmp      r2, #4 */
    /* flags = cmp(r2, #4) */
    /* 0x080068BE: 17d0     beq      #0x80068f0 */
    if (r2 == 4) goto L_080068F0;
    /* 0x080068C0: 052a     cmp      r2, #5 */
    /* flags = cmp(r2, #5) */
    /* 0x080068C2: 15d0     beq      #0x80068f0 */
    if (r2 == 5) goto L_080068F0;
    /* 0x080068C4: 062a     cmp      r2, #6 */
    /* flags = cmp(r2, #6) */
    /* 0x080068C6: 13d0     beq      #0x80068f0 */
    if (r2 == 6) goto L_080068F0;
    /* 0x080068C8: 072a     cmp      r2, #7 */
    /* flags = cmp(r2, #7) */
    /* 0x080068CA: 11d0     beq      #0x80068f0 */
    if (r2 == 7) goto L_080068F0;
    /* 0x080068CC: 0268     ldr      r2, [r0] */
    r2 = *(uint32_t *)(r0);
    /* 0x080068CE: 8b68     ldr      r3, [r1, #8] */
    r3 = *(uint32_t *)(r1 + 8);
    /* 0x080068D0: 5669     ldr      r6, [r2, #0x14] */
    r6 = *(uint32_t *)(r2 + 20);
    /* 0x080068D2: 7607     lsls     r6, r6, #0x1d */
    r6 = r6 << 0x1d;
    /* 0x080068D4: 760f     lsrs     r6, r6, #0x1d */
    r6 = r6 >> 0x1d;
    /* 0x080068D6: b342     cmp      r3, r6 */
    /* flags = cmp(r3, r6) */
    /* 0x080068D8: 0ad0     beq      #0x80068f0 */
    if (r3 == r6) goto L_080068F0;
    /* 0x080068DA: 5369     ldr      r3, [r2, #0x14] */
    r3 = *(uint32_t *)(r2 + 20);
    /* 0x080068DC: db08     lsrs     r3, r3, #3 */
    r3 = r3 >> 3;
    /* 0x080068DE: db00     lsls     r3, r3, #3 */
    r3 = r3 << 3;
    /* 0x080068E0: 5361     str      r3, [r2, #0x14] */
    *(uint32_t *)(r2 + 20) = r3;   /* store */
    /* 0x080068E2: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);
    /* 0x080068E4: 4269     ldr      r2, [r0, #0x14] */
    r2 = *(uint32_t *)(r0 + 20);
    /* 0x080068E6: 0b7a     ldrb     r3, [r1, #8] */
    r3 = *(uint8_t *)(r1 + 8);
    /* 0x080068E8: 5b07     lsls     r3, r3, #0x1d */
    r3 = r3 << 0x1d;
    /* 0x080068EA: 5b0f     lsrs     r3, r3, #0x1d */
    r3 = r3 >> 0x1d;
    /* 0x080068EC: 1a43     orrs     r2, r3 */
    r2 = r2 | r3;
    /* 0x080068EE: 4261     str      r2, [r0, #0x14] */
    *(uint32_t *)(r0 + 20) = r2;   /* store */

    L_080068F0:
    /* 0x080068F0: 0868     ldr      r0, [r1] */
    r0 = *(uint32_t *)(r1);
    /* 0x080068F2: 1028     cmp      r0, #0x10 */
    /* flags = cmp(r0, #0x10) */
    /* 0x080068F4: 06d0     beq      #0x8006904 */
    if (r0 == 0x10) goto L_08006904;
    /* 0x080068F6: 1128     cmp      r0, #0x11 */
    /* flags = cmp(r0, #0x11) */
    /* 0x080068F8: 04d0     beq      #0x8006904 */
    if (r0 == 0x11) goto L_08006904;
    /* 0x080068FA: 1228     cmp      r0, #0x12 */
    /* flags = cmp(r0, #0x12) */
    /* 0x080068FC: 02d0     beq      #0x8006904 */
    if (r0 == 0x12) goto L_08006904;
    /* 0x080068FE: 45e0     b        #0x800698c */
    goto L_0800698C;

    L_08006900:
    /* 0x08006900: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08006902: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_08006904:
    /* 0x08006904: 244a     ldr      r2, [pc, #0x90]  ; =ADC.0x300 */
    r2 = *(uint32_t *)0x08006998;   /* = MMIO ADC+0x300 */
    /* 0x08006906: 9368     ldr      r3, [r2, #8] */
    r3 = *(uint32_t *)(r2 + 8);  /* MMIO: ADC.+0x308(r) */
    /* 0x08006908: 1028     cmp      r0, #0x10 */
    /* flags = cmp(r0, #0x10) */
    /* 0x0800690A: 09d0     beq      #0x8006920 */
    if (r0 == 0x10) goto L_08006920;
    /* 0x0800690C: 1128     cmp      r0, #0x11 */
    /* flags = cmp(r0, #0x11) */
    /* 0x0800690E: 0ad0     beq      #0x8006926 */
    if (r0 == 0x11) goto L_08006926;
    /* 0x08006910: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08006912: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;

    L_08006914:
    /* 0x08006914: 0343     orrs     r3, r0 */
    r3 = r3 | r0;
    /* 0x08006916: 9360     str      r3, [r2, #8] */
    *(uint32_t *)(r2 + 8) = r3;   /* store */  /* MMIO: ADC.+0x308(w) */
    /* 0x08006918: 0868     ldr      r0, [r1] */
    r0 = *(uint32_t *)(r1);
    /* 0x0800691A: 1028     cmp      r0, #0x10 */
    /* flags = cmp(r0, #0x10) */
    /* 0x0800691C: 06d0     beq      #0x800692c */
    if (r0 == 0x10) goto L_0800692C;
    /* 0x0800691E: 35e0     b        #0x800698c */
    goto L_0800698C;

    L_08006920:
    /* 0x08006920: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08006922: c005     lsls     r0, r0, #0x17 */
    r0 = r0 << 0x17;
    /* 0x08006924: f6e7     b        #0x8006914 */
    goto L_08006914;   /* loop */

    L_08006926:
    /* 0x08006926: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08006928: 8005     lsls     r0, r0, #0x16 */
    r0 = r0 << 0x16;
    /* 0x0800692A: f3e7     b        #0x8006914 */
    goto L_08006914;   /* loop */

    L_0800692C:
    /* 0x0800692C: 1c48     ldr      r0, [pc, #0x70]  ; =RAM_20000100 */
    r0 = *(uint32_t *)0x080069A0;   /* = RAM 0x20000100 */
    /* 0x0800692E: 1b49     ldr      r1, [pc, #0x6c]  ; =0x000F4240 */
    r1 = *(uint32_t *)0x0800699C;   /* = 0x000F4240 */
    /* 0x08006930: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);
    /* 0x08006932: fcf7ddfb bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x08006936: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x08006938: 4843     muls     r0, r1, r0 */
    r0 = r1 * r0;
    /* 0x0800693A: 00e0     b        #0x800693e */
    goto L_0800693E;

    L_0800693C:
    /* 0x0800693C: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;

    L_0800693E:
    /* 0x0800693E: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08006940: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08006942: fbd1     bne      #0x800693c */
    if (r0 != 0) goto L_0800693C;
    /* 0x08006944: 22e0     b        #0x800698c */
    goto L_0800698C;

    L_08006946:
    /* 0x08006946: 986a     ldr      r0, [r3, #0x28] */
    r0 = *(uint32_t *)(r3 + 40);
    /* 0x08006948: 0e78     ldrb     r6, [r1] */
    r6 = *(uint8_t *)(r1);
    /* 0x0800694A: b240     lsls     r2, r6 */
    r2 = r2 << r6;
    /* 0x0800694C: 9043     bics     r0, r2 */
    r0 = r0 & ~r2;
    /* 0x0800694E: 9862     str      r0, [r3, #0x28] */
    *(uint32_t *)(r3 + 40) = r0;   /* store */
    /* 0x08006950: 0868     ldr      r0, [r1] */
    r0 = *(uint32_t *)(r1);
    /* 0x08006952: 1028     cmp      r0, #0x10 */
    /* flags = cmp(r0, #0x10) */
    /* 0x08006954: 03d0     beq      #0x800695e */
    if (r0 == 0x10) goto L_0800695E;
    /* 0x08006956: 1128     cmp      r0, #0x11 */
    /* flags = cmp(r0, #0x11) */
    /* 0x08006958: 01d0     beq      #0x800695e */
    if (r0 == 0x11) goto L_0800695E;
    /* 0x0800695A: 1228     cmp      r0, #0x12 */
    /* flags = cmp(r0, #0x12) */
    /* 0x0800695C: 16d1     bne      #0x800698c */
    if (r0 != 0x12) goto L_0800698C;

    L_0800695E:
    /* 0x0800695E: 0e49     ldr      r1, [pc, #0x38]  ; =ADC.0x300 */
    r1 = *(uint32_t *)0x08006998;   /* = MMIO ADC+0x300 */
    /* 0x08006960: 8a68     ldr      r2, [r1, #8] */
    r2 = *(uint32_t *)(r1 + 8);  /* MMIO: ADC.+0x308(r) */
    /* 0x08006962: 1028     cmp      r0, #0x10 */
    /* flags = cmp(r0, #0x10) */
    /* 0x08006964: 07d0     beq      #0x8006976 */
    if (r0 == 0x10) goto L_08006976;
    /* 0x08006966: 1128     cmp      r0, #0x11 */
    /* flags = cmp(r0, #0x11) */
    /* 0x08006968: 08d0     beq      #0x800697c */
    if (r0 == 0x11) goto L_0800697C;
    /* 0x0800696A: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800696C: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;

    L_0800696E:
    /* 0x0800696E: 8243     bics     r2, r0 */
    r2 = r2 & ~r0;
    /* 0x08006970: 8a60     str      r2, [r1, #8] */
    *(uint32_t *)(r1 + 8) = r2;   /* store */  /* MMIO: ADC.+0x308(w) */
    /* 0x08006972: 0be0     b        #0x800698c */
    goto L_0800698C;

    L_08006976:
    /* 0x08006976: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08006978: c005     lsls     r0, r0, #0x17 */
    r0 = r0 << 0x17;
    /* 0x0800697A: f8e7     b        #0x800696e */
    goto L_0800696E;   /* loop */

    L_0800697C:
    /* 0x0800697C: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800697E: 8005     lsls     r0, r0, #0x16 */
    r0 = r0 << 0x16;
    /* 0x08006980: f5e7     b        #0x800696e */
    goto L_0800696E;   /* loop */

    L_08006982:
    /* 0x08006982: 416c     ldr      r1, [r0, #0x44] */
    r1 = *(uint32_t *)(r0 + 68);
    /* 0x08006984: 2022     movs     r2, #0x20 */
    r2 = 0x20;
    /* 0x08006986: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x08006988: 4164     str      r1, [r0, #0x44] */
    *(uint32_t *)(r0 + 68) = r1;   /* store */
    /* 0x0800698A: 0125     movs     r5, #1 */
    r5 = 1;

    L_0800698C:
    /* 0x0800698C: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800698E: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x08006990: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08006992: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
