/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004FC4
 * Name      : FUN_08004FC4
 * End       : 0x08004FFE
 * Size      : 58 bytes, 29 instructions decoded in body (functions.json insns=29, blocks=4)
 * Class     : function
 * Callers   : 2  -> 0x0800C2B8 FUN_0800C2B8, 0x0800C38C FUN_0800C38C
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004FC4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004FC4: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x08004FC6: 032a     cmp      r2, #3 */
    /* flags = cmp(r2, #3) */
    /* 0x08004FC8: 18d8     bhi      #0x8004ffc */
    if (r2 > 3) goto L_08004FFC;   /* unsigned */
    /* 0x08004FCA: 0e4d     ldr      r5, [pc, #0x38]  ; =flash_0800EBBA */
    r5 = *(uint32_t *)0x08005004;   /* = flash data/const 0x0800EBBA */
    /* 0x08004FCC: 1226     movs     r6, #0x12 */
    r6 = 0x12;
    /* 0x08004FCE: 0c4c     ldr      r4, [pc, #0x30]  ; =RAM_200004CC */
    r4 = *(uint32_t *)0x08005000;   /* = RAM 0x200004CC */
    /* 0x08004FD0: 7243     muls     r2, r6, r2 */
    r2 = r6 * r2;
    /* 0x08004FD2: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x08004FD4: 5219     adds     r2, r2, r5 */
    r2 = r2 + r5;
    /* 0x08004FD6: c001     lsls     r0, r0, #7 */
    r0 = r0 << 7;
    /* 0x08004FD8: 0019     adds     r0, r0, r4 */
    r0 = r0 + r4;

    L_08004FDA:
    /* 0x08004FDA: 5d00     lsls     r5, r3, #1 */
    r5 = r3 << 1;
    /* 0x08004FDC: cc18     adds     r4, r1, r3 */
    r4 = r1 + r3;
    /* 0x08004FDE: 065d     ldrb     r6, [r0, r4] */
    r6 = *(uint8_t *)(r0 + r4);
    /* 0x08004FE0: 575d     ldrb     r7, [r2, r5] */
    r7 = *(uint8_t *)(r2 + r5);
    /* 0x08004FE2: 5519     adds     r5, r2, r5 */
    r5 = r2 + r5;
    /* 0x08004FE4: 3e43     orrs     r6, r7 */
    r6 = r6 | r7;
    /* 0x08004FE6: 0655     strb     r6, [r0, r4] */
    *(uint8_t *)(r0 + r4) = r6;   /* store */
    /* 0x08004FE8: 0419     adds     r4, r0, r4 */
    r4 = r0 + r4;
    /* 0x08004FEA: 8034     adds     r4, #0x80 */
    r4 = r4 + 0x80;
    /* 0x08004FEC: 2678     ldrb     r6, [r4] */
    r6 = *(uint8_t *)(r4);
    /* 0x08004FEE: 6d78     ldrb     r5, [r5, #1] */
    r5 = *(uint8_t *)(r5 + 1);
    /* 0x08004FF0: 5b1c     adds     r3, r3, #1 */
    r3 = r3 + 1;
    /* 0x08004FF2: 2e43     orrs     r6, r5 */
    r6 = r6 | r5;
    /* 0x08004FF4: dbb2     uxtb     r3, r3 */
    r3 = uxtb(r3);
    /* 0x08004FF6: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */
    /* 0x08004FF8: 062b     cmp      r3, #6 */
    /* flags = cmp(r3, #6) */
    /* 0x08004FFA: eed3     blo      #0x8004fda */
    if (r3 < 6) goto L_08004FDA;   /* unsigned */

    L_08004FFC:
    /* 0x08004FFC: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
