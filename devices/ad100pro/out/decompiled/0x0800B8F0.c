/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B8F0
 * Name      : FUN_0800B8F0
 * End       : 0x0800B962
 * Size      : 114 bytes, 49 instructions decoded in body (functions.json insns=49, blocks=7)
 * Class     : hub
 * Callers   : 1  -> 0x0800CF74 main
 * Callees   : 8  -> 0x080030F0 FUN_080030F0, 0x08007670 FUN_08007670, 0x08007678 FUN_08007678, 0x08007798 FUN_08007798, 0x080078EC FUN_080078EC, 0x08007980 FUN_08007980, 0x08008148 FUN_08008148, 0x08008164 FUN_08008164
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B8F0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B8F0: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800B8F2: 98b0     sub      sp, #0x60 */
    sp = sp - 0x60;
    /* 0x0800B8F4: 2820     movs     r0, #0x28 */
    r0 = 0x28;
    /* 0x0800B8F6: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x0800B8F8: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800B8FA: 0890     str      r0, [sp, #0x20] */
    *(uint32_t *)(sp + 32) = r0;   /* store */
    /* 0x0800B8FC: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x0800B8FE: 0790     str      r0, [sp, #0x1c] */
    *(uint32_t *)(sp + 28) = r0;   /* store */
    /* 0x0800B900: 0994     str      r4, [sp, #0x24] */
    *(uint32_t *)(sp + 36) = r4;   /* store */
    /* 0x0800B902: 6846     mov      r0, sp */
    r0 = sp;
    /* 0x0800B904: fcf73cf8 bl       #0x8007980 */
    FUN_08007980();   /* BL -> 0x08007980 FUN_08007980 */
    /* 0x0800B908: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800B90A: 00d0     beq      #0x800b90e */
    if (r0 == 0) goto L_0800B90E;

    L_0800B90C:
    /* 0x0800B90C: fee7     b        #0x800b90c */
    goto L_0800B90C;   /* loop */

    L_0800B90E:
    /* 0x0800B90E: 0720     movs     r0, #7 */
    r0 = 7;
    /* 0x0800B910: 0d90     str      r0, [sp, #0x34] */
    *(uint32_t *)(sp + 52) = r0;   /* store */
    /* 0x0800B912: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x0800B914: 0e90     str      r0, [sp, #0x38] */
    *(uint32_t *)(sp + 56) = r0;   /* store */
    /* 0x0800B916: 0f94     str      r4, [sp, #0x3c] */
    *(uint32_t *)(sp + 60) = r4;   /* store */
    /* 0x0800B918: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800B91A: 1094     str      r4, [sp, #0x40] */
    *(uint32_t *)(sp + 64) = r4;   /* store */
    /* 0x0800B91C: 0da8     add      r0, sp, #0x34 */
    r0 = sp + 0x34;
    /* 0x0800B91E: fbf73bff bl       #0x8007798 */
    FUN_08007798();   /* BL -> 0x08007798 FUN_08007798 */
    /* 0x0800B922: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800B924: 00d0     beq      #0x800b928 */
    if (r0 == 0) goto L_0800B928;

    L_0800B926:
    /* 0x0800B926: fee7     b        #0x800b926 */
    goto L_0800B926;   /* loop */

    L_0800B928:
    /* 0x0800B928: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800B92A: 0004     lsls     r0, r0, #0x10 */
    r0 = r0 << 0x10;
    /* 0x0800B92C: 1190     str      r0, [sp, #0x44] */
    *(uint32_t *)(sp + 68) = r0;   /* store */
    /* 0x0800B92E: c011     asrs     r0, r0, #7 */
    r0 = r0 >> 7;
    /* 0x0800B930: 1290     str      r0, [sp, #0x48] */
    *(uint32_t *)(sp + 72) = r0;   /* store */
    /* 0x0800B932: 11a8     add      r0, sp, #0x44 */
    r0 = sp + 0x44;
    /* 0x0800B934: fbf7a0fe bl       #0x8007678 */
    FUN_08007678();   /* BL -> 0x08007678 FUN_08007678 */
    /* 0x0800B938: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800B93A: 00d0     beq      #0x800b93e */
    if (r0 == 0) goto L_0800B93E;

    L_0800B93C:
    /* 0x0800B93C: fee7     b        #0x800b93c */
    goto L_0800B93C;   /* loop */

    L_0800B93E:
    /* 0x0800B93E: fbf7d5ff bl       #0x80078ec */
    FUN_080078EC();   /* BL -> 0x080078EC FUN_080078EC */
    /* 0x0800B942: 7d21     movs     r1, #0x7d */
    r1 = 0x7d;
    /* 0x0800B944: c900     lsls     r1, r1, #3 */
    r1 = r1 << 3;
    /* 0x0800B946: f7f7d3fb bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x0800B94A: fcf70bfc bl       #0x8008164 */
    FUN_08008164();   /* BL -> 0x08008164 FUN_08008164 */
    /* 0x0800B94E: 0420     movs     r0, #4 */
    r0 = 4;
    /* 0x0800B950: fcf7fafb bl       #0x8008148 */
    FUN_08008148();   /* BL -> 0x08008148 FUN_08008148 */
    /* 0x0800B954: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800B956: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x0800B958: 501e     subs     r0, r2, #1 */
    r0 = r2 - 1;
    /* 0x0800B95A: fbf789fe bl       #0x8007670 */
    FUN_08007670();   /* BL -> 0x08007670 FUN_08007670 */
    /* 0x0800B95E: 18b0     add      sp, #0x60 */
    sp = sp + 0x60;
    /* 0x0800B960: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
