/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007256
 * Name      : FUN_08007256
 * End       : 0x0800725A
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08007208 FUN_08007208  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * MMIO      : FLASH_IF.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007256(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007258: 0949     ldr      r1, [pc, #0x24]  ; =FLASH_IF.ACR */
    r1 = *(uint32_t *)0x08007280;   /* = MMIO FLASH+0x00 */
}
