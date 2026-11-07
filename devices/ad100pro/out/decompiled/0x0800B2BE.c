/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B2BE
 * Name      : FUN_0800B2BE
 * End       : 0x0800B2C8
 * Size      : 10 bytes, 5 instructions decoded in body (functions.json insns=5, blocks=5)
 * Class     : nested_block
 * Parent    : 0x0800B2A4 FUN_0800B2A4  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B2BE(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B2BE: 1d48     ldr      r0, [pc, #0x74]  ; =RAM_2000016A */
    r0 = *(uint32_t *)0x0800B334;   /* = RAM 0x2000016A */
    /* 0x0800B2C0: 9107     lsls     r1, r2, #0x1e */
    r1 = r2 << 0x1e;
    /* 0x0800B2C2: 00d1     bne      #0x800b2c6 */
    if (/* bne */) goto L_0800B2C6;
    /* 0x0800B2C4: cbe6     b        #0x800b05e */
    goto L_0800B05E;   /* loop */

    L_0800B2C6:
    /* 0x0800B2C6: c8e6     b        #0x800b05a */
    goto L_0800B05A;   /* loop */
}
