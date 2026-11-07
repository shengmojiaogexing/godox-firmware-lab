/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800663C
 * Name      : FUN_0800663C
 * End       : 0x080066B2
 * Size      : 118 bytes, 56 instructions decoded in body (functions.json insns=56, blocks=20)
 * Class     : function
 * Callers   : 2  -> 0x0800C4E8 FUN_0800C4E8, 0x0800C578 FUN_0800C578
 * Callees   : 3  -> 0x080030F0 FUN_080030F0, 0x080086E0 FUN_080086E0, 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800663C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800663C: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800663E: 02f04ff8 bl       #0x80086e0 */
    FUN_080086E0();   /* BL -> 0x080086E0 FUN_080086E0 */
    /* 0x08006642: 1c48     ldr      r0, [pc, #0x70]  ; =RAM_20000208 */
    r0 = *(uint32_t *)0x080066B4;   /* = RAM 0x20000208 */
    /* 0x08006644: 1c4c     ldr      r4, [pc, #0x70]  ; =RAM_200001C6 */
    r4 = *(uint32_t *)0x080066B8;   /* = RAM 0x200001C6 */
    /* 0x08006646: 0088     ldrh     r0, [r0] */
    r0 = *(uint16_t *)(r0);
    /* 0x08006648: 1c4d     ldr      r5, [pc, #0x70]  ; =RAM_200001D8 */
    r5 = *(uint32_t *)0x080066BC;   /* = RAM 0x200001D8 */
    /* 0x0800664A: 4108     lsrs     r1, r0, #1 */
    r1 = r0 >> 1;
    /* 0x0800664C: fa29     cmp      r1, #0xfa */
    /* flags = cmp(r1, #0xfa) */
    /* 0x0800664E: 0bd2     bhs      #0x8006668 */
    if (r1 >= 0xfa) goto L_08006668;   /* unsigned */
    /* 0x08006650: 2888     ldrh     r0, [r5] */
    r0 = *(uint16_t *)(r5);
    /* 0x08006652: fa22     movs     r2, #0xfa */
    r2 = 0xfa;
    /* 0x08006654: 5043     muls     r0, r2, r0 */
    r0 = r2 * r0;
    /* 0x08006656: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x08006658: fcf74afd bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x0800665C: ff21     movs     r1, #0xff */
    r1 = 0xff;
    /* 0x0800665E: 2d31     adds     r1, #0x2d */
    r1 = r1 + 0x2d;
    /* 0x08006660: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x08006662: 05d9     bls      #0x8006670 */
    if (r0 <= r1) goto L_08006670;   /* unsigned */
    /* 0x08006664: 0846     mov      r0, r1 */
    r0 = r1;
    /* 0x08006666: 03e0     b        #0x8006670 */
    goto L_08006670;

    L_08006668:
    /* 0x08006668: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x0800666A: 06f0e5f9 bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800666E: 2088     ldrh     r0, [r4] */
    r0 = *(uint16_t *)(r4);

    L_08006670:
    /* 0x08006670: 2188     ldrh     r1, [r4] */
    r1 = *(uint16_t *)(r4);
    /* 0x08006672: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x08006674: 05d9     bls      #0x8006682 */
    if (r1 <= r0) goto L_08006682;   /* unsigned */
    /* 0x08006676: 9629     cmp      r1, #0x96 */
    /* flags = cmp(r1, #0x96) */
    /* 0x08006678: 01d2     bhs      #0x800667e */
    if (r1 >= 0x96) goto L_0800667E;   /* unsigned */
    /* 0x0800667A: 491e     subs     r1, r1, #1 */
    r1 = r1 - 1;
    /* 0x0800667C: 08e0     b        #0x8006690 */
    goto L_08006690;

    L_0800667E:
    /* 0x0800667E: 891e     subs     r1, r1, #2 */
    r1 = r1 - 2;
    /* 0x08006680: 06e0     b        #0x8006690 */
    goto L_08006690;

    L_08006682:
    /* 0x08006682: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x08006684: 05d2     bhs      #0x8006692 */
    if (r1 >= r0) goto L_08006692;   /* unsigned */
    /* 0x08006686: 9629     cmp      r1, #0x96 */
    /* flags = cmp(r1, #0x96) */
    /* 0x08006688: 01d2     bhs      #0x800668e */
    if (r1 >= 0x96) goto L_0800668E;   /* unsigned */
    /* 0x0800668A: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x0800668C: 00e0     b        #0x8006690 */
    goto L_08006690;

    L_0800668E:
    /* 0x0800668E: 891c     adds     r1, r1, #2 */
    r1 = r1 + 2;

    L_08006690:
    /* 0x08006690: 2180     strh     r1, [r4] */
    *(uint16_t *)(r4) = r1;   /* store */

    L_08006692:
    /* 0x08006692: 88b2     uxth     r0, r1 */
    r0 = uxth(r1);
    /* 0x08006694: ff21     movs     r1, #0xff */
    r1 = 0xff;
    /* 0x08006696: 5f31     adds     r1, #0x5f */
    r1 = r1 + 0x5f;
    /* 0x08006698: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x0800669A: 01d9     bls      #0x80066a0 */
    if (r0 <= r1) goto L_080066A0;   /* unsigned */
    /* 0x0800669C: 2180     strh     r1, [r4] */
    *(uint16_t *)(r4) = r1;   /* store */
    /* 0x0800669E: 03e0     b        #0x80066a8 */
    goto L_080066A8;

    L_080066A0:
    /* 0x080066A0: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x080066A2: 01d2     bhs      #0x80066a8 */
    if (r0 >= 0xa) goto L_080066A8;   /* unsigned */
    /* 0x080066A4: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x080066A6: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */

    L_080066A8:
    /* 0x080066A8: 2888     ldrh     r0, [r5] */
    r0 = *(uint16_t *)(r5);
    /* 0x080066AA: 3228     cmp      r0, #0x32 */
    /* flags = cmp(r0, #0x32) */
    /* 0x080066AC: 00d2     bhs      #0x80066b0 */
    if (r0 >= 0x32) goto L_080066B0;   /* unsigned */
    /* 0x080066AE: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */

    L_080066B0:
    /* 0x080066B0: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
