/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005058
 * Name      : FUN_08005058
 * End       : 0x08005088
 * Size      : 48 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=3)
 * Class     : function
 * Callers   : 3  -> 0x08003F0C FUN_08003F0C, 0x08003FD4 FUN_08003FD4, 0x08004C5C FUN_08004C5C
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005058(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005058: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x0800505A: 0b4c     ldr      r4, [pc, #0x2c]  ; =RAM_200004CC */
    r4 = *(uint32_t *)0x08005088;   /* = RAM 0x200004CC */
    /* 0x0800505C: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800505E: 0b4b     ldr      r3, [pc, #0x2c]  ; =flash_0800EBA8 */
    r3 = *(uint32_t *)0x0800508C;   /* = flash data/const 0x0800EBA8 */
    /* 0x08005060: c001     lsls     r0, r0, #7 */
    r0 = r0 << 7;
    /* 0x08005062: 0019     adds     r0, r0, r4 */
    r0 = r0 + r4;

    L_08005064:
    /* 0x08005064: 5500     lsls     r5, r2, #1 */
    r5 = r2 << 1;
    /* 0x08005066: 8c18     adds     r4, r1, r2 */
    r4 = r1 + r2;
    /* 0x08005068: 065d     ldrb     r6, [r0, r4] */
    r6 = *(uint8_t *)(r0 + r4);
    /* 0x0800506A: 5f5d     ldrb     r7, [r3, r5] */
    r7 = *(uint8_t *)(r3 + r5);
    /* 0x0800506C: ed18     adds     r5, r5, r3 */
    r5 = r5 + r3;
    /* 0x0800506E: 3e43     orrs     r6, r7 */
    r6 = r6 | r7;
    /* 0x08005070: 0655     strb     r6, [r0, r4] */
    *(uint8_t *)(r0 + r4) = r6;   /* store */
    /* 0x08005072: 0419     adds     r4, r0, r4 */
    r4 = r0 + r4;
    /* 0x08005074: 8034     adds     r4, #0x80 */
    r4 = r4 + 0x80;
    /* 0x08005076: 2678     ldrb     r6, [r4] */
    r6 = *(uint8_t *)(r4);
    /* 0x08005078: 6d78     ldrb     r5, [r5, #1] */
    r5 = *(uint8_t *)(r5 + 1);
    /* 0x0800507A: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x0800507C: 2e43     orrs     r6, r5 */
    r6 = r6 | r5;
    /* 0x0800507E: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);
    /* 0x08005080: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */
    /* 0x08005082: 092a     cmp      r2, #9 */
    /* flags = cmp(r2, #9) */
    /* 0x08005084: eed3     blo      #0x8005064 */
    if (r2 < 9) goto L_08005064;   /* unsigned */
    /* 0x08005086: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
