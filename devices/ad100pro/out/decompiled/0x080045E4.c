/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080045E4
 * Name      : FUN_080045E4
 * End       : 0x0800464E
 * Size      : 106 bytes, 52 instructions decoded in body (functions.json insns=52, blocks=13)
 * Class     : function
 * Callers   : 5  -> 0x0800610C FUN_0800610C, 0x080061EA FUN_080061EA, 0x0800625A FUN_0800625A, 0x0800629C FUN_0800629C, 0x080062C4 FUN_080062C4
 * Callees   : 1  -> 0x08003C30 FUN_08003C30
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080045E4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080045E4: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x080045E6: 1a48     ldr      r0, [pc, #0x68]  ; =RAM_200001B9 */
    r0 = *(uint32_t *)0x08004650;   /* = RAM 0x200001B9 */
    /* 0x080045E8: 1a49     ldr      r1, [pc, #0x68]  ; =RAM_200001BB */
    r1 = *(uint32_t *)0x08004654;   /* = RAM 0x200001BB */
    /* 0x080045EA: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x080045EC: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x080045EE: 1b4d     ldr      r5, [pc, #0x6c]  ; =RAM_2000013B */
    r5 = *(uint32_t *)0x0800465C;   /* = RAM 0x2000013B */
    /* 0x080045F0: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x080045F2: 1949     ldr      r1, [pc, #0x64]  ; =RAM_20000138 */
    r1 = *(uint32_t *)0x08004658;   /* = RAM 0x20000138 */
    /* 0x080045F4: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x080045F6: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x080045F8: 2978     ldrb     r1, [r5] */
    r1 = *(uint8_t *)(r5);
    /* 0x080045FA: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x080045FC: 21d1     bne      #0x8004642 */
    if (/* bne */) goto L_08004642;
    /* 0x080045FE: 1849     ldr      r1, [pc, #0x60]  ; =RAM_200001B8 */
    r1 = *(uint32_t *)0x08004660;   /* = RAM 0x200001B8 */
    /* 0x08004600: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08004602: 0f78     ldrb     r7, [r1] */
    r7 = *(uint8_t *)(r1);
    /* 0x08004604: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x08004606: 174b     ldr      r3, [pc, #0x5c]  ; =RAM_200001CC */
    r3 = *(uint32_t *)0x08004664;   /* = RAM 0x200001CC */
    /* 0x08004608: 174e     ldr      r6, [pc, #0x5c]  ; =RAM_200001AF */
    r6 = *(uint32_t *)0x08004668;   /* = RAM 0x200001AF */
    /* 0x0800460A: 184c     ldr      r4, [pc, #0x60]  ; =RAM_200001DB */
    r4 = *(uint32_t *)0x0800466C;   /* = RAM 0x200001DB */
    /* 0x0800460C: 002f     cmp      r7, #0 */
    /* flags = cmp(r7, #0) */
    /* 0x0800460E: 03d0     beq      #0x8004618 */
    if (r7 == 0) goto L_08004618;
    /* 0x08004610: 2178     ldrb     r1, [r4] */
    r1 = *(uint8_t *)(r4);
    /* 0x08004612: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x08004614: 03d0     beq      #0x800461e */
    if (r1 == 1) goto L_0800461E;
    /* 0x08004616: 0ce0     b        #0x8004632 */
    goto L_08004632;

    L_08004618:
    /* 0x08004618: 0127     movs     r7, #1 */
    r7 = 1;
    /* 0x0800461A: 0f70     strb     r7, [r1] */
    *(uint8_t *)(r1) = r7;   /* store */
    /* 0x0800461C: 02e0     b        #0x8004624 */
    goto L_08004624;

    L_0800461E:
    /* 0x0800461E: 3178     ldrb     r1, [r6] */
    r1 = *(uint8_t *)(r6);
    /* 0x08004620: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x08004622: 0ed1     bne      #0x8004642 */
    if (r1 != 0) goto L_08004642;

    L_08004624:
    /* 0x08004624: 2270     strb     r2, [r4] */
    *(uint8_t *)(r4) = r2;   /* store */
    /* 0x08004626: 1870     strb     r0, [r3] */
    *(uint8_t *)(r3) = r0;   /* store */
    /* 0x08004628: 3222     movs     r2, #0x32 */
    r2 = 0x32;
    /* 0x0800462A: 2821     movs     r1, #0x28 */
    r1 = 0x28;
    /* 0x0800462C: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800462E: fff7fffa bl       #0x8003c30 */
    FUN_08003C30();   /* BL -> 0x08003C30 FUN_08003C30 */

    L_08004632:
    /* 0x08004632: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x08004634: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08004636: 04d0     beq      #0x8004642 */
    if (r0 == 0) goto L_08004642;
    /* 0x08004638: 3178     ldrb     r1, [r6] */
    r1 = *(uint8_t *)(r6);
    /* 0x0800463A: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x0800463C: 01d1     bne      #0x8004642 */
    if (r1 != 0) goto L_08004642;
    /* 0x0800463E: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x08004640: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */

    L_08004642:
    /* 0x08004642: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);
    /* 0x08004644: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08004646: 01d0     beq      #0x800464c */
    if (r0 == 0) goto L_0800464C;
    /* 0x08004648: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x0800464A: 2870     strb     r0, [r5] */
    *(uint8_t *)(r5) = r0;   /* store */

    L_0800464C:
    /* 0x0800464C: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
