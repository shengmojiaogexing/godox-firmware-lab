/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008F3A
 * Name      : FUN_08008F3A
 * End       : 0x08008F3E
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08008D00 dispatcher_large  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x080067EC FUN_080067EC, 0x08009198 FUN_08009198
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08008F3A(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08008F3C: 687b     ldrb     r0, [r5, #0xd] */
    r0 = *(uint8_t *)(r5 + 13);
}
