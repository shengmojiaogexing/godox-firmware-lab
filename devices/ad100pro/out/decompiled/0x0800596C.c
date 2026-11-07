/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800596C
 * Name      : FUN_0800596C
 * End       : 0x080059CA
 * Size      : 94 bytes, 40 instructions decoded in body (functions.json insns=40, blocks=1)
 * Class     : function
 * Callers   : 2  -> 0x080054E4 FUN_080054E4, 0x080054FC FUN_080054FC
 * Callees   : 2  -> 0x080030F0 FUN_080030F0, 0x08004E98 FUN_08004E98
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800596C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800596C: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x0800596E: 0c46     mov      r4, r1 */
    r4 = r1;
    /* 0x08005970: 0646     mov      r6, r0 */
    r6 = r0;
    /* 0x08005972: 6421     movs     r1, #0x64 */
    r1 = 0x64;
    /* 0x08005974: 1046     mov      r0, r2 */
    r0 = r2;
    /* 0x08005976: fdf7bbfb bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x0800597A: 0f46     mov      r7, r1 */
    r7 = r1;
    /* 0x0800597C: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800597E: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x08005980: fdf7b6fb bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x08005984: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x08005986: 0d46     mov      r5, r1 */
    r5 = r1;
    /* 0x08005988: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x0800598A: 2146     mov      r1, r4 */
    r1 = r4;
    /* 0x0800598C: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x0800598E: fff783fa bl       #0x8004e98 */
    FUN_08004E98();   /* BL -> 0x08004E98 FUN_08004E98 */
    /* 0x08005992: e01d     adds     r0, r4, #7 */
    r0 = r4 + 7;
    /* 0x08005994: c1b2     uxtb     r1, r0 */
    r1 = uxtb(r0);
    /* 0x08005996: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x08005998: 2a46     mov      r2, r5 */
    r2 = r5;
    /* 0x0800599A: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x0800599C: fff77cfa bl       #0x8004e98 */
    FUN_08004E98();   /* BL -> 0x08004E98 FUN_08004E98 */
    /* 0x080059A0: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x080059A2: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x080059A4: fdf7a4fb bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x080059A8: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x080059AA: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080059AC: 0d46     mov      r5, r1 */
    r5 = r1;
    /* 0x080059AE: 0e30     adds     r0, #0xe */
    r0 = r0 + 0xe;
    /* 0x080059B0: c1b2     uxtb     r1, r0 */
    r1 = uxtb(r0);
    /* 0x080059B2: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x080059B4: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x080059B6: fff76ffa bl       #0x8004e98 */
    FUN_08004E98();   /* BL -> 0x08004E98 FUN_08004E98 */
    /* 0x080059BA: 1534     adds     r4, #0x15 */
    r4 = r4 + 0x15;
    /* 0x080059BC: e1b2     uxtb     r1, r4 */
    r1 = uxtb(r4);
    /* 0x080059BE: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x080059C0: 2a46     mov      r2, r5 */
    r2 = r5;
    /* 0x080059C2: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x080059C4: fff768fa bl       #0x8004e98 */
    FUN_08004E98();   /* BL -> 0x08004E98 FUN_08004E98 */
    /* 0x080059C8: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
