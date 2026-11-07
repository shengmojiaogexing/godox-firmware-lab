/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080034A4
 * Name      : FUN_080034A4
 * End       : 0x08003548
 * Size      : 164 bytes, 76 instructions decoded in body (functions.json insns=76, blocks=14)
 * Class     : function
 * Callers   : 3  -> 0x080031E8 FUN_080031E8, 0x080032CC FUN_080032CC, 0x0800339C FUN_0800339C
 * Callees   : 4  -> 0x08003144 FUN_08003144, 0x08003444 FUN_08003444, 0x0800348A FUN_0800348A, 0x0800C8C4 FUN_0800C8C4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080034A4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080034A4: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x080034A6: 1fb4     push     {r0, r1, r2, r3, r4} */
    push({r0, r1, r2, r3, r4});
    /* 0x080034A8: 82b0     sub      sp, #8 */
    sp = sp - 8;
    /* 0x080034AA: 0546     mov      r5, r0 */
    r5 = r0;
    /* 0x080034AC: 0c00     movs     r4, r1 */
    r4 = r1;
    /* 0x080034AE: 03d0     beq      #0x80034b8 */
    if (/* beq */) goto L_080034B8;
    /* 0x080034B0: 0846     mov      r0, r1 */
    r0 = r1;
    /* 0x080034B2: 09f007fa bl       #0x800c8c4 */
    FUN_0800C8C4();   /* BL -> 0x0800C8C4 FUN_0800C8C4 */
    /* 0x080034B6: 02e0     b        #0x80034be */
    goto L_080034BE;

    L_080034B8:
    /* 0x080034B8: 09f004fa bl       #0x800c8c4 */
    FUN_0800C8C4();   /* BL -> 0x0800C8C4 FUN_0800C8C4 */
    /* 0x080034BC: 2030     adds     r0, #0x20 */
    r0 = r0 + 0x20;

    L_080034BE:
    /* 0x080034BE: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x080034C0: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x080034C2: 2146     mov      r1, r4 */
    r1 = r4;
    /* 0x080034C4: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x080034C6: fff7bdff bl       #0x8003444 */
    FUN_08003444();   /* BL -> 0x08003444 FUN_08003444 */
    /* 0x080034CA: 049b     ldr      r3, [sp, #0x10] */
    r3 = *(uint32_t *)(sp + 16);
    /* 0x080034CC: 059a     ldr      r2, [sp, #0x14] */
    r2 = *(uint32_t *)(sp + 20);
    /* 0x080034CE: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x080034D0: 0f46     mov      r7, r1 */
    r7 = r1;
    /* 0x080034D2: 1843     orrs     r0, r3 */
    r0 = r0 | r3;
    /* 0x080034D4: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x080034D6: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x080034D8: 14d0     beq      #0x8003504 */
    if (/* beq */) goto L_08003504;
    /* 0x080034DA: 1846     mov      r0, r3 */
    r0 = r3;
    /* 0x080034DC: 1043     orrs     r0, r2 */
    r0 = r0 | r2;
    /* 0x080034DE: 1bd0     beq      #0x8003518 */
    if (/* beq */) goto L_08003518;
    /* 0x080034E0: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x080034E2: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x080034E4: 0a1a     subs     r2, r1, r0 */
    r2 = r1 - r0;
    /* 0x080034E6: 1846     mov      r0, r3 */
    r0 = r3;
    /* 0x080034E8: 0599     ldr      r1, [sp, #0x14] */
    r1 = *(uint32_t *)(sp + 20);
    /* 0x080034EA: fff72bfe bl       #0x8003144 */
    FUN_08003144();   /* BL -> 0x08003144 FUN_08003144 */
    /* 0x080034EE: 0546     mov      r5, r0 */
    r5 = r0;
    /* 0x080034F0: 0e46     mov      r6, r1 */
    r6 = r1;
    /* 0x080034F2: 009a     ldr      r2, [sp] */
    r2 = *(uint32_t *)(sp);
    /* 0x080034F4: 0599     ldr      r1, [sp, #0x14] */
    r1 = *(uint32_t *)(sp + 20);
    /* 0x080034F6: 0498     ldr      r0, [sp, #0x10] */
    r0 = *(uint32_t *)(sp + 16);
    /* 0x080034F8: fff7a4ff bl       #0x8003444 */
    FUN_08003444();   /* BL -> 0x08003444 FUN_08003444 */
    /* 0x080034FC: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x080034FE: 05d0     beq      #0x800350c */
    if (/* beq */) goto L_0800350C;
    /* 0x08003500: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08003502: 04e0     b        #0x800350e */
    goto L_0800350E;

    L_08003504:
    /* 0x08003504: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08003506: 3946     mov      r1, r7 */
    r1 = r7;

    L_08003508:
    /* 0x08003508: 07b0     add      sp, #0x1c */
    sp = sp + 0x1c;
    /* 0x0800350A: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */

    L_0800350C:
    /* 0x0800350C: 0020     movs     r0, #0 */
    r0 = 0;

    L_0800350E:
    /* 0x0800350E: c117     asrs     r1, r0, #0x1f */
    r1 = r0 >> 0x1f;
    /* 0x08003510: 0543     orrs     r5, r0 */
    r5 = r5 | r0;
    /* 0x08003512: 0e43     orrs     r6, r1 */
    r6 = r6 | r1;
    /* 0x08003514: 2c43     orrs     r4, r5 */
    r4 = r4 | r5;
    /* 0x08003516: 3743     orrs     r7, r6 */
    r7 = r7 | r6;

    L_08003518:
    /* 0x08003518: 0099     ldr      r1, [sp] */
    r1 = *(uint32_t *)(sp);
    /* 0x0800351A: 0e98     ldr      r0, [sp, #0x38] */
    r0 = *(uint32_t *)(sp + 56);
    /* 0x0800351C: 6305     lsls     r3, r4, #0x15 */
    r3 = r4 << 0x15;
    /* 0x0800351E: 401a     subs     r0, r0, r1 */
    r0 = r0 - r1;
    /* 0x08003520: 7905     lsls     r1, r7, #0x15 */
    r1 = r7 << 0x15;
    /* 0x08003522: e40a     lsrs     r4, r4, #0xb */
    r4 = r4 >> 0xb;
    /* 0x08003524: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08003526: 0c43     orrs     r4, r1 */
    r4 = r4 | r1;
    /* 0x08003528: fd0a     lsrs     r5, r7, #0xb */
    r5 = r7 >> 0xb;
    /* 0x0800352A: 0a30     adds     r0, #0xa */
    r0 = r0 + 0xa;
    /* 0x0800352C: 02d5     bpl      #0x8003534 */
    if (/* bpl */) goto L_08003534;   /* result non-negative */
    /* 0x0800352E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003530: 0146     mov      r1, r0 */
    r1 = r0;
    /* 0x08003532: e9e7     b        #0x8003508 */
    goto L_08003508;   /* loop */

    L_08003534:
    /* 0x08003534: 0105     lsls     r1, r0, #0x14 */
    r1 = r0 << 0x14;
    /* 0x08003536: 1019     adds     r0, r2, r4 */
    r0 = r2 + r4;
    /* 0x08003538: 6941     adcs     r1, r5 */
    r1 = r1 + r5;
    /* 0x0800353A: 0c9c     ldr      r4, [sp, #0x30] */
    r4 = *(uint32_t *)(sp + 48);
    /* 0x0800353C: 0d9d     ldr      r5, [sp, #0x34] */
    r5 = *(uint32_t *)(sp + 52);
    /* 0x0800353E: 0019     adds     r0, r0, r4 */
    r0 = r0 + r4;
    /* 0x08003540: 6941     adcs     r1, r5 */
    r1 = r1 + r5;
    /* 0x08003542: fff7a2ff bl       #0x800348a */
    FUN_0800348A();   /* BL -> 0x0800348A FUN_0800348A */
    /* 0x08003546: dfe7     b        #0x8003508 */
    goto L_08003508;   /* loop */
}
