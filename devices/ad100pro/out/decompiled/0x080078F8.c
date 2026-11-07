/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080078F8
 * Name      : FUN_080078F8
 * End       : 0x08007952
 * Size      : 90 bytes, 44 instructions decoded in body (functions.json insns=44, blocks=9)
 * Class     : function
 * Callers   : 2  -> 0x08007798 FUN_08007798, 0x0800786C FUN_0800786C
 * Callees   : 1  -> 0x080030F0 FUN_080030F0
 * MMIO      : RCC.CFGR, RCC.CFGR2, RCC.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080078F8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080078F8: 30b5     push     {r4, r5, lr} */
    push({r4, r5, lr});
    /* 0x080078FA: 16a3     adr      r3, #0x58 */
    r3 = 0x00000000;   /* address */
    /* 0x080078FC: 0fcb     ldm      r3, {r0, r1, r2, r3} */
    /* ldm r3, {r0, r1, r2, r3} */
    /* 0x080078FE: 89b0     sub      sp, #0x24 */
    sp = sp - 0x24;
    /* 0x08007900: 6c46     mov      r4, sp */
    r4 = sp;
    /* 0x08007902: 0fc4     stm      r4!, {r0, r1, r2, r3} */
    /* stm r4!, {r0, r1, r2, r3} */
    /* 0x08007904: 17a3     adr      r3, #0x5c */
    r3 = 0x00000000;   /* address */
    /* 0x08007906: 0fcb     ldm      r3, {r0, r1, r2, r3} */
    /* ldm r3, {r0, r1, r2, r3} */
    /* 0x08007908: 0fc4     stm      r4!, {r0, r1, r2, r3} */
    /* stm r4!, {r0, r1, r2, r3} */
    /* 0x0800790A: 1a49     ldr      r1, [pc, #0x68]  ; =RCC.CR */
    r1 = *(uint32_t *)0x08007974;   /* = MMIO RCC+0x00 */
    /* 0x0800790C: 4a68     ldr      r2, [r1, #4] */
    r2 = *(uint32_t *)(r1 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x0800790E: 0c20     movs     r0, #0xc */
    r0 = 0xc;
    /* 0x08007910: 1446     mov      r4, r2 */
    r4 = r2;
    /* 0x08007912: 0440     ands     r4, r0 */
    r4 = r4 & r0;
    /* 0x08007914: 1848     ldr      r0, [pc, #0x60]  ; =0x007A1200 */
    r0 = *(uint32_t *)0x08007978;   /* = 0x007A1200 */
    /* 0x08007916: 042c     cmp      r4, #4 */
    /* flags = cmp(r4, #4) */
    /* 0x08007918: 05d0     beq      #0x8007926 */
    if (r4 == 4) goto L_08007926;
    /* 0x0800791A: 184b     ldr      r3, [pc, #0x60]  ; =0x02DC6C00 */
    r3 = *(uint32_t *)0x0800797C;   /* = 0x02DC6C00 */
    /* 0x0800791C: 082c     cmp      r4, #8 */
    /* flags = cmp(r4, #8) */
    /* 0x0800791E: 04d0     beq      #0x800792a */
    if (r4 == 8) goto L_0800792A;
    /* 0x08007920: 0c2c     cmp      r4, #0xc */
    /* flags = cmp(r4, #0xc) */
    /* 0x08007922: 00d1     bne      #0x8007926 */
    if (r4 != 0xc) goto L_08007926;
    /* 0x08007924: 1846     mov      r0, r3 */
    r0 = r3;

    L_08007926:
    /* 0x08007926: 09b0     add      sp, #0x24 */
    sp = sp + 0x24;
    /* 0x08007928: 30bd     pop      {r4, r5, pc} */
    pop({r4, r5, pc});   /* return */

    L_0800792A:
    /* 0x0800792A: 9402     lsls     r4, r2, #0xa */
    r4 = r2 << 0xa;
    /* 0x0800792C: 240f     lsrs     r4, r4, #0x1c */
    r4 = r4 >> 0x1c;
    /* 0x0800792E: 6d46     mov      r5, sp */
    r5 = sp;
    /* 0x08007930: 2c5d     ldrb     r4, [r5, r4] */
    r4 = *(uint8_t *)(r5 + r4);  /* MMIO: RCC.CR(r) */
    /* 0x08007932: c96a     ldr      r1, [r1, #0x2c] */
    r1 = *(uint32_t *)(r1 + 44);  /* MMIO: RCC.CFGR2(r) */
    /* 0x08007934: 04ad     add      r5, sp, #0x10 */
    r5 = sp + 0x10;
    /* 0x08007936: 0907     lsls     r1, r1, #0x1c */
    r1 = r1 << 0x1c;
    /* 0x08007938: 090f     lsrs     r1, r1, #0x1c */
    r1 = r1 >> 0x1c;
    /* 0x0800793A: d203     lsls     r2, r2, #0xf */
    r2 = r2 << 0xf;
    /* 0x0800793C: 695c     ldrb     r1, [r5, r1] */
    r1 = *(uint8_t *)(r5 + r1);  /* MMIO: RCC.CR(r) */
    /* 0x0800793E: 920f     lsrs     r2, r2, #0x1e */
    r2 = r2 >> 0x1e;
    /* 0x08007940: 022a     cmp      r2, #2 */
    /* flags = cmp(r2, #2) */
    /* 0x08007942: 02d0     beq      #0x800794a */
    if (r2 == 2) goto L_0800794A;
    /* 0x08007944: 032a     cmp      r2, #3 */
    /* flags = cmp(r2, #3) */
    /* 0x08007946: 00d1     bne      #0x800794a */
    if (r2 != 3) goto L_0800794A;
    /* 0x08007948: 1846     mov      r0, r3 */
    r0 = r3;

    L_0800794A:
    /* 0x0800794A: fbf7d1fb bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x0800794E: 6043     muls     r0, r4, r0 */
    r0 = r4 * r0;
    /* 0x08007950: e9e7     b        #0x8007926 */
    goto L_08007926;   /* loop */
}
