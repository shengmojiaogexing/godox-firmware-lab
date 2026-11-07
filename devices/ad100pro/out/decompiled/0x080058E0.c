/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080058E0
 * Name      : FUN_080058E0
 * End       : 0x08005962
 * Size      : 130 bytes, 57 instructions decoded in body (functions.json insns=57, blocks=3)
 * Class     : function
 * Callers   : 2  -> 0x0800C2B8 FUN_0800C2B8, 0x0800C2EA FUN_0800C2EA
 * Callees   : 3  -> 0x080030F0 FUN_080030F0, 0x08004D74 FUN_08004D74, 0x08005190 FUN_08005190
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080058E0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080058E0: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x080058E2: 6d21     movs     r1, #0x6d */
    r1 = 0x6d;
    /* 0x080058E4: 0620     movs     r0, #6 */
    r0 = 6;
    /* 0x080058E6: fff753fc bl       #0x8005190 */
    FUN_08005190();   /* BL -> 0x08005190 FUN_08005190 */
    /* 0x080058EA: 1e48     ldr      r0, [pc, #0x78]  ; =RAM_2000018E */
    r0 = *(uint32_t *)0x08005964;   /* = RAM 0x2000018E */
    /* 0x080058EC: 7d21     movs     r1, #0x7d */
    r1 = 0x7d;
    /* 0x080058EE: 0088     ldrh     r0, [r0] */
    r0 = *(uint16_t *)(r0);
    /* 0x080058F0: c900     lsls     r1, r1, #3 */
    r1 = r1 << 3;
    /* 0x080058F2: fdf7fdfb bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x080058F6: c7b2     uxtb     r7, r0 */
    r7 = uxtb(r0);
    /* 0x080058F8: 6846     mov      r0, sp */
    r0 = sp;
    /* 0x080058FA: c770     strb     r7, [r0, #3] */
    *(uint8_t *)(r0 + 3) = r7;   /* store */
    /* 0x080058FC: 0846     mov      r0, r1 */
    r0 = r1;
    /* 0x080058FE: 6421     movs     r1, #0x64 */
    r1 = 0x64;
    /* 0x08005900: fdf7f6fb bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x08005904: c6b2     uxtb     r6, r0 */
    r6 = uxtb(r0);
    /* 0x08005906: 6846     mov      r0, sp */
    r0 = sp;
    /* 0x08005908: 8670     strb     r6, [r0, #2] */
    *(uint8_t *)(r0 + 2) = r6;   /* store */
    /* 0x0800590A: 0846     mov      r0, r1 */
    r0 = r1;
    /* 0x0800590C: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x0800590E: fdf7effb bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x08005912: c5b2     uxtb     r5, r0 */
    r5 = uxtb(r0);
    /* 0x08005914: 6846     mov      r0, sp */
    r0 = sp;
    /* 0x08005916: 4570     strb     r5, [r0, #1] */
    *(uint8_t *)(r0 + 1) = r5;   /* store */
    /* 0x08005918: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x0800591A: 0c46     mov      r4, r1 */
    r4 = r1;
    /* 0x0800591C: 3a46     mov      r2, r7 */
    r2 = r7;
    /* 0x0800591E: 3221     movs     r1, #0x32 */
    r1 = 0x32;
    /* 0x08005920: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x08005922: fff727fa bl       #0x8004d74 */
    FUN_08004D74();   /* BL -> 0x08004D74 FUN_08004D74 */
    /* 0x08005926: 3246     mov      r2, r6 */
    r2 = r6;
    /* 0x08005928: 3d21     movs     r1, #0x3d */
    r1 = 0x3d;
    /* 0x0800592A: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x0800592C: fff722fa bl       #0x8004d74 */
    FUN_08004D74();   /* BL -> 0x08004D74 FUN_08004D74 */
    /* 0x08005930: 2a46     mov      r2, r5 */
    r2 = r5;
    /* 0x08005932: 4821     movs     r1, #0x48 */
    r1 = 0x48;
    /* 0x08005934: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x08005936: fff71dfa bl       #0x8004d74 */
    FUN_08004D74();   /* BL -> 0x08004D74 FUN_08004D74 */
    /* 0x0800593A: 2246     mov      r2, r4 */
    r2 = r4;
    /* 0x0800593C: 5321     movs     r1, #0x53 */
    r1 = 0x53;
    /* 0x0800593E: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x08005940: fff718fa bl       #0x8004d74 */
    FUN_08004D74();   /* BL -> 0x08004D74 FUN_08004D74 */
    /* 0x08005944: 0321     movs     r1, #3 */
    r1 = 3;
    /* 0x08005946: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005948: 074b     ldr      r3, [pc, #0x1c]  ; =RAM_200004CC */
    r3 = *(uint32_t *)0x08005968;   /* = RAM 0x200004CC */
    /* 0x0800594A: 0902     lsls     r1, r1, #8 */
    r1 = r1 << 8;
    /* 0x0800594C: 8024     movs     r4, #0x80 */
    r4 = 0x80;

    L_0800594E:
    /* 0x0800594E: 1a18     adds     r2, r3, r0 */
    r2 = r3 + r0;
    /* 0x08005950: 5218     adds     r2, r2, r1 */
    r2 = r2 + r1;
    /* 0x08005952: 1578     ldrb     r5, [r2] */
    r5 = *(uint8_t *)(r2);
    /* 0x08005954: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08005956: 2543     orrs     r5, r4 */
    r5 = r5 | r4;
    /* 0x08005958: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800595A: 1570     strb     r5, [r2] */
    *(uint8_t *)(r2) = r5;   /* store */
    /* 0x0800595C: 8028     cmp      r0, #0x80 */
    /* flags = cmp(r0, #0x80) */
    /* 0x0800595E: f6d3     blo      #0x800594e */
    if (r0 < 0x80) goto L_0800594E;   /* unsigned */
    /* 0x08005960: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
