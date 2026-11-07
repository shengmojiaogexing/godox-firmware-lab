/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A230
 * Name      : FUN_0800A230
 * End       : 0x0800A2B0
 * Size      : 128 bytes, 59 instructions decoded in body (functions.json insns=59, blocks=13)
 * Class     : function
 * Callers   : 2  -> 0x0800CF74 main, 0x0800D014 FUN_0800D014
 * Callees   : 3  -> 0x08007426 FUN_08007426, 0x0800A034 FUN_0800A034, 0x0800A140 FUN_0800A140
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A230(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A230: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800A232: 1f4e     ldr      r6, [pc, #0x7c]  ; =GPIOC.MODER */
    r6 = *(uint32_t *)0x0800A2B0;   /* = MMIO GPIOC+0x00 */
    /* 0x0800A234: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800A236: 0821     movs     r1, #8 */
    r1 = 8;
    /* 0x0800A238: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x0800A23A: fdf7f4f8 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A23E: 1d48     ldr      r0, [pc, #0x74]  ; =RAM_200001B2 */
    r0 = *(uint32_t *)0x0800A2B4;   /* = RAM 0x200001B2 */
    /* 0x0800A240: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800A242: 1a28     cmp      r0, #0x1a */
    /* flags = cmp(r0, #0x1a) */
    /* 0x0800A244: 30d0     beq      #0x800a2a8 */
    if (r0 == 0x1a) goto L_0800A2A8;
    /* 0x0800A246: 1c48     ldr      r0, [pc, #0x70]  ; =RAM_200001BC */
    r0 = *(uint32_t *)0x0800A2B8;   /* = RAM 0x200001BC */
    /* 0x0800A248: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800A24A: 1a28     cmp      r0, #0x1a */
    /* flags = cmp(r0, #0x1a) */
    /* 0x0800A24C: 2cd0     beq      #0x800a2a8 */
    if (r0 == 0x1a) goto L_0800A2A8;
    /* 0x0800A24E: 1b4c     ldr      r4, [pc, #0x6c]  ; =RAM_2000021C */
    r4 = *(uint32_t *)0x0800A2BC;   /* = RAM 0x2000021C */
    /* 0x0800A250: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x0800A252: 607a     ldrb     r0, [r4, #9] */
    r0 = *(uint8_t *)(r4 + 9);
    /* 0x0800A254: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A256: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A258: 6072     strb     r0, [r4, #9] */
    *(uint8_t *)(r4 + 9) = r0;   /* store */
    /* 0x0800A25A: 1428     cmp      r0, #0x14 */
    /* flags = cmp(r0, #0x14) */
    /* 0x0800A25C: 04d9     bls      #0x800a268 */
    if (r0 <= 0x14) goto L_0800A268;   /* unsigned */
    /* 0x0800A25E: 6572     strb     r5, [r4, #9] */
    *(uint8_t *)(r4 + 9) = r5;   /* store */
    /* 0x0800A260: fff76eff bl       #0x800a140 */
    FUN_0800A140();   /* BL -> 0x0800A140 FUN_0800A140 */
    /* 0x0800A264: fff7e6fe bl       #0x800a034 */
    FUN_0800A034();   /* BL -> 0x0800A034 FUN_0800A034 */

    L_0800A268:
    /* 0x0800A268: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800A26A: 1549     ldr      r1, [pc, #0x54]  ; =RAM_2000016A */
    r1 = *(uint32_t *)0x0800A2C0;   /* = RAM 0x2000016A */
    /* 0x0800A26C: 6378     ldrb     r3, [r4, #1] */
    r3 = *(uint8_t *)(r4 + 1);
    /* 0x0800A26E: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x0800A270: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800A272: 1a43     orrs     r2, r3 */
    r2 = r2 | r3;
    /* 0x0800A274: 0a43     orrs     r2, r1 */
    r2 = r2 | r1;
    /* 0x0800A276: 1349     ldr      r1, [pc, #0x4c]  ; =RAM_20000140 */
    r1 = *(uint32_t *)0x0800A2C4;   /* = RAM 0x20000140 */
    /* 0x0800A278: 0988     ldrh     r1, [r1] */
    r1 = *(uint16_t *)(r1);
    /* 0x0800A27A: 0a43     orrs     r2, r1 */
    r2 = r2 | r1;
    /* 0x0800A27C: 0ed0     beq      #0x800a29c */
    if (r0 == 0x14) goto L_0800A29C;
    /* 0x0800A27E: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x0800A280: 05d0     beq      #0x800a28e */
    if (r0 == 2) goto L_0800A28E;
    /* 0x0800A282: 022b     cmp      r3, #2 */
    /* flags = cmp(r3, #2) */
    /* 0x0800A284: 03d0     beq      #0x800a28e */
    if (r3 == 2) goto L_0800A28E;
    /* 0x0800A286: 4b20     movs     r0, #0x4b */
    r0 = 0x4b;
    /* 0x0800A288: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x0800A28A: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x0800A28C: 0dd9     bls      #0x800a2aa */
    if (r1 <= r0) goto L_0800A2AA;   /* unsigned */

    L_0800A28E:
    /* 0x0800A28E: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800A290: 1021     movs     r1, #0x10 */
    r1 = 0x10;
    /* 0x0800A292: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x0800A294: fdf7c7f8 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A298: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x0800A29A: 07e0     b        #0x800a2ac */
    goto L_0800A2AC;

    L_0800A29C:
    /* 0x0800A29C: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800A29E: 1021     movs     r1, #0x10 */
    r1 = 0x10;
    /* 0x0800A2A0: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x0800A2A2: fdf7c0f8 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A2A6: a570     strb     r5, [r4, #2] */
    *(uint8_t *)(r4 + 2) = r5;   /* store */

    L_0800A2A8:
    /* 0x0800A2A8: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_0800A2AA:
    /* 0x0800A2AA: 0120     movs     r0, #1 */
    r0 = 1;

    L_0800A2AC:
    /* 0x0800A2AC: a070     strb     r0, [r4, #2] */
    *(uint8_t *)(r4 + 2) = r0;   /* store */
    /* 0x0800A2AE: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
