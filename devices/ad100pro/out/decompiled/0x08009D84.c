/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009D84
 * Name      : FUN_08009D84
 * End       : 0x08009DEE
 * Size      : 106 bytes, 45 instructions decoded in body (functions.json insns=45, blocks=7)
 * Class     : function
 * Callers   : 1  -> 0x0800B5EC FUN_0800B5EC
 * Callees   : 6  -> 0x080030F0 FUN_080030F0, 0x08005C40 FUN_08005C40, 0x080081A0 FUN_080081A0, 0x080081E8 FUN_080081E8, 0x080082B4 FUN_080082B4, 0x080082F8 FUN_080082F8
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009D84(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009D84: 30b5     push     {r4, r5, lr} */
    push({r4, r5, lr});
    /* 0x08009D86: 1a49     ldr      r1, [pc, #0x68]  ; =RAM_20000108 */
    r1 = *(uint32_t *)0x08009DF0;   /* = RAM 0x20000108 */
    /* 0x08009D88: 87b0     sub      sp, #0x1c */
    sp = sp - 0x1c;
    /* 0x08009D8A: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08009D8C: 4870     strb     r0, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r0;   /* store */
    /* 0x08009D8E: 194c     ldr      r4, [pc, #0x64]  ; =RAM_200002D0 */
    r4 = *(uint32_t *)0x08009DF4;   /* = RAM 0x200002D0 */
    /* 0x08009D90: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08009D92: 2060     str      r0, [r4] */
    *(uint32_t *)(r4) = r0;   /* store */
    /* 0x08009D94: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x08009D96: 1848     ldr      r0, [pc, #0x60]  ; =0x000003BF */
    r0 = *(uint32_t *)0x08009DF8;   /* = 0x000003BF */
    /* 0x08009D98: a560     str      r5, [r4, #8] */
    *(uint32_t *)(r4 + 8) = r5;   /* store */
    /* 0x08009D9A: 6060     str      r0, [r4, #4] */
    *(uint32_t *)(r4 + 4) = r0;   /* store */
    /* 0x08009D9C: 1748     ldr      r0, [pc, #0x5c]  ; =RAM_20000166 */
    r0 = *(uint32_t *)0x08009DFC;   /* = RAM 0x20000166 */
    /* 0x08009D9E: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x08009DA0: 1748     ldr      r0, [pc, #0x5c]  ; =0x0000C350 */
    r0 = *(uint32_t *)0x08009E00;   /* = 0x0000C350 */
    /* 0x08009DA2: f9f7a5f9 bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x08009DA6: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x08009DA8: 2561     str      r5, [r4, #0x10] */
    *(uint32_t *)(r4 + 16) = r5;   /* store */
    /* 0x08009DAA: e060     str      r0, [r4, #0xc] */
    *(uint32_t *)(r4 + 12) = r0;   /* store */
    /* 0x08009DAC: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08009DAE: fef71bfa bl       #0x80081e8 */
    FUN_080081E8();   /* BL -> 0x080081E8 FUN_080081E8 */
    /* 0x08009DB2: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009DB4: 01d0     beq      #0x8009dba */
    if (r0 == 0) goto L_08009DBA;
    /* 0x08009DB6: fbf743ff bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_08009DBA:
    /* 0x08009DBA: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08009DBC: 0003     lsls     r0, r0, #0xc */
    r0 = r0 << 0xc;
    /* 0x08009DBE: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08009DC0: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08009DC2: 0c48     ldr      r0, [pc, #0x30]  ; =RAM_200002D0 */
    r0 = *(uint32_t *)0x08009DF4;   /* = RAM 0x200002D0 */
    /* 0x08009DC4: fef798fa bl       #0x80082f8 */
    FUN_080082F8();   /* BL -> 0x080082F8 FUN_080082F8 */
    /* 0x08009DC8: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009DCA: 01d0     beq      #0x8009dd0 */
    if (r0 == 0) goto L_08009DD0;
    /* 0x08009DCC: fbf738ff bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_08009DD0:
    /* 0x08009DD0: 0495     str      r5, [sp, #0x10] */
    *(uint32_t *)(sp + 16) = r5;   /* store */
    /* 0x08009DD2: 04a9     add      r1, sp, #0x10 */
    r1 = sp + 0x10;
    /* 0x08009DD4: 0748     ldr      r0, [pc, #0x1c]  ; =RAM_200002D0 */
    r0 = *(uint32_t *)0x08009DF4;   /* = RAM 0x200002D0 */
    /* 0x08009DD6: 0595     str      r5, [sp, #0x14] */
    *(uint32_t *)(sp + 20) = r5;   /* store */
    /* 0x08009DD8: fef7e2f9 bl       #0x80081a0 */
    FUN_080081A0();   /* BL -> 0x080081A0 FUN_080081A0 */
    /* 0x08009DDC: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009DDE: 01d0     beq      #0x8009de4 */
    if (r0 == 0) goto L_08009DE4;
    /* 0x08009DE0: fbf72eff bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_08009DE4:
    /* 0x08009DE4: 0348     ldr      r0, [pc, #0xc]  ; =RAM_200002D0 */
    r0 = *(uint32_t *)0x08009DF4;   /* = RAM 0x200002D0 */
    /* 0x08009DE6: fef765fa bl       #0x80082b4 */
    FUN_080082B4();   /* BL -> 0x080082B4 FUN_080082B4 */
    /* 0x08009DEA: 07b0     add      sp, #0x1c */
    sp = sp + 0x1c;
    /* 0x08009DEC: 30bd     pop      {r4, r5, pc} */
    pop({r4, r5, pc});   /* return */
}
