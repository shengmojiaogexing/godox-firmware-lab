/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080078EC
 * Name      : FUN_080078EC
 * End       : 0x080078F2
 * Size      : 6 bytes, 3 instructions decoded in body (functions.json insns=3, blocks=1)
 * Class     : function
 * Callers   : 2  -> 0x080075EC FUN_080075EC, 0x0800B8F0 FUN_0800B8F0
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080078EC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080078EC: 0148     ldr      r0, [pc, #4]  ; =RAM_20000100 */
    r0 = *(uint32_t *)0x080078F4;   /* = RAM 0x20000100 */
    /* 0x080078EE: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);
    /* 0x080078F0: 7047     bx       lr */
    return;
}
