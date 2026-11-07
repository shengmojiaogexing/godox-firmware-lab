/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005BC8
 * Name      : FUN_08005BC8
 * End       : 0x08005C2A
 * Size      : 98 bytes, 48 instructions decoded in body (functions.json insns=48, blocks=15)
 * Class     : function
 * Callers   : 2  -> 0x0800C2B8 FUN_0800C2B8, 0x0800C2D8 FUN_0800C2D8
 * Callees   : 1  -> 0x08003C30 FUN_08003C30
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005BC8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005BC8: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x08005BCA: 1848     ldr      r0, [pc, #0x60]  ; =RAM_200001CF */
    r0 = *(uint32_t *)0x08005C2C;   /* = RAM 0x200001CF */
    /* 0x08005BCC: 0126     movs     r6, #1 */
    r6 = 1;
    /* 0x08005BCE: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08005BD0: 174c     ldr      r4, [pc, #0x5c]  ; =RAM_20000228 */
    r4 = *(uint32_t *)0x08005C30;   /* = RAM 0x20000228 */
    /* 0x08005BD2: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x08005BD4: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08005BD6: 09d0     beq      #0x8005bec */
    if (r0 == 1) goto L_08005BEC;
    /* 0x08005BD8: 1648     ldr      r0, [pc, #0x58]  ; =RAM_20000158 */
    r0 = *(uint32_t *)0x08005C34;   /* = RAM 0x20000158 */
    /* 0x08005BDA: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08005BDC: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08005BDE: 07d0     beq      #0x8005bf0 */
    if (r0 == 1) goto L_08005BF0;
    /* 0x08005BE0: 1548     ldr      r0, [pc, #0x54]  ; =RAM_200001D0 */
    r0 = *(uint32_t *)0x08005C38;   /* = RAM 0x200001D0 */
    /* 0x08005BE2: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08005BE4: 7b28     cmp      r0, #0x7b */
    /* flags = cmp(r0, #0x7b) */
    /* 0x08005BE6: 09d0     beq      #0x8005bfc */
    if (r0 == 0x7b) goto L_08005BFC;
    /* 0x08005BE8: e920     movs     r0, #0xe9 */
    r0 = 0xe9;
    /* 0x08005BEA: 02e0     b        #0x8005bf2 */
    goto L_08005BF2;

    L_08005BEC:
    /* 0x08005BEC: e120     movs     r0, #0xe1 */
    r0 = 0xe1;
    /* 0x08005BEE: 00e0     b        #0x8005bf2 */
    goto L_08005BF2;

    L_08005BF0:
    /* 0x08005BF0: e320     movs     r0, #0xe3 */
    r0 = 0xe3;

    L_08005BF2:
    /* 0x08005BF2: 6071     strb     r0, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r0;   /* store */
    /* 0x08005BF4: 2079     ldrb     r0, [r4, #4] */
    r0 = *(uint8_t *)(r4 + 4);
    /* 0x08005BF6: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08005BF8: 03d0     beq      #0x8005c02 */
    if (r0 == 1) goto L_08005C02;
    /* 0x08005BFA: 00e0     b        #0x8005bfe */
    goto L_08005BFE;

    L_08005BFC:
    /* 0x08005BFC: 6571     strb     r5, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r5;   /* store */

    L_08005BFE:
    /* 0x08005BFE: 6670     strb     r6, [r4, #1] */
    *(uint8_t *)(r4 + 1) = r6;   /* store */
    /* 0x08005C00: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_08005C02:
    /* 0x08005C02: 6078     ldrb     r0, [r4, #1] */
    r0 = *(uint8_t *)(r4 + 1);
    /* 0x08005C04: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08005C06: 0ed1     bne      #0x8005c26 */
    if (r0 != 1) goto L_08005C26;
    /* 0x08005C08: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x08005C0A: 0c4f     ldr      r7, [pc, #0x30]  ; =RAM_200001E2 */
    r7 = *(uint32_t *)0x08005C3C;   /* = RAM 0x200001E2 */
    /* 0x08005C0C: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08005C0E: 09d0     beq      #0x8005c24 */
    if (r0 == 0) goto L_08005C24;
    /* 0x08005C10: 3222     movs     r2, #0x32 */
    r2 = 0x32;
    /* 0x08005C12: 1421     movs     r1, #0x14 */
    r1 = 0x14;
    /* 0x08005C14: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x08005C16: fef70bf8 bl       #0x8003c30 */
    FUN_08003C30();   /* BL -> 0x08003C30 FUN_08003C30 */
    /* 0x08005C1A: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x08005C1C: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x08005C1E: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x08005C20: 3e70     strb     r6, [r7] */
    *(uint8_t *)(r7) = r6;   /* store */
    /* 0x08005C22: 00e0     b        #0x8005c26 */
    goto L_08005C26;

    L_08005C24:
    /* 0x08005C24: 3d70     strb     r5, [r7] */
    *(uint8_t *)(r7) = r5;   /* store */

    L_08005C26:
    /* 0x08005C26: 6570     strb     r5, [r4, #1] */
    *(uint8_t *)(r4 + 1) = r5;   /* store */
    /* 0x08005C28: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
