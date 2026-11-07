/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B9C0
 * Name      : FUN_0800B9C0
 * End       : 0x0800BA20
 * Size      : 96 bytes, 38 instructions decoded in body (functions.json insns=38, blocks=1)
 * Class     : hub
 * Callers   : 2  -> 0x0800A934 FUN_0800A934, 0x0800A9E2 FUN_0800A9E2
 * Callees   : 9  -> 0x08003AC8 FUN_08003AC8, 0x08004AF0 FUN_08004AF0, 0x08007426 FUN_08007426, 0x080082B4 FUN_080082B4, 0x08008A2C FUN_08008A2C, 0x0800ABE4 FUN_0800ABE4, 0x0800C080 FUN_0800C080, 0x0800CA98 FUN_0800CA98, ... +1
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B9C0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B9C0: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800B9C2: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x0800B9C4: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800B9C6: 0221     movs     r1, #2 */
    r1 = 2;
    /* 0x0800B9C8: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x0800B9CA: fbf72cfd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800B9CE: 6420     movs     r0, #0x64 */
    r0 = 0x64;
    /* 0x0800B9D0: 01f062f8 bl       #0x800ca98 */
    FUN_0800CA98();   /* BL -> 0x0800CA98 FUN_0800CA98 */
    /* 0x0800B9D4: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800B9D6: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x0800B9D8: 1148     ldr      r0, [pc, #0x44]  ; =GPIOF.MODER */
    r0 = *(uint32_t *)0x0800BA20;   /* = MMIO GPIOF+0x00 */
    /* 0x0800B9DA: fbf724fd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800B9DE: f8f773f8 bl       #0x8003ac8 */
    FUN_08003AC8();   /* BL -> 0x08003AC8 FUN_08003AC8 */
    /* 0x0800B9E2: 1048     ldr      r0, [pc, #0x40]  ; =RAM_20000180 */
    r0 = *(uint32_t *)0x0800BA24;   /* = RAM 0x20000180 */
    /* 0x0800B9E4: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x0800B9E6: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */
    /* 0x0800B9E8: 0f48     ldr      r0, [pc, #0x3c]  ; =RAM_20000194 */
    r0 = *(uint32_t *)0x0800BA28;   /* = RAM 0x20000194 */
    /* 0x0800B9EA: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x0800B9EC: 0580     strh     r5, [r0] */
    *(uint16_t *)(r0) = r5;   /* store */
    /* 0x0800B9EE: 0f48     ldr      r0, [pc, #0x3c]  ; =RAM_20000181 */
    r0 = *(uint32_t *)0x0800BA2C;   /* = RAM 0x20000181 */
    /* 0x0800B9F0: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */
    /* 0x0800B9F2: 0f48     ldr      r0, [pc, #0x3c]  ; =RAM_20000184 */
    r0 = *(uint32_t *)0x0800BA30;   /* = RAM 0x20000184 */
    /* 0x0800B9F4: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */
    /* 0x0800B9F6: fdf719f8 bl       #0x8008a2c */
    FUN_08008A2C();   /* BL -> 0x08008A2C FUN_08008A2C */
    /* 0x0800B9FA: 0e49     ldr      r1, [pc, #0x38]  ; =RAM_200001DA */
    r1 = *(uint32_t *)0x0800BA34;   /* = RAM 0x200001DA */
    /* 0x0800B9FC: fa20     movs     r0, #0xfa */
    r0 = 0xfa;
    /* 0x0800B9FE: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x0800BA00: f9f776f8 bl       #0x8004af0 */
    FUN_08004AF0();   /* BL -> 0x08004AF0 FUN_08004AF0 */
    /* 0x0800BA04: 0c48     ldr      r0, [pc, #0x30]  ; =RAM_20000185 */
    r0 = *(uint32_t *)0x0800BA38;   /* = RAM 0x20000185 */
    /* 0x0800BA06: 0570     strb     r5, [r0] */
    *(uint8_t *)(r0) = r5;   /* store */
    /* 0x0800BA08: 0c48     ldr      r0, [pc, #0x30]  ; =RAM_20000233 */
    r0 = *(uint32_t *)0x0800BA3C;   /* = RAM 0x20000233 */
    /* 0x0800BA0A: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */
    /* 0x0800BA0C: 01f03cfb bl       #0x800d088 */
    FUN_0800D088();   /* BL -> 0x0800D088 FUN_0800D088 */
    /* 0x0800BA10: fff7e8f8 bl       #0x800abe4 */
    FUN_0800ABE4();   /* BL -> 0x0800ABE4 FUN_0800ABE4 */
    /* 0x0800BA14: 00f034fb bl       #0x800c080 */
    FUN_0800C080();   /* BL -> 0x0800C080 FUN_0800C080 */
    /* 0x0800BA18: 0948     ldr      r0, [pc, #0x24]  ; =RAM_20000348 */
    r0 = *(uint32_t *)0x0800BA40;   /* = RAM 0x20000348 */
    /* 0x0800BA1A: fcf74bfc bl       #0x80082b4 */
    FUN_080082B4();   /* BL -> 0x080082B4 FUN_080082B4 */
    /* 0x0800BA1E: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
