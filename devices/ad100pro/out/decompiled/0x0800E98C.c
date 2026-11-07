/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800E98C
 * Name      : FUN_0800E98C
 * End       : 0x0800EA0E
 * Size      : 130 bytes, 53 instructions decoded in body (functions.json insns=53, blocks=2)
 * Class     : nested_block
 * Parent    : 0x0800E800 FUN_0800E800  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800E98C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800E98C: 000d     lsrs     r0, r0, #0x14 */
    r0 = r0 >> 0x14;
    /* 0x0800E98E: 00ffc003 vcgt.u8  q0, q8, q0 */
    /* TODO: vcgt.u8 q0, q8, q0 */
    /* 0x0800E992: fff00fff bl       #0x810e7b4 */
    FUN_0810E7B4();   /* BL -> 0x0810E7B4 FUN_0810E7B4 */
    /* 0x0800E996: fc0f     lsrs     r4, r7, #0x1f */
    r4 = r7 >> 0x1f;
    /* 0x0800E998: fffc1f00 ldc2l    p0, c0, [pc], #0x7c */
    /* TODO: ldc2l p0, c0, [pc], #0x7c */
    /* 0x0800E99C: 3e1c     adds     r6, r7, #0 */
    r6 = r7 + 0;
    /* 0x0800E99E: 000e     lsrs     r0, r0, #0x18 */
    r0 = r0 >> 0x18;
    /* 0x0800E9A0: 1c00     movs     r4, r3 */
    r4 = r3;
    /* 0x0800E9A2: 0e1c     adds     r6, r1, #0 */
    r6 = r1 + 0;
    /* 0x0800E9A4: 000e     lsrs     r0, r0, #0x18 */
    r0 = r0 >> 0x18;
    /* 0x0800E9A6: 1f00     movs     r7, r3 */
    r7 = r3;
    /* 0x0800E9A8: 3e0f     lsrs     r6, r7, #0x1c */
    r6 = r7 >> 0x1c;
    /* 0x0800E9AA: fffc0fff ldc2l    p15, c15, [pc], #0x3c */
    /* TODO: ldc2l p15, c15, [pc], #0x3c */
    /* 0x0800E9AE: fc03     lsls     r4, r7, #0xf */
    r4 = r7 << 0xf;
    /* 0x0800E9B0: fff000ff bl       #0x810e7b4 */
    FUN_0810E7B4();   /* BL -> 0x0810E7B4 FUN_0810E7B4 */
    /* 0x0800E9B4: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x0800E9B6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9B8: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9BA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9BC: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9BE: 0d00     movs     r5, r1 */
    r5 = r1;
    /* 0x0800E9C0: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9C2: 00f00001 and      r1, r0, #0 */
    r1 = r0 & 0;
    /* 0x0800E9C6: e000     lsls     r0, r4, #3 */
    r0 = r4 << 3;
    /* 0x0800E9C8: 01e0     b        #0x800e9ce */
    goto L_0800E9CE;

    L_0800E9CE:
    /* 0x0800E9CE: 0780     strh     r7, [r0] */
    *(uint16_t *)(r0) = r7;   /* store */
    /* 0x0800E9D0: 000f     lsrs     r0, r0, #0x1c */
    r0 = r0 >> 0x1c;
    /* 0x0800E9D2: fffe1fff mrc2     p15, #7, apsr_nzcv, c15, c15, #0 */
    /* TODO: mrc2 p15, #7, apsr_nzcv, c15, c15, #0 */
    /* 0x0800E9D6: fe1f     subs     r6, r7, #7 */
    r6 = r7 - 7;
    /* 0x0800E9D8: fffe1fff mrc2     p15, #7, apsr_nzcv, c15, c15, #0 */
    /* TODO: mrc2 p15, #7, apsr_nzcv, c15, c15, #0 */
    /* 0x0800E9DC: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800E9DE: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9E0: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9E2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9E4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9E6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9E8: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9EA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9EC: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E9EE: 000d     lsrs     r0, r0, #0x14 */
    r0 = r0 >> 0x14;
    /* 0x0800E9F0: 0180     strh     r1, [r0] */
    *(uint16_t *)(r0) = r1;   /* store */
    /* 0x0800E9F2: 0607     lsls     r6, r0, #0x1c */
    r6 = r0 << 0x1c;
    /* 0x0800E9F4: 801e     subs     r0, r0, #2 */
    r0 = r0 - 2;
    /* 0x0800E9F6: 0f80     strh     r7, [r1] */
    *(uint16_t *)(r1) = r7;   /* store */
    /* 0x0800E9F8: 7e0f     lsrs     r6, r7, #0x1d */
    r6 = r7 >> 0x1d;
    /* 0x0800E9FA: 80fe1e01 mcr2     p1, #4, r0, c0, c14, #0 */
    /* TODO: mcr2 p1, #4, r0, c0, c14, #0 */
    /* 0x0800E9FE: fe1c     adds     r6, r7, #3 */
    r6 = r7 + 3;
    /* 0x0800EA00: 03ee1c07 mcr      p7, #0, r0, c3, c12, #0 */
    /* TODO: mcr p7, #0, r0, c3, c12, #0 */
    /* 0x0800EA04: ce1c     adds     r6, r1, #3 */
    r6 = r1 + 3;
    /* 0x0800EA06: 0f8e     ldrh     r7, [r1, #0x30] */
    r7 = *(uint16_t *)(r1 + 48);
    /* 0x0800EA08: 1e1f     subs     r6, r3, #4 */
    r6 = r3 - 4;
    /* 0x0800EA0A: 0e1f     subs     r6, r1, #4 */
    r6 = r1 - 4;
    /* 0x0800EA0C: fe0e     lsrs     r6, r7, #0x1b */
    r6 = r7 >> 0x1b;
}
