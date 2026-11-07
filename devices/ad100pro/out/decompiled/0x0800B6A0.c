/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B6A0
 * Name      : FUN_0800B6A0
 * End       : 0x0800B712
 * Size      : 114 bytes, 57 instructions decoded in body (functions.json insns=57, blocks=19)
 * Class     : function
 * Callers   : 2  -> 0x08008D00 dispatcher_large, 0x08008F9E FUN_08008F9E
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B6A0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B6A0: 1c48     ldr      r0, [pc, #0x70]  ; =RAM_20000164 */
    r0 = *(uint32_t *)0x0800B714;   /* = RAM 0x20000164 */
    /* 0x0800B6A2: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800B6A4: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x0800B6A6: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x0800B6A8: 02d0     beq      #0x800b6b0 */
    if (r1 == 1) goto L_0800B6B0;
    /* 0x0800B6AA: 817c     ldrb     r1, [r0, #0x12] */
    r1 = *(uint8_t *)(r0 + 18);
    /* 0x0800B6AC: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x0800B6AE: 03d0     beq      #0x800b6b8 */
    if (r1 == 0) goto L_0800B6B8;

    L_0800B6B0:
    /* 0x0800B6B0: 0270     strb     r2, [r0] */
    *(uint8_t *)(r0) = r2;   /* store */
    /* 0x0800B6B2: 8274     strb     r2, [r0, #0x12] */
    *(uint8_t *)(r0 + 18) = r2;   /* store */
    /* 0x0800B6B4: 4263     str      r2, [r0, #0x34] */
    *(uint32_t *)(r0 + 52) = r2;   /* store */

    L_0800B6B6:
    /* 0x0800B6B6: 7047     bx       lr */
    return;

    L_0800B6B8:
    /* 0x0800B6B8: c378     ldrb     r3, [r0, #3] */
    r3 = *(uint8_t *)(r0 + 3);
    /* 0x0800B6BA: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800B6BC: 012b     cmp      r3, #1 */
    /* flags = cmp(r3, #1) */
    /* 0x0800B6BE: 17d0     beq      #0x800b6f0 */
    if (r3 == 1) goto L_0800B6F0;
    /* 0x0800B6C0: 437d     ldrb     r3, [r0, #0x15] */
    r3 = *(uint8_t *)(r0 + 21);
    /* 0x0800B6C2: 012b     cmp      r3, #1 */
    /* flags = cmp(r3, #1) */
    /* 0x0800B6C4: 18d0     beq      #0x800b6f8 */
    if (r3 == 1) goto L_0800B6F8;
    /* 0x0800B6C6: 134b     ldr      r3, [pc, #0x4c]  ; =RAM_20000164 */
    r3 = *(uint32_t *)0x0800B714;   /* = RAM 0x20000164 */
    /* 0x0800B6C8: 2033     adds     r3, #0x20 */
    r3 = r3 + 0x20;
    /* 0x0800B6CA: 9b78     ldrb     r3, [r3, #2] */
    r3 = *(uint8_t *)(r3 + 2);
    /* 0x0800B6CC: 012b     cmp      r3, #1 */
    /* flags = cmp(r3, #1) */
    /* 0x0800B6CE: 17d0     beq      #0x800b700 */
    if (r3 == 1) goto L_0800B700;
    /* 0x0800B6D0: 417c     ldrb     r1, [r0, #0x11] */
    r1 = *(uint8_t *)(r0 + 17);
    /* 0x0800B6D2: 8b07     lsls     r3, r1, #0x1e */
    r3 = r1 << 0x1e;
    /* 0x0800B6D4: 0ad0     beq      #0x800b6ec */
    if (r3 == 1) goto L_0800B6EC;
    /* 0x0800B6D6: 8907     lsls     r1, r1, #0x1e */
    r1 = r1 << 0x1e;
    /* 0x0800B6D8: 890f     lsrs     r1, r1, #0x1e */
    r1 = r1 >> 0x1e;
    /* 0x0800B6DA: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x0800B6DC: 06d0     beq      #0x800b6ec */
    if (r1 == 1) goto L_0800B6EC;
    /* 0x0800B6DE: 0229     cmp      r1, #2 */
    /* flags = cmp(r1, #2) */
    /* 0x0800B6E0: e9d1     bne      #0x800b6b6 */
    if (r1 != 2) goto L_0800B6B6;
    /* 0x0800B6E2: c17c     ldrb     r1, [r0, #0x13] */
    r1 = *(uint8_t *)(r0 + 19);
    /* 0x0800B6E4: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x0800B6E6: 0fd0     beq      #0x800b708 */
    if (r1 == 0) goto L_0800B708;
    /* 0x0800B6E8: 0329     cmp      r1, #3 */
    /* flags = cmp(r1, #3) */
    /* 0x0800B6EA: 0fd0     beq      #0x800b70c */
    if (r1 == 3) goto L_0800B70C;

    L_0800B6EC:
    /* 0x0800B6EC: c274     strb     r2, [r0, #0x13] */
    *(uint8_t *)(r0 + 19) = r2;   /* store */
    /* 0x0800B6EE: 7047     bx       lr */
    return;

    L_0800B6F0:
    /* 0x0800B6F0: 0279     ldrb     r2, [r0, #4] */
    r2 = *(uint8_t *)(r0 + 4);
    /* 0x0800B6F2: 4a40     eors     r2, r1 */
    r2 = r2 ^ r1;
    /* 0x0800B6F4: 0271     strb     r2, [r0, #4] */
    *(uint8_t *)(r0 + 4) = r2;   /* store */
    /* 0x0800B6F6: 7047     bx       lr */
    return;

    L_0800B6F8:
    /* 0x0800B6F8: 827d     ldrb     r2, [r0, #0x16] */
    r2 = *(uint8_t *)(r0 + 22);
    /* 0x0800B6FA: 4a40     eors     r2, r1 */
    r2 = r2 ^ r1;
    /* 0x0800B6FC: 8275     strb     r2, [r0, #0x16] */
    *(uint8_t *)(r0 + 22) = r2;   /* store */
    /* 0x0800B6FE: 7047     bx       lr */
    return;

    L_0800B700:
    /* 0x0800B700: 8279     ldrb     r2, [r0, #6] */
    r2 = *(uint8_t *)(r0 + 6);
    /* 0x0800B702: 4a40     eors     r2, r1 */
    r2 = r2 ^ r1;
    /* 0x0800B704: 8271     strb     r2, [r0, #6] */
    *(uint8_t *)(r0 + 6) = r2;   /* store */
    /* 0x0800B706: 7047     bx       lr */
    return;

    L_0800B708:
    /* 0x0800B708: 0321     movs     r1, #3 */
    r1 = 3;
    /* 0x0800B70A: 00e0     b        #0x800b70e */
    goto L_0800B70E;

    L_0800B70C:
    /* 0x0800B70C: 0421     movs     r1, #4 */
    r1 = 4;

    L_0800B70E:
    /* 0x0800B70E: c174     strb     r1, [r0, #0x13] */
    *(uint8_t *)(r0 + 19) = r1;   /* store */
    /* 0x0800B710: 7047     bx       lr */
    return;
}
