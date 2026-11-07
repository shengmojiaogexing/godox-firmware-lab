/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C1E8
 * Name      : FUN_0800C1E8
 * End       : 0x0800C200
 * Size      : 24 bytes, 12 instructions decoded in body (functions.json insns=12, blocks=3)
 * Class     : function
 * Callers   : 1  -> 0x0800BA44 IRQ19_Handler
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C1E8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C1E8: 0548     ldr      r0, [pc, #0x14]  ; =RAM_200001E0 */
    r0 = *(uint32_t *)0x0800C200;   /* = RAM 0x200001E0 */
    /* 0x0800C1EA: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x0800C1EC: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x0800C1EE: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x0800C1F0: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x0800C1F2: 1929     cmp      r1, #0x19 */
    /* flags = cmp(r1, #0x19) */
    /* 0x0800C1F4: 03d3     blo      #0x800c1fe */
    if (r1 < 0x19) goto L_0800C1FE;   /* unsigned */
    /* 0x0800C1F6: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800C1F8: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x0800C1FA: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800C1FC: 4170     strb     r1, [r0, #1] */
    *(uint8_t *)(r0 + 1) = r1;   /* store */

    L_0800C1FE:
    /* 0x0800C1FE: 7047     bx       lr */
    return;
}
