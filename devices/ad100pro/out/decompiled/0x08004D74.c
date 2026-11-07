/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004D74
 * Name      : FUN_08004D74
 * End       : 0x08004DB8
 * Size      : 68 bytes, 34 instructions decoded in body (functions.json insns=34, blocks=4)
 * Class     : hub
 * Callers   : 9  -> 0x0800557C FUN_0800557C, 0x080055C6 FUN_080055C6, 0x080055FE FUN_080055FE, 0x08005614 FUN_08005614, 0x080057E4 FUN_080057E4, 0x08005806 FUN_08005806, 0x0800585A FUN_0800585A, 0x080058E0 FUN_080058E0, ... +1
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004D74(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004D74: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x08004D76: 8446     mov      ip, r0 */
    ip = r0;
    /* 0x08004D78: 5001     lsls     r0, r2, #5 */
    r0 = r2 << 5;
    /* 0x08004D7A: 1718     adds     r7, r2, r0 */
    r7 = r2 + r0;
    /* 0x08004D7C: 0e48     ldr      r0, [pc, #0x38]  ; =func_0800E800 */
    r0 = *(uint32_t *)0x08004DB8;   /* = flash data/const 0x0800E801 */
    /* 0x08004D7E: 8e46     mov      lr, r1 */
    lr = r1;
    /* 0x08004D80: c55d     ldrb     r5, [r0, r7] */
    r5 = *(uint8_t *)(r0 + r7);
    /* 0x08004D82: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08004D84: 15e0     b        #0x8004db2 */
    goto L_08004DB2;

    L_08004D86:
    /* 0x08004D86: 6046     mov      r0, ip */
    r0 = ip;
    /* 0x08004D88: c101     lsls     r1, r0, #7 */
    r1 = r0 << 7;
    /* 0x08004D8A: 0c48     ldr      r0, [pc, #0x30]  ; =RAM_200004CC */
    r0 = *(uint32_t *)0x08004DBC;   /* = RAM 0x200004CC */
    /* 0x08004D8C: 0a4e     ldr      r6, [pc, #0x28]  ; =func_0800E800 */
    r6 = *(uint32_t *)0x08004DB8;   /* = flash data/const 0x0800E801 */
    /* 0x08004D8E: 0b18     adds     r3, r1, r0 */
    r3 = r1 + r0;
    /* 0x08004D90: 7046     mov      r0, lr */
    r0 = lr;
    /* 0x08004D92: be19     adds     r6, r7, r6 */
    r6 = r7 + r6;
    /* 0x08004D94: 5100     lsls     r1, r2, #1 */
    r1 = r2 << 1;
    /* 0x08004D96: 8418     adds     r4, r0, r2 */
    r4 = r0 + r2;
    /* 0x08004D98: 7618     adds     r6, r6, r1 */
    r6 = r6 + r1;
    /* 0x08004D9A: 185d     ldrb     r0, [r3, r4] */
    r0 = *(uint8_t *)(r3 + r4);
    /* 0x08004D9C: 7178     ldrb     r1, [r6, #1] */
    r1 = *(uint8_t *)(r6 + 1);
    /* 0x08004D9E: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08004DA0: 1855     strb     r0, [r3, r4] */
    *(uint8_t *)(r3 + r4) = r0;   /* store */
    /* 0x08004DA2: 1819     adds     r0, r3, r4 */
    r0 = r3 + r4;
    /* 0x08004DA4: 8030     adds     r0, #0x80 */
    r0 = r0 + 0x80;
    /* 0x08004DA6: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x08004DA8: b378     ldrb     r3, [r6, #2] */
    r3 = *(uint8_t *)(r6 + 2);
    /* 0x08004DAA: 1943     orrs     r1, r3 */
    r1 = r1 | r3;
    /* 0x08004DAC: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x08004DAE: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x08004DB0: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);

    L_08004DB2:
    /* 0x08004DB2: aa42     cmp      r2, r5 */
    /* flags = cmp(r2, r5) */
    /* 0x08004DB4: e7d3     blo      #0x8004d86 */
    if (r2 < r5) goto L_08004D86;   /* unsigned */
    /* 0x08004DB6: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
