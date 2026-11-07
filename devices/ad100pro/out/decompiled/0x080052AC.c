/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080052AC
 * Name      : FUN_080052AC
 * End       : 0x080052DC
 * Size      : 48 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=3)
 * Class     : function
 * Callers   : 1  -> 0x08004F94 FUN_08004F94
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080052AC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080052AC: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x080052AE: 0b4c     ldr      r4, [pc, #0x2c]  ; =RAM_200004CC */
    r4 = *(uint32_t *)0x080052DC;   /* = RAM 0x200004CC */
    /* 0x080052B0: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x080052B2: 0b4b     ldr      r3, [pc, #0x2c]  ; =flash_0800EC52 */
    r3 = *(uint32_t *)0x080052E0;   /* = flash data/const 0x0800EC52 */
    /* 0x080052B4: c001     lsls     r0, r0, #7 */
    r0 = r0 << 7;
    /* 0x080052B6: 0019     adds     r0, r0, r4 */
    r0 = r0 + r4;

    L_080052B8:
    /* 0x080052B8: 5500     lsls     r5, r2, #1 */
    r5 = r2 << 1;
    /* 0x080052BA: 8c18     adds     r4, r1, r2 */
    r4 = r1 + r2;
    /* 0x080052BC: 065d     ldrb     r6, [r0, r4] */
    r6 = *(uint8_t *)(r0 + r4);
    /* 0x080052BE: 5f5d     ldrb     r7, [r3, r5] */
    r7 = *(uint8_t *)(r3 + r5);
    /* 0x080052C0: ed18     adds     r5, r5, r3 */
    r5 = r5 + r3;
    /* 0x080052C2: 3e43     orrs     r6, r7 */
    r6 = r6 | r7;
    /* 0x080052C4: 0655     strb     r6, [r0, r4] */
    *(uint8_t *)(r0 + r4) = r6;   /* store */
    /* 0x080052C6: 0419     adds     r4, r0, r4 */
    r4 = r0 + r4;
    /* 0x080052C8: 8034     adds     r4, #0x80 */
    r4 = r4 + 0x80;
    /* 0x080052CA: 2678     ldrb     r6, [r4] */
    r6 = *(uint8_t *)(r4);
    /* 0x080052CC: 6d78     ldrb     r5, [r5, #1] */
    r5 = *(uint8_t *)(r5 + 1);
    /* 0x080052CE: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x080052D0: 2e43     orrs     r6, r5 */
    r6 = r6 | r5;
    /* 0x080052D2: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);
    /* 0x080052D4: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */
    /* 0x080052D6: 0c2a     cmp      r2, #0xc */
    /* flags = cmp(r2, #0xc) */
    /* 0x080052D8: eed3     blo      #0x80052b8 */
    if (r2 < 0xc) goto L_080052B8;   /* unsigned */
    /* 0x080052DA: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
