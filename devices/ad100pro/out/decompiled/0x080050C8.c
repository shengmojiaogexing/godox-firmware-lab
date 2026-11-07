/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080050C8
 * Name      : FUN_080050C8
 * End       : 0x08005118
 * Size      : 80 bytes, 40 instructions decoded in body (functions.json insns=40, blocks=3)
 * Class     : function
 * Callers   : 1  -> 0x08004D18 FUN_08004D18
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080050C8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080050C8: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x080050CA: 8c46     mov      ip, r1 */
    ip = r1;
    /* 0x080050CC: 134c     ldr      r4, [pc, #0x4c]  ; =flash_0800EDC2 */
    r4 = *(uint32_t *)0x0800511C;   /* = flash data/const 0x0800EDC2 */
    /* 0x080050CE: 1249     ldr      r1, [pc, #0x48]  ; =RAM_200004CC */
    r1 = *(uint32_t *)0x08005118;   /* = RAM 0x200004CC */
    /* 0x080050D0: d201     lsls     r2, r2, #7 */
    r2 = r2 << 7;
    /* 0x080050D2: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x080050D4: 1519     adds     r5, r2, r4 */
    r5 = r2 + r4;
    /* 0x080050D6: c001     lsls     r0, r0, #7 */
    r0 = r0 << 7;
    /* 0x080050D8: 4418     adds     r4, r0, r1 */
    r4 = r0 + r1;

    L_080050DA:
    /* 0x080050DA: 6046     mov      r0, ip */
    r0 = ip;
    /* 0x080050DC: 9e00     lsls     r6, r3, #2 */
    r6 = r3 << 2;
    /* 0x080050DE: c018     adds     r0, r0, r3 */
    r0 = r0 + r3;
    /* 0x080050E0: 215c     ldrb     r1, [r4, r0] */
    r1 = *(uint8_t *)(r4 + r0);
    /* 0x080050E2: aa5d     ldrb     r2, [r5, r6] */
    r2 = *(uint8_t *)(r5 + r6);
    /* 0x080050E4: ae19     adds     r6, r5, r6 */
    r6 = r5 + r6;
    /* 0x080050E6: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x080050E8: 2154     strb     r1, [r4, r0] */
    *(uint8_t *)(r4 + r0) = r1;   /* store */
    /* 0x080050EA: 2018     adds     r0, r4, r0 */
    r0 = r4 + r0;
    /* 0x080050EC: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x080050EE: 8032     adds     r2, #0x80 */
    r2 = r2 + 0x80;
    /* 0x080050F0: 1778     ldrb     r7, [r2] */
    r7 = *(uint8_t *)(r2);
    /* 0x080050F2: 7178     ldrb     r1, [r6, #1] */
    r1 = *(uint8_t *)(r6 + 1);
    /* 0x080050F4: ff30     adds     r0, #0xff */
    r0 = r0 + 0xff;
    /* 0x080050F6: 0f43     orrs     r7, r1 */
    r7 = r7 | r1;
    /* 0x080050F8: 1770     strb     r7, [r2] */
    *(uint8_t *)(r2) = r7;   /* store */
    /* 0x080050FA: 8032     adds     r2, #0x80 */
    r2 = r2 + 0x80;
    /* 0x080050FC: 1778     ldrb     r7, [r2] */
    r7 = *(uint8_t *)(r2);
    /* 0x080050FE: b178     ldrb     r1, [r6, #2] */
    r1 = *(uint8_t *)(r6 + 2);
    /* 0x08005100: 8130     adds     r0, #0x81 */
    r0 = r0 + 0x81;
    /* 0x08005102: 0f43     orrs     r7, r1 */
    r7 = r7 | r1;
    /* 0x08005104: 1770     strb     r7, [r2] */
    *(uint8_t *)(r2) = r7;   /* store */
    /* 0x08005106: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x08005108: f278     ldrb     r2, [r6, #3] */
    r2 = *(uint8_t *)(r6 + 3);
    /* 0x0800510A: 5b1c     adds     r3, r3, #1 */
    r3 = r3 + 1;
    /* 0x0800510C: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x0800510E: dbb2     uxtb     r3, r3 */
    r3 = uxtb(r3);
    /* 0x08005110: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x08005112: 202b     cmp      r3, #0x20 */
    /* flags = cmp(r3, #0x20) */
    /* 0x08005114: e1d3     blo      #0x80050da */
    if (r3 < 0x20) goto L_080050DA;   /* unsigned */
    /* 0x08005116: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
