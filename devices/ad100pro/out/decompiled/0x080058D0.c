/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080058D0
 * Name      : FUN_080058D0
 * End       : 0x080058E0
 * Size      : 16 bytes, 7 instructions decoded in body (functions.json insns=7, blocks=3)
 * Class     : function
 * Callers   : 2  -> 0x08005464 FUN_08005464, 0x080054E4 FUN_080054E4
 * Callees   : 1  -> 0x08005158 FUN_08005158
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080058D0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080058D0: 00b5     push     {lr} */
    push({lr});
    /* 0x080058D2: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x080058D4: 03d5     bpl      #0x80058de */
    if (/* bpl */) goto L_080058DE;   /* result non-negative */
    /* 0x080058D6: 6421     movs     r1, #0x64 */
    r1 = 0x64;
    /* 0x080058D8: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080058DA: fff73dfc bl       #0x8005158 */
    FUN_08005158();   /* BL -> 0x08005158 FUN_08005158 */

    L_080058DE:
    /* 0x080058DE: 00bd     pop      {pc} */
    pop({pc});   /* return */
}
