/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080059CC
 * Name      : FUN_080059CC
 * End       : 0x08005A14
 * Size      : 72 bytes, 30 instructions decoded in body (functions.json insns=30, blocks=1)
 * Class     : function
 * Callers   : 2  -> 0x0800C2B8 FUN_0800C2B8, 0x0800C2EA FUN_0800C2EA
 * Callees   : 1  -> 0x08004EE4 FUN_08004EE4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080059CC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080059CC: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x080059CE: 0324     movs     r4, #3 */
    r4 = 3;
    /* 0x080059D0: 2721     movs     r1, #0x27 */
    r1 = 0x27;
    /* 0x080059D2: 0e22     movs     r2, #0xe */
    r2 = 0xe;
    /* 0x080059D4: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080059D6: fff785fa bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x080059DA: 2e21     movs     r1, #0x2e */
    r1 = 0x2e;
    /* 0x080059DC: 3522     movs     r2, #0x35 */
    r2 = 0x35;
    /* 0x080059DE: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080059E0: fff780fa bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x080059E4: 3521     movs     r1, #0x35 */
    r1 = 0x35;
    /* 0x080059E6: 0a46     mov      r2, r1 */
    r2 = r1;
    /* 0x080059E8: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080059EA: fff77bfa bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x080059EE: 3c21     movs     r1, #0x3c */
    r1 = 0x3c;
    /* 0x080059F0: 3222     movs     r2, #0x32 */
    r2 = 0x32;
    /* 0x080059F2: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080059F4: fff776fa bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x080059F8: 4321     movs     r1, #0x43 */
    r1 = 0x43;
    /* 0x080059FA: 3522     movs     r2, #0x35 */
    r2 = 0x35;
    /* 0x080059FC: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080059FE: fff771fa bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x08005A02: 0448     ldr      r0, [pc, #0x10]  ; =RAM_20000228 */
    r0 = *(uint32_t *)0x08005A14;   /* = RAM 0x20000228 */
    /* 0x08005A04: 4f21     movs     r1, #0x4f */
    r1 = 0x4f;
    /* 0x08005A06: 4079     ldrb     r0, [r0, #5] */
    r0 = *(uint8_t *)(r0 + 5);
    /* 0x08005A08: 0207     lsls     r2, r0, #0x1c */
    r2 = r0 << 0x1c;
    /* 0x08005A0A: 120f     lsrs     r2, r2, #0x1c */
    r2 = r2 >> 0x1c;
    /* 0x08005A0C: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08005A0E: fff769fa bl       #0x8004ee4 */
    FUN_08004EE4();   /* BL -> 0x08004EE4 FUN_08004EE4 */
    /* 0x08005A12: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
