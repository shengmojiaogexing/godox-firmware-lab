/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005190
 * Name      : FUN_08005190
 * End       : 0x080051C0
 * Size      : 48 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=3)
 * Class     : function
 * Callers   : 5  -> 0x08005474 FUN_08005474, 0x08005490 FUN_08005490, 0x080058E0 FUN_080058E0, 0x0800C2B8 FUN_0800C2B8, 0x0800C3BE FUN_0800C3BE
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005190(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005190: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x08005192: 0b4c     ldr      r4, [pc, #0x2c]  ; =RAM_200004CC */
    r4 = *(uint32_t *)0x080051C0;   /* = RAM 0x200004CC */
    /* 0x08005194: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08005196: 0b4b     ldr      r3, [pc, #0x2c]  ; =flash_0800ECEA */
    r3 = *(uint32_t *)0x080051C4;   /* = flash data/const 0x0800ECEA */
    /* 0x08005198: c001     lsls     r0, r0, #7 */
    r0 = r0 << 7;
    /* 0x0800519A: 0019     adds     r0, r0, r4 */
    r0 = r0 + r4;

    L_0800519C:
    /* 0x0800519C: 5500     lsls     r5, r2, #1 */
    r5 = r2 << 1;
    /* 0x0800519E: 8c18     adds     r4, r1, r2 */
    r4 = r1 + r2;
    /* 0x080051A0: 065d     ldrb     r6, [r0, r4] */
    r6 = *(uint8_t *)(r0 + r4);
    /* 0x080051A2: 5f5d     ldrb     r7, [r3, r5] */
    r7 = *(uint8_t *)(r3 + r5);
    /* 0x080051A4: ed18     adds     r5, r5, r3 */
    r5 = r5 + r3;
    /* 0x080051A6: 3e43     orrs     r6, r7 */
    r6 = r6 | r7;
    /* 0x080051A8: 0655     strb     r6, [r0, r4] */
    *(uint8_t *)(r0 + r4) = r6;   /* store */
    /* 0x080051AA: 0419     adds     r4, r0, r4 */
    r4 = r0 + r4;
    /* 0x080051AC: 8034     adds     r4, #0x80 */
    r4 = r4 + 0x80;
    /* 0x080051AE: 2678     ldrb     r6, [r4] */
    r6 = *(uint8_t *)(r4);
    /* 0x080051B0: 6d78     ldrb     r5, [r5, #1] */
    r5 = *(uint8_t *)(r5 + 1);
    /* 0x080051B2: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x080051B4: 2e43     orrs     r6, r5 */
    r6 = r6 | r5;
    /* 0x080051B6: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);
    /* 0x080051B8: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */
    /* 0x080051BA: 0c2a     cmp      r2, #0xc */
    /* flags = cmp(r2, #0xc) */
    /* 0x080051BC: eed3     blo      #0x800519c */
    if (r2 < 0xc) goto L_0800519C;   /* unsigned */
    /* 0x080051BE: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
