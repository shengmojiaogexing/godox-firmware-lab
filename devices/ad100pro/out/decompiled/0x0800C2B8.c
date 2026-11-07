/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C2B8
 * Name      : FUN_0800C2B8
 * End       : 0x0800C402
 * Size      : 330 bytes, 144 instructions decoded in body (functions.json insns=144, blocks=37)
 * Class     : hub
 * Callers   : 2  -> 0x0800D098 FUN_0800D098, 0x0800D0C4 FUN_0800D0C4
 * Callees   : 21  -> 0x08003F0C FUN_08003F0C, 0x08004C0C FUN_08004C0C, 0x08004C50 FUN_08004C50, 0x08004C5C FUN_08004C5C, 0x08004CD4 FUN_08004CD4, 0x08004D18 FUN_08004D18, 0x08004F94 FUN_08004F94, 0x08004FC4 FUN_08004FC4, ... +13
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C2B8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C2B8: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x0800C2BA: 524c     ldr      r4, [pc, #0x148]  ; =RAM_20000228 */
    r4 = *(uint32_t *)0x0800C404;   /* = RAM 0x20000228 */
    /* 0x0800C2BC: 0127     movs     r7, #1 */
    r7 = 1;
    /* 0x0800C2BE: e078     ldrb     r0, [r4, #3] */
    r0 = *(uint8_t *)(r4 + 3);
    /* 0x0800C2C0: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800C2C2: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800C2C4: e070     strb     r0, [r4, #3] */
    *(uint8_t *)(r4 + 3) = r0;   /* store */
    /* 0x0800C2C6: 1928     cmp      r0, #0x19 */
    /* flags = cmp(r0, #0x19) */
    /* 0x0800C2C8: 04d3     blo      #0x800c2d4 */
    if (r0 < 0x19) goto L_0800C2D4;   /* unsigned */
    /* 0x0800C2CA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C2CC: e070     strb     r0, [r4, #3] */
    *(uint8_t *)(r4 + 3) = r0;   /* store */
    /* 0x0800C2CE: 2079     ldrb     r0, [r4, #4] */
    r0 = *(uint8_t *)(r4 + 4);
    /* 0x0800C2D0: 7840     eors     r0, r7 */
    r0 = r0 ^ r7;
    /* 0x0800C2D2: 2071     strb     r0, [r4, #4] */
    *(uint8_t *)(r4 + 4) = r0;   /* store */

    L_0800C2D4:
    /* 0x0800C2D4: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C2D6: f8f799fc bl       #0x8004c0c */
    FUN_08004C0C();   /* BL -> 0x08004C0C FUN_08004C0C */
    /* 0x0800C2DA: 4b48     ldr      r0, [pc, #0x12c]  ; =RAM_200001E3 */
    r0 = *(uint32_t *)0x0800C408;   /* = RAM 0x200001E3 */
    /* 0x0800C2DC: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800C2DE: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800C2E0: 02d0     beq      #0x800c2e8 */
    if (r0 == 0) goto L_0800C2E8;
    /* 0x0800C2E2: f9f799fb bl       #0x8005a18 */
    FUN_08005A18();   /* BL -> 0x08005A18 FUN_08005A18 */
    /* 0x0800C2E6: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_0800C2E8:
    /* 0x0800C2E8: f9f76efc bl       #0x8005bc8 */
    FUN_08005BC8();   /* BL -> 0x08005BC8 FUN_08005BC8 */
    /* 0x0800C2EC: 6079     ldrb     r0, [r4, #5] */
    r0 = *(uint8_t *)(r4 + 5);
    /* 0x0800C2EE: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800C2F0: 02d0     beq      #0x800c2f8 */
    if (r0 == 0) goto L_0800C2F8;
    /* 0x0800C2F2: f9f76bfb bl       #0x80059cc */
    FUN_080059CC();   /* BL -> 0x080059CC FUN_080059CC */
    /* 0x0800C2F6: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_0800C2F8:
    /* 0x0800C2F8: 4448     ldr      r0, [pc, #0x110]  ; =RAM_200001CD */
    r0 = *(uint32_t *)0x0800C40C;   /* = RAM 0x200001CD */
    /* 0x0800C2FA: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800C2FC: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800C2FE: 06d0     beq      #0x800c30e */
    if (r0 == 1) goto L_0800C30E;
    /* 0x0800C300: 4348     ldr      r0, [pc, #0x10c]  ; =RAM_2000016C */
    r0 = *(uint32_t *)0x0800C410;   /* = RAM 0x2000016C */
    /* 0x0800C302: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800C304: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800C306: 05d0     beq      #0x800c314 */
    if (r0 == 0) goto L_0800C314;
    /* 0x0800C308: f9f7eafa bl       #0x80058e0 */
    FUN_080058E0();   /* BL -> 0x080058E0 FUN_080058E0 */
    /* 0x0800C30C: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_0800C30E:
    /* 0x0800C30E: f8f79ffc bl       #0x8004c50 */
    FUN_08004C50();   /* BL -> 0x08004C50 FUN_08004C50 */
    /* 0x0800C312: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_0800C314:
    /* 0x0800C314: 3f48     ldr      r0, [pc, #0xfc]  ; =RAM_20000179 */
    r0 = *(uint32_t *)0x0800C414;   /* = RAM 0x20000179 */
    /* 0x0800C316: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800C318: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800C31A: 0cd0     beq      #0x800c336 */
    if (r0 == 1) goto L_0800C336;
    /* 0x0800C31C: 3e48     ldr      r0, [pc, #0xf8]  ; =RAM_20000186 */
    r0 = *(uint32_t *)0x0800C418;   /* = RAM 0x20000186 */
    /* 0x0800C31E: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800C320: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800C322: 0bd0     beq      #0x800c33c */
    if (r0 == 1) goto L_0800C33C;
    /* 0x0800C324: 3d48     ldr      r0, [pc, #0xf4]  ; =RAM_2000017C */
    r0 = *(uint32_t *)0x0800C41C;   /* = RAM 0x2000017C */
    /* 0x0800C326: 3e4d     ldr      r5, [pc, #0xf8]  ; =RAM_20000175 */
    r5 = *(uint32_t *)0x0800C420;   /* = RAM 0x20000175 */
    /* 0x0800C328: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800C32A: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800C32C: 09d0     beq      #0x800c342 */
    if (r0 == 1) goto L_0800C342;
    /* 0x0800C32E: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);
    /* 0x0800C330: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x0800C332: 1fd0     beq      #0x800c374 */
    if (r0 == 1) goto L_0800C374;
    /* 0x0800C334: 14e0     b        #0x800c360 */
    goto L_0800C360;

    L_0800C336:
    /* 0x0800C336: f7f7e9fd bl       #0x8003f0c */
    FUN_08003F0C();   /* BL -> 0x08003F0C FUN_08003F0C */
    /* 0x0800C33A: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_0800C33C:
    /* 0x0800C33C: f9f79af8 bl       #0x8005474 */
    FUN_08005474();   /* BL -> 0x08005474 FUN_08005474 */
    /* 0x0800C340: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_0800C342:
    /* 0x0800C342: 384e     ldr      r6, [pc, #0xe0]  ; =RAM_2000017D */
    r6 = *(uint32_t *)0x0800C424;   /* = RAM 0x2000017D */
    /* 0x0800C344: 3178     ldrb     r1, [r6] */
    r1 = *(uint8_t *)(r6);
    /* 0x0800C346: 0529     cmp      r1, #5 */
    /* flags = cmp(r1, #5) */
    /* 0x0800C348: 01d8     bhi      #0x800c34e */
    if (r1 > 5) goto L_0800C34E;   /* unsigned */
    /* 0x0800C34A: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x0800C34C: 00d1     bne      #0x800c350 */
    if (r1 != 0) goto L_0800C350;

    L_0800C34E:
    /* 0x0800C34E: 3770     strb     r7, [r6] */
    *(uint8_t *)(r6) = r7;   /* store */

    L_0800C350:
    /* 0x0800C350: f9f748f8 bl       #0x80053e4 */
    FUN_080053E4();   /* BL -> 0x080053E4 FUN_080053E4 */
    /* 0x0800C354: 3078     ldrb     r0, [r6] */
    r0 = *(uint8_t *)(r6);
    /* 0x0800C356: f8f7dffc bl       #0x8004d18 */
    FUN_08004D18();   /* BL -> 0x08004D18 FUN_08004D18 */
    /* 0x0800C35A: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);
    /* 0x0800C35C: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x0800C35E: 06d0     beq      #0x800c36e */
    if (r1 == 0) goto L_0800C36E;

    L_0800C360:
    /* 0x0800C360: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x0800C362: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x0800C364: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800C366: 0ad0     beq      #0x800c37e */
    if (r0 == 1) goto L_0800C37E;
    /* 0x0800C368: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x0800C36A: 0bd0     beq      #0x800c384 */
    if (r0 == 2) goto L_0800C384;
    /* 0x0800C36C: 0ce0     b        #0x800c388 */
    goto L_0800C388;

    L_0800C36E:
    /* 0x0800C36E: f9f779f8 bl       #0x8005464 */
    FUN_08005464();   /* BL -> 0x08005464 FUN_08005464 */
    /* 0x0800C372: 09e0     b        #0x800c388 */
    goto L_0800C388;

    L_0800C374:
    /* 0x0800C374: 8008     lsrs     r0, r0, #2 */
    r0 = r0 >> 2;
    /* 0x0800C376: 8000     lsls     r0, r0, #2 */
    r0 = r0 << 2;
    /* 0x0800C378: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800C37A: 2870     strb     r0, [r5] */
    *(uint8_t *)(r5) = r0;   /* store */
    /* 0x0800C37C: 04e0     b        #0x800c388 */
    goto L_0800C388;

    L_0800C37E:
    /* 0x0800C37E: f9f7b1f8 bl       #0x80054e4 */
    FUN_080054E4();   /* BL -> 0x080054E4 FUN_080054E4 */
    /* 0x0800C382: 01e0     b        #0x800c388 */
    goto L_0800C388;

    L_0800C384:
    /* 0x0800C384: f9f7a4f9 bl       #0x80056d0 */
    FUN_080056D0();   /* BL -> 0x080056D0 FUN_080056D0 */

    L_0800C388:
    /* 0x0800C388: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);
    /* 0x0800C38A: f8f703fe bl       #0x8004f94 */
    FUN_08004F94();   /* BL -> 0x08004F94 FUN_08004F94 */
    /* 0x0800C38E: 2648     ldr      r0, [pc, #0x98]  ; =RAM_200001CE */
    r0 = *(uint32_t *)0x0800C428;   /* = RAM 0x200001CE */
    /* 0x0800C390: 7921     movs     r1, #0x79 */
    r1 = 0x79;
    /* 0x0800C392: 0278     ldrb     r2, [r0] */
    r2 = *(uint8_t *)(r0);
    /* 0x0800C394: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C396: f8f715fe bl       #0x8004fc4 */
    FUN_08004FC4();   /* BL -> 0x08004FC4 FUN_08004FC4 */
    /* 0x0800C39A: 244d     ldr      r5, [pc, #0x90]  ; =RAM_20000480 */
    r5 = *(uint32_t *)0x0800C42C;   /* = RAM 0x20000480 */
    /* 0x0800C39C: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);
    /* 0x0800C39E: f8f75dfc bl       #0x8004c5c */
    FUN_08004C5C();   /* BL -> 0x08004C5C FUN_08004C5C */
    /* 0x0800C3A2: 2348     ldr      r0, [pc, #0x8c]  ; =RAM_20000226 */
    r0 = *(uint32_t *)0x0800C430;   /* = RAM 0x20000226 */
    /* 0x0800C3A4: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800C3A6: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800C3A8: 03d0     beq      #0x800c3b2 */
    if (r0 == 1) goto L_0800C3B2;
    /* 0x0800C3AA: 2248     ldr      r0, [pc, #0x88]  ; =RAM_200001BA */
    r0 = *(uint32_t *)0x0800C434;   /* = RAM 0x200001BA */
    /* 0x0800C3AC: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800C3AE: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800C3B0: 06d1     bne      #0x800c3c0 */
    if (r0 != 1) goto L_0800C3C0;

    L_0800C3B2:
    /* 0x0800C3B2: 2079     ldrb     r0, [r4, #4] */
    r0 = *(uint8_t *)(r4 + 4);
    /* 0x0800C3B4: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800C3B6: 03d1     bne      #0x800c3c0 */
    if (r0 != 1) goto L_0800C3C0;
    /* 0x0800C3B8: 7a21     movs     r1, #0x7a */
    r1 = 0x7a;
    /* 0x0800C3BA: 0620     movs     r0, #6 */
    r0 = 6;
    /* 0x0800C3BC: f8f7b0fe bl       #0x8005120 */
    FUN_08005120();   /* BL -> 0x08005120 FUN_08005120 */

    L_0800C3C0:
    /* 0x0800C3C0: 1d48     ldr      r0, [pc, #0x74]  ; =RAM_2000016A */
    r0 = *(uint32_t *)0x0800C438;   /* = RAM 0x2000016A */
    /* 0x0800C3C2: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800C3C4: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800C3C6: 03d1     bne      #0x800c3d0 */
    if (r0 != 1) goto L_0800C3D0;
    /* 0x0800C3C8: 6d21     movs     r1, #0x6d */
    r1 = 0x6d;
    /* 0x0800C3CA: 0620     movs     r0, #6 */
    r0 = 6;
    /* 0x0800C3CC: f8f7e0fe bl       #0x8005190 */
    FUN_08005190();   /* BL -> 0x08005190 FUN_08005190 */

    L_0800C3D0:
    /* 0x0800C3D0: e878     ldrb     r0, [r5, #3] */
    r0 = *(uint8_t *)(r5 + 3);
    /* 0x0800C3D2: f8f77ffc bl       #0x8004cd4 */
    FUN_08004CD4();   /* BL -> 0x08004CD4 FUN_08004CD4 */
    /* 0x0800C3D6: 6878     ldrb     r0, [r5, #1] */
    r0 = *(uint8_t *)(r5 + 1);
    /* 0x0800C3D8: f8f7f0ff bl       #0x80053bc */
    FUN_080053BC();   /* BL -> 0x080053BC FUN_080053BC */
    /* 0x0800C3DC: 1748     ldr      r0, [pc, #0x5c]  ; =RAM_20000231 */
    r0 = *(uint32_t *)0x0800C43C;   /* = RAM 0x20000231 */
    /* 0x0800C3DE: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800C3E0: f9f70ef8 bl       #0x8005400 */
    FUN_08005400();   /* BL -> 0x08005400 FUN_08005400 */
    /* 0x0800C3E4: 0322     movs     r2, #3 */
    r2 = 3;
    /* 0x0800C3E6: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C3E8: 1549     ldr      r1, [pc, #0x54]  ; =RAM_200004CC */
    r1 = *(uint32_t *)0x0800C440;   /* = RAM 0x200004CC */
    /* 0x0800C3EA: 1202     lsls     r2, r2, #8 */
    r2 = r2 << 8;
    /* 0x0800C3EC: 8024     movs     r4, #0x80 */
    r4 = 0x80;

    L_0800C3EE:
    /* 0x0800C3EE: 0b18     adds     r3, r1, r0 */
    r3 = r1 + r0;
    /* 0x0800C3F0: 9b18     adds     r3, r3, r2 */
    r3 = r3 + r2;
    /* 0x0800C3F2: 1d78     ldrb     r5, [r3] */
    r5 = *(uint8_t *)(r3);  /* MMIO: TIM1.CR1(r) */
    /* 0x0800C3F4: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800C3F6: 2543     orrs     r5, r4 */
    r5 = r5 | r4;
    /* 0x0800C3F8: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800C3FA: 1d70     strb     r5, [r3] */
    *(uint8_t *)(r3) = r5;   /* store */  /* MMIO: TIM1.CR1(w) */
    /* 0x0800C3FC: 8028     cmp      r0, #0x80 */
    /* flags = cmp(r0, #0x80) */
    /* 0x0800C3FE: f6d3     blo      #0x800c3ee */
    if (r0 < 0x80) goto L_0800C3EE;   /* unsigned */
    /* 0x0800C400: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
