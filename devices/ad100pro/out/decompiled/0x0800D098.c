/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800D098
 * Name      : FUN_0800D098
 * End       : 0x0800D0C0
 * Size      : 40 bytes, 18 instructions decoded in body (functions.json insns=18, blocks=5)
 * Class     : function
 * Callers   : 2  -> 0x0800CF74 main, 0x0800D014 FUN_0800D014
 * Callees   : 2  -> 0x0800C2B8 FUN_0800C2B8, 0x0800C82C FUN_0800C82C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800D098(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800D098: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800D09A: 0948     ldr      r0, [pc, #0x24]  ; =RAM_20000228 */
    r0 = *(uint32_t *)0x0800D0C0;   /* = RAM 0x20000228 */
    /* 0x0800D09C: 8178     ldrb     r1, [r0, #2] */
    r1 = *(uint8_t *)(r0 + 2);
    /* 0x0800D09E: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x0800D0A0: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x0800D0A2: 8170     strb     r1, [r0, #2] */
    *(uint8_t *)(r0 + 2) = r1;   /* store */
    /* 0x0800D0A4: 0429     cmp      r1, #4 */
    /* flags = cmp(r1, #4) */
    /* 0x0800D0A6: 02d9     bls      #0x800d0ae */
    if (r1 <= 4) goto L_0800D0AE;   /* unsigned */
    /* 0x0800D0A8: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800D0AA: 8170     strb     r1, [r0, #2] */
    *(uint8_t *)(r0 + 2) = r1;   /* store */
    /* 0x0800D0AC: 01e0     b        #0x800d0b2 */
    goto L_0800D0B2;

    L_0800D0AE:
    /* 0x0800D0AE: 0429     cmp      r1, #4 */
    /* flags = cmp(r1, #4) */
    /* 0x0800D0B0: 03d0     beq      #0x800d0ba */
    if (r1 == 4) goto L_0800D0BA;

    L_0800D0B2:
    /* 0x0800D0B2: c8b2     uxtb     r0, r1 */
    r0 = uxtb(r1);
    /* 0x0800D0B4: fff7bafb bl       #0x800c82c */
    FUN_0800C82C();   /* BL -> 0x0800C82C FUN_0800C82C */
    /* 0x0800D0B8: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800D0BA:
    /* 0x0800D0BA: fff7fdf8 bl       #0x800c2b8 */
    FUN_0800C2B8();   /* BL -> 0x0800C2B8 FUN_0800C2B8 */
    /* 0x0800D0BE: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
