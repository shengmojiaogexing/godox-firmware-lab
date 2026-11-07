/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B718
 * Name      : FUN_0800B718
 * End       : 0x0800B754
 * Size      : 60 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=1)
 * Class     : function
 * Callers   : 4  -> 0x0800AB7C FUN_0800AB7C, 0x0800ABA8 FUN_0800ABA8, 0x0800ABE4 FUN_0800ABE4, 0x0800AC36 FUN_0800AC36
 * Callees   : 3  -> 0x0800CAB8 FUN_0800CAB8, 0x0800CD32 FUN_0800CD32, 0x0800CDF4 FUN_0800CDF4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B718(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B718: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800B71A: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x0800B71C: 0d48     ldr      r0, [pc, #0x34]  ; =flash_0800DAAC */
    r0 = *(uint32_t *)0x0800B754;   /* = FUN_0800DAAC (0x0800DAAC) */   /* function pointer -> FUN_0800DAAC */
    /* 0x0800B71E: 015d     ldrb     r1, [r0, r4] */
    r1 = *(uint8_t *)(r0 + r4);
    /* 0x0800B720: 0d20     movs     r0, #0xd */
    r0 = 0xd;
    /* 0x0800B722: 01f006fb bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800B726: 0b48     ldr      r0, [pc, #0x2c]  ; =flash_0800DAAC */
    r0 = *(uint32_t *)0x0800B754;   /* = FUN_0800DAAC (0x0800DAAC) */   /* function pointer -> FUN_0800DAAC */
    /* 0x0800B728: 2030     adds     r0, #0x20 */
    r0 = r0 + 0x20;
    /* 0x0800B72A: 015d     ldrb     r1, [r0, r4] */
    r1 = *(uint8_t *)(r0 + r4);
    /* 0x0800B72C: 0e20     movs     r0, #0xe */
    r0 = 0xe;
    /* 0x0800B72E: 01f000fb bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800B732: 0848     ldr      r0, [pc, #0x20]  ; =flash_0800DAAC */
    r0 = *(uint32_t *)0x0800B754;   /* = FUN_0800DAAC (0x0800DAAC) */   /* function pointer -> FUN_0800DAAC */
    /* 0x0800B734: 4030     adds     r0, #0x40 */
    r0 = r0 + 0x40;
    /* 0x0800B736: 015d     ldrb     r1, [r0, r4] */
    r1 = *(uint8_t *)(r0 + r4);
    /* 0x0800B738: 0f20     movs     r0, #0xf */
    r0 = 0xf;
    /* 0x0800B73A: 01f0fafa bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800B73E: 1420     movs     r0, #0x14 */
    r0 = 0x14;
    /* 0x0800B740: 01f0baf9 bl       #0x800cab8 */
    FUN_0800CAB8();   /* BL -> 0x0800CAB8 FUN_0800CAB8 */
    /* 0x0800B744: 3320     movs     r0, #0x33 */
    r0 = 0x33;
    /* 0x0800B746: 01f055fb bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800B74A: 7d20     movs     r0, #0x7d */
    r0 = 0x7d;
    /* 0x0800B74C: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x0800B74E: 01f0b3f9 bl       #0x800cab8 */
    FUN_0800CAB8();   /* BL -> 0x0800CAB8 FUN_0800CAB8 */
    /* 0x0800B752: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
