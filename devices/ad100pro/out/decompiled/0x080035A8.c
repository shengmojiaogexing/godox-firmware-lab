/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080035A8
 * Name      : FUN_080035A8
 * End       : 0x080036D4
 * Size      : 300 bytes, 147 instructions decoded in body (functions.json insns=147, blocks=25)
 * Class     : function
 * Callers   : 5  -> 0x08003AC8 FUN_08003AC8, 0x0800C080 FUN_0800C080, 0x0800C0C8 FUN_0800C0C8, 0x0800CF74 main, 0x0800D014 FUN_0800D014
 * Callees   : 3  -> 0x08006BA4 FUN_08006BA4, 0x08006C44 FUN_08006C44, 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080035A8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080035A8: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x080035AA: 85b0     sub      sp, #0x14 */
    sp = sp - 0x14;
    /* 0x080035AC: 1022     movs     r2, #0x10 */
    r2 = 0x10;
    /* 0x080035AE: 4949     ldr      r1, [pc, #0x124]  ; =RAM_2000048C */
    r1 = *(uint32_t *)0x080036D4;   /* = RAM 0x2000048C */
    /* 0x080035B0: 4948     ldr      r0, [pc, #0x124]  ; =RAM_200003AC */
    r0 = *(uint32_t *)0x080036D8;   /* = RAM 0x200003AC */
    /* 0x080035B2: 03f0f7fa bl       #0x8006ba4 */
    FUN_08006BA4();   /* BL -> 0x08006BA4 FUN_08006BA4 */
    /* 0x080035B6: ff20     movs     r0, #0xff */
    r0 = 0xff;
    /* 0x080035B8: 2d30     adds     r0, #0x2d */
    r0 = r0 + 0x2d;
    /* 0x080035BA: 09f03dfa bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x080035BE: 4648     ldr      r0, [pc, #0x118]  ; =RAM_200003AC */
    r0 = *(uint32_t *)0x080036D8;   /* = RAM 0x200003AC */
    /* 0x080035C0: 03f040fb bl       #0x8006c44 */
    FUN_08006C44();   /* BL -> 0x08006C44 FUN_08006C44 */
    /* 0x080035C4: 434f     ldr      r7, [pc, #0x10c]  ; =RAM_2000048C */
    r7 = *(uint32_t *)0x080036D4;   /* = RAM 0x2000048C */
    /* 0x080035C6: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x080035C8: f868     ldr      r0, [r7, #0xc] */
    r0 = *(uint32_t *)(r7 + 12);
    /* 0x080035CA: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x080035CC: f869     ldr      r0, [r7, #0x1c] */
    r0 = *(uint32_t *)(r7 + 28);
    /* 0x080035CE: 0190     str      r0, [sp, #4] */
    *(uint32_t *)(sp + 4) = r0;   /* store */
    /* 0x080035D0: f86a     ldr      r0, [r7, #0x2c] */
    r0 = *(uint32_t *)(r7 + 44);
    /* 0x080035D2: 0290     str      r0, [sp, #8] */
    *(uint32_t *)(sp + 8) = r0;   /* store */
    /* 0x080035D4: f86b     ldr      r0, [r7, #0x3c] */
    r0 = *(uint32_t *)(r7 + 60);
    /* 0x080035D6: 0390     str      r0, [sp, #0xc] */
    *(uint32_t *)(sp + 12) = r0;   /* store */
    /* 0x080035D8: 6846     mov      r0, sp */
    r0 = sp;

    L_080035DA:
    /* 0x080035DA: 0021     movs     r1, #0 */
    r1 = 0;

    L_080035DC:
    /* 0x080035DC: 8c00     lsls     r4, r1, #2 */
    r4 = r1 << 2;
    /* 0x080035DE: 2518     adds     r5, r4, r0 */
    r5 = r4 + r0;
    /* 0x080035E0: 0659     ldr      r6, [r0, r4] */
    r6 = *(uint32_t *)(r0 + r4);
    /* 0x080035E2: 6b68     ldr      r3, [r5, #4] */
    r3 = *(uint32_t *)(r5 + 4);
    /* 0x080035E4: 9e42     cmp      r6, r3 */
    /* flags = cmp(r6, r3) */
    /* 0x080035E6: 01d9     bls      #0x80035ec */
    if (r6 <= r3) goto L_080035EC;   /* unsigned */
    /* 0x080035E8: 6e60     str      r6, [r5, #4] */
    *(uint32_t *)(r5 + 4) = r6;   /* store */
    /* 0x080035EA: 0351     str      r3, [r0, r4] */
    *(uint32_t *)(r0 + r4) = r3;   /* store */

    L_080035EC:
    /* 0x080035EC: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x080035EE: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x080035F0: 0329     cmp      r1, #3 */
    /* flags = cmp(r1, #3) */
    /* 0x080035F2: f3d3     blo      #0x80035dc */
    if (r1 < 3) goto L_080035DC;   /* unsigned */
    /* 0x080035F4: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x080035F6: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);
    /* 0x080035F8: 042a     cmp      r2, #4 */
    /* flags = cmp(r2, #4) */
    /* 0x080035FA: eed3     blo      #0x80035da */
    if (r2 < 4) goto L_080035DA;   /* unsigned */
    /* 0x080035FC: 029a     ldr      r2, [sp, #8] */
    r2 = *(uint32_t *)(sp + 8);
    /* 0x080035FE: 0199     ldr      r1, [sp, #4] */
    r1 = *(uint32_t *)(sp + 4);
    /* 0x08003600: 8918     adds     r1, r1, r2 */
    r1 = r1 + r2;
    /* 0x08003602: 4908     lsrs     r1, r1, #1 */
    r1 = r1 >> 1;
    /* 0x08003604: 354a     ldr      r2, [pc, #0xd4]  ; =RAM_200001E0 */
    r2 = *(uint32_t *)0x080036DC;   /* = RAM 0x200001E0 */
    /* 0x08003606: 0e31     adds     r1, #0xe */
    r1 = r1 + 0xe;
    /* 0x08003608: 1185     strh     r1, [r2, #0x28] */
    *(uint16_t *)(r2 + 40) = r1;   /* store */
    /* 0x0800360A: 7968     ldr      r1, [r7, #4] */
    r1 = *(uint32_t *)(r7 + 4);
    /* 0x0800360C: 0091     str      r1, [sp] */
    *(uint32_t *)(sp) = r1;   /* store */
    /* 0x0800360E: 7969     ldr      r1, [r7, #0x14] */
    r1 = *(uint32_t *)(r7 + 20);
    /* 0x08003610: 0191     str      r1, [sp, #4] */
    *(uint32_t *)(sp + 4) = r1;   /* store */
    /* 0x08003612: 796a     ldr      r1, [r7, #0x24] */
    r1 = *(uint32_t *)(r7 + 36);
    /* 0x08003614: 0291     str      r1, [sp, #8] */
    *(uint32_t *)(sp + 8) = r1;   /* store */
    /* 0x08003616: 796b     ldr      r1, [r7, #0x34] */
    r1 = *(uint32_t *)(r7 + 52);
    /* 0x08003618: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800361A: 0391     str      r1, [sp, #0xc] */
    *(uint32_t *)(sp + 12) = r1;   /* store */

    L_0800361C:
    /* 0x0800361C: 0021     movs     r1, #0 */
    r1 = 0;

    L_0800361E:
    /* 0x0800361E: 8c00     lsls     r4, r1, #2 */
    r4 = r1 << 2;
    /* 0x08003620: 2518     adds     r5, r4, r0 */
    r5 = r4 + r0;
    /* 0x08003622: 0659     ldr      r6, [r0, r4] */
    r6 = *(uint32_t *)(r0 + r4);
    /* 0x08003624: 6b68     ldr      r3, [r5, #4] */
    r3 = *(uint32_t *)(r5 + 4);
    /* 0x08003626: 9e42     cmp      r6, r3 */
    /* flags = cmp(r6, r3) */
    /* 0x08003628: 01d9     bls      #0x800362e */
    if (r6 <= r3) goto L_0800362E;   /* unsigned */
    /* 0x0800362A: 6e60     str      r6, [r5, #4] */
    *(uint32_t *)(r5 + 4) = r6;   /* store */
    /* 0x0800362C: 0351     str      r3, [r0, r4] */
    *(uint32_t *)(r0 + r4) = r3;   /* store */

    L_0800362E:
    /* 0x0800362E: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x08003630: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x08003632: 0329     cmp      r1, #3 */
    /* flags = cmp(r1, #3) */
    /* 0x08003634: f3d3     blo      #0x800361e */
    if (r1 < 3) goto L_0800361E;   /* unsigned */
    /* 0x08003636: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x08003638: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);
    /* 0x0800363A: 042a     cmp      r2, #4 */
    /* flags = cmp(r2, #4) */
    /* 0x0800363C: eed3     blo      #0x800361c */
    if (r2 < 4) goto L_0800361C;   /* unsigned */
    /* 0x0800363E: 029a     ldr      r2, [sp, #8] */
    r2 = *(uint32_t *)(sp + 8);
    /* 0x08003640: 0199     ldr      r1, [sp, #4] */
    r1 = *(uint32_t *)(sp + 4);
    /* 0x08003642: 8918     adds     r1, r1, r2 */
    r1 = r1 + r2;
    /* 0x08003644: 4908     lsrs     r1, r1, #1 */
    r1 = r1 >> 1;
    /* 0x08003646: 254a     ldr      r2, [pc, #0x94]  ; =RAM_200001E0 */
    r2 = *(uint32_t *)0x080036DC;   /* = RAM 0x200001E0 */
    /* 0x08003648: 0a31     adds     r1, #0xa */
    r1 = r1 + 0xa;
    /* 0x0800364A: 5182     strh     r1, [r2, #0x12] */
    *(uint16_t *)(r2 + 18) = r1;   /* store */
    /* 0x0800364C: b968     ldr      r1, [r7, #8] */
    r1 = *(uint32_t *)(r7 + 8);
    /* 0x0800364E: 0091     str      r1, [sp] */
    *(uint32_t *)(sp) = r1;   /* store */
    /* 0x08003650: b969     ldr      r1, [r7, #0x18] */
    r1 = *(uint32_t *)(r7 + 24);
    /* 0x08003652: 0191     str      r1, [sp, #4] */
    *(uint32_t *)(sp + 4) = r1;   /* store */
    /* 0x08003654: b96a     ldr      r1, [r7, #0x28] */
    r1 = *(uint32_t *)(r7 + 40);
    /* 0x08003656: 0291     str      r1, [sp, #8] */
    *(uint32_t *)(sp + 8) = r1;   /* store */
    /* 0x08003658: b96b     ldr      r1, [r7, #0x38] */
    r1 = *(uint32_t *)(r7 + 56);
    /* 0x0800365A: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800365C: 0391     str      r1, [sp, #0xc] */
    *(uint32_t *)(sp + 12) = r1;   /* store */

    L_0800365E:
    /* 0x0800365E: 0021     movs     r1, #0 */
    r1 = 0;

    L_08003660:
    /* 0x08003660: 8c00     lsls     r4, r1, #2 */
    r4 = r1 << 2;
    /* 0x08003662: 2518     adds     r5, r4, r0 */
    r5 = r4 + r0;
    /* 0x08003664: 0659     ldr      r6, [r0, r4] */
    r6 = *(uint32_t *)(r0 + r4);
    /* 0x08003666: 6b68     ldr      r3, [r5, #4] */
    r3 = *(uint32_t *)(r5 + 4);
    /* 0x08003668: 9e42     cmp      r6, r3 */
    /* flags = cmp(r6, r3) */
    /* 0x0800366A: 01d9     bls      #0x8003670 */
    if (r6 <= r3) goto L_08003670;   /* unsigned */
    /* 0x0800366C: 6e60     str      r6, [r5, #4] */
    *(uint32_t *)(r5 + 4) = r6;   /* store */
    /* 0x0800366E: 0351     str      r3, [r0, r4] */
    *(uint32_t *)(r0 + r4) = r3;   /* store */

    L_08003670:
    /* 0x08003670: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x08003672: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x08003674: 0329     cmp      r1, #3 */
    /* flags = cmp(r1, #3) */
    /* 0x08003676: f3d3     blo      #0x8003660 */
    if (r1 < 3) goto L_08003660;   /* unsigned */
    /* 0x08003678: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x0800367A: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);
    /* 0x0800367C: 042a     cmp      r2, #4 */
    /* flags = cmp(r2, #4) */
    /* 0x0800367E: eed3     blo      #0x800365e */
    if (r2 < 4) goto L_0800365E;   /* unsigned */
    /* 0x08003680: 029a     ldr      r2, [sp, #8] */
    r2 = *(uint32_t *)(sp + 8);
    /* 0x08003682: 0199     ldr      r1, [sp, #4] */
    r1 = *(uint32_t *)(sp + 4);
    /* 0x08003684: 8918     adds     r1, r1, r2 */
    r1 = r1 + r2;
    /* 0x08003686: 4908     lsrs     r1, r1, #1 */
    r1 = r1 >> 1;
    /* 0x08003688: 144a     ldr      r2, [pc, #0x50]  ; =RAM_200001E0 */
    r2 = *(uint32_t *)0x080036DC;   /* = RAM 0x200001E0 */
    /* 0x0800368A: 0a31     adds     r1, #0xa */
    r1 = r1 + 0xa;
    /* 0x0800368C: 5185     strh     r1, [r2, #0x2a] */
    *(uint16_t *)(r2 + 42) = r1;   /* store */
    /* 0x0800368E: 3968     ldr      r1, [r7] */
    r1 = *(uint32_t *)(r7);
    /* 0x08003690: 0091     str      r1, [sp] */
    *(uint32_t *)(sp) = r1;   /* store */
    /* 0x08003692: 3969     ldr      r1, [r7, #0x10] */
    r1 = *(uint32_t *)(r7 + 16);
    /* 0x08003694: 0191     str      r1, [sp, #4] */
    *(uint32_t *)(sp + 4) = r1;   /* store */
    /* 0x08003696: 396a     ldr      r1, [r7, #0x20] */
    r1 = *(uint32_t *)(r7 + 32);
    /* 0x08003698: 0291     str      r1, [sp, #8] */
    *(uint32_t *)(sp + 8) = r1;   /* store */
    /* 0x0800369A: 396b     ldr      r1, [r7, #0x30] */
    r1 = *(uint32_t *)(r7 + 48);
    /* 0x0800369C: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800369E: 0391     str      r1, [sp, #0xc] */
    *(uint32_t *)(sp + 12) = r1;   /* store */

    L_080036A0:
    /* 0x080036A0: 0021     movs     r1, #0 */
    r1 = 0;

    L_080036A2:
    /* 0x080036A2: 8c00     lsls     r4, r1, #2 */
    r4 = r1 << 2;
    /* 0x080036A4: 2518     adds     r5, r4, r0 */
    r5 = r4 + r0;
    /* 0x080036A6: 0659     ldr      r6, [r0, r4] */
    r6 = *(uint32_t *)(r0 + r4);
    /* 0x080036A8: 6b68     ldr      r3, [r5, #4] */
    r3 = *(uint32_t *)(r5 + 4);
    /* 0x080036AA: 9e42     cmp      r6, r3 */
    /* flags = cmp(r6, r3) */
    /* 0x080036AC: 01d9     bls      #0x80036b2 */
    if (r6 <= r3) goto L_080036B2;   /* unsigned */
    /* 0x080036AE: 6e60     str      r6, [r5, #4] */
    *(uint32_t *)(r5 + 4) = r6;   /* store */
    /* 0x080036B0: 0351     str      r3, [r0, r4] */
    *(uint32_t *)(r0 + r4) = r3;   /* store */

    L_080036B2:
    /* 0x080036B2: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x080036B4: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x080036B6: 0329     cmp      r1, #3 */
    /* flags = cmp(r1, #3) */
    /* 0x080036B8: f3d3     blo      #0x80036a2 */
    if (r1 < 3) goto L_080036A2;   /* unsigned */
    /* 0x080036BA: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x080036BC: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);
    /* 0x080036BE: 042a     cmp      r2, #4 */
    /* flags = cmp(r2, #4) */
    /* 0x080036C0: eed3     blo      #0x80036a0 */
    if (r2 < 4) goto L_080036A0;   /* unsigned */
    /* 0x080036C2: 0299     ldr      r1, [sp, #8] */
    r1 = *(uint32_t *)(sp + 8);
    /* 0x080036C4: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x080036C6: 4018     adds     r0, r0, r1 */
    r0 = r0 + r1;
    /* 0x080036C8: 4008     lsrs     r0, r0, #1 */
    r0 = r0 >> 1;
    /* 0x080036CA: 0449     ldr      r1, [pc, #0x10]  ; =RAM_200001E0 */
    r1 = *(uint32_t *)0x080036DC;   /* = RAM 0x200001E0 */
    /* 0x080036CC: 0a30     adds     r0, #0xa */
    r0 = r0 + 0xa;
    /* 0x080036CE: 8885     strh     r0, [r1, #0x2c] */
    *(uint16_t *)(r1 + 44) = r0;   /* store */
    /* 0x080036D0: 05b0     add      sp, #0x14 */
    sp = sp + 0x14;
    /* 0x080036D2: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
