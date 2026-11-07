/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080066C0
 * Name      : FUN_080066C0
 * End       : 0x08006726
 * Size      : 102 bytes, 44 instructions decoded in body (functions.json insns=44, blocks=20)
 * Class     : function
 * Callers   : 4  -> 0x080091C4 FUN_080091C4, 0x080091F4 FUN_080091F4, 0x0800C4E8 FUN_0800C4E8, 0x0800C50C FUN_0800C50C
 * Callees   : 7  -> 0x08003D2C FUN_08003D2C, 0x08005CCC FUN_08005CCC, 0x080090E8 FUN_080090E8, 0x08009EE0 FUN_08009EE0, 0x0800A570 FUN_0800A570, 0x0800A768 FUN_0800A768, 0x0800B448 FUN_0800B448
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080066C0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080066C0: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x080066C2: 1948     ldr      r0, [pc, #0x64]  ; =RAM_20000185 */
    r0 = *(uint32_t *)0x08006728;   /* = RAM 0x20000185 */
    /* 0x080066C4: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x080066C6: 1948     ldr      r0, [pc, #0x64]  ; =RAM_2000017C */
    r0 = *(uint32_t *)0x0800672C;   /* = RAM 0x2000017C */
    /* 0x080066C8: 0229     cmp      r1, #2 */
    /* flags = cmp(r1, #2) */
    /* 0x080066CA: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x080066CC: 0ad0     beq      #0x80066e4 */
    if (r1 == 2) goto L_080066E4;
    /* 0x080066CE: 0329     cmp      r1, #3 */
    /* flags = cmp(r1, #3) */
    /* 0x080066D0: 0dd0     beq      #0x80066ee */
    if (r1 == 3) goto L_080066EE;
    /* 0x080066D2: 0429     cmp      r1, #4 */
    /* flags = cmp(r1, #4) */
    /* 0x080066D4: 10d0     beq      #0x80066f8 */
    if (r1 == 4) goto L_080066F8;
    /* 0x080066D6: 0529     cmp      r1, #5 */
    /* flags = cmp(r1, #5) */
    /* 0x080066D8: 13d0     beq      #0x8006702 */
    if (r1 == 5) goto L_08006702;
    /* 0x080066DA: 0629     cmp      r1, #6 */
    /* flags = cmp(r1, #6) */
    /* 0x080066DC: 16d0     beq      #0x800670c */
    if (r1 == 6) goto L_0800670C;
    /* 0x080066DE: 0729     cmp      r1, #7 */
    /* flags = cmp(r1, #7) */
    /* 0x080066E0: 19d0     beq      #0x8006716 */
    if (r1 == 7) goto L_08006716;
    /* 0x080066E2: 1ae0     b        #0x800671a */
    goto L_0800671A;

    L_080066E4:
    /* 0x080066E4: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x080066E6: 18d1     bne      #0x800671a */
    if (r0 != 1) goto L_0800671A;
    /* 0x080066E8: 03f0fafb bl       #0x8009ee0 */
    FUN_08009EE0();   /* BL -> 0x08009EE0 FUN_08009EE0 */
    /* 0x080066EC: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_080066EE:
    /* 0x080066EE: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x080066F0: 13d1     bne      #0x800671a */
    if (r0 != 1) goto L_0800671A;
    /* 0x080066F2: 04f0a9fe bl       #0x800b448 */
    FUN_0800B448();   /* BL -> 0x0800B448 FUN_0800B448 */
    /* 0x080066F6: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_080066F8:
    /* 0x080066F8: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x080066FA: 0ed1     bne      #0x800671a */
    if (r0 != 1) goto L_0800671A;
    /* 0x080066FC: 03f038ff bl       #0x800a570 */
    FUN_0800A570();   /* BL -> 0x0800A570 FUN_0800A570 */
    /* 0x08006700: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_08006702:
    /* 0x08006702: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08006704: 09d1     bne      #0x800671a */
    if (r0 != 1) goto L_0800671A;
    /* 0x08006706: fff7e1fa bl       #0x8005ccc */
    FUN_08005CCC();   /* BL -> 0x08005CCC FUN_08005CCC */
    /* 0x0800670A: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800670C:
    /* 0x0800670C: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800670E: 04d1     bne      #0x800671a */
    if (r0 != 1) goto L_0800671A;
    /* 0x08006710: 04f02af8 bl       #0x800a768 */
    FUN_0800A768();   /* BL -> 0x0800A768 FUN_0800A768 */
    /* 0x08006714: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_08006716:
    /* 0x08006716: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08006718: 02d0     beq      #0x8006720 */
    if (r0 == 1) goto L_08006720;

    L_0800671A:
    /* 0x0800671A: fdf707fb bl       #0x8003d2c */
    FUN_08003D2C();   /* BL -> 0x08003D2C FUN_08003D2C */
    /* 0x0800671E: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_08006720:
    /* 0x08006720: 02f0e2fc bl       #0x80090e8 */
    FUN_080090E8();   /* BL -> 0x080090E8 FUN_080090E8 */
    /* 0x08006724: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
