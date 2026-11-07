/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004DC0
 * Name      : FUN_08004DC0
 * End       : 0x08004E12
 * Size      : 82 bytes, 41 instructions decoded in body (functions.json insns=41, blocks=4)
 * Class     : function
 * Callers   : 3  -> 0x08005474 FUN_08005474, 0x08005498 FUN_08005498, 0x080054C0 FUN_080054C0
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004DC0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004DC0: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x08004DC2: 8446     mov      ip, r0 */
    ip = r0;
    /* 0x08004DC4: 3120     movs     r0, #0x31 */
    r0 = 0x31;
    /* 0x08004DC6: 4243     muls     r2, r0, r2 */
    r2 = r0 * r2;
    /* 0x08004DC8: 1248     ldr      r0, [pc, #0x48]  ; =func_0800E98C */
    r0 = *(uint32_t *)0x08004E14;   /* = flash data/const 0x0800E98D */
    /* 0x08004DCA: 8e46     mov      lr, r1 */
    lr = r1;
    /* 0x08004DCC: 875c     ldrb     r7, [r0, r2] */
    r7 = *(uint8_t *)(r0 + r2);
    /* 0x08004DCE: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x08004DD0: 1ce0     b        #0x8004e0c */
    goto L_08004E0C;

    L_08004DD2:
    /* 0x08004DD2: 6046     mov      r0, ip */
    r0 = ip;
    /* 0x08004DD4: c101     lsls     r1, r0, #7 */
    r1 = r0 << 7;
    /* 0x08004DD6: 1048     ldr      r0, [pc, #0x40]  ; =RAM_200004CC */
    r0 = *(uint32_t *)0x08004E18;   /* = RAM 0x200004CC */
    /* 0x08004DD8: 0e4e     ldr      r6, [pc, #0x38]  ; =func_0800E98C */
    r6 = *(uint32_t *)0x08004E14;   /* = flash data/const 0x0800E98D */
    /* 0x08004DDA: 0c18     adds     r4, r1, r0 */
    r4 = r1 + r0;
    /* 0x08004DDC: 5900     lsls     r1, r3, #1 */
    r1 = r3 << 1;
    /* 0x08004DDE: 7046     mov      r0, lr */
    r0 = lr;
    /* 0x08004DE0: 9619     adds     r6, r2, r6 */
    r6 = r2 + r6;
    /* 0x08004DE2: 5918     adds     r1, r3, r1 */
    r1 = r3 + r1;
    /* 0x08004DE4: c518     adds     r5, r0, r3 */
    r5 = r0 + r3;
    /* 0x08004DE6: 7618     adds     r6, r6, r1 */
    r6 = r6 + r1;
    /* 0x08004DE8: 605d     ldrb     r0, [r4, r5] */
    r0 = *(uint8_t *)(r4 + r5);
    /* 0x08004DEA: 7178     ldrb     r1, [r6, #1] */
    r1 = *(uint8_t *)(r6 + 1);
    /* 0x08004DEC: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08004DEE: 6055     strb     r0, [r4, r5] */
    *(uint8_t *)(r4 + r5) = r0;   /* store */
    /* 0x08004DF0: 6519     adds     r5, r4, r5 */
    r5 = r4 + r5;
    /* 0x08004DF2: 2c46     mov      r4, r5 */
    r4 = r5;
    /* 0x08004DF4: 8034     adds     r4, #0x80 */
    r4 = r4 + 0x80;
    /* 0x08004DF6: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x08004DF8: b178     ldrb     r1, [r6, #2] */
    r1 = *(uint8_t *)(r6 + 2);
    /* 0x08004DFA: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08004DFC: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x08004DFE: 8034     adds     r4, #0x80 */
    r4 = r4 + 0x80;
    /* 0x08004E00: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x08004E02: f178     ldrb     r1, [r6, #3] */
    r1 = *(uint8_t *)(r6 + 3);
    /* 0x08004E04: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08004E06: 5b1c     adds     r3, r3, #1 */
    r3 = r3 + 1;
    /* 0x08004E08: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x08004E0A: dbb2     uxtb     r3, r3 */
    r3 = uxtb(r3);

    L_08004E0C:
    /* 0x08004E0C: bb42     cmp      r3, r7 */
    /* flags = cmp(r3, r7) */
    /* 0x08004E0E: e0d3     blo      #0x8004dd2 */
    if (r3 < r7) goto L_08004DD2;   /* unsigned */
    /* 0x08004E10: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
