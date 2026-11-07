/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B758
 * Name      : FUN_0800B758
 * End       : 0x0800B78C
 * Size      : 52 bytes, 20 instructions decoded in body (functions.json insns=20, blocks=1)
 * Class     : hub
 * Callers   : 8  -> 0x08004880 FUN_08004880, 0x08004894 FUN_08004894, 0x08004AF0 FUN_08004AF0, 0x08004B50 FUN_08004B50, 0x08008A5C FUN_08008A5C, 0x08008A70 FUN_08008A70, 0x08008D00 dispatcher_large, 0x08008FDA FUN_08008FDA
 * Callees   : 4  -> 0x080030F0 FUN_080030F0, 0x0800CAB8 FUN_0800CAB8, 0x0800CD32 FUN_0800CD32, 0x0800CDF4 FUN_0800CDF4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B758(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B758: 0c4a     ldr      r2, [pc, #0x30]  ; =0x0000C368 */
    r2 = *(uint32_t *)0x0800B78C;   /* = 0x0000C368 */
    /* 0x0800B75A: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800B75C: 411c     adds     r1, r0, #1 */
    r1 = r0 + 1;
    /* 0x0800B75E: 1046     mov      r0, r2 */
    r0 = r2;
    /* 0x0800B760: f7f7c6fc bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x0800B764: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x0800B766: 010a     lsrs     r1, r0, #8 */
    r1 = r0 >> 8;
    /* 0x0800B768: 0420     movs     r0, #4 */
    r0 = 4;
    /* 0x0800B76A: 01f0e2fa bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800B76E: e1b2     uxtb     r1, r4 */
    r1 = uxtb(r4);
    /* 0x0800B770: 0520     movs     r0, #5 */
    r0 = 5;
    /* 0x0800B772: 01f0defa bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800B776: 1420     movs     r0, #0x14 */
    r0 = 0x14;
    /* 0x0800B778: 01f09ef9 bl       #0x800cab8 */
    FUN_0800CAB8();   /* BL -> 0x0800CAB8 FUN_0800CAB8 */
    /* 0x0800B77C: 3320     movs     r0, #0x33 */
    r0 = 0x33;
    /* 0x0800B77E: 01f039fb bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */
    /* 0x0800B782: 7d20     movs     r0, #0x7d */
    r0 = 0x7d;
    /* 0x0800B784: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x0800B786: 01f097f9 bl       #0x800cab8 */
    FUN_0800CAB8();   /* BL -> 0x0800CAB8 FUN_0800CAB8 */
    /* 0x0800B78A: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
