/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B394
 * Name      : FUN_0800B394
 * End       : 0x0800B3B4
 * Size      : 32 bytes, 15 instructions decoded in body (functions.json insns=15, blocks=5)
 * Class     : function
 * Callers   : 2  -> 0x0800CF74 main, 0x0800D014 FUN_0800D014
 * Callees   : 1  -> 0x0800AB7C FUN_0800AB7C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B394(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B394: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800B396: 0748     ldr      r0, [pc, #0x1c]  ; =RAM_2000017C */
    r0 = *(uint32_t *)0x0800B3B4;   /* = RAM 0x2000017C */
    /* 0x0800B398: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800B39A: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800B39C: 09d0     beq      #0x800b3b2 */
    if (r0 == 0) goto L_0800B3B2;
    /* 0x0800B39E: 064c     ldr      r4, [pc, #0x18]  ; =RAM_2000020E */
    r4 = *(uint32_t *)0x0800B3B8;   /* = RAM 0x2000020E */
    /* 0x0800B3A0: 064d     ldr      r5, [pc, #0x18]  ; =RAM_20000183 */
    r5 = *(uint32_t *)0x0800B3BC;   /* = RAM 0x20000183 */
    /* 0x0800B3A2: e078     ldrb     r0, [r4, #3] */
    r0 = *(uint8_t *)(r4 + 3);
    /* 0x0800B3A4: 2978     ldrb     r1, [r5] */
    r1 = *(uint8_t *)(r5);
    /* 0x0800B3A6: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x0800B3A8: 01d0     beq      #0x800b3ae */
    if (r0 == r1) goto L_0800B3AE;
    /* 0x0800B3AA: fff7e7fb bl       #0x800ab7c */
    FUN_0800AB7C();   /* BL -> 0x0800AB7C FUN_0800AB7C */

    L_0800B3AE:
    /* 0x0800B3AE: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);
    /* 0x0800B3B0: e070     strb     r0, [r4, #3] */
    *(uint8_t *)(r4 + 3) = r0;   /* store */

    L_0800B3B2:
    /* 0x0800B3B2: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
