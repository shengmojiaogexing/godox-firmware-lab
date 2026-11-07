/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080091C4
 * Name      : FUN_080091C4
 * End       : 0x080092E6
 * Size      : 290 bytes, 119 instructions decoded in body (functions.json insns=119, blocks=11)
 * Class     : function
 * Callers   : 2  -> 0x08006014 FUN_08006014, 0x080060B4 FUN_080060B4
 * Callees   : 7  -> 0x080066C0 FUN_080066C0, 0x08007426 FUN_08007426, 0x08008738 FUN_08008738, 0x08008A2C FUN_08008A2C, 0x0800A8D0 FUN_0800A8D0, 0x0800A904 FUN_0800A904, 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080091C4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080091C4: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x080091C6: 484d     ldr      r5, [pc, #0x120]  ; =RAM_200001C4 */
    r5 = *(uint32_t *)0x080092E8;   /* = RAM 0x200001C4 */
    /* 0x080091C8: 2888     ldrh     r0, [r5] */
    r0 = *(uint16_t *)(r5);
    /* 0x080091CA: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080091CC: 70d0     beq      #0x80092b0 */
    if (r0 == 0) goto L_080092B0;
    /* 0x080091CE: 4748     ldr      r0, [pc, #0x11c]  ; =RAM_20000182 */
    r0 = *(uint32_t *)0x080092EC;   /* = RAM 0x20000182 */
    /* 0x080091D0: 0924     movs     r4, #9 */
    r4 = 9;
    /* 0x080091D2: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x080091D4: 0026     movs     r6, #0 */
    r6 = 0;
    /* 0x080091D6: 464f     ldr      r7, [pc, #0x118]  ; =RAM_200001BB */
    r7 = *(uint32_t *)0x080092F0;   /* = RAM 0x200001BB */
    /* 0x080091D8: e406     lsls     r4, r4, #0x1b */
    r4 = r4 << 0x1b;
    /* 0x080091DA: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x080091DC: 69d1     bne      #0x80092b2 */
    if (r0 != 1) goto L_080092B2;
    /* 0x080091DE: 4548     ldr      r0, [pc, #0x114]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x080092F4;   /* = RAM 0x20000175 */
    /* 0x080091E0: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x080091E2: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x080091E4: 65d5     bpl      #0x80092b2 */
    if (r0 >= 1) goto L_080092B2;   /* result non-negative */
    /* 0x080091E6: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x080091E8: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x080091EA: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080091EC: fef71bf9 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x080091F0: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x080091F2: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x080091F6: fdf763fa bl       #0x80066c0 */
    FUN_080066C0();   /* BL -> 0x080066C0 FUN_080066C0 */
    /* 0x080091FA: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x080091FC: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x080091FE: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08009200: fef711f9 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009204: 3220     movs     r0, #0x32 */
    r0 = 0x32;
    /* 0x08009206: 03f017fc bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800920A: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800920C: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800920E: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08009210: fef709f9 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009214: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x08009216: 03f00ffc bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800921A: 0025     movs     r5, #0 */
    r5 = 0;

    L_0800921C:
    /* 0x0800921C: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800921E: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08009220: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08009222: fef700f9 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009226: 0520     movs     r0, #5 */
    r0 = 5;
    /* 0x08009228: 03f006fc bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800922C: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800922E: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08009230: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08009232: fef7f8f8 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009236: 0c20     movs     r0, #0xc */
    r0 = 0xc;
    /* 0x08009238: 03f0fefb bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800923C: 6d1c     adds     r5, r5, #1 */
    r5 = r5 + 1;
    /* 0x0800923E: adb2     uxth     r5, r5 */
    r5 = uxth(r5);
    /* 0x08009240: 032d     cmp      r5, #3 */
    /* flags = cmp(r5, #3) */
    /* 0x08009242: ebd3     blo      #0x800921c */
    if (r5 < 3) goto L_0800921C;   /* unsigned */
    /* 0x08009244: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x08009246: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08009248: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800924A: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x0800924C: fef7ebf8 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009250: 294d     ldr      r5, [pc, #0xa4]  ; =RAM_200001C8 */
    r5 = *(uint32_t *)0x080092F8;   /* = RAM 0x200001C8 */
    /* 0x08009252: 1821     movs     r1, #0x18 */
    r1 = 0x18;
    /* 0x08009254: 2888     ldrh     r0, [r5] */
    r0 = *(uint16_t *)(r5);
    /* 0x08009256: 4843     muls     r0, r1, r0 */
    r0 = r1 * r0;
    /* 0x08009258: 2849     ldr      r1, [pc, #0xa0]  ; =RAM_200001D8 */
    r1 = *(uint32_t *)0x080092FC;   /* = RAM 0x200001D8 */
    /* 0x0800925A: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x0800925C: 0988     ldrh     r1, [r1] */
    r1 = *(uint16_t *)(r1);
    /* 0x0800925E: 01f037fb bl       #0x800a8d0 */
    FUN_0800A8D0();   /* BL -> 0x0800A8D0 FUN_0800A8D0 */
    /* 0x08009262: 0024     movs     r4, #0 */
    r4 = 0;

    L_08009264:
    /* 0x08009264: 6420     movs     r0, #0x64 */
    r0 = 0x64;
    /* 0x08009266: 03f0e7fb bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800926A: 2888     ldrh     r0, [r5] */
    r0 = *(uint16_t *)(r5);
    /* 0x0800926C: 1821     movs     r1, #0x18 */
    r1 = 0x18;
    /* 0x0800926E: 4843     muls     r0, r1, r0 */
    r0 = r1 * r0;
    /* 0x08009270: 2349     ldr      r1, [pc, #0x8c]  ; =RAM_200001C6 */
    r1 = *(uint32_t *)0x08009300;   /* = RAM 0x200001C6 */
    /* 0x08009272: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x08009274: 0988     ldrh     r1, [r1] */
    r1 = *(uint16_t *)(r1);
    /* 0x08009276: 01f045fb bl       #0x800a904 */
    FUN_0800A904();   /* BL -> 0x0800A904 FUN_0800A904 */
    /* 0x0800927A: 641c     adds     r4, r4, #1 */
    r4 = r4 + 1;
    /* 0x0800927C: a4b2     uxth     r4, r4 */
    r4 = uxth(r4);
    /* 0x0800927E: 642c     cmp      r4, #0x64 */
    /* flags = cmp(r4, #0x64) */
    /* 0x08009280: f0d3     blo      #0x8009264 */
    if (r4 < 0x64) goto L_08009264;   /* unsigned */
    /* 0x08009282: fff7d3fb bl       #0x8008a2c */
    FUN_08008A2C();   /* BL -> 0x08008A2C FUN_08008A2C */
    /* 0x08009286: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x08009288: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800928A: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800928C: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x0800928E: fef7caf8 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009292: 1c48     ldr      r0, [pc, #0x70]  ; =0x00000BB8 */
    r0 = *(uint32_t *)0x08009304;   /* = 0x00000BB8 */
    /* 0x08009294: 03f0d0fb bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08009298: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800929A: 3870     strb     r0, [r7] */
    *(uint8_t *)(r7) = r0;   /* store */
    /* 0x0800929C: 1a49     ldr      r1, [pc, #0x68]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x08009308;   /* = RAM 0x20000134 */
    /* 0x0800929E: 0420     movs     r0, #4 */
    r0 = 4;
    /* 0x080092A0: 0882     strh     r0, [r1, #0x10] */
    *(uint16_t *)(r1 + 16) = r0;   /* store */
    /* 0x080092A2: 1a48     ldr      r0, [pc, #0x68]  ; =RAM_200001B8 */
    r0 = *(uint32_t *)0x0800930C;   /* = RAM 0x200001B8 */
    /* 0x080092A4: 0670     strb     r6, [r0] */
    *(uint8_t *)(r0) = r6;   /* store */

    L_080092A6:
    /* 0x080092A6: fff747fa bl       #0x8008738 */
    FUN_08008738();   /* BL -> 0x08008738 FUN_08008738 */
    /* 0x080092AA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080092AC: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */

    L_080092B0:
    /* 0x080092B0: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_080092B2:
    /* 0x080092B2: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x080092B4: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x080092B6: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080092B8: fef7b5f8 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x080092BC: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x080092BE: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x080092C2: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x080092C4: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x080092C6: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080092C8: fef7adf8 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x080092CC: 2888     ldrh     r0, [r5] */
    r0 = *(uint16_t *)(r5);
    /* 0x080092CE: 03f0b3fb bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x080092D2: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x080092D4: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x080092D6: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080092D8: fef7a5f8 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x080092DC: 0948     ldr      r0, [pc, #0x24]  ; =0x00000BB8 */
    r0 = *(uint32_t *)0x08009304;   /* = 0x00000BB8 */
    /* 0x080092DE: 03f0abfb bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x080092E2: 3e70     strb     r6, [r7] */
    *(uint8_t *)(r7) = r6;   /* store */
    /* 0x080092E4: dfe7     b        #0x80092a6 */
    goto L_080092A6;   /* loop */
}
