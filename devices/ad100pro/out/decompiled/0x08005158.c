/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005158
 * Name      : FUN_08005158
 * End       : 0x08005188
 * Size      : 48 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=3)
 * Class     : function
 * Callers   : 1  -> 0x080058D0 FUN_080058D0
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005158(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005158: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x0800515A: 0b4c     ldr      r4, [pc, #0x2c]  ; =RAM_200004CC */
    r4 = *(uint32_t *)0x08005188;   /* = RAM 0x200004CC */
    /* 0x0800515C: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800515E: 0b4b     ldr      r3, [pc, #0x2c]  ; =flash_0800EC32 */
    r3 = *(uint32_t *)0x0800518C;   /* = flash data/const 0x0800EC32 */
    /* 0x08005160: c001     lsls     r0, r0, #7 */
    r0 = r0 << 7;
    /* 0x08005162: 0019     adds     r0, r0, r4 */
    r0 = r0 + r4;

    L_08005164:
    /* 0x08005164: 5500     lsls     r5, r2, #1 */
    r5 = r2 << 1;
    /* 0x08005166: 8c18     adds     r4, r1, r2 */
    r4 = r1 + r2;
    /* 0x08005168: 065d     ldrb     r6, [r0, r4] */
    r6 = *(uint8_t *)(r0 + r4);
    /* 0x0800516A: 5f5d     ldrb     r7, [r3, r5] */
    r7 = *(uint8_t *)(r3 + r5);
    /* 0x0800516C: ed18     adds     r5, r5, r3 */
    r5 = r5 + r3;
    /* 0x0800516E: 3e43     orrs     r6, r7 */
    r6 = r6 | r7;
    /* 0x08005170: 0655     strb     r6, [r0, r4] */
    *(uint8_t *)(r0 + r4) = r6;   /* store */
    /* 0x08005172: 0419     adds     r4, r0, r4 */
    r4 = r0 + r4;
    /* 0x08005174: 8034     adds     r4, #0x80 */
    r4 = r4 + 0x80;
    /* 0x08005176: 2678     ldrb     r6, [r4] */
    r6 = *(uint8_t *)(r4);
    /* 0x08005178: 6d78     ldrb     r5, [r5, #1] */
    r5 = *(uint8_t *)(r5 + 1);
    /* 0x0800517A: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x0800517C: 2e43     orrs     r6, r5 */
    r6 = r6 | r5;
    /* 0x0800517E: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);
    /* 0x08005180: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */
    /* 0x08005182: 102a     cmp      r2, #0x10 */
    /* flags = cmp(r2, #0x10) */
    /* 0x08005184: eed3     blo      #0x8005164 */
    if (r2 < 0x10) goto L_08005164;   /* unsigned */
    /* 0x08005186: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
