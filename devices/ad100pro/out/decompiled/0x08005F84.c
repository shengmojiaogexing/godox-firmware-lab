/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005F84
 * Name      : FUN_08005F84
 * End       : 0x08005FFA
 * Size      : 118 bytes, 51 instructions decoded in body (functions.json insns=51, blocks=3)
 * Class     : function
 * Callers   : 2  -> 0x08006014 FUN_08006014, 0x0800C6F4 FUN_0800C6F4
 * Callees   : 3  -> 0x080072AC FUN_080072AC, 0x08007426 FUN_08007426, 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005F84(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005F84: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08005F86: 1d4c     ldr      r4, [pc, #0x74]  ; =RAM_2000015E */
    r4 = *(uint32_t *)0x08005FFC;   /* = RAM 0x2000015E */
    /* 0x08005F88: 2088     ldrh     r0, [r4] */
    r0 = *(uint16_t *)(r4);
    /* 0x08005F8A: 0f28     cmp      r0, #0xf */
    /* flags = cmp(r0, #0xf) */
    /* 0x08005F8C: 01d9     bls      #0x8005f92 */
    if (r0 <= 0xf) goto L_08005F92;   /* unsigned */
    /* 0x08005F8E: 0f20     movs     r0, #0xf */
    r0 = 0xf;
    /* 0x08005F90: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */

    L_08005F92:
    /* 0x08005F92: 0925     movs     r5, #9 */
    r5 = 9;
    /* 0x08005F94: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08005F96: ed06     lsls     r5, r5, #0x1b */
    r5 = r5 << 0x1b;
    /* 0x08005F98: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x08005F9A: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08005F9C: 01f043fa bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08005FA0: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08005FA2: 81f31088 msr      primask, r1 */
    /* TODO: msr primask, r1 */
    /* 0x08005FA6: 0a46     mov      r2, r1 */
    r2 = r1;
    /* 0x08005FA8: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08005FAA: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08005FAC: 01f03bfa bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08005FB0: 2088     ldrh     r0, [r4] */
    r0 = *(uint16_t *)(r4);
    /* 0x08005FB2: ff21     movs     r1, #0xff */
    r1 = 0xff;
    /* 0x08005FB4: 9131     adds     r1, #0x91 */
    r1 = r1 + 0x91;
    /* 0x08005FB6: 4843     muls     r0, r1, r0 */
    r0 = r1 * r0;
    /* 0x08005FB8: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x08005FBA: 06f03dfd bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08005FBE: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08005FC0: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08005FC2: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08005FC4: 01f02ffa bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08005FC8: 7d20     movs     r0, #0x7d */
    r0 = 0x7d;
    /* 0x08005FCA: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x08005FCC: 06f034fd bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08005FD0: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005FD2: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x08005FD6: 0a49     ldr      r1, [pc, #0x28]  ; =RAM_200001CC */
    r1 = *(uint32_t *)0x08006000;   /* = RAM 0x200001CC */
    /* 0x08005FD8: 1e20     movs     r0, #0x1e */
    r0 = 0x1e;
    /* 0x08005FDA: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08005FDC: 0949     ldr      r1, [pc, #0x24]  ; =RAM_200001BC */
    r1 = *(uint32_t *)0x08006004;   /* = RAM 0x200001BC */
    /* 0x08005FDE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005FE0: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08005FE2: 0949     ldr      r1, [pc, #0x24]  ; =RAM_200001B2 */
    r1 = *(uint32_t *)0x08006008;   /* = RAM 0x200001B2 */
    /* 0x08005FE4: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08005FE6: 0949     ldr      r1, [pc, #0x24]  ; =RAM_20000182 */
    r1 = *(uint32_t *)0x0800600C;   /* = RAM 0x20000182 */
    /* 0x08005FE8: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08005FEA: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */
    /* 0x08005FEC: 0849     ldr      r1, [pc, #0x20]  ; =RAM_200001DB */
    r1 = *(uint32_t *)0x08006010;   /* = RAM 0x200001DB */
    /* 0x08005FEE: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x08005FF0: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08005FF2: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08005FF4: 01f05af9 bl       #0x80072ac */
    FUN_080072AC();   /* BL -> 0x080072AC FUN_080072AC */
    /* 0x08005FF8: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
