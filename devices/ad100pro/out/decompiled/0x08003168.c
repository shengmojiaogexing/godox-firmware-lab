/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08003168
 * Name      : lookup_or_pad
 * End       : 0x08003168
 * Size      : 0 bytes, 1 instructions decoded in body (functions.json insns=0, blocks=1)
 * Class     : tiny_fragment
 * Callers   : 3  -> 0x080032B0 FUN_080032B0, 0x080032BC FUN_080032BC, 0x0800610C FUN_0800610C
 * Callees   : 2  -> 0x08003444 FUN_08003444, 0x08003464 FUN_08003464
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void lookup_or_pad(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08003168: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
}
