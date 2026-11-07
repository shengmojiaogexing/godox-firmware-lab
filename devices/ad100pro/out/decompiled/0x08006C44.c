/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006C44
 * Name      : FUN_08006C44
 * End       : 0x08006CB4
 * Size      : 112 bytes, 52 instructions decoded in body (functions.json insns=52, blocks=11)
 * Class     : function
 * Callers   : 3  -> 0x080035A8 FUN_080035A8, 0x0800C4E8 FUN_0800C4E8, 0x0800C578 FUN_0800C578
 * Callees   : 3  -> 0x080036E0 FUN_080036E0, 0x080037C2 FUN_080037C2, 0x08006DDE FUN_08006DDE
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006C44(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006C44: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x08006C46: 0646     mov      r6, r0 */
    r6 = r0;
    /* 0x08006C48: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x08006C4A: 4036     adds     r6, #0x40 */
    r6 = r6 + 0x40;
    /* 0x08006C4C: 3078     ldrb     r0, [r6] */
    r0 = *(uint8_t *)(r6);
    /* 0x08006C4E: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08006C50: 07d0     beq      #0x8006c62 */
    if (r0 == 1) goto L_08006C62;
    /* 0x08006C52: 0127     movs     r7, #1 */
    r7 = 1;
    /* 0x08006C54: 3770     strb     r7, [r6] */
    *(uint8_t *)(r6) = r7;   /* store */
    /* 0x08006C56: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08006C58: fcf742fd bl       #0x80036e0 */
    FUN_080036E0();   /* BL -> 0x080036E0 FUN_080036E0 */
    /* 0x08006C5C: 0500     movs     r5, r0 */
    r5 = r0;
    /* 0x08006C5E: 02d0     beq      #0x8006c66 */
    if (r0 == 1) goto L_08006C66;
    /* 0x08006C60: 24e0     b        #0x8006cac */
    goto L_08006CAC;

    L_08006C62:
    /* 0x08006C62: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08006C64: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_08006C66:
    /* 0x08006C66: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x08006C68: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);
    /* 0x08006C6A: 4908     lsrs     r1, r1, #1 */
    r1 = r1 >> 1;
    /* 0x08006C6C: 4900     lsls     r1, r1, #1 */
    r1 = r1 << 1;
    /* 0x08006C6E: c160     str      r1, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r1;   /* store */
    /* 0x08006C70: e06b     ldr      r0, [r4, #0x3c] */
    r0 = *(uint32_t *)(r4 + 60);
    /* 0x08006C72: 00f0b4f8 bl       #0x8006dde */
    FUN_08006DDE();   /* BL -> 0x08006DDE FUN_08006DDE */
    /* 0x08006C76: 0500     movs     r5, r0 */
    r5 = r0;
    /* 0x08006C78: 03d0     beq      #0x8006c82 */
    if (r0 == 1) goto L_08006C82;
    /* 0x08006C7A: 606c     ldr      r0, [r4, #0x44] */
    r0 = *(uint32_t *)(r4 + 68);
    /* 0x08006C7C: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x08006C7E: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08006C80: 6064     str      r0, [r4, #0x44] */
    *(uint32_t *)(r4 + 68) = r0;   /* store */

    L_08006C82:
    /* 0x08006C82: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);
    /* 0x08006C84: 4168     ldr      r1, [r0, #4] */
    r1 = *(uint32_t *)(r0 + 4);
    /* 0x08006C86: 1022     movs     r2, #0x10 */
    r2 = 0x10;
    /* 0x08006C88: 9143     bics     r1, r2 */
    r1 = r1 & ~r2;
    /* 0x08006C8A: 4160     str      r1, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r1;   /* store */
    /* 0x08006C8C: 002d     cmp      r5, #0 */
    /* flags = cmp(r5, #0) */
    /* 0x08006C8E: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08006C90: 02d0     beq      #0x8006c98 */
    if (r5 == 0) goto L_08006C98;
    /* 0x08006C92: fcf796fd bl       #0x80037c2 */
    FUN_080037C2();   /* BL -> 0x080037C2 FUN_080037C2 */
    /* 0x08006C96: 09e0     b        #0x8006cac */
    goto L_08006CAC;

    L_08006C98:
    /* 0x08006C98: fcf793fd bl       #0x80037c2 */
    FUN_080037C2();   /* BL -> 0x080037C2 FUN_080037C2 */
    /* 0x08006C9C: 0500     movs     r5, r0 */
    r5 = r0;
    /* 0x08006C9E: 05d1     bne      #0x8006cac */
    if (r5 != 0) goto L_08006CAC;
    /* 0x08006CA0: 606c     ldr      r0, [r4, #0x44] */
    r0 = *(uint32_t *)(r4 + 68);
    /* 0x08006CA2: ff21     movs     r1, #0xff */
    r1 = 0xff;
    /* 0x08006CA4: 0131     adds     r1, #1 */
    r1 = r1 + 1;
    /* 0x08006CA6: 8843     bics     r0, r1 */
    r0 = r0 & ~r1;
    /* 0x08006CA8: 3843     orrs     r0, r7 */
    r0 = r0 | r7;
    /* 0x08006CAA: 6064     str      r0, [r4, #0x44] */
    *(uint32_t *)(r4 + 68) = r0;   /* store */

    L_08006CAC:
    /* 0x08006CAC: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006CAE: 3070     strb     r0, [r6] */
    *(uint8_t *)(r6) = r0;   /* store */
    /* 0x08006CB0: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08006CB2: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
