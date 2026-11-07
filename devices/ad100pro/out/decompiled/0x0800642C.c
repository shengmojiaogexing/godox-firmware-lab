/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800642C
 * Name      : FUN_0800642C
 * End       : 0x080064CE
 * Size      : 162 bytes, 72 instructions decoded in body (functions.json insns=72, blocks=19)
 * Class     : function
 * Callers   : 2  -> 0x0800CF74 main, 0x0800D014 FUN_0800D014
 * Callees   : 6  -> 0x08003C30 FUN_08003C30, 0x08004568 FUN_08004568, 0x0800610C FUN_0800610C, 0x08006370 FUN_08006370, 0x08007426 FUN_08007426, 0x0800C444 FUN_0800C444
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800642C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800642C: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800642E: 2848     ldr      r0, [pc, #0xa0]  ; =RAM_200001B2 */
    r0 = *(uint32_t *)0x080064D0;   /* = RAM 0x200001B2 */
    /* 0x08006430: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x08006432: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08006434: 274c     ldr      r4, [pc, #0x9c]  ; =RAM_2000014A */
    r4 = *(uint32_t *)0x080064D4;   /* = RAM 0x2000014A */
    /* 0x08006436: 1a28     cmp      r0, #0x1a */
    /* flags = cmp(r0, #0x1a) */
    /* 0x08006438: 19d0     beq      #0x800646e */
    if (r0 == 0x1a) goto L_0800646E;
    /* 0x0800643A: 2748     ldr      r0, [pc, #0x9c]  ; =RAM_200001BC */
    r0 = *(uint32_t *)0x080064D8;   /* = RAM 0x200001BC */
    /* 0x0800643C: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800643E: 1a28     cmp      r0, #0x1a */
    /* flags = cmp(r0, #0x1a) */
    /* 0x08006440: 15d0     beq      #0x800646e */
    if (r0 == 0x1a) goto L_0800646E;
    /* 0x08006442: 05f0ffff bl       #0x800c444 */
    FUN_0800C444();   /* BL -> 0x0800C444 FUN_0800C444 */
    /* 0x08006446: fff761fe bl       #0x800610c */
    FUN_0800610C();   /* BL -> 0x0800610C FUN_0800610C */
    /* 0x0800644A: fff791ff bl       #0x8006370 */
    FUN_08006370();   /* BL -> 0x08006370 FUN_08006370 */
    /* 0x0800644E: fef78bf8 bl       #0x8004568 */
    FUN_08004568();   /* BL -> 0x08004568 FUN_08004568 */
    /* 0x08006452: e07a     ldrb     r0, [r4, #0xb] */
    r0 = *(uint8_t *)(r4 + 11);
    /* 0x08006454: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08006456: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08006458: e072     strb     r0, [r4, #0xb] */
    *(uint8_t *)(r4 + 11) = r0;   /* store */
    /* 0x0800645A: c828     cmp      r0, #0xc8 */
    /* flags = cmp(r0, #0xc8) */
    /* 0x0800645C: 00d9     bls      #0x8006460 */
    if (r0 <= 0xc8) goto L_08006460;   /* unsigned */
    /* 0x0800645E: e572     strb     r5, [r4, #0xb] */
    *(uint8_t *)(r4 + 11) = r5;   /* store */

    L_08006460:
    /* 0x08006460: e278     ldrb     r2, [r4, #3] */
    r2 = *(uint8_t *)(r4 + 3);
    /* 0x08006462: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08006464: 4902     lsls     r1, r1, #9 */
    r1 = r1 << 9;
    /* 0x08006466: 1d48     ldr      r0, [pc, #0x74]  ; =GPIOC.MODER */
    r0 = *(uint32_t *)0x080064DC;   /* = MMIO GPIOC+0x00 */
    /* 0x08006468: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800646A: 08d0     beq      #0x800647e */
    if (r2 == 1) goto L_0800647E;
    /* 0x0800646C: 0be0     b        #0x8006486 */
    goto L_08006486;

    L_0800646E:
    /* 0x0800646E: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08006470: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x08006472: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x08006474: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x08006476: 00f0d6ff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800647A: a570     strb     r5, [r4, #2] */
    *(uint8_t *)(r4 + 2) = r5;   /* store */
    /* 0x0800647C: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_0800647E:
    /* 0x0800647E: 184a     ldr      r2, [pc, #0x60]  ; =RAM_2000022D */
    r2 = *(uint32_t *)0x080064E0;   /* = RAM 0x2000022D */
    /* 0x08006480: 1278     ldrb     r2, [r2] */
    r2 = *(uint8_t *)(r2);
    /* 0x08006482: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x08006484: 07d0     beq      #0x8006496 */
    if (r2 == 0) goto L_08006496;

    L_08006486:
    /* 0x08006486: 174a     ldr      r2, [pc, #0x5c]  ; =RAM_200001B8 */
    r2 = *(uint32_t *)0x080064E4;   /* = RAM 0x200001B8 */
    /* 0x08006488: 1278     ldrb     r2, [r2] */
    r2 = *(uint8_t *)(r2);
    /* 0x0800648A: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800648C: 19d0     beq      #0x80064c2 */
    if (r2 == 1) goto L_080064C2;
    /* 0x0800648E: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08006490: 00f0c9ff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */

    L_08006494:
    /* 0x08006494: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08006496:
    /* 0x08006496: e27a     ldrb     r2, [r4, #0xb] */
    r2 = *(uint8_t *)(r4 + 11);
    /* 0x08006498: 642a     cmp      r2, #0x64 */
    /* flags = cmp(r2, #0x64) */
    /* 0x0800649A: 01d2     bhs      #0x80064a0 */
    if (r2 >= 0x64) goto L_080064A0;   /* unsigned */
    /* 0x0800649C: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800649E: 00e0     b        #0x80064a2 */
    goto L_080064A2;

    L_080064A0:
    /* 0x080064A0: 0022     movs     r2, #0 */
    r2 = 0;

    L_080064A2:
    /* 0x080064A2: 00f0c0ff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x080064A6: e07a     ldrb     r0, [r4, #0xb] */
    r0 = *(uint8_t *)(r4 + 11);
    /* 0x080064A8: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x080064AA: f3d1     bne      #0x8006494 */
    if (r0 != 1) goto L_08006494;
    /* 0x080064AC: 207b     ldrb     r0, [r4, #0xc] */
    r0 = *(uint8_t *)(r4 + 12);
    /* 0x080064AE: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080064B0: f0d0     beq      #0x8006494 */
    if (r0 == 0) goto L_08006494;
    /* 0x080064B2: 1e22     movs     r2, #0x1e */
    r2 = 0x1e;
    /* 0x080064B4: 1421     movs     r1, #0x14 */
    r1 = 0x14;
    /* 0x080064B6: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x080064B8: fdf7bafb bl       #0x8003c30 */
    FUN_08003C30();   /* BL -> 0x08003C30 FUN_08003C30 */
    /* 0x080064BC: 207b     ldrb     r0, [r4, #0xc] */
    r0 = *(uint8_t *)(r4 + 12);
    /* 0x080064BE: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x080064C0: 03e0     b        #0x80064ca */
    goto L_080064CA;

    L_080064C2:
    /* 0x080064C2: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x080064C4: 00f0afff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x080064C8: 3c20     movs     r0, #0x3c */
    r0 = 0x3c;

    L_080064CA:
    /* 0x080064CA: 2073     strb     r0, [r4, #0xc] */
    *(uint8_t *)(r4 + 12) = r0;   /* store */
    /* 0x080064CC: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
