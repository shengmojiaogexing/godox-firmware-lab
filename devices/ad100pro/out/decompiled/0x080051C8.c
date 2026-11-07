/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080051C8
 * Name      : FUN_080051C8
 * End       : 0x080051F8
 * Size      : 48 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=3)
 * Class     : function
 * Callers   : 1  -> 0x080053E4 FUN_080053E4
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080051C8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080051C8: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x080051CA: 0b4c     ldr      r4, [pc, #0x2c]  ; =RAM_200004CC */
    r4 = *(uint32_t *)0x080051F8;   /* = RAM 0x200004CC */
    /* 0x080051CC: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x080051CE: 0b4b     ldr      r3, [pc, #0x2c]  ; =flash_0800EC02 */
    r3 = *(uint32_t *)0x080051FC;   /* = flash data/const 0x0800EC02 */
    /* 0x080051D0: c001     lsls     r0, r0, #7 */
    r0 = r0 << 7;
    /* 0x080051D2: 0019     adds     r0, r0, r4 */
    r0 = r0 + r4;

    L_080051D4:
    /* 0x080051D4: 5500     lsls     r5, r2, #1 */
    r5 = r2 << 1;
    /* 0x080051D6: 8c18     adds     r4, r1, r2 */
    r4 = r1 + r2;
    /* 0x080051D8: 065d     ldrb     r6, [r0, r4] */
    r6 = *(uint8_t *)(r0 + r4);
    /* 0x080051DA: 5f5d     ldrb     r7, [r3, r5] */
    r7 = *(uint8_t *)(r3 + r5);
    /* 0x080051DC: ed18     adds     r5, r5, r3 */
    r5 = r5 + r3;
    /* 0x080051DE: 3e43     orrs     r6, r7 */
    r6 = r6 | r7;
    /* 0x080051E0: 0655     strb     r6, [r0, r4] */
    *(uint8_t *)(r0 + r4) = r6;   /* store */
    /* 0x080051E2: 0419     adds     r4, r0, r4 */
    r4 = r0 + r4;
    /* 0x080051E4: 8034     adds     r4, #0x80 */
    r4 = r4 + 0x80;
    /* 0x080051E6: 2678     ldrb     r6, [r4] */
    r6 = *(uint8_t *)(r4);
    /* 0x080051E8: 6d78     ldrb     r5, [r5, #1] */
    r5 = *(uint8_t *)(r5 + 1);
    /* 0x080051EA: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x080051EC: 2e43     orrs     r6, r5 */
    r6 = r6 | r5;
    /* 0x080051EE: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);
    /* 0x080051F0: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */
    /* 0x080051F2: 102a     cmp      r2, #0x10 */
    /* flags = cmp(r2, #0x10) */
    /* 0x080051F4: eed3     blo      #0x80051d4 */
    if (r2 < 0x10) goto L_080051D4;   /* unsigned */
    /* 0x080051F6: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
