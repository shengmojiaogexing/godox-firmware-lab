/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007434
 * Name      : delay_or_timing
 * End       : 0x0800743A
 * Size      : 6 bytes, 3 instructions decoded in body (functions.json insns=3, blocks=1)
 * Class     : hub
 * Callers   : 54  -> 0x080036E0 FUN_080036E0, 0x08003704 FUN_08003704, 0x080037C2 FUN_080037C2, 0x080037FC FUN_080037FC, 0x08003828 FUN_08003828, 0x08003864 FUN_08003864, 0x08003872 FUN_08003872, 0x08005F34 FUN_08005F34, ... +46
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void delay_or_timing(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007434: 0148     ldr      r0, [pc, #4]  ; =RAM_20000104 */
    r0 = *(uint32_t *)0x0800743C;   /* = RAM 0x20000104 */
    /* 0x08007436: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);
    /* 0x08007438: 7047     bx       lr */
    return;
}
