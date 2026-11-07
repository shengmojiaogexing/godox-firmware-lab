/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080030C0
 * Name      : __start
 * End       : 0x080030C2
 * Size      : 2 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void __start(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080030C0: 0348     ldr      r0, [pc, #0xc]  ; =RAM_20000CD0 */
    r0 = *(uint32_t *)0x080030D0;   /* = initial_SP (0x20000CD0) */
}
