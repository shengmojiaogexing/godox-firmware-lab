/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B790
 * Name      : FUN_0800B790
 * End       : 0x0800B7AC
 * Size      : 28 bytes, 14 instructions decoded in body (functions.json insns=14, blocks=4)
 * Class     : function
 * Callers   : 1  -> 0x08008D00 dispatcher_large
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B790(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B790: 0648     ldr      r0, [pc, #0x18]  ; =RAM_20000164 */
    r0 = *(uint32_t *)0x0800B7AC;   /* = RAM 0x20000164 */
    /* 0x0800B792: 418e     ldrh     r1, [r0, #0x32] */
    r1 = *(uint16_t *)(r0 + 50);
    /* 0x0800B794: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x0800B796: 08d0     beq      #0x800b7aa */
    if (r1 == 0) goto L_0800B7AA;
    /* 0x0800B798: 491e     subs     r1, r1, #1 */
    r1 = r1 - 1;
    /* 0x0800B79A: 0904     lsls     r1, r1, #0x10 */
    r1 = r1 << 0x10;
    /* 0x0800B79C: 090c     lsrs     r1, r1, #0x10 */
    r1 = r1 >> 0x10;
    /* 0x0800B79E: 4186     strh     r1, [r0, #0x32] */
    *(uint16_t *)(r0 + 50) = r1;   /* store */
    /* 0x0800B7A0: 03d1     bne      #0x800b7aa */
    if (r1 != 0) goto L_0800B7AA;
    /* 0x0800B7A2: c174     strb     r1, [r0, #0x13] */
    *(uint8_t *)(r0 + 19) = r1;   /* store */
    /* 0x0800B7A4: 4175     strb     r1, [r0, #0x15] */
    *(uint8_t *)(r0 + 21) = r1;   /* store */
    /* 0x0800B7A6: 2030     adds     r0, #0x20 */
    r0 = r0 + 0x20;
    /* 0x0800B7A8: 8170     strb     r1, [r0, #2] */
    *(uint8_t *)(r0 + 2) = r1;   /* store */

    L_0800B7AA:
    /* 0x0800B7AA: 7047     bx       lr */
    return;
}
