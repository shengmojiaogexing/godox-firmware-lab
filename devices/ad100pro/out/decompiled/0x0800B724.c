/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B724
 * Name      : FUN_0800B724
 * End       : 0x0800B728
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800B718 FUN_0800B718  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x0800CD32 FUN_0800CD32
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B724(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B726: 0b48     ldr      r0, [pc, #0x2c]  ; =flash_0800DAAC */
    r0 = *(uint32_t *)0x0800B754;   /* = FUN_0800DAAC (0x0800DAAC) */   /* function pointer -> FUN_0800DAAC */
}
