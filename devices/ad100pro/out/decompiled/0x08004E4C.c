/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004E4C
 * Name      : FUN_08004E4C
 * End       : 0x08004E90
 * Size      : 68 bytes, 34 instructions decoded in body (functions.json insns=34, blocks=4)
 * Class     : function
 * Callers   : 6  -> 0x08005400 FUN_08005400, 0x08005430 FUN_08005430, 0x0800543C FUN_0800543C, 0x080056E4 FUN_080056E4, 0x08005712 FUN_08005712, 0x080057AE FUN_080057AE
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004E4C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004E4C: ffb5     push     {r0, r1, r2, r3, r4, r5, r6, r7, lr} */
    push({r0, r1, r2, r3, r4, r5, r6, r7, lr});
    /* 0x08004E4E: 8446     mov      ip, r0 */
    ip = r0;
    /* 0x08004E50: 8e46     mov      lr, r1 */
    lr = r1;
    /* 0x08004E52: d000     lsls     r0, r2, #3 */
    r0 = r2 << 3;
    /* 0x08004E54: 1718     adds     r7, r2, r0 */
    r7 = r2 + r0;
    /* 0x08004E56: 0e49     ldr      r1, [pc, #0x38]  ; =flash_0800DB0C */
    r1 = *(uint32_t *)0x08004E90;   /* = flash data/const 0x0800DB0C */
    /* 0x08004E58: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x08004E5A: ce5d     ldrb     r6, [r1, r7] */
    r6 = *(uint8_t *)(r1 + r7);
    /* 0x08004E5C: 15e0     b        #0x8004e8a */
    goto L_08004E8A;

    L_08004E5E:
    /* 0x08004E5E: 7818     adds     r0, r7, r1 */
    r0 = r7 + r1;
    /* 0x08004E60: c018     adds     r0, r0, r3 */
    r0 = r0 + r3;
    /* 0x08004E62: 4278     ldrb     r2, [r0, #1] */
    r2 = *(uint8_t *)(r0 + 1);
    /* 0x08004E64: 0398     ldr      r0, [sp, #0xc] */
    r0 = *(uint32_t *)(sp + 12);
    /* 0x08004E66: 0b4c     ldr      r4, [pc, #0x2c]  ; =RAM_200004CC */
    r4 = *(uint32_t *)0x08004E94;   /* = RAM 0x200004CC */
    /* 0x08004E68: 8240     lsls     r2, r0 */
    r2 = r2 << r0;
    /* 0x08004E6A: 6046     mov      r0, ip */
    r0 = ip;
    /* 0x08004E6C: c501     lsls     r5, r0, #7 */
    r5 = r0 << 7;
    /* 0x08004E6E: 2c19     adds     r4, r5, r4 */
    r4 = r5 + r4;
    /* 0x08004E70: 7046     mov      r0, lr */
    r0 = lr;
    /* 0x08004E72: c518     adds     r5, r0, r3 */
    r5 = r0 + r3;
    /* 0x08004E74: 605d     ldrb     r0, [r4, r5] */
    r0 = *(uint8_t *)(r4 + r5);
    /* 0x08004E76: 1043     orrs     r0, r2 */
    r0 = r0 | r2;
    /* 0x08004E78: 6055     strb     r0, [r4, r5] */
    *(uint8_t *)(r4 + r5) = r0;   /* store */
    /* 0x08004E7A: 6419     adds     r4, r4, r5 */
    r4 = r4 + r5;
    /* 0x08004E7C: 803c     subs     r4, #0x80 */
    r4 = r4 - 0x80;
    /* 0x08004E7E: 2578     ldrb     r5, [r4] */
    r5 = *(uint8_t *)(r4);
    /* 0x08004E80: 120a     lsrs     r2, r2, #8 */
    r2 = r2 >> 8;
    /* 0x08004E82: 1543     orrs     r5, r2 */
    r5 = r5 | r2;
    /* 0x08004E84: 5b1c     adds     r3, r3, #1 */
    r3 = r3 + 1;
    /* 0x08004E86: 2570     strb     r5, [r4] */
    *(uint8_t *)(r4) = r5;   /* store */
    /* 0x08004E88: dbb2     uxtb     r3, r3 */
    r3 = uxtb(r3);

    L_08004E8A:
    /* 0x08004E8A: b342     cmp      r3, r6 */
    /* flags = cmp(r3, r6) */
    /* 0x08004E8C: e7d3     blo      #0x8004e5e */
    if (r3 < r6) goto L_08004E5E;   /* unsigned */
    /* 0x08004E8E: ffbd     pop      {r0, r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r0, r1, r2, r3, r4, r5, r6, r7, pc});   /* return */
}
