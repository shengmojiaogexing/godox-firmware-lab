/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C204
 * Name      : FUN_0800C204
 * End       : 0x0800C22C
 * Size      : 40 bytes, 14 instructions decoded in body (functions.json insns=14, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x0800AA54 FUN_0800AA54
 * Callees   : 5  -> 0x0800A6A4 FUN_0800A6A4, 0x0800A6CC FUN_0800A6CC, 0x0800CAB8 FUN_0800CAB8, 0x0800CD24 FUN_0800CD24, 0x0800CDF4 FUN_0800CDF4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C204(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C204: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800C206: fef761fa bl       #0x800a6cc */
    FUN_0800A6CC();   /* BL -> 0x0800A6CC FUN_0800A6CC */
    /* 0x0800C20A: 0421     movs     r1, #4 */
    r1 = 4;
    /* 0x0800C20C: 0748     ldr      r0, [pc, #0x1c]  ; =RAM_20000214 */
    r0 = *(uint32_t *)0x0800C22C;   /* = RAM 0x20000214 */
    /* 0x0800C20E: 00f089fd bl       #0x800cd24 */
    FUN_0800CD24();   /* BL -> 0x0800CD24 FUN_0800CD24 */
    /* 0x0800C212: 3520     movs     r0, #0x35 */
    r0 = 0x35;
    /* 0x0800C214: 00f0eefd bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800C218: 4b20     movs     r0, #0x4b */
    r0 = 0x4b;
    /* 0x0800C21A: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x0800C21C: 00f04cfc bl       #0x800cab8 */
    FUN_0800CAB8();   /* BL -> 0x0800CAB8 FUN_0800CAB8 */
    /* 0x0800C220: 3b20     movs     r0, #0x3b */
    r0 = 0x3b;
    /* 0x0800C222: 00f0e7fd bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800C226: fef73dfa bl       #0x800a6a4 */
    FUN_0800A6A4();   /* BL -> 0x0800A6A4 FUN_0800A6A4 */
    /* 0x0800C22A: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
