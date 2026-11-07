/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B24A
 * Name      : FUN_0800B24A
 * End       : 0x0800B266
 * Size      : 28 bytes, 12 instructions decoded in body (functions.json insns=15, blocks=4)
 * Class     : nested_block
 * Parent    : 0x0800B0BE FUN_0800B0BE  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x0800C6F4 FUN_0800C6F4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B24A(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B24A: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800B24C: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800B250: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800B252: 1843     orrs     r0, r3 */
    r0 = r0 | r3;
    /* 0x0800B254: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800B256: a807     lsls     r0, r5, #0x1e */
    r0 = r5 << 0x1e;
    /* 0x0800B258: 02d1     bne      #0x800b260 */
    if (/* bne */) goto L_0800B260;
    /* 0x0800B25A: 3049     ldr      r1, [pc, #0xc0]  ; =RAM_200001C4 */
    r1 = *(uint32_t *)0x0800B31C;   /* = RAM 0x200001C4 */
    /* 0x0800B25C: 6420     movs     r0, #0x64 */
    r0 = 0x64;
    /* 0x0800B25E: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */

    L_0800B260:
    /* 0x0800B260: 01f048fa bl       #0x800c6f4 */
    FUN_0800C6F4();   /* BL -> 0x0800C6F4 FUN_0800C6F4 */
    /* 0x0800B264: ece7     b        #0x800b240 */
    goto L_0800B240;   /* loop */
}
