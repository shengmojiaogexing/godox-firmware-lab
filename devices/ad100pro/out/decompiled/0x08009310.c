/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009310
 * Name      : FUN_08009310
 * End       : 0x080093A0
 * Size      : 144 bytes, 62 instructions decoded in body (functions.json insns=62, blocks=11)
 * Class     : function
 * Callers   : 1  -> 0x0800CF74 main
 * Callees   : 3  -> 0x08005C40 FUN_08005C40, 0x08006878 FUN_08006878, 0x080069AC FUN_080069AC
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009310(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009310: 0eb5     push     {r1, r2, r3, lr} */
    push({r1, r2, r3, lr});
    /* 0x08009312: 2448     ldr      r0, [pc, #0x90]  ; =RAM_200003AC */
    r0 = *(uint32_t *)0x080093A4;   /* = RAM 0x200003AC */
    /* 0x08009314: 2249     ldr      r1, [pc, #0x88]  ; =ADC.ISR */
    r1 = *(uint32_t *)0x080093A0;   /* = MMIO ADC+0x00 */
    /* 0x08009316: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */
    /* 0x08009318: 4905     lsls     r1, r1, #0x15 */
    r1 = r1 << 0x15;
    /* 0x0800931A: 4160     str      r1, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r1;   /* store */
    /* 0x0800931C: 0821     movs     r1, #8 */
    r1 = 8;
    /* 0x0800931E: 8160     str      r1, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r1;   /* store */
    /* 0x08009320: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x08009322: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08009324: 0423     movs     r3, #4 */
    r3 = 4;
    /* 0x08009326: 0c30     adds     r0, #0xc */
    r0 = r0 + 0xc;
    /* 0x08009328: 0ec0     stm      r0!, {r1, r2, r3} */
    /* stm r0!, {r1, r2, r3} */
    /* 0x0800932A: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */
    /* 0x0800932C: 4160     str      r1, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r1;   /* store */
    /* 0x0800932E: c160     str      r1, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r1;   /* store */
    /* 0x08009330: 8260     str      r2, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r2;   /* store */
    /* 0x08009332: 8261     str      r2, [r0, #0x18] */
    *(uint32_t *)(r0 + 24) = r2;   /* store */
    /* 0x08009334: c261     str      r2, [r0, #0x1c] */
    *(uint32_t *)(r0 + 28) = r2;   /* store */
    /* 0x08009336: 4161     str      r1, [r0, #0x14] */
    *(uint32_t *)(r0 + 20) = r1;   /* store */
    /* 0x08009338: 1838     subs     r0, #0x18 */
    r0 = r0 - 0x18;
    /* 0x0800933A: fdf737fb bl       #0x80069ac */
    FUN_080069AC();   /* BL -> 0x080069AC FUN_080069AC */
    /* 0x0800933E: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009340: 01d0     beq      #0x8009346 */
    if (r0 == 0) goto L_08009346;
    /* 0x08009342: fcf77dfc bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_08009346:
    /* 0x08009346: 0321     movs     r1, #3 */
    r1 = 3;
    /* 0x08009348: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800934A: 0003     lsls     r0, r0, #0xc */
    r0 = r0 << 0xc;
    /* 0x0800934C: 0091     str      r1, [sp] */
    *(uint32_t *)(sp) = r1;   /* store */
    /* 0x0800934E: 0291     str      r1, [sp, #8] */
    *(uint32_t *)(sp + 8) = r1;   /* store */
    /* 0x08009350: 0190     str      r0, [sp, #4] */
    *(uint32_t *)(sp + 4) = r0;   /* store */
    /* 0x08009352: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08009354: 1348     ldr      r0, [pc, #0x4c]  ; =RAM_200003AC */
    r0 = *(uint32_t *)0x080093A4;   /* = RAM 0x200003AC */
    /* 0x08009356: fdf78ffa bl       #0x8006878 */
    FUN_08006878();   /* BL -> 0x08006878 FUN_08006878 */
    /* 0x0800935A: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800935C: 01d0     beq      #0x8009362 */
    if (r0 == 0) goto L_08009362;
    /* 0x0800935E: fcf76ffc bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_08009362:
    /* 0x08009362: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x08009364: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08009366: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08009368: 0e48     ldr      r0, [pc, #0x38]  ; =RAM_200003AC */
    r0 = *(uint32_t *)0x080093A4;   /* = RAM 0x200003AC */
    /* 0x0800936A: fdf785fa bl       #0x8006878 */
    FUN_08006878();   /* BL -> 0x08006878 FUN_08006878 */
    /* 0x0800936E: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009370: 01d0     beq      #0x8009376 */
    if (r0 == 0) goto L_08009376;
    /* 0x08009372: fcf765fc bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_08009376:
    /* 0x08009376: 0b20     movs     r0, #0xb */
    r0 = 0xb;
    /* 0x08009378: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x0800937A: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x0800937C: 0948     ldr      r0, [pc, #0x24]  ; =RAM_200003AC */
    r0 = *(uint32_t *)0x080093A4;   /* = RAM 0x200003AC */
    /* 0x0800937E: fdf77bfa bl       #0x8006878 */
    FUN_08006878();   /* BL -> 0x08006878 FUN_08006878 */
    /* 0x08009382: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009384: 01d0     beq      #0x800938a */
    if (r0 == 0) goto L_0800938A;
    /* 0x08009386: fcf75bfc bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_0800938A:
    /* 0x0800938A: 0c20     movs     r0, #0xc */
    r0 = 0xc;
    /* 0x0800938C: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x0800938E: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08009390: 0448     ldr      r0, [pc, #0x10]  ; =RAM_200003AC */
    r0 = *(uint32_t *)0x080093A4;   /* = RAM 0x200003AC */
    /* 0x08009392: fdf771fa bl       #0x8006878 */
    FUN_08006878();   /* BL -> 0x08006878 FUN_08006878 */
    /* 0x08009396: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009398: 01d0     beq      #0x800939e */
    if (r0 == 0) goto L_0800939E;
    /* 0x0800939A: fcf751fc bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_0800939E:
    /* 0x0800939E: 0ebd     pop      {r1, r2, r3, pc} */
    pop({r1, r2, r3, pc});   /* return */
}
