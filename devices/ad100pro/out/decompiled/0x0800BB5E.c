/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800BB5E
 * Name      : FUN_0800BB5E
 * End       : 0x0800BB74
 * Size      : 22 bytes, 11 instructions decoded in body (functions.json insns=11, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x080082F8 FUN_080082F8
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800BB5E(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800BB5E: 30b5     push     {r4, r5, lr} */
    push({r4, r5, lr});
    /* 0x0800BB60: 8468     ldr      r4, [r0, #8] */
    r4 = *(uint32_t *)(r0 + 8);
    /* 0x0800BB62: ff25     movs     r5, #0xff */
    r5 = 0xff;
    /* 0x0800BB64: 2d02     lsls     r5, r5, #8 */
    r5 = r5 << 8;
    /* 0x0800BB66: 1b02     lsls     r3, r3, #8 */
    r3 = r3 << 8;
    /* 0x0800BB68: 1343     orrs     r3, r2 */
    r3 = r3 | r2;
    /* 0x0800BB6A: ac43     bics     r4, r5 */
    r4 = r4 & ~r5;
    /* 0x0800BB6C: 0b43     orrs     r3, r1 */
    r3 = r3 | r1;
    /* 0x0800BB6E: 2343     orrs     r3, r4 */
    r3 = r3 | r4;
    /* 0x0800BB70: 8360     str      r3, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r3;   /* store */
    /* 0x0800BB72: 30bd     pop      {r4, r5, pc} */
    pop({r4, r5, pc});   /* return */
}
