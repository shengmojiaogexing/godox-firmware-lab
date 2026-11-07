/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004B50
 * Name      : FUN_08004B50
 * End       : 0x08004B54
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08004AF0 FUN_08004AF0  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x0800AB7C FUN_0800AB7C, 0x0800B758 FUN_0800B758
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004B50(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004B52: 0748     ldr      r0, [pc, #0x1c]  ; =RAM_20000480 */
    r0 = *(uint32_t *)0x08004B70;   /* = RAM 0x20000480 */
}
