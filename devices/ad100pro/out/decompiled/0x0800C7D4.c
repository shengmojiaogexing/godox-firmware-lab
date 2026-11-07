/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C7D4
 * Name      : FUN_0800C7D4
 * End       : 0x0800C824
 * Size      : 80 bytes, 33 instructions decoded in body (functions.json insns=33, blocks=6)
 * Class     : function
 * Callers   : 2  -> 0x0800CF74 main, 0x0800D014 FUN_0800D014
 * Callees   : 4  -> 0x08006584 FUN_08006584, 0x0800CAB8 FUN_0800CAB8, 0x0800CD32 FUN_0800CD32, 0x0800CDF4 FUN_0800CDF4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C7D4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C7D4: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800C7D6: 1349     ldr      r1, [pc, #0x4c]  ; =RAM_200001DE */
    r1 = *(uint32_t *)0x0800C824;   /* = RAM 0x200001DE */
    /* 0x0800C7D8: 0878     ldrb     r0, [r1] */
    r0 = *(uint8_t *)(r1);
    /* 0x0800C7DA: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800C7DC: 1fd0     beq      #0x800c81e */
    if (r0 == 0) goto L_0800C81E;
    /* 0x0800C7DE: 124a     ldr      r2, [pc, #0x48]  ; =RAM_2000017C */
    r2 = *(uint32_t *)0x0800C828;   /* = RAM 0x2000017C */
    /* 0x0800C7E0: 1278     ldrb     r2, [r2] */
    r2 = *(uint8_t *)(r2);
    /* 0x0800C7E2: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x0800C7E4: 1bd0     beq      #0x800c81e */
    if (r2 == 0) goto L_0800C81E;
    /* 0x0800C7E6: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x0800C7E8: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800C7EA: 000e     lsrs     r0, r0, #0x18 */
    r0 = r0 >> 0x18;
    /* 0x0800C7EC: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x0800C7EE: 15d1     bne      #0x800c81c */
    if (r2 != 0) goto L_0800C81C;
    /* 0x0800C7F0: 3620     movs     r0, #0x36 */
    r0 = 0x36;
    /* 0x0800C7F2: 00f0fffa bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800C7F6: 3320     movs     r0, #0x33 */
    r0 = 0x33;
    /* 0x0800C7F8: 00f0fcfa bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800C7FC: 7d20     movs     r0, #0x7d */
    r0 = 0x7d;
    /* 0x0800C7FE: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x0800C800: 00f05af9 bl       #0x800cab8 */
    FUN_0800CAB8();   /* BL -> 0x0800CAB8 FUN_0800CAB8 */
    /* 0x0800C804: 3a20     movs     r0, #0x3a */
    r0 = 0x3a;
    /* 0x0800C806: 00f0f5fa bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800C80A: 3420     movs     r0, #0x34 */
    r0 = 0x34;
    /* 0x0800C80C: 00f0f2fa bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800C810: 0421     movs     r1, #4 */
    r1 = 4;
    /* 0x0800C812: 0620     movs     r0, #6 */
    r0 = 6;
    /* 0x0800C814: 00f08dfa bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800C818: f9f7b4fe bl       #0x8006584 */
    FUN_08006584();   /* BL -> 0x08006584 FUN_08006584 */

    L_0800C81C:
    /* 0x0800C81C: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800C81E:
    /* 0x0800C81E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C820: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x0800C822: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
