/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CCBC
 * Name      : FUN_0800CCBC
 * End       : 0x0800CD1A
 * Size      : 94 bytes, 38 instructions decoded in body (functions.json insns=38, blocks=5)
 * Class     : function
 * Callers   : 2  -> 0x08004AF0 FUN_08004AF0, 0x08004AF6 FUN_08004AF6
 * Callees   : 3  -> 0x08007426 FUN_08007426, 0x0800CAB8 FUN_0800CAB8, 0x0800CDF4 FUN_0800CDF4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CCBC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CCBC: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800CCBE: 174d     ldr      r5, [pc, #0x5c]  ; =GPIOC.MODER */
    r5 = *(uint32_t *)0x0800CD1C;   /* = MMIO GPIOC+0x00 */
    /* 0x0800CCC0: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800CCC2: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x0800CCC4: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800CCC6: faf7aefb bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800CCCA: 1e20     movs     r0, #0x1e */
    r0 = 0x1e;
    /* 0x0800CCCC: fff7f4fe bl       #0x800cab8 */
    FUN_0800CAB8();   /* BL -> 0x0800CAB8 FUN_0800CAB8 */
    /* 0x0800CCD0: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800CCD2: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x0800CCD4: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800CCD6: faf7a6fb bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800CCDA: 1e20     movs     r0, #0x1e */
    r0 = 0x1e;
    /* 0x0800CCDC: fff7ecfe bl       #0x800cab8 */
    FUN_0800CAB8();   /* BL -> 0x0800CAB8 FUN_0800CAB8 */
    /* 0x0800CCE0: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800CCE2: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x0800CCE4: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800CCE6: faf79efb bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800CCEA: 2d20     movs     r0, #0x2d */
    r0 = 0x2d;
    /* 0x0800CCEC: fff7e4fe bl       #0x800cab8 */
    FUN_0800CAB8();   /* BL -> 0x0800CAB8 FUN_0800CAB8 */
    /* 0x0800CCF0: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800CCF2: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x0800CCF4: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800CCF6: faf796fb bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800CCFA: 094c     ldr      r4, [pc, #0x24]  ; =GPIOB.MODER */
    r4 = *(uint32_t *)0x0800CD20;   /* = MMIO GPIOB+0x00 */

    L_0800CCFC:
    /* 0x0800CCFC: 2069     ldr      r0, [r4, #0x10] */
    r0 = *(uint32_t *)(r4 + 16);  /* MMIO: GPIOB.IDR(r) */
    /* 0x0800CCFE: 4004     lsls     r0, r0, #0x11 */
    r0 = r0 << 0x11;
    /* 0x0800CD00: fcd4     bmi      #0x800ccfc */
    if (/* bmi */) goto L_0800CCFC;   /* result negative */
    /* 0x0800CD02: 3020     movs     r0, #0x30 */
    r0 = 0x30;
    /* 0x0800CD04: 00f076f8 bl       #0x800cdf4 */
    FUN_0800CDF4();   /* BL -> 0x0800CDF4 FUN_0800CDF4 */

    L_0800CD08:
    /* 0x0800CD08: 2069     ldr      r0, [r4, #0x10] */
    r0 = *(uint32_t *)(r4 + 16);  /* MMIO: GPIOB.IDR(r) */
    /* 0x0800CD0A: 4004     lsls     r0, r0, #0x11 */
    r0 = r0 << 0x11;
    /* 0x0800CD0C: fcd4     bmi      #0x800cd08 */
    if (/* bmi */) goto L_0800CD08;   /* result negative */
    /* 0x0800CD0E: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800CD10: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x0800CD12: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800CD14: faf787fb bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800CD18: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
