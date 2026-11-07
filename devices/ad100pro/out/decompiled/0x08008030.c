/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008030
 * Name      : FUN_08008030
 * End       : 0x0800810E
 * Size      : 222 bytes, 105 instructions decoded in body (functions.json insns=105, blocks=15)
 * Class     : function
 * Callers   : 1  -> 0x080095F8 FUN_080095F8
 * Callees   : 4  -> 0x08008114 FUN_08008114, 0x0800AC98 FUN_0800AC98, 0x0800ACA8 FUN_0800ACA8, 0x0800ACBE FUN_0800ACBE
 * MMIO      : RCC.+0x1D, RCC.APB1ENR, RCC.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08008030(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08008030: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x08008032: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x08008034: 007f     ldrb     r0, [r0, #0x1c] */
    r0 = *(uint8_t *)(r0 + 28);
    /* 0x08008036: 0d46     mov      r5, r1 */
    r5 = r1;
    /* 0x08008038: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800803A: 0ed0     beq      #0x800805a */
    if (r0 == 1) goto L_0800805A;
    /* 0x0800803C: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800803E: 2077     strb     r0, [r4, #0x1c] */
    *(uint8_t *)(r4 + 28) = r0;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x08008040: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08008042: 6077     strb     r0, [r4, #0x1d] */
    *(uint8_t *)(r4 + 29) = r0;   /* store */  /* MMIO: RCC.+0x1D(w) */
    /* 0x08008044: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08008046: 0027     movs     r7, #0 */
    r7 = 0;
    /* 0x08008048: 8068     ldr      r0, [r0, #8] */
    r0 = *(uint32_t *)(r0 + 8);
    /* 0x0800804A: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x0800804C: 07d0     beq      #0x800805e */
    if (r2 == 0) goto L_0800805E;
    /* 0x0800804E: 4006     lsls     r0, r0, #0x19 */
    r0 = r0 << 0x19;
    /* 0x08008050: 18d5     bpl      #0x8008084 */
    if (r2 >= 0) goto L_08008084;   /* result non-negative */
    /* 0x08008052: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x08008054: 02f020fe bl       #0x800ac98 */
    FUN_0800AC98();   /* BL -> 0x0800AC98 FUN_0800AC98 */
    /* 0x08008058: 15e0     b        #0x8008086 */
    goto L_08008086;

    L_0800805A:
    /* 0x0800805A: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x0800805C: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_0800805E:
    /* 0x0800805E: 4006     lsls     r0, r0, #0x19 */
    r0 = r0 << 0x19;
    /* 0x08008060: 00d4     bmi      #0x8008064 */
    if (r2 < 0) goto L_08008064;   /* result negative */
    /* 0x08008062: ef70     strb     r7, [r5, #3] */
    *(uint8_t *)(r5 + 3) = r7;   /* store */  /* MMIO: RCC.+0x3(w) */

    L_08008064:
    /* 0x08008064: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x08008066: 02f01ffe bl       #0x800aca8 */
    FUN_0800ACA8();   /* BL -> 0x0800ACA8 FUN_0800ACA8 */
    /* 0x0800806A: 0604     lsls     r6, r0, #0x10 */
    r6 = r0 << 0x10;
    /* 0x0800806C: 6878     ldrb     r0, [r5, #1] */
    r0 = *(uint8_t *)(r5 + 1);  /* MMIO: RCC.+0x1(r) */
    /* 0x0800806E: 02f01bfe bl       #0x800aca8 */
    FUN_0800ACA8();   /* BL -> 0x0800ACA8 FUN_0800ACA8 */
    /* 0x08008072: 0002     lsls     r0, r0, #8 */
    r0 = r0 << 8;
    /* 0x08008074: 0643     orrs     r6, r0 */
    r6 = r6 | r0;
    /* 0x08008076: a878     ldrb     r0, [r5, #2] */
    r0 = *(uint8_t *)(r5 + 2);  /* MMIO: RCC.+0x2(r) */
    /* 0x08008078: 02f016fe bl       #0x800aca8 */
    FUN_0800ACA8();   /* BL -> 0x0800ACA8 FUN_0800ACA8 */
    /* 0x0800807C: 0643     orrs     r6, r0 */
    r6 = r6 | r0;
    /* 0x0800807E: e878     ldrb     r0, [r5, #3] */
    r0 = *(uint8_t *)(r5 + 3);  /* MMIO: RCC.+0x3(r) */
    /* 0x08008080: 0004     lsls     r0, r0, #0x10 */
    r0 = r0 << 0x10;
    /* 0x08008082: 09e0     b        #0x8008098 */
    goto L_08008098;

    L_08008084:
    /* 0x08008084: ef70     strb     r7, [r5, #3] */
    *(uint8_t *)(r5 + 3) = r7;   /* store */  /* MMIO: RCC.+0x3(w) */

    L_08008086:
    /* 0x08008086: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);  /* MMIO: RCC.CR(r) */
    /* 0x08008088: ae78     ldrb     r6, [r5, #2] */
    r6 = *(uint8_t *)(r5 + 2);  /* MMIO: RCC.+0x2(r) */
    /* 0x0800808A: 0004     lsls     r0, r0, #0x10 */
    r0 = r0 << 0x10;
    /* 0x0800808C: 0643     orrs     r6, r0 */
    r6 = r6 | r0;
    /* 0x0800808E: 6878     ldrb     r0, [r5, #1] */
    r0 = *(uint8_t *)(r5 + 1);  /* MMIO: RCC.+0x1(r) */
    /* 0x08008090: e978     ldrb     r1, [r5, #3] */
    r1 = *(uint8_t *)(r5 + 3);  /* MMIO: RCC.+0x3(r) */
    /* 0x08008092: 0002     lsls     r0, r0, #8 */
    r0 = r0 << 8;
    /* 0x08008094: 0904     lsls     r1, r1, #0x10 */
    r1 = r1 << 0x10;
    /* 0x08008096: 0843     orrs     r0, r1 */
    r0 = r0 | r1;

    L_08008098:
    /* 0x08008098: 0643     orrs     r6, r0 */
    r6 = r6 | r0;
    /* 0x0800809A: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x0800809C: ca20     movs     r0, #0xca */
    r0 = 0xca;
    /* 0x0800809E: 4862     str      r0, [r1, #0x24] */
    *(uint32_t *)(r1 + 36) = r0;   /* store */  /* MMIO: RTC.WPR(w) */
    /* 0x080080A0: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x080080A2: 5320     movs     r0, #0x53 */
    r0 = 0x53;
    /* 0x080080A4: 4862     str      r0, [r1, #0x24] */
    *(uint32_t *)(r1 + 36) = r0;   /* store */  /* MMIO: RTC.WPR(w) */
    /* 0x080080A6: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080080A8: 02f009fe bl       #0x800acbe */
    FUN_0800ACBE();   /* BL -> 0x0800ACBE FUN_0800ACBE */
    /* 0x080080AC: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080080AE: 1ed1     bne      #0x80080ee */
    if (r0 != 0) goto L_080080EE;
    /* 0x080080B0: 1748     ldr      r0, [pc, #0x5c]  ; =0x007F7F7F */
    r0 = *(uint32_t *)0x08008110;   /* = 0x007F7F7F */
    /* 0x080080B2: 0640     ands     r6, r0 */
    r6 = r6 & r0;
    /* 0x080080B4: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x080080B6: 0660     str      r6, [r0] */
    *(uint32_t *)(r0) = r6;   /* store */
    /* 0x080080B8: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x080080BA: 8168     ldr      r1, [r0, #8] */
    r1 = *(uint32_t *)(r0 + 8);
    /* 0x080080BC: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x080080BE: 9204     lsls     r2, r2, #0x12 */
    r2 = r2 << 0x12;
    /* 0x080080C0: 9143     bics     r1, r2 */
    r1 = r1 & ~r2;
    /* 0x080080C2: 8160     str      r1, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r1;   /* store */
    /* 0x080080C4: 2969     ldr      r1, [r5, #0x10] */
    r1 = *(uint32_t *)(r5 + 16);  /* MMIO: RCC.APB1RSTR(r) */
    /* 0x080080C6: e868     ldr      r0, [r5, #0xc] */
    r0 = *(uint32_t *)(r5 + 12);  /* MMIO: RCC.APB2RSTR(r) */
    /* 0x080080C8: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x080080CA: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x080080CC: 8a68     ldr      r2, [r1, #8] */
    r2 = *(uint32_t *)(r1 + 8);  /* MMIO: RTC.CR(r) */
    /* 0x080080CE: 1043     orrs     r0, r2 */
    r0 = r0 | r2;
    /* 0x080080D0: 8860     str      r0, [r1, #8] */
    *(uint32_t *)(r1 + 8) = r0;   /* store */  /* MMIO: RTC.CR(w) */
    /* 0x080080D2: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x080080D4: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);
    /* 0x080080D6: 8022     movs     r2, #0x80 */
    r2 = 0x80;
    /* 0x080080D8: 9143     bics     r1, r2 */
    r1 = r1 & ~r2;
    /* 0x080080DA: c160     str      r1, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r1;   /* store */
    /* 0x080080DC: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x080080DE: 8068     ldr      r0, [r0, #8] */
    r0 = *(uint32_t *)(r0 + 8);
    /* 0x080080E0: 8006     lsls     r0, r0, #0x1a */
    r0 = r0 << 0x1a;
    /* 0x080080E2: 0cd4     bmi      #0x80080fe */
    if (r0 < 0) goto L_080080FE;   /* result negative */
    /* 0x080080E4: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080080E6: 00f015f8 bl       #0x8008114 */
    FUN_08008114();   /* BL -> 0x08008114 FUN_08008114 */
    /* 0x080080EA: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080080EC: 07d0     beq      #0x80080fe */
    if (r0 == 0) goto L_080080FE;

    L_080080EE:
    /* 0x080080EE: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x080080F0: ff20     movs     r0, #0xff */
    r0 = 0xff;
    /* 0x080080F2: 4862     str      r0, [r1, #0x24] */
    *(uint32_t *)(r1 + 36) = r0;   /* store */  /* MMIO: RTC.WPR(w) */
    /* 0x080080F4: 0420     movs     r0, #4 */
    r0 = 4;
    /* 0x080080F6: 6077     strb     r0, [r4, #0x1d] */
    *(uint8_t *)(r4 + 29) = r0;   /* store */  /* MMIO: RCC.+0x1D(w) */
    /* 0x080080F8: 2777     strb     r7, [r4, #0x1c] */
    *(uint8_t *)(r4 + 28) = r7;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x080080FA: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x080080FC: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_080080FE:
    /* 0x080080FE: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08008100: ff20     movs     r0, #0xff */
    r0 = 0xff;
    /* 0x08008102: 4862     str      r0, [r1, #0x24] */
    *(uint32_t *)(r1 + 36) = r0;   /* store */  /* MMIO: RTC.WPR(w) */
    /* 0x08008104: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08008106: 6077     strb     r0, [r4, #0x1d] */
    *(uint8_t *)(r4 + 29) = r0;   /* store */  /* MMIO: RCC.+0x1D(w) */
    /* 0x08008108: 2777     strb     r7, [r4, #0x1c] */
    *(uint8_t *)(r4 + 28) = r7;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x0800810A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800810C: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
