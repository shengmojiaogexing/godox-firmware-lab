/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CFFC
 * Name      : FUN_0800CFFC
 * End       : 0x0800D000
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800CF74 main  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * MMIO      : RCC.CSR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CFFC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CFFE: 606a     ldr      r0, [r4, #0x24] */
    r0 = *(uint32_t *)(r4 + 36);  /* MMIO: RCC.CSR(r) */
}
