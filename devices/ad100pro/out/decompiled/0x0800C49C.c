/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C49C
 * Name      : FUN_0800C49C
 * End       : 0x0800C4DE
 * Size      : 66 bytes, 29 instructions decoded in body (functions.json insns=29, blocks=15)
 * Class     : function
 * Callers   : 2  -> 0x0800C4E8 FUN_0800C4E8, 0x0800C5C8 FUN_0800C5C8
 * Callees   : 4  -> 0x08005C44 FUN_08005C44, 0x08009060 FUN_08009060, 0x0800A4E8 FUN_0800A4E8, 0x0800A6E0 FUN_0800A6E0
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C49C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C49C: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800C49E: 1048     ldr      r0, [pc, #0x40]  ; =RAM_20000185 */
    r0 = *(uint32_t *)0x0800C4E0;   /* = RAM 0x20000185 */
    /* 0x0800C4A0: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x0800C4A2: 1048     ldr      r0, [pc, #0x40]  ; =RAM_2000017C */
    r0 = *(uint32_t *)0x0800C4E4;   /* = RAM 0x2000017C */
    /* 0x0800C4A4: 0529     cmp      r1, #5 */
    /* flags = cmp(r1, #5) */
    /* 0x0800C4A6: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800C4A8: 0ad0     beq      #0x800c4c0 */
    if (r1 == 5) goto L_0800C4C0;
    /* 0x0800C4AA: 0429     cmp      r1, #4 */
    /* flags = cmp(r1, #4) */
    /* 0x0800C4AC: 0dd0     beq      #0x800c4ca */
    if (r1 == 4) goto L_0800C4CA;
    /* 0x0800C4AE: 0629     cmp      r1, #6 */
    /* flags = cmp(r1, #6) */
    /* 0x0800C4B0: 10d0     beq      #0x800c4d4 */
    if (r1 == 6) goto L_0800C4D4;
    /* 0x0800C4B2: 0729     cmp      r1, #7 */
    /* flags = cmp(r1, #7) */
    /* 0x0800C4B4: 03d1     bne      #0x800c4be */
    if (r1 != 7) goto L_0800C4BE;
    /* 0x0800C4B6: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800C4B8: 01d1     bne      #0x800c4be */
    if (r0 != 1) goto L_0800C4BE;
    /* 0x0800C4BA: fcf7d1fd bl       #0x8009060 */
    FUN_08009060();   /* BL -> 0x08009060 FUN_08009060 */

    L_0800C4BE:
    /* 0x0800C4BE: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800C4C0:
    /* 0x0800C4C0: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800C4C2: fcd1     bne      #0x800c4be */
    if (r0 != 1) goto L_0800C4BE;
    /* 0x0800C4C4: f9f7befb bl       #0x8005c44 */
    FUN_08005C44();   /* BL -> 0x08005C44 FUN_08005C44 */

    L_0800C4C8:
    /* 0x0800C4C8: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800C4CA:
    /* 0x0800C4CA: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800C4CC: fcd1     bne      #0x800c4c8 */
    if (r0 != 1) goto L_0800C4C8;
    /* 0x0800C4CE: fef70bf8 bl       #0x800a4e8 */
    FUN_0800A4E8();   /* BL -> 0x0800A4E8 FUN_0800A4E8 */

    L_0800C4D2:
    /* 0x0800C4D2: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800C4D4:
    /* 0x0800C4D4: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800C4D6: fcd1     bne      #0x800c4d2 */
    if (r0 != 1) goto L_0800C4D2;
    /* 0x0800C4D8: fef702f9 bl       #0x800a6e0 */
    FUN_0800A6E0();   /* BL -> 0x0800A6E0 FUN_0800A6E0 */
    /* 0x0800C4DC: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
