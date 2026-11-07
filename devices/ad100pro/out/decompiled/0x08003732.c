/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08003732
 * Name      : FUN_08003732
 * End       : 0x0800379E
 * Size      : 108 bytes, 52 instructions decoded in body (functions.json insns=52, blocks=10)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x080069A4 FUN_080069A4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08003732(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08003732: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08003734: 0146     mov      r1, r0 */
    r1 = r0;
    /* 0x08003736: 406a     ldr      r0, [r0, #0x24] */
    r0 = *(uint32_t *)(r0 + 36);
    /* 0x08003738: 426c     ldr      r2, [r0, #0x44] */
    r2 = *(uint32_t *)(r0 + 68);
    /* 0x0800373A: 5023     movs     r3, #0x50 */
    r3 = 0x50;
    /* 0x0800373C: 1a42     tst      r2, r3 */
    /* flags = tst(r2, r3) */
    /* 0x0800373E: 0ad1     bne      #0x8003756 */
    if ((r2 & r3)) goto L_08003756;   /* test bits */
    /* 0x08003740: 416c     ldr      r1, [r0, #0x44] */
    r1 = *(uint32_t *)(r0 + 68);
    /* 0x08003742: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08003744: 5202     lsls     r2, r2, #9 */
    r2 = r2 << 9;
    /* 0x08003746: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x08003748: 4164     str      r1, [r0, #0x44] */
    *(uint32_t *)(r0 + 68) = r1;   /* store */
    /* 0x0800374A: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);
    /* 0x0800374C: ca68     ldr      r2, [r1, #0xc] */
    r2 = *(uint32_t *)(r1 + 12);
    /* 0x0800374E: 1205     lsls     r2, r2, #0x14 */
    r2 = r2 << 0x14;
    /* 0x08003750: 920f     lsrs     r2, r2, #0x1e */
    r2 = r2 >> 0x1e;
    /* 0x08003752: 05d0     beq      #0x8003760 */
    if ((r2 & r3)) goto L_08003760;   /* test bits */
    /* 0x08003754: 20e0     b        #0x8003798 */
    goto L_08003798;

    L_08003756:
    /* 0x08003756: c06b     ldr      r0, [r0, #0x3c] */
    r0 = *(uint32_t *)(r0 + 60);
    /* 0x08003758: 026b     ldr      r2, [r0, #0x30] */
    r2 = *(uint32_t *)(r0 + 48);
    /* 0x0800375A: 0846     mov      r0, r1 */
    r0 = r1;
    /* 0x0800375C: 9047     blx      r2 */
    ((void(*)())r2)();   /* indirect call */

    L_08003760:
    /* 0x08003760: 026a     ldr      r2, [r0, #0x20] */
    r2 = *(uint32_t *)(r0 + 32);
    /* 0x08003762: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x08003764: 18d1     bne      #0x8003798 */
    if (r2 != 0) goto L_08003798;
    /* 0x08003766: 0a68     ldr      r2, [r1] */
    r2 = *(uint32_t *)(r1);
    /* 0x08003768: 1207     lsls     r2, r2, #0x1c */
    r2 = r2 << 0x1c;
    /* 0x0800376A: 15d5     bpl      #0x8003798 */
    if (r2 >= 0) goto L_08003798;   /* result non-negative */
    /* 0x0800376C: 8a68     ldr      r2, [r1, #8] */
    r2 = *(uint32_t *)(r1 + 8);
    /* 0x0800376E: 5307     lsls     r3, r2, #0x1d */
    r3 = r2 << 0x1d;
    /* 0x08003770: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08003772: 002b     cmp      r3, #0 */
    /* flags = cmp(r3, #0) */
    /* 0x08003774: 09db     blt      #0x800378a */
    if (r3 < 0) goto L_0800378A;   /* signed */
    /* 0x08003776: 4b68     ldr      r3, [r1, #4] */
    r3 = *(uint32_t *)(r1 + 4);
    /* 0x08003778: 0c24     movs     r4, #0xc */
    r4 = 0xc;
    /* 0x0800377A: a343     bics     r3, r4 */
    r3 = r3 & ~r4;
    /* 0x0800377C: 4b60     str      r3, [r1, #4] */
    *(uint32_t *)(r1 + 4) = r3;   /* store */
    /* 0x0800377E: 416c     ldr      r1, [r0, #0x44] */
    r1 = *(uint32_t *)(r0 + 68);
    /* 0x08003780: 1302     lsls     r3, r2, #8 */
    r3 = r2 << 8;
    /* 0x08003782: 9943     bics     r1, r3 */
    r1 = r1 & ~r3;
    /* 0x08003784: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x08003786: 4164     str      r1, [r0, #0x44] */
    *(uint32_t *)(r0 + 68) = r1;   /* store */
    /* 0x08003788: 06e0     b        #0x8003798 */
    goto L_08003798;

    L_0800378A:
    /* 0x0800378A: 416c     ldr      r1, [r0, #0x44] */
    r1 = *(uint32_t *)(r0 + 68);
    /* 0x0800378C: 2023     movs     r3, #0x20 */
    r3 = 0x20;
    /* 0x0800378E: 1943     orrs     r1, r3 */
    r1 = r1 | r3;
    /* 0x08003790: 4164     str      r1, [r0, #0x44] */
    *(uint32_t *)(r0 + 68) = r1;   /* store */
    /* 0x08003792: 816c     ldr      r1, [r0, #0x48] */
    r1 = *(uint32_t *)(r0 + 72);
    /* 0x08003794: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x08003796: 8164     str      r1, [r0, #0x48] */
    *(uint32_t *)(r0 + 72) = r1;   /* store */

    L_08003798:
    /* 0x08003798: 03f004f9 bl       #0x80069a4 */
    FUN_080069A4();   /* BL -> 0x080069A4 FUN_080069A4 */
    /* 0x0800379C: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
