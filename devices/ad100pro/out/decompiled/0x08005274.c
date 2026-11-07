/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005274
 * Name      : FUN_08005274
 * End       : 0x080052A4
 * Size      : 48 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=3)
 * Class     : function
 * Callers   : 2  -> 0x08003F0C FUN_08003F0C, 0x080042B8 FUN_080042B8
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005274(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005274: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x08005276: 0b4c     ldr      r4, [pc, #0x2c]  ; =RAM_200004CC */
    r4 = *(uint32_t *)0x080052A4;   /* = RAM 0x200004CC */
    /* 0x08005278: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800527A: 0b4b     ldr      r3, [pc, #0x2c]  ; =flash_0800EDA2 */
    r3 = *(uint32_t *)0x080052A8;   /* = flash data/const 0x0800EDA2 */
    /* 0x0800527C: c001     lsls     r0, r0, #7 */
    r0 = r0 << 7;
    /* 0x0800527E: 0019     adds     r0, r0, r4 */
    r0 = r0 + r4;

    L_08005280:
    /* 0x08005280: 5500     lsls     r5, r2, #1 */
    r5 = r2 << 1;
    /* 0x08005282: 8c18     adds     r4, r1, r2 */
    r4 = r1 + r2;
    /* 0x08005284: 065d     ldrb     r6, [r0, r4] */
    r6 = *(uint8_t *)(r0 + r4);
    /* 0x08005286: 5f5d     ldrb     r7, [r3, r5] */
    r7 = *(uint8_t *)(r3 + r5);
    /* 0x08005288: ed18     adds     r5, r5, r3 */
    r5 = r5 + r3;
    /* 0x0800528A: 3e43     orrs     r6, r7 */
    r6 = r6 | r7;
    /* 0x0800528C: 0655     strb     r6, [r0, r4] */
    *(uint8_t *)(r0 + r4) = r6;   /* store */
    /* 0x0800528E: 0419     adds     r4, r0, r4 */
    r4 = r0 + r4;
    /* 0x08005290: 8034     adds     r4, #0x80 */
    r4 = r4 + 0x80;
    /* 0x08005292: 2678     ldrb     r6, [r4] */
    r6 = *(uint8_t *)(r4);
    /* 0x08005294: 6d78     ldrb     r5, [r5, #1] */
    r5 = *(uint8_t *)(r5 + 1);
    /* 0x08005296: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x08005298: 2e43     orrs     r6, r5 */
    r6 = r6 | r5;
    /* 0x0800529A: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);
    /* 0x0800529C: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */
    /* 0x0800529E: 102a     cmp      r2, #0x10 */
    /* flags = cmp(r2, #0x10) */
    /* 0x080052A0: eed3     blo      #0x8005280 */
    if (r2 < 0x10) goto L_08005280;   /* unsigned */
    /* 0x080052A2: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
