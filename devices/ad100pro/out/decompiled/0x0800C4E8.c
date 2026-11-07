/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C4E8
 * Name      : FUN_0800C4E8
 * End       : 0x0800C63E
 * Size      : 342 bytes, 139 instructions decoded in body (functions.json insns=139, blocks=14)
 * Class     : hub
 * Callers   : 2  -> 0x0800C6F4 FUN_0800C6F4, 0x0800C758 FUN_0800C758
 * Callees   : 11  -> 0x0800663C FUN_0800663C, 0x080066C0 FUN_080066C0, 0x08006BA4 FUN_08006BA4, 0x08006C44 FUN_08006C44, 0x08007426 FUN_08007426, 0x08008738 FUN_08008738, 0x08008A2C FUN_08008A2C, 0x0800A8D0 FUN_0800A8D0, ... +3
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C4E8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C4E8: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x0800C4EA: 554e     ldr      r6, [pc, #0x154]  ; =RAM_200001B2 */
    r6 = *(uint32_t *)0x0800C640;   /* = RAM 0x200001B2 */
    /* 0x0800C4EC: 0925     movs     r5, #9 */
    r5 = 9;
    /* 0x0800C4EE: 3078     ldrb     r0, [r6] */
    r0 = *(uint8_t *)(r6);
    /* 0x0800C4F0: ed06     lsls     r5, r5, #0x1b */
    r5 = r5 << 0x1b;
    /* 0x0800C4F2: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x0800C4F4: 1a28     cmp      r0, #0x1a */
    /* flags = cmp(r0, #0x1a) */
    /* 0x0800C4F6: 02d0     beq      #0x800c4fe */
    if (r0 == 0x1a) goto L_0800C4FE;
    /* 0x0800C4F8: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x0800C4FA: 6fd0     beq      #0x800c5dc */
    if (r0 == 0xa) goto L_0800C5DC;
    /* 0x0800C4FC: 91e0     b        #0x800c622 */
    goto L_0800C622;

    L_0800C4FE:
    /* 0x0800C4FE: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800C500: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x0800C502: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800C504: faf78fff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800C508: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800C50A: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800C50E: faf7d7f8 bl       #0x80066c0 */
    FUN_080066C0();   /* BL -> 0x080066C0 FUN_080066C0 */
    /* 0x0800C512: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800C514: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800C516: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800C518: faf785ff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800C51C: 3220     movs     r0, #0x32 */
    r0 = 0x32;
    /* 0x0800C51E: 00f08bfa bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800C522: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800C524: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800C526: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800C528: faf77dff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800C52C: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x0800C52E: 00f083fa bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800C532: 0027     movs     r7, #0 */
    r7 = 0;

    L_0800C534:
    /* 0x0800C534: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800C536: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800C538: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800C53A: faf774ff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800C53E: 0520     movs     r0, #5 */
    r0 = 5;
    /* 0x0800C540: 00f07afa bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800C544: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800C546: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800C548: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800C54A: faf76cff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800C54E: 0c20     movs     r0, #0xc */
    r0 = 0xc;
    /* 0x0800C550: 00f072fa bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800C554: 7f1c     adds     r7, r7, #1 */
    r7 = r7 + 1;
    /* 0x0800C556: ffb2     uxtb     r7, r7 */
    r7 = uxtb(r7);
    /* 0x0800C558: 032f     cmp      r7, #3 */
    /* flags = cmp(r7, #3) */
    /* 0x0800C55A: ebd3     blo      #0x800c534 */
    if (r7 < 3) goto L_0800C534;   /* unsigned */
    /* 0x0800C55C: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x0800C55E: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800C560: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800C562: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x0800C564: faf75fff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800C568: 364f     ldr      r7, [pc, #0xd8]  ; =RAM_200001C8 */
    r7 = *(uint32_t *)0x0800C644;   /* = RAM 0x200001C8 */
    /* 0x0800C56A: 1821     movs     r1, #0x18 */
    r1 = 0x18;
    /* 0x0800C56C: 3888     ldrh     r0, [r7] */
    r0 = *(uint16_t *)(r7);
    /* 0x0800C56E: 4843     muls     r0, r1, r0 */
    r0 = r1 * r0;
    /* 0x0800C570: 3549     ldr      r1, [pc, #0xd4]  ; =RAM_200001D8 */
    r1 = *(uint32_t *)0x0800C648;   /* = RAM 0x200001D8 */
    /* 0x0800C572: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x0800C574: 0988     ldrh     r1, [r1] */
    r1 = *(uint16_t *)(r1);
    /* 0x0800C576: fef7abf9 bl       #0x800a8d0 */
    FUN_0800A8D0();   /* BL -> 0x0800A8D0 FUN_0800A8D0 */
    /* 0x0800C57A: 0025     movs     r5, #0 */
    r5 = 0;

    L_0800C57C:
    /* 0x0800C57C: 1022     movs     r2, #0x10 */
    r2 = 0x10;
    /* 0x0800C57E: 3349     ldr      r1, [pc, #0xcc]  ; =RAM_2000048C */
    r1 = *(uint32_t *)0x0800C64C;   /* = RAM 0x2000048C */
    /* 0x0800C580: 3348     ldr      r0, [pc, #0xcc]  ; =RAM_200003AC */
    r0 = *(uint32_t *)0x0800C650;   /* = RAM 0x200003AC */
    /* 0x0800C582: faf70ffb bl       #0x8006ba4 */
    FUN_08006BA4();   /* BL -> 0x08006BA4 FUN_08006BA4 */
    /* 0x0800C586: 3220     movs     r0, #0x32 */
    r0 = 0x32;
    /* 0x0800C588: 00f056fa bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800C58C: 3048     ldr      r0, [pc, #0xc0]  ; =RAM_200003AC */
    r0 = *(uint32_t *)0x0800C650;   /* = RAM 0x200003AC */
    /* 0x0800C58E: faf759fb bl       #0x8006c44 */
    FUN_08006C44();   /* BL -> 0x08006C44 FUN_08006C44 */
    /* 0x0800C592: faf753f8 bl       #0x800663c */
    FUN_0800663C();   /* BL -> 0x0800663C FUN_0800663C */
    /* 0x0800C596: 3888     ldrh     r0, [r7] */
    r0 = *(uint16_t *)(r7);
    /* 0x0800C598: 1821     movs     r1, #0x18 */
    r1 = 0x18;
    /* 0x0800C59A: 4843     muls     r0, r1, r0 */
    r0 = r1 * r0;
    /* 0x0800C59C: 2d49     ldr      r1, [pc, #0xb4]  ; =RAM_200001C6 */
    r1 = *(uint32_t *)0x0800C654;   /* = RAM 0x200001C6 */
    /* 0x0800C59E: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x0800C5A0: 0988     ldrh     r1, [r1] */
    r1 = *(uint16_t *)(r1);
    /* 0x0800C5A2: fef7aff9 bl       #0x800a904 */
    FUN_0800A904();   /* BL -> 0x0800A904 FUN_0800A904 */
    /* 0x0800C5A6: 6d1c     adds     r5, r5, #1 */
    r5 = r5 + 1;
    /* 0x0800C5A8: edb2     uxtb     r5, r5 */
    r5 = uxtb(r5);
    /* 0x0800C5AA: 642d     cmp      r5, #0x64 */
    /* flags = cmp(r5, #0x64) */
    /* 0x0800C5AC: e6d3     blo      #0x800c57c */
    if (r5 < 0x64) goto L_0800C57C;   /* unsigned */
    /* 0x0800C5AE: fcf73dfa bl       #0x8008a2c */
    FUN_08008A2C();   /* BL -> 0x08008A2C FUN_08008A2C */
    /* 0x0800C5B2: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x0800C5B4: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800C5B6: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800C5B8: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x0800C5BA: faf734ff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800C5BE: 2648     ldr      r0, [pc, #0x98]  ; =0x00000BB8 */
    r0 = *(uint32_t *)0x0800C658;   /* = 0x00000BB8 */
    /* 0x0800C5C0: 00f03afa bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800C5C4: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C5C6: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800C5CA: 2448     ldr      r0, [pc, #0x90]  ; =RAM_200001BB */
    r0 = *(uint32_t *)0x0800C65C;   /* = RAM 0x200001BB */
    /* 0x0800C5CC: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800C5CE: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x0800C5D0: 2348     ldr      r0, [pc, #0x8c]  ; =RAM_20000134 */
    r0 = *(uint32_t *)0x0800C660;   /* = RAM 0x20000134 */
    /* 0x0800C5D2: 0421     movs     r1, #4 */
    r1 = 4;
    /* 0x0800C5D4: 0182     strh     r1, [r0, #0x10] */
    *(uint16_t *)(r0 + 16) = r1;   /* store */
    /* 0x0800C5D6: 2348     ldr      r0, [pc, #0x8c]  ; =RAM_200001B8 */
    r0 = *(uint32_t *)0x0800C664;   /* = RAM 0x200001B8 */
    /* 0x0800C5D8: 1fe0     b        #0x800c61a */
    goto L_0800C61A;

    L_0800C5DC:
    /* 0x0800C5DC: fff75eff bl       #0x800c49c */
    FUN_0800C49C();   /* BL -> 0x0800C49C FUN_0800C49C */
    /* 0x0800C5E0: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800C5E2: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x0800C5E4: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800C5E6: faf71eff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800C5EA: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800C5EC: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800C5F0: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x0800C5F2: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800C5F4: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800C5F6: faf716ff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800C5FA: 1b48     ldr      r0, [pc, #0x6c]  ; =RAM_200001C4 */
    r0 = *(uint32_t *)0x0800C668;   /* = RAM 0x200001C4 */
    /* 0x0800C5FC: 0088     ldrh     r0, [r0] */
    r0 = *(uint16_t *)(r0);
    /* 0x0800C5FE: 00f01bfa bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800C602: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800C604: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800C606: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800C608: faf70dff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800C60C: 1248     ldr      r0, [pc, #0x48]  ; =0x00000BB8 */
    r0 = *(uint32_t *)0x0800C658;   /* = 0x00000BB8 */
    /* 0x0800C60E: 00f013fa bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800C612: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C614: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800C618: 1048     ldr      r0, [pc, #0x40]  ; =RAM_200001BB */
    r0 = *(uint32_t *)0x0800C65C;   /* = RAM 0x200001BB */

    L_0800C61A:
    /* 0x0800C61A: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */
    /* 0x0800C61C: fcf78cf8 bl       #0x8008738 */
    FUN_08008738();   /* BL -> 0x08008738 FUN_08008738 */
    /* 0x0800C620: 3470     strb     r4, [r6] */
    *(uint8_t *)(r6) = r4;   /* store */

    L_0800C622:
    /* 0x0800C622: 1248     ldr      r0, [pc, #0x48]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800C66C;   /* = RAM 0x20000175 */
    /* 0x0800C624: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800C626: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x0800C628: 08d1     bne      #0x800c63c */
    if (r5 != 0x64) goto L_0800C63C;
    /* 0x0800C62A: 1148     ldr      r0, [pc, #0x44]  ; =RAM_20000185 */
    r0 = *(uint32_t *)0x0800C670;   /* = RAM 0x20000185 */
    /* 0x0800C62C: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800C62E: 0628     cmp      r0, #6 */
    /* flags = cmp(r0, #6) */
    /* 0x0800C630: 04d1     bne      #0x800c63c */
    if (r0 != 6) goto L_0800C63C;
    /* 0x0800C632: 0c48     ldr      r0, [pc, #0x30]  ; =RAM_200001B8 */
    r0 = *(uint32_t *)0x0800C664;   /* = RAM 0x200001B8 */
    /* 0x0800C634: 0321     movs     r1, #3 */
    r1 = 3;
    /* 0x0800C636: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */
    /* 0x0800C638: 0948     ldr      r0, [pc, #0x24]  ; =RAM_20000134 */
    r0 = *(uint32_t *)0x0800C660;   /* = RAM 0x20000134 */
    /* 0x0800C63A: c171     strb     r1, [r0, #7] */
    *(uint8_t *)(r0 + 7) = r1;   /* store */

    L_0800C63C:
    /* 0x0800C63C: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
