/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800D062
 * Name      : FUN_0800D062
 * End       : 0x0800D088
 * Size      : 38 bytes, 18 instructions decoded in body (functions.json insns=18, blocks=1)
 * Class     : nested_block
 * Parent    : 0x0800CF74 main  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800D062(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800D064: d4e7     b        #0x800d010 */
    goto L_0800D010;   /* loop */
    /* 0x0800D066: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800D068: 0030     adds     r0, #0 */
    r0 = r0 + 0;
    /* 0x0800D06A: 0008     lsrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x0800D06C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800D06E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800D070: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800D072: 0140     ands     r1, r0 */
    r1 = r1 & r0;
    /* 0x0800D074: 0010     asrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x0800D076: 0240     ands     r2, r0 */
    r2 = r2 & r0;
    /* 0x0800D078: 7402     lsls     r4, r6, #9 */
    r4 = r6 << 9;
    /* 0x0800D07A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800D07C: 8403     lsls     r4, r0, #0xe */
    r4 = r0 << 0xe;
    /* 0x0800D07E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800D080: 1b02     lsls     r3, r3, #8 */
    r3 = r3 << 8;
    /* 0x0800D082: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800D084: e101     lsls     r1, r4, #7 */
    r1 = r4 << 7;
    /* 0x0800D086: 0020     movs     r0, #0 */
    r0 = 0;
}
