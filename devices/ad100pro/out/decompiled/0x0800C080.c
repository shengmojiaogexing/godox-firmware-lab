/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C080
 * Name      : FUN_0800C080
 * End       : 0x0800C1D8
 * Size      : 344 bytes, 140 instructions decoded in body (functions.json insns=140, blocks=20)
 * Class     : hub
 * Callers   : 2  -> 0x0800B9C0 FUN_0800B9C0, 0x0800B9F8 FUN_0800B9F8
 * Callees   : 9  -> 0x080035A8 FUN_080035A8, 0x080072C4 FUN_080072C4, 0x0800741C FUN_0800741C, 0x08007426 FUN_08007426, 0x080074F0 FUN_080074F0, 0x0800A034 FUN_0800A034, 0x0800A140 FUN_0800A140, 0x0800BDC4 FUN_0800BDC4, ... +1
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C080(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C080: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x0800C082: 85b0     sub      sp, #0x14 */
    sp = sp - 0x14;
    /* 0x0800C084: 0820     movs     r0, #8 */
    r0 = 8;
    /* 0x0800C086: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x0800C088: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x0800C08A: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800C08C: 0190     str      r0, [sp, #4] */
    *(uint32_t *)(sp + 4) = r0;   /* store */
    /* 0x0800C08E: 2546     mov      r5, r4 */
    r5 = r4;
    /* 0x0800C090: 0294     str      r4, [sp, #8] */
    *(uint32_t *)(sp + 8) = r4;   /* store */
    /* 0x0800C092: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x0800C094: 0394     str      r4, [sp, #0xc] */
    *(uint32_t *)(sp + 12) = r4;   /* store */
    /* 0x0800C096: 5048     ldr      r0, [pc, #0x140]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x0800C1D8;   /* = MMIO GPIOB+0x00 */
    /* 0x0800C098: fbf714f9 bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x0800C09C: 0127     movs     r7, #1 */
    r7 = 1;
    /* 0x0800C09E: 4f4e     ldr      r6, [pc, #0x13c]  ; =GPIOC.MODER */
    r6 = *(uint32_t *)0x0800C1DC;   /* = MMIO GPIOC+0x00 */
    /* 0x0800C0A0: 3f03     lsls     r7, r7, #0xc */
    r7 = r7 << 0xc;
    /* 0x0800C0A2: 3946     mov      r1, r7 */
    r1 = r7;
    /* 0x0800C0A4: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x0800C0A6: fbf7b9f9 bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x0800C0AA: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800C0AC: 00d1     bne      #0x800c0b0 */
    if (r0 != 0) goto L_0800C0B0;
    /* 0x0800C0AE: 0124     movs     r4, #1 */
    r4 = 1;

    L_0800C0B0:
    /* 0x0800C0B0: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x0800C0B2: 00f0f1fc bl       #0x800ca98 */
    FUN_0800CA98();   /* BL -> 0x0800CA98 FUN_0800CA98 */
    /* 0x0800C0B6: 3946     mov      r1, r7 */
    r1 = r7;
    /* 0x0800C0B8: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x0800C0BA: fbf7aff9 bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x0800C0BE: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800C0C0: 00d1     bne      #0x800c0c4 */
    if (r0 != 0) goto L_0800C0C4;
    /* 0x0800C0C2: 641c     adds     r4, r4, #1 */
    r4 = r4 + 1;

    L_0800C0C4:
    /* 0x0800C0C4: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x0800C0C6: 00f0e7fc bl       #0x800ca98 */
    FUN_0800CA98();   /* BL -> 0x0800CA98 FUN_0800CA98 */
    /* 0x0800C0CA: 3946     mov      r1, r7 */
    r1 = r7;
    /* 0x0800C0CC: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x0800C0CE: fbf7a5f9 bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x0800C0D2: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800C0D4: 00d1     bne      #0x800c0d8 */
    if (r0 != 0) goto L_0800C0D8;
    /* 0x0800C0D6: 641c     adds     r4, r4, #1 */
    r4 = r4 + 1;

    L_0800C0D8:
    /* 0x0800C0D8: 032c     cmp      r4, #3 */
    /* flags = cmp(r4, #3) */
    /* 0x0800C0DA: 7bd1     bne      #0x800c1d4 */
    if (r4 != 3) goto L_0800C1D4;
    /* 0x0800C0DC: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800C0DE: 0821     movs     r1, #8 */
    r1 = 8;
    /* 0x0800C0E0: 3e48     ldr      r0, [pc, #0xf8]  ; =GPIOC.MODER */
    r0 = *(uint32_t *)0x0800C1DC;   /* = MMIO GPIOC+0x00 */
    /* 0x0800C0E2: fbf7a0f9 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800C0E6: 3220     movs     r0, #0x32 */
    r0 = 0x32;
    /* 0x0800C0E8: 00f0d6fc bl       #0x800ca98 */
    FUN_0800CA98();   /* BL -> 0x0800CA98 FUN_0800CA98 */
    /* 0x0800C0EC: f7f75cfa bl       #0x80035a8 */
    FUN_080035A8();   /* BL -> 0x080035A8 FUN_080035A8 */
    /* 0x0800C0F0: 3b4c     ldr      r4, [pc, #0xec]  ; =RAM_200001E0 */
    r4 = *(uint32_t *)0x0800C1E0;   /* = RAM 0x200001E0 */
    /* 0x0800C0F2: 0f20     movs     r0, #0xf */
    r0 = 0xf;
    /* 0x0800C0F4: a073     strb     r0, [r4, #0xe] */
    *(uint8_t *)(r4 + 14) = r0;   /* store */
    /* 0x0800C0F6: e073     strb     r0, [r4, #0xf] */
    *(uint8_t *)(r4 + 15) = r0;   /* store */
    /* 0x0800C0F8: 2074     strb     r0, [r4, #0x10] */
    *(uint8_t *)(r4 + 16) = r0;   /* store */
    /* 0x0800C0FA: ff26     movs     r6, #0xff */
    r6 = 0xff;
    /* 0x0800C0FC: 6673     strb     r6, [r4, #0xd] */
    *(uint8_t *)(r4 + 13) = r6;   /* store */
    /* 0x0800C0FE: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800C100: fff760fe bl       #0x800bdc4 */
    FUN_0800BDC4();   /* BL -> 0x0800BDC4 FUN_0800BDC4 */
    /* 0x0800C104: 6573     strb     r5, [r4, #0xd] */
    *(uint8_t *)(r4 + 13) = r5;   /* store */
    /* 0x0800C106: 0027     movs     r7, #0 */
    r7 = 0;

    L_0800C108:
    /* 0x0800C108: fdf794ff bl       #0x800a034 */
    FUN_0800A034();   /* BL -> 0x0800A034 FUN_0800A034 */
    /* 0x0800C10C: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x0800C10E: 00f0c3fc bl       #0x800ca98 */
    FUN_0800CA98();   /* BL -> 0x0800CA98 FUN_0800CA98 */
    /* 0x0800C112: 3448     ldr      r0, [pc, #0xd0]  ; =RAM_20000384 */
    r0 = *(uint32_t *)0x0800C1E4;   /* = RAM 0x20000384 */
    /* 0x0800C114: fbf7ecf9 bl       #0x80074f0 */
    FUN_080074F0();   /* BL -> 0x080074F0 FUN_080074F0 */
    /* 0x0800C118: 7f1c     adds     r7, r7, #1 */
    r7 = r7 + 1;
    /* 0x0800C11A: ffb2     uxtb     r7, r7 */
    r7 = uxtb(r7);
    /* 0x0800C11C: 642f     cmp      r7, #0x64 */
    /* flags = cmp(r7, #0x64) */
    /* 0x0800C11E: f3d3     blo      #0x800c108 */
    if (r7 < 0x64) goto L_0800C108;   /* unsigned */
    /* 0x0800C120: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800C122: fff74ffe bl       #0x800bdc4 */
    FUN_0800BDC4();   /* BL -> 0x0800BDC4 FUN_0800BDC4 */
    /* 0x0800C126: a673     strb     r6, [r4, #0xe] */
    *(uint8_t *)(r4 + 14) = r6;   /* store */
    /* 0x0800C128: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800C12A: fff74bfe bl       #0x800bdc4 */
    FUN_0800BDC4();   /* BL -> 0x0800BDC4 FUN_0800BDC4 */
    /* 0x0800C12E: a573     strb     r5, [r4, #0xe] */
    *(uint8_t *)(r4 + 14) = r5;   /* store */
    /* 0x0800C130: 0025     movs     r5, #0 */
    r5 = 0;

    L_0800C132:
    /* 0x0800C132: fef705f8 bl       #0x800a140 */
    FUN_0800A140();   /* BL -> 0x0800A140 FUN_0800A140 */
    /* 0x0800C136: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x0800C138: 00f0aefc bl       #0x800ca98 */
    FUN_0800CA98();   /* BL -> 0x0800CA98 FUN_0800CA98 */
    /* 0x0800C13C: 2948     ldr      r0, [pc, #0xa4]  ; =RAM_20000384 */
    r0 = *(uint32_t *)0x0800C1E4;   /* = RAM 0x20000384 */
    /* 0x0800C13E: fbf7d7f9 bl       #0x80074f0 */
    FUN_080074F0();   /* BL -> 0x080074F0 FUN_080074F0 */
    /* 0x0800C142: 6d1c     adds     r5, r5, #1 */
    r5 = r5 + 1;
    /* 0x0800C144: edb2     uxtb     r5, r5 */
    r5 = uxtb(r5);
    /* 0x0800C146: 642d     cmp      r5, #0x64 */
    /* flags = cmp(r5, #0x64) */
    /* 0x0800C148: f3d3     blo      #0x800c132 */
    if (r5 < 0x64) goto L_0800C132;   /* unsigned */
    /* 0x0800C14A: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800C14C: fff73afe bl       #0x800bdc4 */
    FUN_0800BDC4();   /* BL -> 0x0800BDC4 FUN_0800BDC4 */
    /* 0x0800C150: 2674     strb     r6, [r4, #0x10] */
    *(uint8_t *)(r4 + 16) = r6;   /* store */
    /* 0x0800C152: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800C154: fff736fe bl       #0x800bdc4 */
    FUN_0800BDC4();   /* BL -> 0x0800BDC4 FUN_0800BDC4 */
    /* 0x0800C158: 204d     ldr      r5, [pc, #0x80]  ; =GPIOC.MODER */
    r5 = *(uint32_t *)0x0800C1DC;   /* = MMIO GPIOC+0x00 */
    /* 0x0800C15A: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800C15C: 1021     movs     r1, #0x10 */
    r1 = 0x10;
    /* 0x0800C15E: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800C160: fbf761f9 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800C164: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800C166: 2074     strb     r0, [r4, #0x10] */
    *(uint8_t *)(r4 + 16) = r0;   /* store */
    /* 0x0800C168: fff72cfe bl       #0x800bdc4 */
    FUN_0800BDC4();   /* BL -> 0x0800BDC4 FUN_0800BDC4 */
    /* 0x0800C16C: 0026     movs     r6, #0 */
    r6 = 0;
    /* 0x0800C16E: ff27     movs     r7, #0xff */
    r7 = 0xff;
    /* 0x0800C170: f537     adds     r7, #0xf5 */
    r7 = r7 + 0xf5;

    L_0800C172:
    /* 0x0800C172: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x0800C174: 00f090fc bl       #0x800ca98 */
    FUN_0800CA98();   /* BL -> 0x0800CA98 FUN_0800CA98 */
    /* 0x0800C178: 1a48     ldr      r0, [pc, #0x68]  ; =RAM_20000384 */
    r0 = *(uint32_t *)0x0800C1E4;   /* = RAM 0x20000384 */
    /* 0x0800C17A: fbf7b9f9 bl       #0x80074f0 */
    FUN_080074F0();   /* BL -> 0x080074F0 FUN_080074F0 */
    /* 0x0800C17E: 761c     adds     r6, r6, #1 */
    r6 = r6 + 1;
    /* 0x0800C180: f6b2     uxtb     r6, r6 */
    r6 = uxtb(r6);
    /* 0x0800C182: 042e     cmp      r6, #4 */
    /* flags = cmp(r6, #4) */
    /* 0x0800C184: f5d3     blo      #0x800c172 */
    if (r6 < 4) goto L_0800C172;   /* unsigned */
    /* 0x0800C186: 0026     movs     r6, #0 */
    r6 = 0;
    /* 0x0800C188: 0127     movs     r7, #1 */
    r7 = 1;
    /* 0x0800C18A: 7f03     lsls     r7, r7, #0xd */
    r7 = r7 << 0xd;

    L_0800C18C:
    /* 0x0800C18C: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800C18E: 3946     mov      r1, r7 */
    r1 = r7;
    /* 0x0800C190: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800C192: fbf748f9 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800C196: c820     movs     r0, #0xc8 */
    r0 = 0xc8;
    /* 0x0800C198: 00f07efc bl       #0x800ca98 */
    FUN_0800CA98();   /* BL -> 0x0800CA98 FUN_0800CA98 */
    /* 0x0800C19C: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800C19E: 3946     mov      r1, r7 */
    r1 = r7;
    /* 0x0800C1A0: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800C1A2: fbf740f9 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800C1A6: c820     movs     r0, #0xc8 */
    r0 = 0xc8;
    /* 0x0800C1A8: 00f076fc bl       #0x800ca98 */
    FUN_0800CA98();   /* BL -> 0x0800CA98 FUN_0800CA98 */
    /* 0x0800C1AC: 0d48     ldr      r0, [pc, #0x34]  ; =RAM_20000384 */
    r0 = *(uint32_t *)0x0800C1E4;   /* = RAM 0x20000384 */
    /* 0x0800C1AE: fbf79ff9 bl       #0x80074f0 */
    FUN_080074F0();   /* BL -> 0x080074F0 FUN_080074F0 */
    /* 0x0800C1B2: 761c     adds     r6, r6, #1 */
    r6 = r6 + 1;
    /* 0x0800C1B4: f6b2     uxtb     r6, r6 */
    r6 = uxtb(r6);
    /* 0x0800C1B6: 022e     cmp      r6, #2 */
    /* flags = cmp(r6, #2) */
    /* 0x0800C1B8: e8d3     blo      #0x800c18c */
    if (r6 < 2) goto L_0800C18C;   /* unsigned */
    /* 0x0800C1BA: 03e0     b        #0x800c1c4 */
    goto L_0800C1C4;

    L_0800C1BE:
    /* 0x0800C1BE: 0948     ldr      r0, [pc, #0x24]  ; =RAM_20000384 */
    r0 = *(uint32_t *)0x0800C1E4;   /* = RAM 0x20000384 */
    /* 0x0800C1C0: fbf796f9 bl       #0x80074f0 */
    FUN_080074F0();   /* BL -> 0x080074F0 FUN_080074F0 */

    L_0800C1C4:
    /* 0x0800C1C4: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800C1C6: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800C1C8: fbf728f9 bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x0800C1CC: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800C1CE: f6d1     bne      #0x800c1be */
    if (r0 != 0) goto L_0800C1BE;
    /* 0x0800C1D0: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800C1D2: 6071     strb     r0, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r0;   /* store */

    L_0800C1D4:
    /* 0x0800C1D4: 05b0     add      sp, #0x14 */
    sp = sp + 0x14;
    /* 0x0800C1D6: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
