/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004B32
 * Name      : FUN_08004B32
 * End       : 0x08004B36
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08004AF0 FUN_08004AF0  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 3  -> 0x0800CAB8 FUN_0800CAB8, 0x0800CD32 FUN_0800CD32, 0x0800CDF4 FUN_0800CDF4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004B32(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004B34: 0c49     ldr      r1, [pc, #0x30]  ; =RAM_20000214 */
    r1 = *(uint32_t *)0x08004B68;   /* = RAM 0x20000214 */
}
