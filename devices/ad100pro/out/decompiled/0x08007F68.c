/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007F68
 * Name      : FUN_08007F68
 * End       : 0x0800802A
 * Size      : 194 bytes, 90 instructions decoded in body (functions.json insns=90, blocks=12)
 * Class     : function
 * Callers   : 2  -> 0x080095F8 FUN_080095F8, 0x0800963C FUN_0800963C
 * Callees   : 4  -> 0x08008114 FUN_08008114, 0x0800AC98 FUN_0800AC98, 0x0800ACA8 FUN_0800ACA8, 0x0800ACBE FUN_0800ACBE
 * MMIO      : RCC.+0x1D, RCC.APB1ENR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007F68(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007F68: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x08007F6A: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x08007F6C: 007f     ldrb     r0, [r0, #0x1c] */
    r0 = *(uint8_t *)(r0 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x08007F6E: 0b46     mov      r3, r1 */
    r3 = r1;
    /* 0x08007F70: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08007F72: 15d0     beq      #0x8007fa0 */
    if (r0 == 1) goto L_08007FA0;
    /* 0x08007F74: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08007F76: 2077     strb     r0, [r4, #0x1c] */
    *(uint8_t *)(r4 + 28) = r0;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x08007F78: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08007F7A: 6077     strb     r0, [r4, #0x1d] */
    *(uint8_t *)(r4 + 29) = r0;   /* store */  /* MMIO: RCC.+0x1D(w) */
    /* 0x08007F7C: 5878     ldrb     r0, [r3, #1] */
    r0 = *(uint8_t *)(r3 + 1);
    /* 0x08007F7E: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x08007F80: 10d0     beq      #0x8007fa4 */
    if (r2 == 0) goto L_08007FA4;
    /* 0x08007F82: 02f089fe bl       #0x800ac98 */
    FUN_0800AC98();   /* BL -> 0x0800AC98 FUN_0800AC98 */
    /* 0x08007F86: 9878     ldrb     r0, [r3, #2] */
    r0 = *(uint8_t *)(r3 + 2);
    /* 0x08007F88: 02f086fe bl       #0x800ac98 */
    FUN_0800AC98();   /* BL -> 0x0800AC98 FUN_0800AC98 */
    /* 0x08007F8C: d878     ldrb     r0, [r3, #3] */
    r0 = *(uint8_t *)(r3 + 3);
    /* 0x08007F8E: 9d78     ldrb     r5, [r3, #2] */
    r5 = *(uint8_t *)(r3 + 2);
    /* 0x08007F90: 0004     lsls     r0, r0, #0x10 */
    r0 = r0 << 0x10;
    /* 0x08007F92: 0543     orrs     r5, r0 */
    r5 = r5 | r0;
    /* 0x08007F94: 5878     ldrb     r0, [r3, #1] */
    r0 = *(uint8_t *)(r3 + 1);
    /* 0x08007F96: 1978     ldrb     r1, [r3] */
    r1 = *(uint8_t *)(r3);
    /* 0x08007F98: 0002     lsls     r0, r0, #8 */
    r0 = r0 << 8;
    /* 0x08007F9A: 4903     lsls     r1, r1, #0xd */
    r1 = r1 << 0xd;
    /* 0x08007F9C: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08007F9E: 16e0     b        #0x8007fce */
    goto L_08007FCE;

    L_08007FA0:
    /* 0x08007FA0: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08007FA2: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_08007FA4:
    /* 0x08007FA4: c106     lsls     r1, r0, #0x1b */
    r1 = r0 << 0x1b;
    /* 0x08007FA6: 03d5     bpl      #0x8007fb0 */
    if (r2 >= 0) goto L_08007FB0;   /* result non-negative */
    /* 0x08007FA8: ef21     movs     r1, #0xef */
    r1 = 0xef;
    /* 0x08007FAA: 0840     ands     r0, r1 */
    r0 = r0 & r1;
    /* 0x08007FAC: 0a30     adds     r0, #0xa */
    r0 = r0 + 0xa;
    /* 0x08007FAE: 5870     strb     r0, [r3, #1] */
    *(uint8_t *)(r3 + 1) = r0;   /* store */

    L_08007FB0:
    /* 0x08007FB0: d878     ldrb     r0, [r3, #3] */
    r0 = *(uint8_t *)(r3 + 3);
    /* 0x08007FB2: 02f079fe bl       #0x800aca8 */
    FUN_0800ACA8();   /* BL -> 0x0800ACA8 FUN_0800ACA8 */
    /* 0x08007FB6: 0504     lsls     r5, r0, #0x10 */
    r5 = r0 << 0x10;
    /* 0x08007FB8: 5878     ldrb     r0, [r3, #1] */
    r0 = *(uint8_t *)(r3 + 1);
    /* 0x08007FBA: 02f075fe bl       #0x800aca8 */
    FUN_0800ACA8();   /* BL -> 0x0800ACA8 FUN_0800ACA8 */
    /* 0x08007FBE: 0002     lsls     r0, r0, #8 */
    r0 = r0 << 8;
    /* 0x08007FC0: 0543     orrs     r5, r0 */
    r5 = r5 | r0;
    /* 0x08007FC2: 9878     ldrb     r0, [r3, #2] */
    r0 = *(uint8_t *)(r3 + 2);
    /* 0x08007FC4: 02f070fe bl       #0x800aca8 */
    FUN_0800ACA8();   /* BL -> 0x0800ACA8 FUN_0800ACA8 */
    /* 0x08007FC8: 0543     orrs     r5, r0 */
    r5 = r5 | r0;
    /* 0x08007FCA: 1878     ldrb     r0, [r3] */
    r0 = *(uint8_t *)(r3);
    /* 0x08007FCC: 4003     lsls     r0, r0, #0xd */
    r0 = r0 << 0xd;

    L_08007FCE:
    /* 0x08007FCE: 0543     orrs     r5, r0 */
    r5 = r5 | r0;
    /* 0x08007FD0: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007FD2: ca20     movs     r0, #0xca */
    r0 = 0xca;
    /* 0x08007FD4: 4862     str      r0, [r1, #0x24] */
    *(uint32_t *)(r1 + 36) = r0;   /* store */  /* MMIO: RTC.WPR(w) */
    /* 0x08007FD6: 2168     ldr      r1, [r4] */
    r1 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007FD8: 5320     movs     r0, #0x53 */
    r0 = 0x53;
    /* 0x08007FDA: 4862     str      r0, [r1, #0x24] */
    *(uint32_t *)(r1 + 36) = r0;   /* store */  /* MMIO: RTC.WPR(w) */
    /* 0x08007FDC: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08007FDE: 02f06efe bl       #0x800acbe */
    FUN_0800ACBE();   /* BL -> 0x0800ACBE FUN_0800ACBE */
    /* 0x08007FE2: 0026     movs     r6, #0 */
    r6 = 0;
    /* 0x08007FE4: ff27     movs     r7, #0xff */
    r7 = 0xff;
    /* 0x08007FE6: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08007FE8: 11d1     bne      #0x800800e */
    if (r0 != 0) goto L_0800800E;
    /* 0x08007FEA: 1048     ldr      r0, [pc, #0x40]  ; =0x00FFFF3F */
    r0 = *(uint32_t *)0x0800802C;   /* = 0x00FFFF3F */
    /* 0x08007FEC: 0540     ands     r5, r0 */
    r5 = r5 & r0;
    /* 0x08007FEE: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007FF0: 4560     str      r5, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r5;   /* store */
    /* 0x08007FF2: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007FF4: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);
    /* 0x08007FF6: 8022     movs     r2, #0x80 */
    r2 = 0x80;
    /* 0x08007FF8: 9143     bics     r1, r2 */
    r1 = r1 & ~r2;
    /* 0x08007FFA: c160     str      r1, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r1;   /* store */
    /* 0x08007FFC: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007FFE: 8068     ldr      r0, [r0, #8] */
    r0 = *(uint32_t *)(r0 + 8);
    /* 0x08008000: 8006     lsls     r0, r0, #0x1a */
    r0 = r0 << 0x1a;
    /* 0x08008002: 0bd4     bmi      #0x800801c */
    if (r0 < 0) goto L_0800801C;   /* result negative */
    /* 0x08008004: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08008006: 00f085f8 bl       #0x8008114 */
    FUN_08008114();   /* BL -> 0x08008114 FUN_08008114 */
    /* 0x0800800A: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800800C: 06d0     beq      #0x800801c */
    if (r0 == 0) goto L_0800801C;

    L_0800800E:
    /* 0x0800800E: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x08008010: 4762     str      r7, [r0, #0x24] */
    *(uint32_t *)(r0 + 36) = r7;   /* store */
    /* 0x08008012: 0420     movs     r0, #4 */
    r0 = 4;
    /* 0x08008014: 6077     strb     r0, [r4, #0x1d] */
    *(uint8_t *)(r4 + 29) = r0;   /* store */  /* MMIO: RCC.+0x1D(w) */
    /* 0x08008016: 2677     strb     r6, [r4, #0x1c] */
    *(uint8_t *)(r4 + 28) = r6;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x08008018: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800801A: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_0800801C:
    /* 0x0800801C: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
    /* 0x0800801E: 4762     str      r7, [r0, #0x24] */
    *(uint32_t *)(r0 + 36) = r7;   /* store */
    /* 0x08008020: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08008022: 6077     strb     r0, [r4, #0x1d] */
    *(uint8_t *)(r4 + 29) = r0;   /* store */  /* MMIO: RCC.+0x1D(w) */
    /* 0x08008024: 2677     strb     r6, [r4, #0x1c] */
    *(uint8_t *)(r4 + 28) = r6;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x08008026: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08008028: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
