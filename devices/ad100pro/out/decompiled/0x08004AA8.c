/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004AA8
 * Name      : FUN_08004AA8
 * End       : 0x08004ADE
 * Size      : 54 bytes, 25 instructions decoded in body (functions.json insns=25, blocks=7)
 * Class     : function
 * Callers   : 2  -> 0x0800BA44 IRQ19_Handler, 0x0800BA48 FUN_0800BA48
 * Callees   : 2  -> 0x08006014 FUN_08006014, 0x0800C6F4 FUN_0800C6F4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004AA8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004AA8: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08004AAA: 0d49     ldr      r1, [pc, #0x34]  ; =RAM_20000198 */
    r1 = *(uint32_t *)0x08004AE0;   /* = RAM 0x20000198 */
    /* 0x08004AAC: 0868     ldr      r0, [r1] */
    r0 = *(uint32_t *)(r1);
    /* 0x08004AAE: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08004AB0: 11d0     beq      #0x8004ad6 */
    if (r0 == 0) goto L_08004AD6;
    /* 0x08004AB2: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x08004AB4: 0860     str      r0, [r1] */
    *(uint32_t *)(r1) = r0;   /* store */
    /* 0x08004AB6: 0ed1     bne      #0x8004ad6 */
    if (r0 != 0) goto L_08004AD6;
    /* 0x08004AB8: 0a4c     ldr      r4, [pc, #0x28]  ; =RAM_200001AB */
    r4 = *(uint32_t *)0x08004AE4;   /* = RAM 0x200001AB */
    /* 0x08004ABA: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08004ABC: 0a49     ldr      r1, [pc, #0x28]  ; =RAM_200001B2 */
    r1 = *(uint32_t *)0x08004AE8;   /* = RAM 0x200001B2 */
    /* 0x08004ABE: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x08004AC0: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08004AC2: 0907     lsls     r1, r1, #0x1c */
    r1 = r1 << 0x1c;
    /* 0x08004AC4: 090f     lsrs     r1, r1, #0x1c */
    r1 = r1 >> 0x1c;
    /* 0x08004AC6: 0a29     cmp      r1, #0xa */
    /* flags = cmp(r1, #0xa) */
    /* 0x08004AC8: 06d0     beq      #0x8004ad8 */
    if (r1 == 0xa) goto L_08004AD8;
    /* 0x08004ACA: 0849     ldr      r1, [pc, #0x20]  ; =RAM_20000182 */
    r1 = *(uint32_t *)0x08004AEC;   /* = RAM 0x20000182 */
    /* 0x08004ACC: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08004ACE: 01f0a1fa bl       #0x8006014 */
    FUN_08006014();   /* BL -> 0x08006014 FUN_08006014 */

    L_08004AD2:
    /* 0x08004AD2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08004AD4: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */

    L_08004AD6:
    /* 0x08004AD6: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_08004AD8:
    /* 0x08004AD8: 07f00cfe bl       #0x800c6f4 */
    FUN_0800C6F4();   /* BL -> 0x0800C6F4 FUN_0800C6F4 */
    /* 0x08004ADC: f9e7     b        #0x8004ad2 */
    goto L_08004AD2;   /* loop */
}
