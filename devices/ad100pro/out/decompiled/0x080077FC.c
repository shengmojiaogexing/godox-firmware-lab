/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080077FC
 * Name      : FUN_080077FC
 * End       : 0x08007800
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08007798 FUN_08007798  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x08007434 delay_or_timing
 * MMIO      : RCC.CFGR, RCC.CR, RCC.CR2
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080077FC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080077FE: 6968     ldr      r1, [r5, #4] */
    r1 = *(uint32_t *)(r5 + 4);  /* MMIO: RCC.CFGR(r) */
}
