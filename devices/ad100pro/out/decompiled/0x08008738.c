/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008738
 * Name      : FUN_08008738
 * End       : 0x08008908
 * Size      : 464 bytes, 231 instructions decoded in body (functions.json insns=231, blocks=67)
 * Class     : hub
 * Callers   : 10  -> 0x080091C4 FUN_080091C4, 0x08009290 FUN_08009290, 0x08009CB4 FUN_08009CB4, 0x08009D08 FUN_08009D08, 0x08009D28 FUN_08009D28, 0x08009D6A FUN_08009D6A, 0x0800C4E8 FUN_0800C4E8, 0x0800C616 FUN_0800C616, ... +2
 * Callees   : 1  -> 0x080072AC FUN_080072AC
 * MMIO      : RCC.+0x5, RCC.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08008738(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08008738: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x0800873A: 7348     ldr      r0, [pc, #0x1cc]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x08008908;   /* = RAM 0x20000175 */
    /* 0x0800873C: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800873E: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x08008740: 36d0     beq      #0x80087b0 */
    if (/* beq */) goto L_080087B0;
    /* 0x08008742: 8207     lsls     r2, r0, #0x1e */
    r2 = r0 << 0x1e;
    /* 0x08008744: 7148     ldr      r0, [pc, #0x1c4]  ; =RAM_2000017D */
    r0 = *(uint32_t *)0x0800890C;   /* = RAM 0x2000017D */
    /* 0x08008746: 920f     lsrs     r2, r2, #0x1e */
    r2 = r2 >> 0x1e;
    /* 0x08008748: 7149     ldr      r1, [pc, #0x1c4]  ; =flash_0800D590 */
    r1 = *(uint32_t *)0x08008910;   /* = flash data/const 0x0800D590 */
    /* 0x0800874A: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800874C: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800874E: 4fd0     beq      #0x80087f0 */
    if (r2 == 1) goto L_080087F0;
    /* 0x08008750: 704a     ldr      r2, [pc, #0x1c0]  ; =RAM_200001A2 */
    r2 = *(uint32_t *)0x08008914;   /* = RAM 0x200001A2 */

    L_08008752:
    /* 0x08008752: 105c     ldrb     r0, [r2, r0] */
    r0 = *(uint8_t *)(r2 + r0);
    /* 0x08008754: 085c     ldrb     r0, [r1, r0] */
    r0 = *(uint8_t *)(r1 + r0);

    L_08008756:
    /* 0x08008756: 7049     ldr      r1, [pc, #0x1c0]  ; =RAM_200001BA */
    r1 = *(uint32_t *)0x08008918;   /* = RAM 0x200001BA */
    /* 0x08008758: 704a     ldr      r2, [pc, #0x1c0]  ; =flash_0800D808 */
    r2 = *(uint32_t *)0x0800891C;   /* = flash data/const 0x0800D808 */
    /* 0x0800875A: 0c78     ldrb     r4, [r1] */
    r4 = *(uint8_t *)(r1);
    /* 0x0800875C: 7049     ldr      r1, [pc, #0x1c0]  ; =RAM_20000231 */
    r1 = *(uint32_t *)0x08008920;   /* = RAM 0x20000231 */
    /* 0x0800875E: 3223     movs     r3, #0x32 */
    r3 = 0x32;
    /* 0x08008760: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08008762: 4907     lsls     r1, r1, #0x1d */
    r1 = r1 << 0x1d;
    /* 0x08008764: 490f     lsrs     r1, r1, #0x1d */
    r1 = r1 >> 0x1d;
    /* 0x08008766: 5943     muls     r1, r3, r1 */
    r1 = r3 * r1;
    /* 0x08008768: 8e18     adds     r6, r1, r2 */
    r6 = r1 + r2;
    /* 0x0800876A: 694a     ldr      r2, [pc, #0x1a4]  ; =flash_0800D590 */
    r2 = *(uint32_t *)0x08008910;   /* = flash data/const 0x0800D590 */
    /* 0x0800876C: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x0800876E: 5232     adds     r2, #0x52 */
    r2 = r2 + 0x52;
    /* 0x08008770: 8d18     adds     r5, r1, r2 */
    r5 = r1 + r2;
    /* 0x08008772: 6c49     ldr      r1, [pc, #0x1b0]  ; =RAM_200001BB */
    r1 = *(uint32_t *)0x08008924;   /* = RAM 0x200001BB */
    /* 0x08008774: 002c     cmp      r4, #0 */
    /* flags = cmp(r4, #0) */
    /* 0x08008776: 0a78     ldrb     r2, [r1] */
    r2 = *(uint8_t *)(r1);
    /* 0x08008778: 3cd0     beq      #0x80087f4 */
    if (r4 == 0) goto L_080087F4;
    /* 0x0800877A: 6b49     ldr      r1, [pc, #0x1ac]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x08008928;   /* = RAM 0x20000134 */
    /* 0x0800877C: 4c78     ldrb     r4, [r1, #1] */
    r4 = *(uint8_t *)(r1 + 1);
    /* 0x0800877E: e41c     adds     r4, r4, #3 */
    r4 = r4 + 3;
    /* 0x08008780: e4b2     uxtb     r4, r4 */
    r4 = uxtb(r4);
    /* 0x08008782: 4c70     strb     r4, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r4;   /* store */
    /* 0x08008784: 322c     cmp      r4, #0x32 */
    /* flags = cmp(r4, #0x32) */
    /* 0x08008786: 01d9     bls      #0x800878c */
    if (r4 <= 0x32) goto L_0800878C;   /* unsigned */
    /* 0x08008788: 3224     movs     r4, #0x32 */
    r4 = 0x32;
    /* 0x0800878A: 4c70     strb     r4, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r4;   /* store */

    L_0800878C:
    /* 0x0800878C: 6649     ldr      r1, [pc, #0x198]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x08008928;   /* = RAM 0x20000134 */
    /* 0x0800878E: 664c     ldr      r4, [pc, #0x198]  ; =RAM_20000134 */
    r4 = *(uint32_t *)0x08008928;   /* = RAM 0x20000134 */
    /* 0x08008790: 4978     ldrb     r1, [r1, #1] */
    r1 = *(uint8_t *)(r1 + 1);
    /* 0x08008792: 2170     strb     r1, [r4] */
    *(uint8_t *)(r4) = r1;   /* store */
    /* 0x08008794: 654f     ldr      r7, [pc, #0x194]  ; =RAM_200001B9 */
    r7 = *(uint32_t *)0x0800892C;   /* = RAM 0x200001B9 */
    /* 0x08008796: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x08008798: 3c70     strb     r4, [r7] */
    *(uint8_t *)(r7) = r4;   /* store */
    /* 0x0800879A: 634f     ldr      r7, [pc, #0x18c]  ; =RAM_20000134 */
    r7 = *(uint32_t *)0x08008928;   /* = RAM 0x20000134 */
    /* 0x0800879C: 644c     ldr      r4, [pc, #0x190]  ; =0x00000BB8 */
    r4 = *(uint32_t *)0x08008930;   /* = 0x00000BB8 */
    /* 0x0800879E: fc81     strh     r4, [r7, #0xe] */
    *(uint16_t *)(r7 + 14) = r4;   /* store */
    /* 0x080087A0: 5d4f     ldr      r7, [pc, #0x174]  ; =RAM_200001BA */
    r7 = *(uint32_t *)0x08008918;   /* = RAM 0x200001BA */
    /* 0x080087A2: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x080087A4: 3c70     strb     r4, [r7] */
    *(uint8_t *)(r7) = r4;   /* store */
    /* 0x080087A6: 634c     ldr      r4, [pc, #0x18c]  ; =RAM_200001B8 */
    r4 = *(uint32_t *)0x08008934;   /* = RAM 0x200001B8 */
    /* 0x080087A8: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x080087AA: 2370     strb     r3, [r4] */
    *(uint8_t *)(r4) = r3;   /* store */
    /* 0x080087AC: 44d0     beq      #0x8008838 */
    if (r2 == 0) goto L_08008838;
    /* 0x080087AE: 5fe0     b        #0x8008870 */
    goto L_08008870;

    L_080087B0:
    /* 0x080087B0: 5d48     ldr      r0, [pc, #0x174]  ; =RAM_20000134 */
    r0 = *(uint32_t *)0x08008928;   /* = RAM 0x20000134 */
    /* 0x080087B2: 8078     ldrb     r0, [r0, #2] */
    r0 = *(uint8_t *)(r0 + 2);
    /* 0x080087B4: 2828     cmp      r0, #0x28 */
    /* flags = cmp(r0, #0x28) */
    /* 0x080087B6: 01d2     bhs      #0x80087bc */
    if (r0 >= 0x28) goto L_080087BC;   /* unsigned */
    /* 0x080087B8: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x080087BA: cce7     b        #0x8008756 */
    goto L_08008756;   /* loop */

    L_080087BC:
    /* 0x080087BC: 3228     cmp      r0, #0x32 */
    /* flags = cmp(r0, #0x32) */
    /* 0x080087BE: 01d2     bhs      #0x80087c4 */
    if (r0 >= 0x32) goto L_080087C4;   /* unsigned */
    /* 0x080087C0: 0620     movs     r0, #6 */
    r0 = 6;
    /* 0x080087C2: c8e7     b        #0x8008756 */
    goto L_08008756;   /* loop */

    L_080087C4:
    /* 0x080087C4: 3c28     cmp      r0, #0x3c */
    /* flags = cmp(r0, #0x3c) */
    /* 0x080087C6: 01d2     bhs      #0x80087cc */
    if (r0 >= 0x3c) goto L_080087CC;   /* unsigned */
    /* 0x080087C8: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x080087CA: c4e7     b        #0x8008756 */
    goto L_08008756;   /* loop */

    L_080087CC:
    /* 0x080087CC: 4628     cmp      r0, #0x46 */
    /* flags = cmp(r0, #0x46) */
    /* 0x080087CE: 01d2     bhs      #0x80087d4 */
    if (r0 >= 0x46) goto L_080087D4;   /* unsigned */
    /* 0x080087D0: 0c20     movs     r0, #0xc */
    r0 = 0xc;
    /* 0x080087D2: c0e7     b        #0x8008756 */
    goto L_08008756;   /* loop */

    L_080087D4:
    /* 0x080087D4: 5028     cmp      r0, #0x50 */
    /* flags = cmp(r0, #0x50) */
    /* 0x080087D6: 01d2     bhs      #0x80087dc */
    if (r0 >= 0x50) goto L_080087DC;   /* unsigned */
    /* 0x080087D8: 0f20     movs     r0, #0xf */
    r0 = 0xf;
    /* 0x080087DA: bce7     b        #0x8008756 */
    goto L_08008756;   /* loop */

    L_080087DC:
    /* 0x080087DC: 5a28     cmp      r0, #0x5a */
    /* flags = cmp(r0, #0x5a) */
    /* 0x080087DE: 01d2     bhs      #0x80087e4 */
    if (r0 >= 0x5a) goto L_080087E4;   /* unsigned */
    /* 0x080087E0: 1220     movs     r0, #0x12 */
    r0 = 0x12;
    /* 0x080087E2: b8e7     b        #0x8008756 */
    goto L_08008756;   /* loop */

    L_080087E4:
    /* 0x080087E4: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x080087E6: 01d2     bhs      #0x80087ec */
    if (r0 >= 0x64) goto L_080087EC;   /* unsigned */
    /* 0x080087E8: 1520     movs     r0, #0x15 */
    r0 = 0x15;
    /* 0x080087EA: b4e7     b        #0x8008756 */
    goto L_08008756;   /* loop */

    L_080087EC:
    /* 0x080087EC: 1820     movs     r0, #0x18 */
    r0 = 0x18;
    /* 0x080087EE: b2e7     b        #0x8008756 */
    goto L_08008756;   /* loop */

    L_080087F0:
    /* 0x080087F0: 514a     ldr      r2, [pc, #0x144]  ; =RAM_2000019C */
    r2 = *(uint32_t *)0x08008938;   /* = RAM 0x2000019C */
    /* 0x080087F2: aee7     b        #0x8008752 */
    goto L_08008752;   /* loop */

    L_080087F4:
    /* 0x080087F4: 0a24     movs     r4, #0xa */
    r4 = 0xa;
    /* 0x080087F6: 4c49     ldr      r1, [pc, #0x130]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x08008928;   /* = RAM 0x20000134 */
    /* 0x080087F8: a446     mov      ip, r4 */
    ip = r4;
    /* 0x080087FA: 4c70     strb     r4, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r4;   /* store */
    /* 0x080087FC: 0c46     mov      r4, r1 */
    r4 = r1;
    /* 0x080087FE: a489     ldrh     r4, [r4, #0xc] */
    r4 = *(uint16_t *)(r4 + 12);
    /* 0x08008800: 4b4f     ldr      r7, [pc, #0x12c]  ; =0x00000BB8 */
    r7 = *(uint32_t *)0x08008930;   /* = 0x00000BB8 */
    /* 0x08008802: 4f81     strh     r7, [r1, #0xa] */
    *(uint16_t *)(r1 + 10) = r7;   /* store */
    /* 0x08008804: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x08008806: 14d0     beq      #0x8008832 */
    if (r2 == 1) goto L_08008832;
    /* 0x08008808: 4700     lsls     r7, r0, #1 */
    r7 = r0 << 1;
    /* 0x0800880A: ef5b     ldrh     r7, [r5, r7] */
    r7 = *(uint16_t *)(r5 + r7);  /* MMIO: RCC.CR(r) */

    L_0800880C:
    /* 0x0800880C: 3c19     adds     r4, r7, r4 */
    r4 = r7 + r4;
    /* 0x0800880E: 8c81     strh     r4, [r1, #0xc] */
    *(uint16_t *)(r1 + 12) = r4;   /* store */
    /* 0x08008810: 4a4f     ldr      r7, [pc, #0x128]  ; =0x00001770 */
    r7 = *(uint32_t *)0x0800893C;   /* = 0x00001770 */
    /* 0x08008812: a4b2     uxth     r4, r4 */
    r4 = uxth(r4);
    /* 0x08008814: bc42     cmp      r4, r7 */
    /* flags = cmp(r4, r7) */
    /* 0x08008816: 2bd3     blo      #0x8008870 */
    if (r4 < r7) goto L_08008870;   /* unsigned */
    /* 0x08008818: 6446     mov      r4, ip */
    r4 = ip;
    /* 0x0800881A: 0c70     strb     r4, [r1] */
    *(uint8_t *)(r1) = r4;   /* store */
    /* 0x0800881C: 434f     ldr      r7, [pc, #0x10c]  ; =RAM_200001B9 */
    r7 = *(uint32_t *)0x0800892C;   /* = RAM 0x200001B9 */
    /* 0x0800881E: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x08008820: 3c70     strb     r4, [r7] */
    *(uint8_t *)(r7) = r4;   /* store */
    /* 0x08008822: 434c     ldr      r4, [pc, #0x10c]  ; =0x00000BB8 */
    r4 = *(uint32_t *)0x08008930;   /* = 0x00000BB8 */
    /* 0x08008824: cc81     strh     r4, [r1, #0xe] */
    *(uint16_t *)(r1 + 14) = r4;   /* store */
    /* 0x08008826: 3c4c     ldr      r4, [pc, #0xf0]  ; =RAM_200001BA */
    r4 = *(uint32_t *)0x08008918;   /* = RAM 0x200001BA */
    /* 0x08008828: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800882A: 2170     strb     r1, [r4] */
    *(uint8_t *)(r4) = r1;   /* store */
    /* 0x0800882C: 4149     ldr      r1, [pc, #0x104]  ; =RAM_200001B8 */
    r1 = *(uint32_t *)0x08008934;   /* = RAM 0x200001B8 */
    /* 0x0800882E: 0b70     strb     r3, [r1] */
    *(uint8_t *)(r1) = r3;   /* store */
    /* 0x08008830: 1ee0     b        #0x8008870 */
    goto L_08008870;

    L_08008832:
    /* 0x08008832: 4700     lsls     r7, r0, #1 */
    r7 = r0 << 1;
    /* 0x08008834: f75b     ldrh     r7, [r6, r7] */
    r7 = *(uint16_t *)(r6 + r7);
    /* 0x08008836: e9e7     b        #0x800880c */
    goto L_0800880C;   /* loop */

    L_08008838:
    /* 0x08008838: 0c28     cmp      r0, #0xc */
    /* flags = cmp(r0, #0xc) */
    /* 0x0800883A: 04d8     bhi      #0x8008846 */
    if (r0 > 0xc) goto L_08008846;   /* unsigned */
    /* 0x0800883C: 3a4c     ldr      r4, [pc, #0xe8]  ; =RAM_20000134 */
    r4 = *(uint32_t *)0x08008928;   /* = RAM 0x20000134 */
    /* 0x0800883E: 0a29     cmp      r1, #0xa */
    /* flags = cmp(r1, #0xa) */
    /* 0x08008840: 16d9     bls      #0x8008870 */
    if (r1 <= 0xa) goto L_08008870;   /* unsigned */
    /* 0x08008842: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x08008844: 13e0     b        #0x800886e */
    goto L_0800886E;

    L_08008846:
    /* 0x08008846: 0f28     cmp      r0, #0xf */
    /* flags = cmp(r0, #0xf) */
    /* 0x08008848: 04d8     bhi      #0x8008854 */
    if (r0 > 0xf) goto L_08008854;   /* unsigned */
    /* 0x0800884A: 374c     ldr      r4, [pc, #0xdc]  ; =RAM_20000134 */
    r4 = *(uint32_t *)0x08008928;   /* = RAM 0x20000134 */
    /* 0x0800884C: 1429     cmp      r1, #0x14 */
    /* flags = cmp(r1, #0x14) */
    /* 0x0800884E: 0fd9     bls      #0x8008870 */
    if (r1 <= 0x14) goto L_08008870;   /* unsigned */
    /* 0x08008850: 1421     movs     r1, #0x14 */
    r1 = 0x14;
    /* 0x08008852: 0ce0     b        #0x800886e */
    goto L_0800886E;

    L_08008854:
    /* 0x08008854: 1228     cmp      r0, #0x12 */
    /* flags = cmp(r0, #0x12) */
    /* 0x08008856: 04d8     bhi      #0x8008862 */
    if (r0 > 0x12) goto L_08008862;   /* unsigned */
    /* 0x08008858: 334c     ldr      r4, [pc, #0xcc]  ; =RAM_20000134 */
    r4 = *(uint32_t *)0x08008928;   /* = RAM 0x20000134 */
    /* 0x0800885A: 1e29     cmp      r1, #0x1e */
    /* flags = cmp(r1, #0x1e) */
    /* 0x0800885C: 08d9     bls      #0x8008870 */
    if (r1 <= 0x1e) goto L_08008870;   /* unsigned */
    /* 0x0800885E: 1e21     movs     r1, #0x1e */
    r1 = 0x1e;
    /* 0x08008860: 05e0     b        #0x800886e */
    goto L_0800886E;

    L_08008862:
    /* 0x08008862: 1528     cmp      r0, #0x15 */
    /* flags = cmp(r0, #0x15) */
    /* 0x08008864: 04d8     bhi      #0x8008870 */
    if (r0 > 0x15) goto L_08008870;   /* unsigned */
    /* 0x08008866: 304c     ldr      r4, [pc, #0xc0]  ; =RAM_20000134 */
    r4 = *(uint32_t *)0x08008928;   /* = RAM 0x20000134 */
    /* 0x08008868: 3229     cmp      r1, #0x32 */
    /* flags = cmp(r1, #0x32) */
    /* 0x0800886A: 01d9     bls      #0x8008870 */
    if (r1 <= 0x32) goto L_08008870;   /* unsigned */
    /* 0x0800886C: 2821     movs     r1, #0x28 */
    r1 = 0x28;

    L_0800886E:
    /* 0x0800886E: 2170     strb     r1, [r4] */
    *(uint8_t *)(r4) = r1;   /* store */

    L_08008870:
    /* 0x08008870: 2d49     ldr      r1, [pc, #0xb4]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x08008928;   /* = RAM 0x20000134 */
    /* 0x08008872: e124     movs     r4, #0xe1 */
    r4 = 0xe1;
    /* 0x08008874: 8f79     ldrb     r7, [r1, #6] */
    r7 = *(uint8_t *)(r1 + 6);
    /* 0x08008876: e400     lsls     r4, r4, #3 */
    r4 = r4 << 3;
    /* 0x08008878: 002f     cmp      r7, #0 */
    /* flags = cmp(r7, #0) */
    /* 0x0800887A: 10d0     beq      #0x800889e */
    if (r7 == 0) goto L_0800889E;
    /* 0x0800887C: 0d46     mov      r5, r1 */
    r5 = r1;
    /* 0x0800887E: 6d79     ldrb     r5, [r5, #5] */
    r5 = *(uint8_t *)(r5 + 5);  /* MMIO: RCC.+0x5(r) */
    /* 0x08008880: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x08008882: 2bd0     beq      #0x80088dc */
    if (r2 == 1) goto L_080088DC;
    /* 0x08008884: 6d1c     adds     r5, r5, #1 */
    r5 = r5 + 1;

    L_08008886:
    /* 0x08008886: 4d71     strb     r5, [r1, #5] */
    *(uint8_t *)(r1 + 5) = r5;   /* store */
    /* 0x08008888: edb2     uxtb     r5, r5 */
    r5 = uxtb(r5);
    /* 0x0800888A: 0f2d     cmp      r5, #0xf */
    /* flags = cmp(r5, #0xf) */
    /* 0x0800888C: 01d9     bls      #0x8008892 */
    if (r5 <= 0xf) goto L_08008892;   /* unsigned */
    /* 0x0800888E: 0f25     movs     r5, #0xf */
    r5 = 0xf;
    /* 0x08008890: 4d71     strb     r5, [r1, #5] */
    *(uint8_t *)(r1 + 5) = r5;   /* store */

    L_08008892:
    /* 0x08008892: edb2     uxtb     r5, r5 */
    r5 = uxtb(r5);
    /* 0x08008894: 0d71     strb     r5, [r1, #4] */
    *(uint8_t *)(r1 + 4) = r5;   /* store */
    /* 0x08008896: 4c82     strh     r4, [r1, #0x12] */
    *(uint16_t *)(r1 + 18) = r4;   /* store */
    /* 0x08008898: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x0800889A: 21d0     beq      #0x80088e0 */
    if (r2 == 0) goto L_080088E0;
    /* 0x0800889C: 28e0     b        #0x80088f0 */
    goto L_080088F0;

    L_0800889E:
    /* 0x0800889E: 0527     movs     r7, #5 */
    r7 = 5;
    /* 0x080088A0: 4f71     strb     r7, [r1, #5] */
    *(uint8_t *)(r1 + 5) = r7;   /* store */
    /* 0x080088A2: 4c82     strh     r4, [r1, #0x12] */
    *(uint16_t *)(r1 + 18) = r4;   /* store */
    /* 0x080088A4: 204c     ldr      r4, [pc, #0x80]  ; =RAM_20000134 */
    r4 = *(uint32_t *)0x08008928;   /* = RAM 0x20000134 */
    /* 0x080088A6: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x080088A8: a48a     ldrh     r4, [r4, #0x14] */
    r4 = *(uint16_t *)(r4 + 20);
    /* 0x080088AA: 0dd0     beq      #0x80088c8 */
    if (r2 == 1) goto L_080088C8;
    /* 0x080088AC: 4600     lsls     r6, r0, #1 */
    r6 = r0 << 1;
    /* 0x080088AE: ad5b     ldrh     r5, [r5, r6] */
    r5 = *(uint16_t *)(r5 + r6);  /* MMIO: RCC.CR(r) */

    L_080088B0:
    /* 0x080088B0: 2c19     adds     r4, r5, r4 */
    r4 = r5 + r4;
    /* 0x080088B2: 8c82     strh     r4, [r1, #0x14] */
    *(uint16_t *)(r1 + 20) = r4;   /* store */
    /* 0x080088B4: 224d     ldr      r5, [pc, #0x88]  ; =0x000005DC */
    r5 = *(uint32_t *)0x08008940;   /* = 0x000005DC */
    /* 0x080088B6: a4b2     uxth     r4, r4 */
    r4 = uxth(r4);
    /* 0x080088B8: ac42     cmp      r4, r5 */
    /* flags = cmp(r4, r5) */
    /* 0x080088BA: 1ed3     blo      #0x80088fa */
    if (r4 < r5) goto L_080088FA;   /* unsigned */
    /* 0x080088BC: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x080088BE: 0f71     strb     r7, [r1, #4] */
    *(uint8_t *)(r1 + 4) = r7;   /* store */
    /* 0x080088C0: 8c71     strb     r4, [r1, #6] */
    *(uint8_t *)(r1 + 6) = r4;   /* store */
    /* 0x080088C2: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x080088C4: 03d0     beq      #0x80088ce */
    if (r2 == 0) goto L_080088CE;
    /* 0x080088C6: 16e0     b        #0x80088f6 */
    goto L_080088F6;

    L_080088C8:
    /* 0x080088C8: 4500     lsls     r5, r0, #1 */
    r5 = r0 << 1;
    /* 0x080088CA: 755b     ldrh     r5, [r6, r5] */
    r5 = *(uint16_t *)(r6 + r5);
    /* 0x080088CC: f0e7     b        #0x80088b0 */
    goto L_080088B0;   /* loop */

    L_080088CE:
    /* 0x080088CE: 1228     cmp      r0, #0x12 */
    /* flags = cmp(r0, #0x12) */
    /* 0x080088D0: 08d9     bls      #0x80088e4 */
    if (r0 <= 0x12) goto L_080088E4;   /* unsigned */
    /* 0x080088D2: 1528     cmp      r0, #0x15 */
    /* flags = cmp(r0, #0x15) */
    /* 0x080088D4: 0fd8     bhi      #0x80088f6 */
    if (r0 > 0x15) goto L_080088F6;   /* unsigned */
    /* 0x080088D6: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x080088D8: 0871     strb     r0, [r1, #4] */
    *(uint8_t *)(r1 + 4) = r0;   /* store */
    /* 0x080088DA: 0ce0     b        #0x80088f6 */
    goto L_080088F6;

    L_080088DC:
    /* 0x080088DC: 6d1d     adds     r5, r5, #5 */
    r5 = r5 + 5;
    /* 0x080088DE: d2e7     b        #0x8008886 */
    goto L_08008886;   /* loop */

    L_080088E0:
    /* 0x080088E0: 1228     cmp      r0, #0x12 */
    /* flags = cmp(r0, #0x12) */
    /* 0x080088E2: 01d8     bhi      #0x80088e8 */
    if (r0 > 0x12) goto L_080088E8;   /* unsigned */

    L_080088E4:
    /* 0x080088E4: 0b71     strb     r3, [r1, #4] */
    *(uint8_t *)(r1 + 4) = r3;   /* store */
    /* 0x080088E6: 08e0     b        #0x80088fa */
    goto L_080088FA;

    L_080088E8:
    /* 0x080088E8: 1528     cmp      r0, #0x15 */
    /* flags = cmp(r0, #0x15) */
    /* 0x080088EA: 01d8     bhi      #0x80088f0 */
    if (r0 > 0x15) goto L_080088F0;   /* unsigned */
    /* 0x080088EC: 6808     lsrs     r0, r5, #1 */
    r0 = r5 >> 1;
    /* 0x080088EE: 0871     strb     r0, [r1, #4] */
    *(uint8_t *)(r1 + 4) = r0;   /* store */

    L_080088F0:
    /* 0x080088F0: 0879     ldrb     r0, [r1, #4] */
    r0 = *(uint8_t *)(r1 + 4);
    /* 0x080088F2: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080088F4: 01d0     beq      #0x80088fa */
    if (r0 == 0) goto L_080088FA;

    L_080088F6:
    /* 0x080088F6: 0f48     ldr      r0, [pc, #0x3c]  ; =RAM_200001B8 */
    r0 = *(uint32_t *)0x08008934;   /* = RAM 0x200001B8 */
    /* 0x080088F8: 0370     strb     r3, [r0] */
    *(uint8_t *)(r0) = r3;   /* store */

    L_080088FA:
    /* 0x080088FA: 1249     ldr      r1, [pc, #0x48]  ; =RAM_200001DB */
    r1 = *(uint32_t *)0x08008944;   /* = RAM 0x200001DB */
    /* 0x080088FC: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x080088FE: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08008900: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08008902: fef7d3fc bl       #0x80072ac */
    FUN_080072AC();   /* BL -> 0x080072AC FUN_080072AC */
    /* 0x08008906: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
