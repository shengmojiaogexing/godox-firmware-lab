/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800786C
 * Name      : FUN_0800786C
 * End       : 0x08007870
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08007798 FUN_08007798  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x080075EC FUN_080075EC, 0x080078F8 FUN_080078F8
 * MMIO      : FLASH_IF.ACR, RCC.APB2RSTR, RCC.CFGR, RCC.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800786C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800786E: c11b     subs     r1, r0, r7 */
    r1 = r0 - r7;
}
