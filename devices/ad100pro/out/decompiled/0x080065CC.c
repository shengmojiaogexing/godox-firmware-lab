/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080065CC
 * Name      : FUN_080065CC
 * End       : 0x08006632
 * Size      : 102 bytes, 44 instructions decoded in body (functions.json insns=44, blocks=20)
 * Class     : function
 * Callers   : 2  -> 0x0800CF74 main, 0x0800D014 FUN_0800D014
 * Callees   : 7  -> 0x08003C50 FUN_08003C50, 0x08005C44 FUN_08005C44, 0x08009060 FUN_08009060, 0x08009E30 FUN_08009E30, 0x0800A4E8 FUN_0800A4E8, 0x0800A6E0 FUN_0800A6E0, 0x0800B3C0 FUN_0800B3C0
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080065CC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080065CC: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x080065CE: 1948     ldr      r0, [pc, #0x64]  ; =RAM_20000185 */
    r0 = *(uint32_t *)0x08006634;   /* = RAM 0x20000185 */
    /* 0x080065D0: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x080065D2: 1948     ldr      r0, [pc, #0x64]  ; =RAM_2000017C */
    r0 = *(uint32_t *)0x08006638;   /* = RAM 0x2000017C */
    /* 0x080065D4: 0229     cmp      r1, #2 */
    /* flags = cmp(r1, #2) */
    /* 0x080065D6: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x080065D8: 0ad0     beq      #0x80065f0 */
    if (r1 == 2) goto L_080065F0;
    /* 0x080065DA: 0329     cmp      r1, #3 */
    /* flags = cmp(r1, #3) */
    /* 0x080065DC: 0dd0     beq      #0x80065fa */
    if (r1 == 3) goto L_080065FA;
    /* 0x080065DE: 0429     cmp      r1, #4 */
    /* flags = cmp(r1, #4) */
    /* 0x080065E0: 10d0     beq      #0x8006604 */
    if (r1 == 4) goto L_08006604;
    /* 0x080065E2: 0529     cmp      r1, #5 */
    /* flags = cmp(r1, #5) */
    /* 0x080065E4: 13d0     beq      #0x800660e */
    if (r1 == 5) goto L_0800660E;
    /* 0x080065E6: 0629     cmp      r1, #6 */
    /* flags = cmp(r1, #6) */
    /* 0x080065E8: 16d0     beq      #0x8006618 */
    if (r1 == 6) goto L_08006618;
    /* 0x080065EA: 0729     cmp      r1, #7 */
    /* flags = cmp(r1, #7) */
    /* 0x080065EC: 19d0     beq      #0x8006622 */
    if (r1 == 7) goto L_08006622;
    /* 0x080065EE: 1ae0     b        #0x8006626 */
    goto L_08006626;

    L_080065F0:
    /* 0x080065F0: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x080065F2: 18d1     bne      #0x8006626 */
    if (r0 != 1) goto L_08006626;
    /* 0x080065F4: 03f01cfc bl       #0x8009e30 */
    FUN_08009E30();   /* BL -> 0x08009E30 FUN_08009E30 */
    /* 0x080065F8: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_080065FA:
    /* 0x080065FA: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x080065FC: 13d1     bne      #0x8006626 */
    if (r0 != 1) goto L_08006626;
    /* 0x080065FE: 04f0dffe bl       #0x800b3c0 */
    FUN_0800B3C0();   /* BL -> 0x0800B3C0 FUN_0800B3C0 */
    /* 0x08006602: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_08006604:
    /* 0x08006604: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08006606: 0ed1     bne      #0x8006626 */
    if (r0 != 1) goto L_08006626;
    /* 0x08006608: 03f06eff bl       #0x800a4e8 */
    FUN_0800A4E8();   /* BL -> 0x0800A4E8 FUN_0800A4E8 */
    /* 0x0800660C: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800660E:
    /* 0x0800660E: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08006610: 09d1     bne      #0x8006626 */
    if (r0 != 1) goto L_08006626;
    /* 0x08006612: fff717fb bl       #0x8005c44 */
    FUN_08005C44();   /* BL -> 0x08005C44 FUN_08005C44 */
    /* 0x08006616: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_08006618:
    /* 0x08006618: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800661A: 04d1     bne      #0x8006626 */
    if (r0 != 1) goto L_08006626;
    /* 0x0800661C: 04f060f8 bl       #0x800a6e0 */
    FUN_0800A6E0();   /* BL -> 0x0800A6E0 FUN_0800A6E0 */
    /* 0x08006620: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_08006622:
    /* 0x08006622: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08006624: 02d0     beq      #0x800662c */
    if (r0 == 1) goto L_0800662C;

    L_08006626:
    /* 0x08006626: fdf713fb bl       #0x8003c50 */
    FUN_08003C50();   /* BL -> 0x08003C50 FUN_08003C50 */
    /* 0x0800662A: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800662C:
    /* 0x0800662C: 02f018fd bl       #0x8009060 */
    FUN_08009060();   /* BL -> 0x08009060 FUN_08009060 */
    /* 0x08006630: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
