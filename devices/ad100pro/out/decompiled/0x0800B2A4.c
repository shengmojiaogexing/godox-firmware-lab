/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B2A4
 * Name      : FUN_0800B2A4
 * End       : 0x0800B2D2
 * Size      : 46 bytes, 19 instructions decoded in body (functions.json insns=4, blocks=3)
 * Class     : nested_block
 * Parent    : 0x0800B0BE FUN_0800B0BE  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x0800311C FUN_0800311C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B2A4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B2A4: 1548     ldr      r0, [pc, #0x54]  ; =RAM_20000185 */
    r0 = *(uint32_t *)0x0800B2FC;   /* = RAM 0x20000185 */
    /* 0x0800B2A6: 12e0     b        #0x800b2ce */
    goto L_0800B2CE;
    /* 0x0800B2A8: 214d     ldr      r5, [pc, #0x84]  ; =RAM_20000169 */
    r5 = *(uint32_t *)0x0800B330;   /* = RAM 0x20000169 */
    /* 0x0800B2AA: 0a2a     cmp      r2, #0xa */
    /* flags = cmp(r2, #0xa) */
    /* 0x0800B2AC: 01d2     bhs      #0x800b2b2 */
    if (r2 >= 0xa) goto L_0800B2B2;   /* unsigned */
    /* 0x0800B2AE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800B2B0: 03e0     b        #0x800b2ba */
    goto L_0800B2BA;

    L_0800B2B2:
    /* 0x0800B2B2: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x0800B2B4: 501e     subs     r0, r2, #1 */
    r0 = r2 - 1;
    /* 0x0800B2B6: f7f731ff bl       #0x800311c */
    FUN_0800311C();   /* BL -> 0x0800311C FUN_0800311C */

    L_0800B2BA:
    /* 0x0800B2BA: 2870     strb     r0, [r5] */
    *(uint8_t *)(r5) = r0;   /* store */
    /* 0x0800B2BC: 0de0     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */
    /* 0x0800B2BE: 1d48     ldr      r0, [pc, #0x74]  ; =RAM_2000016A */
    r0 = *(uint32_t *)0x0800B334;   /* = RAM 0x2000016A */
    /* 0x0800B2C0: 9107     lsls     r1, r2, #0x1e */
    r1 = r2 << 0x1e;
    /* 0x0800B2C2: 00d1     bne      #0x800b2c6 */
    if (r2 != 0xa) goto L_0800B2C6;
    /* 0x0800B2C4: cbe6     b        #0x800b05e */
    goto L_0800B05E;   /* loop */

    L_0800B2C6:
    /* 0x0800B2C6: c8e6     b        #0x800b05a */
    goto L_0800B05A;   /* loop */

    L_0800B2CE:
    /* 0x0800B2CE: 0270     strb     r2, [r0] */
    *(uint8_t *)(r0) = r2;   /* store */
    /* 0x0800B2D0: 03e0     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */
}
