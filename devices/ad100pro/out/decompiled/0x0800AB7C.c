/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800AB7C
 * Name      : FUN_0800AB7C
 * End       : 0x0800ABDC
 * Size      : 96 bytes, 34 instructions decoded in body (functions.json insns=34, blocks=3)
 * Class     : function
 * Callers   : 6  -> 0x08004AF0 FUN_08004AF0, 0x08004B50 FUN_08004B50, 0x080064E8 FUN_080064E8, 0x0800ABE4 FUN_0800ABE4, 0x0800AC78 FUN_0800AC78, 0x0800B394 FUN_0800B394
 * Callees   : 7  -> 0x08006584 FUN_08006584, 0x080065AC FUN_080065AC, 0x0800A6A4 FUN_0800A6A4, 0x0800A6B8 FUN_0800A6B8, 0x0800B718 FUN_0800B718, 0x0800CAB8 FUN_0800CAB8, 0x0800CDF4 FUN_0800CDF4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800AB7C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800AB7C: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800AB7E: 1748     ldr      r0, [pc, #0x5c]  ; =RAM_2000017C */
    r0 = *(uint32_t *)0x0800ABDC;   /* = RAM 0x2000017C */
    /* 0x0800AB80: 7d24     movs     r4, #0x7d */
    r4 = 0x7d;
    /* 0x0800AB82: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800AB84: e400     lsls     r4, r4, #3 */
    r4 = r4 << 3;
    /* 0x0800AB86: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800AB88: 10d0     beq      #0x800abac */
    if (r0 == 1) goto L_0800ABAC;
    /* 0x0800AB8A: fbf70ffd bl       #0x80065ac */
    FUN_080065AC();   /* BL -> 0x080065AC FUN_080065AC */
    /* 0x0800AB8E: 3620     movs     r0, #0x36 */
    r0 = 0x36;
    /* 0x0800AB90: 02f030f9 bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800AB94: 3320     movs     r0, #0x33 */
    r0 = 0x33;
    /* 0x0800AB96: 02f02df9 bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800AB9A: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800AB9C: 01f08cff bl       #0x800cab8 */
    FUN_0800CAB8();   /* BL -> 0x0800CAB8 FUN_0800CAB8 */
    /* 0x0800ABA0: 3920     movs     r0, #0x39 */
    r0 = 0x39;
    /* 0x0800ABA2: 02f027f9 bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800ABA6: fff77dfd bl       #0x800a6a4 */
    FUN_0800A6A4();   /* BL -> 0x0800A6A4 FUN_0800A6A4 */
    /* 0x0800ABAA: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800ABAC:
    /* 0x0800ABAC: 0c48     ldr      r0, [pc, #0x30]  ; =RAM_20000183 */
    r0 = *(uint32_t *)0x0800ABE0;   /* = RAM 0x20000183 */
    /* 0x0800ABAE: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800ABB0: 00f0b2fd bl       #0x800b718 */
    FUN_0800B718();   /* BL -> 0x0800B718 FUN_0800B718 */
    /* 0x0800ABB4: 3620     movs     r0, #0x36 */
    r0 = 0x36;
    /* 0x0800ABB6: 02f01df9 bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800ABBA: 3320     movs     r0, #0x33 */
    r0 = 0x33;
    /* 0x0800ABBC: 02f01af9 bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800ABC0: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800ABC2: 01f079ff bl       #0x800cab8 */
    FUN_0800CAB8();   /* BL -> 0x0800CAB8 FUN_0800CAB8 */
    /* 0x0800ABC6: fff777fd bl       #0x800a6b8 */
    FUN_0800A6B8();   /* BL -> 0x0800A6B8 FUN_0800A6B8 */
    /* 0x0800ABCA: 3a20     movs     r0, #0x3a */
    r0 = 0x3a;
    /* 0x0800ABCC: 02f012f9 bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800ABD0: 3420     movs     r0, #0x34 */
    r0 = 0x34;
    /* 0x0800ABD2: 02f00ff9 bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800ABD6: fbf7d5fc bl       #0x8006584 */
    FUN_08006584();   /* BL -> 0x08006584 FUN_08006584 */
    /* 0x0800ABDA: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
