/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C444
 * Name      : FUN_0800C444
 * End       : 0x0800C494
 * Size      : 80 bytes, 39 instructions decoded in body (functions.json insns=39, blocks=7)
 * Class     : function
 * Callers   : 1  -> 0x0800642C FUN_0800642C
 * Callees   : 1  -> 0x0800741C FUN_0800741C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C444(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C444: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800C446: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x0800C448: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x0800C44A: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);
    /* 0x0800C44C: 3022     movs     r2, #0x30 */
    r2 = 0x30;
    /* 0x0800C44E: 9143     bics     r1, r2 */
    r1 = r1 & ~r2;
    /* 0x0800C450: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */
    /* 0x0800C452: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);
    /* 0x0800C454: 2022     movs     r2, #0x20 */
    r2 = 0x20;
    /* 0x0800C456: 9143     bics     r1, r2 */
    r1 = r1 & ~r2;
    /* 0x0800C458: c160     str      r1, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r1;   /* store */
    /* 0x0800C45A: c168     ldr      r1, [r0, #0xc] */
    r1 = *(uint32_t *)(r0 + 12);
    /* 0x0800C45C: 1022     movs     r2, #0x10 */
    r2 = 0x10;
    /* 0x0800C45E: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x0800C460: c160     str      r1, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r1;   /* store */
    /* 0x0800C462: 0c4a     ldr      r2, [pc, #0x30]  ; =RAM_200001DA */
    r2 = *(uint32_t *)0x0800C494;   /* = RAM 0x200001DA */
    /* 0x0800C464: 1178     ldrb     r1, [r2] */
    r1 = *(uint8_t *)(r2);
    /* 0x0800C466: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x0800C468: 02d0     beq      #0x800c470 */
    if (r1 == 0) goto L_0800C470;
    /* 0x0800C46A: 491e     subs     r1, r1, #1 */
    r1 = r1 - 1;
    /* 0x0800C46C: 1170     strb     r1, [r2] */
    *(uint8_t *)(r2) = r1;   /* store */

    L_0800C46E:
    /* 0x0800C46E: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800C470:
    /* 0x0800C470: 0421     movs     r1, #4 */
    r1 = 4;
    /* 0x0800C472: faf7d3ff bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x0800C476: 0849     ldr      r1, [pc, #0x20]  ; =RAM_2000014A */
    r1 = *(uint32_t *)0x0800C498;   /* = RAM 0x2000014A */
    /* 0x0800C478: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800C47A: 08d0     beq      #0x800c48e */
    if (r0 == 0) goto L_0800C48E;
    /* 0x0800C47C: 487b     ldrb     r0, [r1, #0xd] */
    r0 = *(uint8_t *)(r1 + 13);
    /* 0x0800C47E: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800C480: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800C482: 4873     strb     r0, [r1, #0xd] */
    *(uint8_t *)(r1 + 13) = r0;   /* store */
    /* 0x0800C484: 2828     cmp      r0, #0x28 */
    /* flags = cmp(r0, #0x28) */
    /* 0x0800C486: f2d3     blo      #0x800c46e */
    if (r0 < 0x28) goto L_0800C46E;   /* unsigned */
    /* 0x0800C488: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800C48A: 8873     strb     r0, [r1, #0xe] */
    *(uint8_t *)(r1 + 14) = r0;   /* store */
    /* 0x0800C48C: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800C48E:
    /* 0x0800C48E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C490: 4873     strb     r0, [r1, #0xd] */
    *(uint8_t *)(r1 + 13) = r0;   /* store */
    /* 0x0800C492: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
