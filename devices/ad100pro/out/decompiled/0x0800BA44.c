/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800BA44
 * Name      : IRQ19_Handler
 * End       : 0x0800BA6E
 * Size      : 42 bytes, 17 instructions decoded in body (functions.json insns=17, blocks=5)
 * Class     : vector_handler
 * Callers   : 0  -> -
 * Callees   : 4  -> 0x08004AA8 FUN_08004AA8, 0x080083E8 FUN_080083E8, 0x080099B0 FUN_080099B0, 0x0800C1E8 FUN_0800C1E8
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void IRQ19_Handler(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800BA44: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800BA46: 00f0cffb bl       #0x800c1e8 */
    FUN_0800C1E8();   /* BL -> 0x0800C1E8 FUN_0800C1E8 */
    /* 0x0800BA4A: 0948     ldr      r0, [pc, #0x24]  ; =RAM_200001D4 */
    r0 = *(uint32_t *)0x0800BA70;   /* = RAM 0x200001D4 */
    /* 0x0800BA4C: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800BA4E: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800BA50: 09d1     bne      #0x800ba66 */
    if (r0 != 0) goto L_0800BA66;
    /* 0x0800BA52: f9f729f8 bl       #0x8004aa8 */
    FUN_08004AA8();   /* BL -> 0x08004AA8 FUN_08004AA8 */
    /* 0x0800BA56: 0748     ldr      r0, [pc, #0x1c]  ; =RAM_200001CA */
    r0 = *(uint32_t *)0x0800BA74;   /* = RAM 0x200001CA */
    /* 0x0800BA58: 0188     ldrh     r1, [r0] */
    r1 = *(uint16_t *)(r0);
    /* 0x0800BA5A: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x0800BA5C: 01d0     beq      #0x800ba62 */
    if (r1 == 0) goto L_0800BA62;
    /* 0x0800BA5E: 491e     subs     r1, r1, #1 */
    r1 = r1 - 1;
    /* 0x0800BA60: 0180     strh     r1, [r0] */
    *(uint16_t *)(r0) = r1;   /* store */

    L_0800BA62:
    /* 0x0800BA62: fdf7a5ff bl       #0x80099b0 */
    FUN_080099B0();   /* BL -> 0x080099B0 FUN_080099B0 */

    L_0800BA66:
    /* 0x0800BA66: 0448     ldr      r0, [pc, #0x10]  ; =RAM_20000348 */
    r0 = *(uint32_t *)0x0800BA78;   /* = RAM 0x20000348 */
    /* 0x0800BA68: fcf7befc bl       #0x80083e8 */
    FUN_080083E8();   /* BL -> 0x080083E8 FUN_080083E8 */
    /* 0x0800BA6C: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
