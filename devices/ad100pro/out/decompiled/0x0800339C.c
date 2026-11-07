/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800339C
 * Name      : FUN_0800339C
 * End       : 0x080033B4
 * Size      : 24 bytes, 11 instructions decoded in body (functions.json insns=11, blocks=1)
 * Class     : hub
 * Callers   : 9  -> 0x080038C0 FUN_080038C0, 0x08003900 FUN_08003900, 0x08003AC8 FUN_08003AC8, 0x08003ADA FUN_08003ADA, 0x0800610C FUN_0800610C, 0x08006274 FUN_08006274, 0x08006282 FUN_08006282, 0x0800A034 FUN_0800A034, ... +1
 * Callees   : 1  -> 0x080034A4 FUN_080034A4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800339C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800339C: 0eb5     push     {r1, r2, r3, lr} */
    push({r1, r2, r3, lr});
    /* 0x0800339E: 0549     ldr      r1, [pc, #0x14]  ; =0x00000433 */
    r1 = *(uint32_t *)0x080033B4;   /* = 0x00000433 */
    /* 0x080033A0: 0291     str      r1, [sp, #8] */
    *(uint32_t *)(sp + 8) = r1;   /* store */
    /* 0x080033A2: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x080033A4: 0091     str      r1, [sp] */
    *(uint32_t *)(sp) = r1;   /* store */
    /* 0x080033A6: 0a46     mov      r2, r1 */
    r2 = r1;
    /* 0x080033A8: 0b46     mov      r3, r1 */
    r3 = r1;
    /* 0x080033AA: 0191     str      r1, [sp, #4] */
    *(uint32_t *)(sp + 4) = r1;   /* store */
    /* 0x080033AC: 00f07af8 bl       #0x80034a4 */
    FUN_080034A4();   /* BL -> 0x080034A4 FUN_080034A4 */
    /* 0x080033B0: 03b0     add      sp, #0xc */
    sp = sp + 0xc;
    /* 0x080033B2: 00bd     pop      {pc} */
    pop({pc});   /* return */
}
