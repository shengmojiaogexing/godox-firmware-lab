/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800BA1C
 * Name      : FUN_0800BA1C
 * End       : 0x0800BA44
 * Size      : 40 bytes, 19 instructions decoded in body (functions.json insns=19, blocks=1)
 * Class     : nested_block
 * Parent    : 0x0800B9C0 FUN_0800B9C0  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800BA1C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800BA1E: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
    /* 0x0800BA20: 0014     asrs     r0, r0, #0x10 */
    r0 = r0 >> 0x10;
    /* 0x0800BA22: 0048     ldr      r0, [pc, #0]  ; =RAM_20000180 */
    r0 = *(uint32_t *)0x0800BA24;   /* = RAM 0x20000180 */
    /* 0x0800BA24: 8001     lsls     r0, r0, #6 */
    r0 = r0 << 6;
    /* 0x0800BA26: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800BA28: 9401     lsls     r4, r2, #6 */
    r4 = r2 << 6;
    /* 0x0800BA2A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800BA2C: 8101     lsls     r1, r0, #6 */
    r1 = r0 << 6;
    /* 0x0800BA2E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800BA30: 8401     lsls     r4, r0, #6 */
    r4 = r0 << 6;
    /* 0x0800BA32: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800BA34: da01     lsls     r2, r3, #7 */
    r2 = r3 << 7;
    /* 0x0800BA36: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800BA38: 8501     lsls     r5, r0, #6 */
    r5 = r0 << 6;
    /* 0x0800BA3A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800BA3C: 3302     lsls     r3, r6, #8 */
    r3 = r6 << 8;
    /* 0x0800BA3E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800BA40: 4803     lsls     r0, r1, #0xd */
    r0 = r1 << 0xd;
    /* 0x0800BA42: 0020     movs     r0, #0 */
    r0 = 0;
}
