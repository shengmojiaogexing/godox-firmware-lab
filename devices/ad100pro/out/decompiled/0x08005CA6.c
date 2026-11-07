/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005CA6
 * Name      : FUN_08005CA6
 * End       : 0x08005CCC
 * Size      : 38 bytes, 18 instructions decoded in body (functions.json insns=18, blocks=3)
 * Class     : nested_block
 * Parent    : 0x08005C44 FUN_08005C44  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005CA6(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005CA8: d7e7     b        #0x8005c5a */
    goto L_08005C5A;   /* loop */
    /* 0x08005CAA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x08005CAC: 7501     lsls     r5, r6, #5 */
    r5 = r6 << 5;
    /* 0x08005CAE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005CB0: b601     lsls     r6, r6, #6 */
    r6 = r6 << 6;
    /* 0x08005CB2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005CB4: c401     lsls     r4, r0, #7 */
    r4 = r0 << 7;
    /* 0x08005CB6: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005CB8: bd01     lsls     r5, r7, #6 */
    r5 = r7 << 6;
    /* 0x08005CBA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005CBC: 7d01     lsls     r5, r7, #5 */
    r5 = r7 << 5;
    /* 0x08005CBE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005CC0: b101     lsls     r1, r6, #6 */
    r1 = r6 << 6;
    /* 0x08005CC2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005CC4: 3401     lsls     r4, r6, #4 */
    r4 = r6 << 4;
    /* 0x08005CC6: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005CC8: 5ad3     blo      #0x8005d80 */
    if (/* blo */) goto L_08005D80;   /* unsigned */
    /* 0x08005CCA: 0008     lsrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
}
