/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004EE4
 * Name      : FUN_08004EE4
 * End       : 0x08004F28
 * Size      : 68 bytes, 34 instructions decoded in body (functions.json insns=34, blocks=4)
 * Class     : hub
 * Callers   : 21  -> 0x080059CC FUN_080059CC, 0x0800BDC4 FUN_0800BDC4, 0x0800BDD8 FUN_0800BDD8, 0x0800BE2A FUN_0800BE2A, 0x0800BE84 FUN_0800BE84, 0x0800BEA4 FUN_0800BEA4, 0x0800BEE8 FUN_0800BEE8, 0x0800BEF2 FUN_0800BEF2, ... +13
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004EE4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004EE4: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x08004EE6: 8446     mov      ip, r0 */
    ip = r0;
    /* 0x08004EE8: 1001     lsls     r0, r2, #4 */
    r0 = r2 << 4;
    /* 0x08004EEA: 1718     adds     r7, r2, r0 */
    r7 = r2 + r0;
    /* 0x08004EEC: 0e48     ldr      r0, [pc, #0x38]  ; =flash_0800DEC6 */
    r0 = *(uint32_t *)0x08004F28;   /* = flash data/const 0x0800DEC6 */
    /* 0x08004EEE: 8e46     mov      lr, r1 */
    lr = r1;
    /* 0x08004EF0: c55d     ldrb     r5, [r0, r7] */
    r5 = *(uint8_t *)(r0 + r7);
    /* 0x08004EF2: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08004EF4: 15e0     b        #0x8004f22 */
    goto L_08004F22;

    L_08004EF6:
    /* 0x08004EF6: 6046     mov      r0, ip */
    r0 = ip;
    /* 0x08004EF8: c101     lsls     r1, r0, #7 */
    r1 = r0 << 7;
    /* 0x08004EFA: 0c48     ldr      r0, [pc, #0x30]  ; =RAM_200004CC */
    r0 = *(uint32_t *)0x08004F2C;   /* = RAM 0x200004CC */
    /* 0x08004EFC: 0a4e     ldr      r6, [pc, #0x28]  ; =flash_0800DEC6 */
    r6 = *(uint32_t *)0x08004F28;   /* = flash data/const 0x0800DEC6 */
    /* 0x08004EFE: 0b18     adds     r3, r1, r0 */
    r3 = r1 + r0;
    /* 0x08004F00: 7046     mov      r0, lr */
    r0 = lr;
    /* 0x08004F02: be19     adds     r6, r7, r6 */
    r6 = r7 + r6;
    /* 0x08004F04: 5100     lsls     r1, r2, #1 */
    r1 = r2 << 1;
    /* 0x08004F06: 8418     adds     r4, r0, r2 */
    r4 = r0 + r2;
    /* 0x08004F08: 7618     adds     r6, r6, r1 */
    r6 = r6 + r1;
    /* 0x08004F0A: 185d     ldrb     r0, [r3, r4] */
    r0 = *(uint8_t *)(r3 + r4);
    /* 0x08004F0C: 7178     ldrb     r1, [r6, #1] */
    r1 = *(uint8_t *)(r6 + 1);
    /* 0x08004F0E: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08004F10: 1855     strb     r0, [r3, r4] */
    *(uint8_t *)(r3 + r4) = r0;   /* store */
    /* 0x08004F12: 1819     adds     r0, r3, r4 */
    r0 = r3 + r4;
    /* 0x08004F14: 8030     adds     r0, #0x80 */
    r0 = r0 + 0x80;
    /* 0x08004F16: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x08004F18: b378     ldrb     r3, [r6, #2] */
    r3 = *(uint8_t *)(r6 + 2);
    /* 0x08004F1A: 1943     orrs     r1, r3 */
    r1 = r1 | r3;
    /* 0x08004F1C: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x08004F1E: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x08004F20: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);

    L_08004F22:
    /* 0x08004F22: aa42     cmp      r2, r5 */
    /* flags = cmp(r2, r5) */
    /* 0x08004F24: e7d3     blo      #0x8004ef6 */
    if (r2 < r5) goto L_08004EF6;   /* unsigned */
    /* 0x08004F26: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
