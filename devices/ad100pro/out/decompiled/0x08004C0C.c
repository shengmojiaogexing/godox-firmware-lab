/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004C0C
 * Name      : FUN_08004C0C
 * End       : 0x08004C4C
 * Size      : 64 bytes, 32 instructions decoded in body (functions.json insns=32, blocks=3)
 * Class     : function
 * Callers   : 3  -> 0x0800BDC4 FUN_0800BDC4, 0x0800BE2A FUN_0800BE2A, 0x0800C2B8 FUN_0800C2B8
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004C0C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004C0C: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x08004C0E: 0524     movs     r4, #5 */
    r4 = 5;
    /* 0x08004C10: 0325     movs     r5, #3 */
    r5 = 3;
    /* 0x08004C12: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08004C14: 0d4b     ldr      r3, [pc, #0x34]  ; =RAM_200004CC */
    r3 = *(uint32_t *)0x08004C4C;   /* = RAM 0x200004CC */
    /* 0x08004C16: e401     lsls     r4, r4, #7 */
    r4 = r4 << 7;
    /* 0x08004C18: 2d02     lsls     r5, r5, #8 */
    r5 = r5 << 8;
    /* 0x08004C1A: 0727     movs     r7, #7 */
    r7 = 7;
    /* 0x08004C1C: ff01     lsls     r7, r7, #7 */
    r7 = r7 << 7;

    L_08004C1E:
    /* 0x08004C1E: 9854     strb     r0, [r3, r2] */
    *(uint8_t *)(r3 + r2) = r0;   /* store */
    /* 0x08004C20: 9918     adds     r1, r3, r2 */
    r1 = r3 + r2;
    /* 0x08004C22: 8026     movs     r6, #0x80 */
    r6 = 0x80;
    /* 0x08004C24: 7054     strb     r0, [r6, r1] */
    *(uint8_t *)(r6 + r1) = r0;   /* store */
    /* 0x08004C26: 7600     lsls     r6, r6, #1 */
    r6 = r6 << 1;
    /* 0x08004C28: 7054     strb     r0, [r6, r1] */
    *(uint8_t *)(r6 + r1) = r0;   /* store */
    /* 0x08004C2A: ff26     movs     r6, #0xff */
    r6 = 0xff;
    /* 0x08004C2C: 8136     adds     r6, #0x81 */
    r6 = r6 + 0x81;
    /* 0x08004C2E: 7054     strb     r0, [r6, r1] */
    *(uint8_t *)(r6 + r1) = r0;   /* store */
    /* 0x08004C30: 0126     movs     r6, #1 */
    r6 = 1;
    /* 0x08004C32: 7602     lsls     r6, r6, #9 */
    r6 = r6 << 9;
    /* 0x08004C34: 7054     strb     r0, [r6, r1] */
    *(uint8_t *)(r6 + r1) = r0;   /* store */
    /* 0x08004C36: 0e19     adds     r6, r1, r4 */
    r6 = r1 + r4;
    /* 0x08004C38: 3070     strb     r0, [r6] */
    *(uint8_t *)(r6) = r0;   /* store */
    /* 0x08004C3A: 4e19     adds     r6, r1, r5 */
    r6 = r1 + r5;
    /* 0x08004C3C: c919     adds     r1, r1, r7 */
    r1 = r1 + r7;
    /* 0x08004C3E: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x08004C40: 3070     strb     r0, [r6] */
    *(uint8_t *)(r6) = r0;   /* store */
    /* 0x08004C42: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);
    /* 0x08004C44: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08004C46: 802a     cmp      r2, #0x80 */
    /* flags = cmp(r2, #0x80) */
    /* 0x08004C48: e9d3     blo      #0x8004c1e */
    if (r2 < 0x80) goto L_08004C1E;   /* unsigned */
    /* 0x08004C4A: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
