/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B55C
 * Name      : FUN_0800B55C
 * End       : 0x0800B574
 * Size      : 24 bytes, 12 instructions decoded in body (functions.json insns=12, blocks=4)
 * Class     : function
 * Callers   : 1  -> 0x080097F8 FUN_080097F8
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B55C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B55C: 0548     ldr      r0, [pc, #0x14]  ; =RAM_20000120 */
    r0 = *(uint32_t *)0x0800B574;   /* = RAM 0x20000120 */
    /* 0x0800B55E: 8188     ldrh     r1, [r0, #4] */
    r1 = *(uint16_t *)(r0 + 4);
    /* 0x0800B560: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x0800B562: 06d0     beq      #0x800b572 */
    if (r1 == 0) goto L_0800B572;
    /* 0x0800B564: 491e     subs     r1, r1, #1 */
    r1 = r1 - 1;
    /* 0x0800B566: 0904     lsls     r1, r1, #0x10 */
    r1 = r1 << 0x10;
    /* 0x0800B568: 090c     lsrs     r1, r1, #0x10 */
    r1 = r1 >> 0x10;
    /* 0x0800B56A: 8180     strh     r1, [r0, #4] */
    *(uint16_t *)(r0 + 4) = r1;   /* store */
    /* 0x0800B56C: 01d1     bne      #0x800b572 */
    if (r1 != 0) goto L_0800B572;
    /* 0x0800B56E: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800B570: 8170     strb     r1, [r0, #2] */
    *(uint8_t *)(r0 + 2) = r1;   /* store */

    L_0800B572:
    /* 0x0800B572: 7047     bx       lr */
    return;
}
