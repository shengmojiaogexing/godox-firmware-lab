/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C3D4
 * Name      : FUN_0800C3D4
 * End       : 0x0800C3D8
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800C2B8 FUN_0800C2B8  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x080053BC FUN_080053BC, 0x08005400 FUN_08005400
 * MMIO      : TIM1.CR1
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C3D4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C3D6: 6878     ldrb     r0, [r5, #1] */
    r0 = *(uint8_t *)(r5 + 1);
}
