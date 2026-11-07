/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080093A8
 * Name      : FUN_080093A8
 * End       : 0x080093EA
 * Size      : 66 bytes, 27 instructions decoded in body (functions.json insns=27, blocks=7)
 * Class     : function
 * Callers   : 1  -> 0x0800CF74 main
 * Callees   : 3  -> 0x08005C40 FUN_08005C40, 0x08006CB4 FUN_08006CB4, 0x08006CEC FUN_08006CEC
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080093A8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080093A8: 1cb5     push     {r2, r3, r4, lr} */
    push({r2, r3, r4, lr});
    /* 0x080093AA: 1149     ldr      r1, [pc, #0x44]  ; =RAM_20000260 */
    r1 = *(uint32_t *)0x080093F0;   /* = RAM 0x20000260 */
    /* 0x080093AC: 0f48     ldr      r0, [pc, #0x3c]  ; =0x40007400 */
    r0 = *(uint32_t *)0x080093EC;   /* = MMIO APB+0x7400 */
    /* 0x080093AE: 0860     str      r0, [r1] */
    *(uint32_t *)(r1) = r0;   /* store */
    /* 0x080093B0: 0846     mov      r0, r1 */
    r0 = r1;
    /* 0x080093B2: fdf79bfc bl       #0x8006cec */
    FUN_08006CEC();   /* BL -> 0x08006CEC FUN_08006CEC */
    /* 0x080093B6: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080093B8: 01d0     beq      #0x80093be */
    if (r0 == 0) goto L_080093BE;
    /* 0x080093BA: fcf741fc bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_080093BE:
    /* 0x080093BE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080093C0: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x080093C2: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x080093C4: 0190     str      r0, [sp, #4] */
    *(uint32_t *)(sp + 4) = r0;   /* store */
    /* 0x080093C6: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x080093C8: 0948     ldr      r0, [pc, #0x24]  ; =RAM_20000260 */
    r0 = *(uint32_t *)0x080093F0;   /* = RAM 0x20000260 */
    /* 0x080093CA: fdf773fc bl       #0x8006cb4 */
    FUN_08006CB4();   /* BL -> 0x08006CB4 FUN_08006CB4 */
    /* 0x080093CE: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080093D0: 01d0     beq      #0x80093d6 */
    if (r0 == 0) goto L_080093D6;
    /* 0x080093D2: fcf735fc bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_080093D6:
    /* 0x080093D6: 1022     movs     r2, #0x10 */
    r2 = 0x10;
    /* 0x080093D8: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x080093DA: 0548     ldr      r0, [pc, #0x14]  ; =RAM_20000260 */
    r0 = *(uint32_t *)0x080093F0;   /* = RAM 0x20000260 */
    /* 0x080093DC: fdf76afc bl       #0x8006cb4 */
    FUN_08006CB4();   /* BL -> 0x08006CB4 FUN_08006CB4 */
    /* 0x080093E0: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080093E2: 01d0     beq      #0x80093e8 */
    if (r0 == 0) goto L_080093E8;
    /* 0x080093E4: fcf72cfc bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_080093E8:
    /* 0x080093E8: 1cbd     pop      {r2, r3, r4, pc} */
    pop({r2, r3, r4, pc});   /* return */
}
