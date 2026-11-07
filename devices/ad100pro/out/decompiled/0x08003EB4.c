/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08003EB4
 * Name      : FUN_08003EB4
 * End       : 0x08003F04
 * Size      : 80 bytes, 33 instructions decoded in body (functions.json insns=33, blocks=4)
 * Class     : function
 * Callers   : 4  -> 0x0800AFC4 FUN_0800AFC4, 0x0800B03C FUN_0800B03C, 0x0800B0BE FUN_0800B0BE, 0x0800B232 FUN_0800B232
 * Callees   : 2  -> 0x08007426 FUN_08007426, 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08003EB4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08003EB4: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08003EB6: 134c     ldr      r4, [pc, #0x4c]  ; =RAM_200001B2 */
    r4 = *(uint32_t *)0x08003F04;   /* = RAM 0x200001B2 */
    /* 0x08003EB8: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x08003EBA: 0528     cmp      r0, #5 */
    /* flags = cmp(r0, #5) */
    /* 0x08003EBC: 21d1     bne      #0x8003f02 */
    if (r0 != 5) goto L_08003F02;
    /* 0x08003EBE: 1248     ldr      r0, [pc, #0x48]  ; =RAM_200001B8 */
    r0 = *(uint32_t *)0x08003F08;   /* = RAM 0x200001B8 */
    /* 0x08003EC0: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08003EC2: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08003EC4: 1dd1     bne      #0x8003f02 */
    if (r0 != 1) goto L_08003F02;
    /* 0x08003EC6: 0925     movs     r5, #9 */
    r5 = 9;
    /* 0x08003EC8: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08003ECA: ed06     lsls     r5, r5, #0x1b */
    r5 = r5 << 0x1b;
    /* 0x08003ECC: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x08003ECE: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08003ED0: 03f0a9fa bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08003ED4: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08003ED6: 81f31088 msr      primask, r1 */
    /* TODO: msr primask, r1 */
    /* 0x08003EDA: 0a46     mov      r2, r1 */
    r2 = r1;
    /* 0x08003EDC: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08003EDE: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08003EE0: 03f0a1fa bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08003EE4: 5a20     movs     r0, #0x5a */
    r0 = 0x5a;
    /* 0x08003EE6: 08f0a7fd bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08003EEA: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08003EEC: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08003EEE: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08003EF0: 03f099fa bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08003EF4: 6420     movs     r0, #0x64 */
    r0 = 0x64;
    /* 0x08003EF6: 08f09ffd bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08003EFA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003EFC: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x08003F00: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */

    L_08003F02:
    /* 0x08003F02: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
