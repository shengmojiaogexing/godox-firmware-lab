/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800BA48
 * Name      : FUN_0800BA48
 * End       : 0x0800BA4C
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800BA44 IRQ19_Handler  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x08004AA8 FUN_08004AA8, 0x080099B0 FUN_080099B0
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800BA48(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800BA4A: 0948     ldr      r0, [pc, #0x24]  ; =RAM_200001D4 */
    r0 = *(uint32_t *)0x0800BA70;   /* = RAM 0x200001D4 */
}
