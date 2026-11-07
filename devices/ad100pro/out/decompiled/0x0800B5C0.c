/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B5C0
 * Name      : FUN_0800B5C0
 * End       : 0x0800B5E6
 * Size      : 38 bytes, 18 instructions decoded in body (functions.json insns=18, blocks=5)
 * Class     : function
 * Callers   : 1  -> 0x0800D088 FUN_0800D088
 * Callees   : 1  -> 0x0800A30C FUN_0800A30C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B5C0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B5C0: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800B5C2: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800B5C4: 084c     ldr      r4, [pc, #0x20]  ; =RAM_200004CC */
    r4 = *(uint32_t *)0x0800B5E8;   /* = RAM 0x200004CC */
    /* 0x0800B5C6: 0a46     mov      r2, r1 */
    r2 = r1;

    L_0800B5C8:
    /* 0x0800B5C8: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800B5CA: cb01     lsls     r3, r1, #7 */
    r3 = r1 << 7;
    /* 0x0800B5CC: 1b19     adds     r3, r3, r4 */
    r3 = r3 + r4;

    L_0800B5CE:
    /* 0x0800B5CE: 1a54     strb     r2, [r3, r0] */
    *(uint8_t *)(r3 + r0) = r2;   /* store */
    /* 0x0800B5D0: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800B5D2: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800B5D4: 8028     cmp      r0, #0x80 */
    /* flags = cmp(r0, #0x80) */
    /* 0x0800B5D6: fad3     blo      #0x800b5ce */
    if (r0 < 0x80) goto L_0800B5CE;   /* unsigned */
    /* 0x0800B5D8: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x0800B5DA: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x0800B5DC: 0829     cmp      r1, #8 */
    /* flags = cmp(r1, #8) */
    /* 0x0800B5DE: f3d3     blo      #0x800b5c8 */
    if (r1 < 8) goto L_0800B5C8;   /* unsigned */
    /* 0x0800B5E0: fef794fe bl       #0x800a30c */
    FUN_0800A30C();   /* BL -> 0x0800A30C FUN_0800A30C */
    /* 0x0800B5E4: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
