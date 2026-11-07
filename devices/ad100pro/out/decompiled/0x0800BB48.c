/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800BB48
 * Name      : FUN_0800BB48
 * End       : 0x0800BB5E
 * Size      : 22 bytes, 11 instructions decoded in body (functions.json insns=11, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x0800868C FUN_0800868C
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800BB48(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800BB48: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800BB4A: 0123     movs     r3, #1 */
    r3 = 1;
    /* 0x0800BB4C: 046a     ldr      r4, [r0, #0x20] */
    r4 = *(uint32_t *)(r0 + 32);
    /* 0x0800BB4E: 8b40     lsls     r3, r1 */
    r3 = r3 << r1;
    /* 0x0800BB50: 9c43     bics     r4, r3 */
    r4 = r4 & ~r3;
    /* 0x0800BB52: 0462     str      r4, [r0, #0x20] */
    *(uint32_t *)(r0 + 32) = r4;   /* store */
    /* 0x0800BB54: 036a     ldr      r3, [r0, #0x20] */
    r3 = *(uint32_t *)(r0 + 32);
    /* 0x0800BB56: 8a40     lsls     r2, r1 */
    r2 = r2 << r1;
    /* 0x0800BB58: 1343     orrs     r3, r2 */
    r3 = r3 | r2;
    /* 0x0800BB5A: 0362     str      r3, [r0, #0x20] */
    *(uint32_t *)(r0 + 32) = r3;   /* store */
    /* 0x0800BB5C: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
