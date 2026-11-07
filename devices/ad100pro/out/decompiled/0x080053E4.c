/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080053E4
 * Name      : FUN_080053E4
 * End       : 0x080053FC
 * Size      : 24 bytes, 10 instructions decoded in body (functions.json insns=10, blocks=3)
 * Class     : function
 * Callers   : 2  -> 0x0800C2B8 FUN_0800C2B8, 0x0800C33E FUN_0800C33E
 * Callees   : 2  -> 0x08004C6C FUN_08004C6C, 0x080051C8 FUN_080051C8
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080053E4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080053E4: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x080053E6: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x080053E8: 07d1     bne      #0x80053fa */
    if (r0 != 1) goto L_080053FA;
    /* 0x080053EA: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x080053EC: 0846     mov      r0, r1 */
    r0 = r1;
    /* 0x080053EE: fff7ebfe bl       #0x80051c8 */
    FUN_080051C8();   /* BL -> 0x080051C8 FUN_080051C8 */
    /* 0x080053F2: 0248     ldr      r0, [pc, #8]  ; =RAM_20000183 */
    r0 = *(uint32_t *)0x080053FC;   /* = RAM 0x20000183 */
    /* 0x080053F4: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x080053F6: fff739fc bl       #0x8004c6c */
    FUN_08004C6C();   /* BL -> 0x08004C6C FUN_08004C6C */

    L_080053FA:
    /* 0x080053FA: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
