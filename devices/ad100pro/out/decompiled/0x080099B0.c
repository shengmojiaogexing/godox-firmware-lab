/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080099B0
 * Name      : FUN_080099B0
 * End       : 0x080099E6
 * Size      : 54 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=7)
 * Class     : function
 * Callers   : 2  -> 0x0800BA44 IRQ19_Handler, 0x0800BA48 FUN_0800BA48
 * Callees   : 3  -> 0x080099EC FUN_080099EC, 0x08009AD0 FUN_08009AD0, 0x08009B7C FUN_08009B7C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080099B0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080099B0: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x080099B2: 0d48     ldr      r0, [pc, #0x34]  ; =RAM_20000230 */
    r0 = *(uint32_t *)0x080099E8;   /* = RAM 0x20000230 */
    /* 0x080099B4: 4178     ldrb     r1, [r0, #1] */
    r1 = *(uint8_t *)(r0 + 1);
    /* 0x080099B6: 0429     cmp      r1, #4 */
    /* flags = cmp(r1, #4) */
    /* 0x080099B8: 01d9     bls      #0x80099be */
    if (r1 <= 4) goto L_080099BE;   /* unsigned */
    /* 0x080099BA: 0421     movs     r1, #4 */
    r1 = 4;
    /* 0x080099BC: 4170     strb     r1, [r0, #1] */
    *(uint8_t *)(r0 + 1) = r1;   /* store */

    L_080099BE:
    /* 0x080099BE: 8178     ldrb     r1, [r0, #2] */
    r1 = *(uint8_t *)(r0 + 2);
    /* 0x080099C0: 0429     cmp      r1, #4 */
    /* flags = cmp(r1, #4) */
    /* 0x080099C2: 01d9     bls      #0x80099c8 */
    if (r1 <= 4) goto L_080099C8;   /* unsigned */
    /* 0x080099C4: 4178     ldrb     r1, [r0, #1] */
    r1 = *(uint8_t *)(r0 + 1);
    /* 0x080099C6: 8170     strb     r1, [r0, #2] */
    *(uint8_t *)(r0 + 2) = r1;   /* store */

    L_080099C8:
    /* 0x080099C8: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x080099CA: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x080099CC: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x080099CE: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x080099D0: 0529     cmp      r1, #5 */
    /* flags = cmp(r1, #5) */
    /* 0x080099D2: 07d3     blo      #0x80099e4 */
    if (r1 < 5) goto L_080099E4;   /* unsigned */
    /* 0x080099D4: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x080099D6: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x080099D8: 00f0d0f8 bl       #0x8009b7c */
    FUN_08009B7C();   /* BL -> 0x08009B7C FUN_08009B7C */
    /* 0x080099DC: 00f078f8 bl       #0x8009ad0 */
    FUN_08009AD0();   /* BL -> 0x08009AD0 FUN_08009AD0 */
    /* 0x080099E0: 00f004f8 bl       #0x80099ec */
    FUN_080099EC();   /* BL -> 0x080099EC FUN_080099EC */

    L_080099E4:
    /* 0x080099E4: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
