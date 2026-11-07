/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009CCE
 * Name      : FUN_08009CCE
 * End       : 0x08009CD2
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08009CB4 FUN_08009CB4  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009CCE(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009CD0: 1148     ldr      r0, [pc, #0x44]  ; =RAM_20000165 */
    r0 = *(uint32_t *)0x08009D18;   /* = RAM 0x20000165 */
}
