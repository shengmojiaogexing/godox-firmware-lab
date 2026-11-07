/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004D28
 * Name      : FUN_08004D28
 * End       : 0x08004D6C
 * Size      : 68 bytes, 34 instructions decoded in body (functions.json insns=34, blocks=4)
 * Class     : function
 * Callers   : 5  -> 0x080055FE FUN_080055FE, 0x08005614 FUN_08005614, 0x08005666 FUN_08005666, 0x080057E4 FUN_080057E4, 0x0800585A FUN_0800585A
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004D28(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004D28: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x08004D2A: 8446     mov      ip, r0 */
    ip = r0;
    /* 0x08004D2C: 5001     lsls     r0, r2, #5 */
    r0 = r2 << 5;
    /* 0x08004D2E: 1718     adds     r7, r2, r0 */
    r7 = r2 + r0;
    /* 0x08004D30: 0e48     ldr      r0, [pc, #0x38]  ; =func_0800E39E */
    r0 = *(uint32_t *)0x08004D6C;   /* = flash data/const 0x0800E39F */
    /* 0x08004D32: 8e46     mov      lr, r1 */
    lr = r1;
    /* 0x08004D34: c55d     ldrb     r5, [r0, r7] */
    r5 = *(uint8_t *)(r0 + r7);
    /* 0x08004D36: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08004D38: 15e0     b        #0x8004d66 */
    goto L_08004D66;

    L_08004D3A:
    /* 0x08004D3A: 6046     mov      r0, ip */
    r0 = ip;
    /* 0x08004D3C: c101     lsls     r1, r0, #7 */
    r1 = r0 << 7;
    /* 0x08004D3E: 0c48     ldr      r0, [pc, #0x30]  ; =RAM_200004CC */
    r0 = *(uint32_t *)0x08004D70;   /* = RAM 0x200004CC */
    /* 0x08004D40: 0a4e     ldr      r6, [pc, #0x28]  ; =func_0800E39E */
    r6 = *(uint32_t *)0x08004D6C;   /* = flash data/const 0x0800E39F */
    /* 0x08004D42: 0b18     adds     r3, r1, r0 */
    r3 = r1 + r0;
    /* 0x08004D44: 7046     mov      r0, lr */
    r0 = lr;
    /* 0x08004D46: be19     adds     r6, r7, r6 */
    r6 = r7 + r6;
    /* 0x08004D48: 5100     lsls     r1, r2, #1 */
    r1 = r2 << 1;
    /* 0x08004D4A: 8418     adds     r4, r0, r2 */
    r4 = r0 + r2;
    /* 0x08004D4C: 7618     adds     r6, r6, r1 */
    r6 = r6 + r1;
    /* 0x08004D4E: 185d     ldrb     r0, [r3, r4] */
    r0 = *(uint8_t *)(r3 + r4);
    /* 0x08004D50: 7178     ldrb     r1, [r6, #1] */
    r1 = *(uint8_t *)(r6 + 1);
    /* 0x08004D52: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08004D54: 1855     strb     r0, [r3, r4] */
    *(uint8_t *)(r3 + r4) = r0;   /* store */
    /* 0x08004D56: 1819     adds     r0, r3, r4 */
    r0 = r3 + r4;
    /* 0x08004D58: 8030     adds     r0, #0x80 */
    r0 = r0 + 0x80;
    /* 0x08004D5A: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x08004D5C: b378     ldrb     r3, [r6, #2] */
    r3 = *(uint8_t *)(r6 + 2);
    /* 0x08004D5E: 1943     orrs     r1, r3 */
    r1 = r1 | r3;
    /* 0x08004D60: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x08004D62: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x08004D64: d2b2     uxtb     r2, r2 */
    r2 = uxtb(r2);

    L_08004D66:
    /* 0x08004D66: aa42     cmp      r2, r5 */
    /* flags = cmp(r2, r5) */
    /* 0x08004D68: e7d3     blo      #0x8004d3a */
    if (r2 < r5) goto L_08004D3A;   /* unsigned */
    /* 0x08004D6A: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
