/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800821C
 * Name      : FUN_0800821C
 * End       : 0x080082A4
 * Size      : 136 bytes, 64 instructions decoded in body (functions.json insns=64, blocks=10)
 * Class     : function
 * Callers   : 1  -> 0x080081E8 FUN_080081E8
 * Callees   : 2  -> 0x0800765C FUN_0800765C, 0x08007670 FUN_08007670
 * MMIO      : RCC.APB1ENR, RCC.APB2ENR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800821C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800821C: 08b5     push     {r3, lr} */
    push({r3, lr});
    /* 0x0800821E: 214a     ldr      r2, [pc, #0x84]  ; =TIM1.CR1 */
    r2 = *(uint32_t *)0x080082A4;   /* = MMIO TIM1+0x00 */
    /* 0x08008220: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);
    /* 0x08008222: 2148     ldr      r0, [pc, #0x84]  ; =RCC.CR */
    r0 = *(uint32_t *)0x080082A8;   /* = MMIO RCC+0x00 */
    /* 0x08008224: 9142     cmp      r1, r2 */
    /* flags = cmp(r1, r2) */
    /* 0x08008226: 0dd1     bne      #0x8008244 */
    if (r1 != r2) goto L_08008244;
    /* 0x08008228: 8269     ldr      r2, [r0, #0x18] */
    r2 = *(uint32_t *)(r0 + 24);  /* MMIO: RCC.APB2ENR(r) */
    /* 0x0800822A: c114     asrs     r1, r0, #0x13 */
    r1 = r0 >> 0x13;
    /* 0x0800822C: 0a43     orrs     r2, r1 */
    r2 = r2 | r1;
    /* 0x0800822E: 8261     str      r2, [r0, #0x18] */
    *(uint32_t *)(r0 + 24) = r2;   /* store */  /* MMIO: RCC.APB2ENR(w) */
    /* 0x08008230: 8069     ldr      r0, [r0, #0x18] */
    r0 = *(uint32_t *)(r0 + 24);  /* MMIO: RCC.APB2ENR(r) */
    /* 0x08008232: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08008234: 0840     ands     r0, r1 */
    r0 = r0 & r1;
    /* 0x08008236: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08008238: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x0800823A: 0d20     movs     r0, #0xd */
    r0 = 0xd;
    /* 0x0800823C: fff718fa bl       #0x8007670 */
    FUN_08007670();   /* BL -> 0x08007670 FUN_08007670 */
    /* 0x08008240: 0d20     movs     r0, #0xd */
    r0 = 0xd;
    /* 0x08008242: 10e0     b        #0x8008266 */
    goto L_08008266;

    L_08008244:
    /* 0x08008244: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08008246: 9207     lsls     r2, r2, #0x1e */
    r2 = r2 << 0x1e;
    /* 0x08008248: 9142     cmp      r1, r2 */
    /* flags = cmp(r1, r2) */
    /* 0x0800824A: 0fd1     bne      #0x800826c */
    if (r1 != r2) goto L_0800826C;
    /* 0x0800824C: c169     ldr      r1, [r0, #0x1c] */
    r1 = *(uint32_t *)(r0 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x0800824E: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08008250: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x08008252: c161     str      r1, [r0, #0x1c] */
    *(uint32_t *)(r0 + 28) = r1;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x08008254: c069     ldr      r0, [r0, #0x1c] */
    r0 = *(uint32_t *)(r0 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x08008256: 1040     ands     r0, r2 */
    r0 = r0 & r2;
    /* 0x08008258: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800825A: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x0800825C: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x0800825E: 0f20     movs     r0, #0xf */
    r0 = 0xf;
    /* 0x08008260: fff706fa bl       #0x8007670 */
    FUN_08007670();   /* BL -> 0x08007670 FUN_08007670 */
    /* 0x08008264: 0f20     movs     r0, #0xf */
    r0 = 0xf;

    L_08008266:
    /* 0x08008266: fff7f9f9 bl       #0x800765c */
    FUN_0800765C();   /* BL -> 0x0800765C FUN_0800765C */
    /* 0x0800826A: 08bd     pop      {r3, pc} */
    pop({r3, pc});   /* return */

    L_0800826C:
    /* 0x0800826C: 0f4a     ldr      r2, [pc, #0x3c]  ; =TIM3.CR1 */
    r2 = *(uint32_t *)0x080082AC;   /* = MMIO TIM3+0x00 */
    /* 0x0800826E: 9142     cmp      r1, r2 */
    /* flags = cmp(r1, r2) */
    /* 0x08008270: 07d1     bne      #0x8008282 */
    if (r1 != r2) goto L_08008282;
    /* 0x08008272: c269     ldr      r2, [r0, #0x1c] */
    r2 = *(uint32_t *)(r0 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x08008274: 0221     movs     r1, #2 */
    r1 = 2;
    /* 0x08008276: 0a43     orrs     r2, r1 */
    r2 = r2 | r1;
    /* 0x08008278: c261     str      r2, [r0, #0x1c] */
    *(uint32_t *)(r0 + 28) = r2;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x0800827A: c069     ldr      r0, [r0, #0x1c] */
    r0 = *(uint32_t *)(r0 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x0800827C: 0840     ands     r0, r1 */
    r0 = r0 & r1;
    /* 0x0800827E: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */

    L_08008280:
    /* 0x08008280: 08bd     pop      {r3, pc} */
    pop({r3, pc});   /* return */

    L_08008282:
    /* 0x08008282: 0b4a     ldr      r2, [pc, #0x2c]  ; =TIM14.CR1 */
    r2 = *(uint32_t *)0x080082B0;   /* = MMIO TIM14+0x00 */
    /* 0x08008284: 9142     cmp      r1, r2 */
    /* flags = cmp(r1, r2) */
    /* 0x08008286: fbd1     bne      #0x8008280 */
    if (r1 != r2) goto L_08008280;
    /* 0x08008288: c269     ldr      r2, [r0, #0x1c] */
    r2 = *(uint32_t *)(r0 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x0800828A: 8915     asrs     r1, r1, #0x16 */
    r1 = r1 >> 0x16;
    /* 0x0800828C: 0a43     orrs     r2, r1 */
    r2 = r2 | r1;
    /* 0x0800828E: c261     str      r2, [r0, #0x1c] */
    *(uint32_t *)(r0 + 28) = r2;   /* store */  /* MMIO: RCC.APB1ENR(w) */
    /* 0x08008290: c069     ldr      r0, [r0, #0x1c] */
    r0 = *(uint32_t *)(r0 + 28);  /* MMIO: RCC.APB1ENR(r) */
    /* 0x08008292: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08008294: 0840     ands     r0, r1 */
    r0 = r0 & r1;
    /* 0x08008296: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08008298: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x0800829A: 1320     movs     r0, #0x13 */
    r0 = 0x13;
    /* 0x0800829C: fff7e8f9 bl       #0x8007670 */
    FUN_08007670();   /* BL -> 0x08007670 FUN_08007670 */
    /* 0x080082A0: 1320     movs     r0, #0x13 */
    r0 = 0x13;
    /* 0x080082A2: e0e7     b        #0x8008266 */
    goto L_08008266;   /* loop */
}
