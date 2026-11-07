/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08003E2C
 * Name      : FUN_08003E2C
 * End       : 0x08003EAC
 * Size      : 128 bytes, 53 instructions decoded in body (functions.json insns=53, blocks=6)
 * Class     : function
 * Callers   : 4  -> 0x0800AFC4 FUN_0800AFC4, 0x0800B03C FUN_0800B03C, 0x0800B0BE FUN_0800B0BE, 0x0800B210 FUN_0800B210
 * Callees   : 2  -> 0x08007426 FUN_08007426, 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08003E2C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08003E2C: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x08003E2E: 1f4e     ldr      r6, [pc, #0x7c]  ; =RAM_200001B2 */
    r6 = *(uint32_t *)0x08003EAC;   /* = RAM 0x200001B2 */
    /* 0x08003E30: 3078     ldrb     r0, [r6] */
    r0 = *(uint8_t *)(r6);
    /* 0x08003E32: 0528     cmp      r0, #5 */
    /* flags = cmp(r0, #5) */
    /* 0x08003E34: 39d1     bne      #0x8003eaa */
    if (r0 != 5) goto L_08003EAA;
    /* 0x08003E36: 1e48     ldr      r0, [pc, #0x78]  ; =RAM_200001B8 */
    r0 = *(uint32_t *)0x08003EB0;   /* = RAM 0x200001B8 */
    /* 0x08003E38: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08003E3A: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08003E3C: 35d1     bne      #0x8003eaa */
    if (r0 != 1) goto L_08003EAA;
    /* 0x08003E3E: 0620     movs     r0, #6 */
    r0 = 6;
    /* 0x08003E40: 0925     movs     r5, #9 */
    r5 = 9;
    /* 0x08003E42: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08003E44: ed06     lsls     r5, r5, #0x1b */
    r5 = r5 << 0x1b;
    /* 0x08003E46: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08003E48: 1127     movs     r7, #0x11 */
    r7 = 0x11;
    /* 0x08003E4A: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x08003E4C: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08003E4E: 03f0eafa bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08003E52: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08003E54: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x08003E58: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x08003E5A: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08003E5C: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08003E5E: 03f0e2fa bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08003E62: 3220     movs     r0, #0x32 */
    r0 = 0x32;
    /* 0x08003E64: 08f0e8fd bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08003E68: 0024     movs     r4, #0 */
    r4 = 0;

    L_08003E6A:
    /* 0x08003E6A: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08003E6C: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08003E6E: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08003E70: 03f0d9fa bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08003E74: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x08003E76: 08f0dffd bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08003E7A: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08003E7C: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08003E7E: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08003E80: 03f0d1fa bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08003E84: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x08003E86: 08f0d7fd bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08003E8A: 641c     adds     r4, r4, #1 */
    r4 = r4 + 1;
    /* 0x08003E8C: e4b2     uxtb     r4, r4 */
    r4 = uxtb(r4);
    /* 0x08003E8E: 142c     cmp      r4, #0x14 */
    /* flags = cmp(r4, #0x14) */
    /* 0x08003E90: ebd3     blo      #0x8003e6a */
    if (r4 < 0x14) goto L_08003E6A;   /* unsigned */
    /* 0x08003E92: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08003E94: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08003E96: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08003E98: 03f0c5fa bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08003E9C: 6420     movs     r0, #0x64 */
    r0 = 0x64;
    /* 0x08003E9E: 08f0cbfd bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08003EA2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003EA4: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x08003EA8: 3070     strb     r0, [r6] */
    *(uint8_t *)(r6) = r0;   /* store */

    L_08003EAA:
    /* 0x08003EAA: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
