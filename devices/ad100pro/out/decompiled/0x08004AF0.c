/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004AF0
 * Name      : FUN_08004AF0
 * End       : 0x08004B60
 * Size      : 112 bytes, 43 instructions decoded in body (functions.json insns=43, blocks=4)
 * Class     : hub
 * Callers   : 2  -> 0x0800B9C0 FUN_0800B9C0, 0x0800B9F8 FUN_0800B9F8
 * Callees   : 10  -> 0x08008C78 GPIO_init, 0x0800AB7C FUN_0800AB7C, 0x0800B758 FUN_0800B758, 0x0800CA38 FUN_0800CA38, 0x0800CAB8 FUN_0800CAB8, 0x0800CAE8 FUN_0800CAE8, 0x0800CCA6 FUN_0800CCA6, 0x0800CCBC FUN_0800CCBC, ... +2
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004AF0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004AF0: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08004AF2: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x08004AF4: 04f0c0f8 bl       #0x8008c78 */
    GPIO_init();   /* BL -> 0x08008C78 GPIO_init */
    /* 0x08004AF8: 1948     ldr      r0, [pc, #0x64]  ; =0x00004E20 */
    r0 = *(uint32_t *)0x08004B60;   /* = 0x00004E20 */
    /* 0x08004AFA: 07f09dff bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08004AFE: 194e     ldr      r6, [pc, #0x64]  ; =0x00002710 */
    r6 = *(uint32_t *)0x08004B64;   /* = 0x00002710 */
    /* 0x08004B00: 194d     ldr      r5, [pc, #0x64]  ; =RAM_20000214 */
    r5 = *(uint32_t *)0x08004B68;   /* = RAM 0x20000214 */

    L_08004B02:
    /* 0x08004B02: 08f0dbf8 bl       #0x800ccbc */
    FUN_0800CCBC();   /* BL -> 0x0800CCBC FUN_0800CCBC */
    /* 0x08004B06: 07f0efff bl       #0x800cae8 */
    FUN_0800CAE8();   /* BL -> 0x0800CAE8 FUN_0800CAE8 */
    /* 0x08004B0A: 0d20     movs     r0, #0xd */
    r0 = 0xd;
    /* 0x08004B0C: 08f0cbf8 bl       #0x800cca6 */
    FUN_0800CCA6();   /* BL -> 0x0800CCA6 FUN_0800CCA6 */
    /* 0x08004B10: 2870     strb     r0, [r5] */
    *(uint8_t *)(r5) = r0;   /* store */
    /* 0x08004B12: 0e20     movs     r0, #0xe */
    r0 = 0xe;
    /* 0x08004B14: 08f0c7f8 bl       #0x800cca6 */
    FUN_0800CCA6();   /* BL -> 0x0800CCA6 FUN_0800CCA6 */
    /* 0x08004B18: 6870     strb     r0, [r5, #1] */
    *(uint8_t *)(r5 + 1) = r0;   /* store */
    /* 0x08004B1A: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x08004B1C: 07f0ccff bl       #0x800cab8 */
    FUN_0800CAB8();   /* BL -> 0x0800CAB8 FUN_0800CAB8 */
    /* 0x08004B20: 641c     adds     r4, r4, #1 */
    r4 = r4 + 1;
    /* 0x08004B22: e4b2     uxtb     r4, r4 */
    r4 = uxtb(r4);
    /* 0x08004B24: 0a2c     cmp      r4, #0xa */
    /* flags = cmp(r4, #0xa) */
    /* 0x08004B26: 02d8     bhi      #0x8004b2e */
    if (r4 > 0xa) goto L_08004B2E;   /* unsigned */
    /* 0x08004B28: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);
    /* 0x08004B2A: 5c28     cmp      r0, #0x5c */
    /* flags = cmp(r0, #0x5c) */
    /* 0x08004B2C: e9d1     bne      #0x8004b02 */
    if (r0 != 0x5c) goto L_08004B02;

    L_08004B2E:
    /* 0x08004B2E: 3620     movs     r0, #0x36 */
    r0 = 0x36;
    /* 0x08004B30: 08f060f9 bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x08004B34: 0c49     ldr      r1, [pc, #0x30]  ; =RAM_20000214 */
    r1 = *(uint32_t *)0x08004B68;   /* = RAM 0x20000214 */
    /* 0x08004B36: 0d48     ldr      r0, [pc, #0x34]  ; =flash_0800DA9A */
    r0 = *(uint32_t *)0x08004B6C;   /* = flash data/const 0x0800DA9A */
    /* 0x08004B38: 891f     subs     r1, r1, #6 */
    r1 = r1 - 6;
    /* 0x08004B3A: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08004B3C: 415c     ldrb     r1, [r0, r1] */
    r1 = *(uint8_t *)(r0 + r1);
    /* 0x08004B3E: 3e20     movs     r0, #0x3e */
    r0 = 0x3e;
    /* 0x08004B40: 08f0f7f8 bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x08004B44: 3320     movs     r0, #0x33 */
    r0 = 0x33;
    /* 0x08004B46: 08f055f9 bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x08004B4A: 7d20     movs     r0, #0x7d */
    r0 = 0x7d;
    /* 0x08004B4C: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x08004B4E: 07f0b3ff bl       #0x800cab8 */
    FUN_0800CAB8();   /* BL -> 0x0800CAB8 FUN_0800CAB8 */
    /* 0x08004B52: 0748     ldr      r0, [pc, #0x1c]  ; =RAM_20000480 */
    r0 = *(uint32_t *)0x08004B70;   /* = RAM 0x20000480 */
    /* 0x08004B54: 4079     ldrb     r0, [r0, #5] */
    r0 = *(uint8_t *)(r0 + 5);
    /* 0x08004B56: 06f0fffd bl       #0x800b758 */
    FUN_0800B758();   /* BL -> 0x0800B758 FUN_0800B758 */
    /* 0x08004B5A: 06f00ff8 bl       #0x800ab7c */
    FUN_0800AB7C();   /* BL -> 0x0800AB7C FUN_0800AB7C */
    /* 0x08004B5E: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
