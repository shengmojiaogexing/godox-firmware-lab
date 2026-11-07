/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004CD4
 * Name      : FUN_08004CD4
 * End       : 0x08004D06
 * Size      : 50 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=8)
 * Class     : function
 * Callers   : 2  -> 0x0800C2B8 FUN_0800C2B8, 0x0800C3CE FUN_0800C3CE
 * Callees   : 1  -> 0x08005090 FUN_08005090
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004CD4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004CD4: 00b5     push     {lr} */
    push({lr});
    /* 0x08004CD6: 0c49     ldr      r1, [pc, #0x30]  ; =RAM_20000198 */
    r1 = *(uint32_t *)0x08004D08;   /* = RAM 0x20000198 */
    /* 0x08004CD8: 0a68     ldr      r2, [r1] */
    r2 = *(uint32_t *)(r1);
    /* 0x08004CDA: 0c49     ldr      r1, [pc, #0x30]  ; =RAM_20000164 */
    r1 = *(uint32_t *)0x08004D0C;   /* = RAM 0x20000164 */
    /* 0x08004CDC: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x08004CDE: 00d1     bne      #0x8004ce2 */
    if (r2 != 0) goto L_08004CE2;
    /* 0x08004CE0: 0a70     strb     r2, [r1] */
    *(uint8_t *)(r1) = r2;   /* store */

    L_08004CE2:
    /* 0x08004CE2: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08004CE4: 0ed0     beq      #0x8004d04 */
    if (r0 == 0) goto L_08004D04;
    /* 0x08004CE6: 0a48     ldr      r0, [pc, #0x28]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x08004D10;   /* = RAM 0x20000175 */
    /* 0x08004CE8: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08004CEA: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08004CEC: 0ad0     beq      #0x8004d04 */
    if (r0 == 0) goto L_08004D04;
    /* 0x08004CEE: 0878     ldrb     r0, [r1] */
    r0 = *(uint8_t *)(r1);
    /* 0x08004CF0: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08004CF2: 03d0     beq      #0x8004cfc */
    if (r0 == 0) goto L_08004CFC;
    /* 0x08004CF4: 0748     ldr      r0, [pc, #0x1c]  ; =RAM_20000228 */
    r0 = *(uint32_t *)0x08004D14;   /* = RAM 0x20000228 */
    /* 0x08004CF6: 0079     ldrb     r0, [r0, #4] */
    r0 = *(uint8_t *)(r0 + 4);
    /* 0x08004CF8: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08004CFA: 03d1     bne      #0x8004d04 */
    if (r0 != 1) goto L_08004D04;

    L_08004CFC:
    /* 0x08004CFC: 4d21     movs     r1, #0x4d */
    r1 = 0x4d;
    /* 0x08004CFE: 0620     movs     r0, #6 */
    r0 = 6;
    /* 0x08004D00: 00f0c6f9 bl       #0x8005090 */
    FUN_08005090();   /* BL -> 0x08005090 FUN_08005090 */

    L_08004D04:
    /* 0x08004D04: 00bd     pop      {pc} */
    pop({pc});   /* return */
}
