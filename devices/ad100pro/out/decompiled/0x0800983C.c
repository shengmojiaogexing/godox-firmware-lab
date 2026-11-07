/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800983C
 * Name      : FUN_0800983C
 * End       : 0x080098F2
 * Size      : 182 bytes, 91 instructions decoded in body (functions.json insns=91, blocks=28)
 * Class     : function
 * Callers   : 2  -> 0x08008D00 dispatcher_large, 0x08008F98 FUN_08008F98
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800983C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800983C: 2d48     ldr      r0, [pc, #0xb4]  ; =RAM_20000164 */
    r0 = *(uint32_t *)0x080098F4;   /* = RAM 0x20000164 */
    /* 0x0800983E: 017a     ldrb     r1, [r0, #8] */
    r1 = *(uint8_t *)(r0 + 8);
    /* 0x08009840: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x08009842: 07d0     beq      #0x8009854 */
    if (r1 == 0) goto L_08009854;
    /* 0x08009844: 418d     ldrh     r1, [r0, #0x2a] */
    r1 = *(uint16_t *)(r0 + 42);
    /* 0x08009846: e122     movs     r2, #0xe1 */
    r2 = 0xe1;
    /* 0x08009848: 1201     lsls     r2, r2, #4 */
    r2 = r2 << 4;
    /* 0x0800984A: 9142     cmp      r1, r2 */
    /* flags = cmp(r1, r2) */
    /* 0x0800984C: 01d2     bhs      #0x8009852 */
    if (r1 >= r2) goto L_08009852;   /* unsigned */
    /* 0x0800984E: c831     adds     r1, #0xc8 */
    r1 = r1 + 0xc8;
    /* 0x08009850: 4185     strh     r1, [r0, #0x2a] */
    *(uint16_t *)(r0 + 42) = r1;   /* store */

    L_08009852:
    /* 0x08009852: 7047     bx       lr */
    return;

    L_08009854:
    /* 0x08009854: 2749     ldr      r1, [pc, #0x9c]  ; =RAM_20000164 */
    r1 = *(uint32_t *)0x080098F4;   /* = RAM 0x20000164 */
    /* 0x08009856: c278     ldrb     r2, [r0, #3] */
    r2 = *(uint8_t *)(r0 + 3);
    /* 0x08009858: 497c     ldrb     r1, [r1, #0x11] */
    r1 = *(uint8_t *)(r1 + 17);
    /* 0x0800985A: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800985C: 12d0     beq      #0x8009884 */
    if (r2 == 1) goto L_08009884;
    /* 0x0800985E: 417d     ldrb     r1, [r0, #0x15] */
    r1 = *(uint8_t *)(r0 + 21);
    /* 0x08009860: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08009862: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x08009864: 28d0     beq      #0x80098b8 */
    if (r1 == 1) goto L_080098B8;
    /* 0x08009866: 2349     ldr      r1, [pc, #0x8c]  ; =RAM_20000164 */
    r1 = *(uint32_t *)0x080098F4;   /* = RAM 0x20000164 */
    /* 0x08009868: 2031     adds     r1, #0x20 */
    r1 = r1 + 0x20;
    /* 0x0800986A: 8b78     ldrb     r3, [r1, #2] */
    r3 = *(uint8_t *)(r1 + 2);
    /* 0x0800986C: 012b     cmp      r3, #1 */
    /* flags = cmp(r3, #1) */
    /* 0x0800986E: 25d0     beq      #0x80098bc */
    if (r3 == 1) goto L_080098BC;
    /* 0x08009870: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x08009872: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x08009874: 02d0     beq      #0x800987c */
    if (r1 == 1) goto L_0800987C;
    /* 0x08009876: 817c     ldrb     r1, [r0, #0x12] */
    r1 = *(uint8_t *)(r0 + 18);
    /* 0x08009878: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x0800987A: 21d0     beq      #0x80098c0 */
    if (r1 == 0) goto L_080098C0;

    L_0800987C:
    /* 0x0800987C: 0270     strb     r2, [r0] */
    *(uint8_t *)(r0) = r2;   /* store */
    /* 0x0800987E: 8274     strb     r2, [r0, #0x12] */
    *(uint8_t *)(r0 + 18) = r2;   /* store */
    /* 0x08009880: 4263     str      r2, [r0, #0x34] */
    *(uint32_t *)(r0 + 52) = r2;   /* store */

    L_08009882:
    /* 0x08009882: 7047     bx       lr */
    return;

    L_08009884:
    /* 0x08009884: c906     lsls     r1, r1, #0x1b */
    r1 = r1 << 0x1b;
    /* 0x08009886: 08d5     bpl      #0x800989a */
    if (r1 >= 0) goto L_0800989A;   /* result non-negative */
    /* 0x08009888: c18c     ldrh     r1, [r0, #0x26] */
    r1 = *(uint16_t *)(r0 + 38);
    /* 0x0800988A: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x0800988C: 89b2     uxth     r1, r1 */
    r1 = uxth(r1);
    /* 0x0800988E: c184     strh     r1, [r0, #0x26] */
    *(uint16_t *)(r0 + 38) = r1;   /* store */
    /* 0x08009890: 3229     cmp      r1, #0x32 */
    /* flags = cmp(r1, #0x32) */
    /* 0x08009892: f6d9     bls      #0x8009882 */
    if (r1 <= 0x32) goto L_08009882;   /* unsigned */
    /* 0x08009894: 3221     movs     r1, #0x32 */
    r1 = 0x32;
    /* 0x08009896: c184     strh     r1, [r0, #0x26] */
    *(uint16_t *)(r0 + 38) = r1;   /* store */
    /* 0x08009898: 7047     bx       lr */
    return;

    L_0800989A:
    /* 0x0800989A: 018d     ldrh     r1, [r0, #0x28] */
    r1 = *(uint16_t *)(r0 + 40);
    /* 0x0800989C: 491d     adds     r1, r1, #5 */
    r1 = r1 + 5;
    /* 0x0800989E: 89b2     uxth     r1, r1 */
    r1 = uxth(r1);
    /* 0x080098A0: 0185     strh     r1, [r0, #0x28] */
    *(uint16_t *)(r0 + 40) = r1;   /* store */
    /* 0x080098A2: 6429     cmp      r1, #0x64 */
    /* flags = cmp(r1, #0x64) */
    /* 0x080098A4: 02d2     bhs      #0x80098ac */
    if (r1 >= 0x64) goto L_080098AC;   /* unsigned */
    /* 0x080098A6: 6421     movs     r1, #0x64 */
    r1 = 0x64;
    /* 0x080098A8: 0185     strh     r1, [r0, #0x28] */
    *(uint16_t *)(r0 + 40) = r1;   /* store */

    L_080098AA:
    /* 0x080098AA: 7047     bx       lr */
    return;

    L_080098AC:
    /* 0x080098AC: ff22     movs     r2, #0xff */
    r2 = 0xff;
    /* 0x080098AE: 4b32     adds     r2, #0x4b */
    r2 = r2 + 0x4b;
    /* 0x080098B0: 9142     cmp      r1, r2 */
    /* flags = cmp(r1, r2) */
    /* 0x080098B2: fad9     bls      #0x80098aa */
    if (r1 <= r2) goto L_080098AA;   /* unsigned */
    /* 0x080098B4: 0285     strh     r2, [r0, #0x28] */
    *(uint16_t *)(r0 + 40) = r2;   /* store */
    /* 0x080098B6: 7047     bx       lr */
    return;

    L_080098B8:
    /* 0x080098B8: 4275     strb     r2, [r0, #0x15] */
    *(uint8_t *)(r0 + 21) = r2;   /* store */
    /* 0x080098BA: 7047     bx       lr */
    return;

    L_080098BC:
    /* 0x080098BC: 8a70     strb     r2, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r2;   /* store */
    /* 0x080098BE: 0ee0     b        #0x80098de */
    goto L_080098DE;

    L_080098C0:
    /* 0x080098C0: 0c49     ldr      r1, [pc, #0x30]  ; =RAM_20000164 */
    r1 = *(uint32_t *)0x080098F4;   /* = RAM 0x20000164 */
    /* 0x080098C2: 037e     ldrb     r3, [r0, #0x18] */
    r3 = *(uint8_t *)(r0 + 24);
    /* 0x080098C4: 497c     ldrb     r1, [r1, #0x11] */
    r1 = *(uint8_t *)(r1 + 17);
    /* 0x080098C6: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x080098C8: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x080098CA: 002b     cmp      r3, #0 */
    /* flags = cmp(r3, #0) */
    /* 0x080098CC: 4174     strb     r1, [r0, #0x11] */
    *(uint8_t *)(r0 + 17) = r1;   /* store */
    /* 0x080098CE: 08d0     beq      #0x80098e2 */
    if (r3 == 0) goto L_080098E2;
    /* 0x080098D0: 8b07     lsls     r3, r1, #0x1e */
    r3 = r1 << 0x1e;
    /* 0x080098D2: 9b0f     lsrs     r3, r3, #0x1e */
    r3 = r3 >> 0x1e;
    /* 0x080098D4: 022b     cmp      r3, #2 */
    /* flags = cmp(r3, #2) */
    /* 0x080098D6: 02d9     bls      #0x80098de */
    if (r3 <= 2) goto L_080098DE;   /* unsigned */
    /* 0x080098D8: 8908     lsrs     r1, r1, #2 */
    r1 = r1 >> 2;
    /* 0x080098DA: 8900     lsls     r1, r1, #2 */
    r1 = r1 << 2;

    L_080098DC:
    /* 0x080098DC: 4174     strb     r1, [r0, #0x11] */
    *(uint8_t *)(r0 + 17) = r1;   /* store */

    L_080098DE:
    /* 0x080098DE: c274     strb     r2, [r0, #0x13] */
    *(uint8_t *)(r0 + 19) = r2;   /* store */
    /* 0x080098E0: 7047     bx       lr */
    return;

    L_080098E2:
    /* 0x080098E2: 8b07     lsls     r3, r1, #0x1e */
    r3 = r1 << 0x1e;
    /* 0x080098E4: 9b0f     lsrs     r3, r3, #0x1e */
    r3 = r3 >> 0x1e;
    /* 0x080098E6: 022b     cmp      r3, #2 */
    /* flags = cmp(r3, #2) */
    /* 0x080098E8: f9d9     bls      #0x80098de */
    if (r3 <= 2) goto L_080098DE;   /* unsigned */
    /* 0x080098EA: 8908     lsrs     r1, r1, #2 */
    r1 = r1 >> 2;
    /* 0x080098EC: 8900     lsls     r1, r1, #2 */
    r1 = r1 << 2;
    /* 0x080098EE: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x080098F0: f4e7     b        #0x80098dc */
    goto L_080098DC;   /* loop */
}
