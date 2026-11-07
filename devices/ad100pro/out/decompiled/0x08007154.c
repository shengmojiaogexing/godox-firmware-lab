/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007154
 * Name      : FUN_08007154
 * End       : 0x080071E8
 * Size      : 148 bytes, 68 instructions decoded in body (functions.json insns=68, blocks=13)
 * Class     : function
 * Callers   : 1  -> 0x08005DFC FUN_08005DFC
 * Callees   : 3  -> 0x08005E24 FUN_08005E24, 0x08005E48 FUN_08005E48, 0x08005F34 FUN_08005F34
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007154(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007154: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x08007156: 0546     mov      r5, r0 */
    r5 = r0;
    /* 0x08007158: 2348     ldr      r0, [pc, #0x8c]  ; =RAM_20000240 */
    r0 = *(uint32_t *)0x080071E8;   /* = RAM 0x20000240 */
    /* 0x0800715A: 0f46     mov      r7, r1 */
    r7 = r1;
    /* 0x0800715C: 017e     ldrb     r1, [r0, #0x18] */
    r1 = *(uint8_t *)(r0 + 24);
    /* 0x0800715E: 0126     movs     r6, #1 */
    r6 = 1;
    /* 0x08007160: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x08007162: 0bd0     beq      #0x800717c */
    if (r1 == 1) goto L_0800717C;
    /* 0x08007164: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08007166: 0176     strb     r1, [r0, #0x18] */
    *(uint8_t *)(r0 + 24) = r1;   /* store */
    /* 0x08007168: 2868     ldr      r0, [r5] */
    r0 = *(uint32_t *)(r5);
    /* 0x0800716A: 204c     ldr      r4, [pc, #0x80]  ; =0x0000C350 */
    r4 = *(uint32_t *)0x080071EC;   /* = 0x0000C350 */
    /* 0x0800716C: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800716E: 07d0     beq      #0x8007180 */
    if (r0 == 1) goto L_08007180;
    /* 0x08007170: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08007172: fef7dffe bl       #0x8005f34 */
    FUN_08005F34();   /* BL -> 0x08005F34 FUN_08005F34 */
    /* 0x08007176: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08007178: 13d0     beq      #0x80071a2 */
    if (r0 == 0) goto L_080071A2;
    /* 0x0800717A: 30e0     b        #0x80071de */
    goto L_080071DE;

    L_0800717C:
    /* 0x0800717C: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x0800717E: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_08007180:
    /* 0x08007180: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08007182: fef7d7fe bl       #0x8005f34 */
    FUN_08005F34();   /* BL -> 0x08005F34 FUN_08005F34 */
    /* 0x08007186: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08007188: 29d1     bne      #0x80071de */
    if (r0 != 0) goto L_080071DE;
    /* 0x0800718A: fef74bfe bl       #0x8005e24 */
    FUN_08005E24();   /* BL -> 0x08005E24 FUN_08005E24 */
    /* 0x0800718E: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08007190: fef7d0fe bl       #0x8005f34 */
    FUN_08005F34();   /* BL -> 0x08005F34 FUN_08005F34 */
    /* 0x08007194: 0646     mov      r6, r0 */
    r6 = r0;
    /* 0x08007196: 1648     ldr      r0, [pc, #0x58]  ; =FLASH_IF.ACR */
    r0 = *(uint32_t *)0x080071F0;   /* = MMIO FLASH+0x00 */
    /* 0x08007198: 0169     ldr      r1, [r0, #0x10] */
    r1 = *(uint32_t *)(r0 + 16);  /* MMIO: FLASH_IF.CR(r) */
    /* 0x0800719A: 0422     movs     r2, #4 */
    r2 = 4;
    /* 0x0800719C: 9143     bics     r1, r2 */
    r1 = r1 & ~r2;
    /* 0x0800719E: 0161     str      r1, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r1;   /* store */  /* MMIO: FLASH_IF.CR(w) */
    /* 0x080071A0: 1de0     b        #0x80071de */
    goto L_080071DE;

    L_080071A2:
    /* 0x080071A2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080071A4: c043     mvns     r0, r0 */
    r0 = ~r0;
    /* 0x080071A6: 3860     str      r0, [r7] */
    *(uint32_t *)(r7) = r0;   /* store */  /* MMIO: DMA1.ISR(w) */
    /* 0x080071A8: 6c68     ldr      r4, [r5, #4] */
    r4 = *(uint32_t *)(r5 + 4);
    /* 0x080071AA: 12e0     b        #0x80071d2 */
    goto L_080071D2;

    L_080071AC:
    /* 0x080071AC: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080071AE: fef74bfe bl       #0x8005e48 */
    FUN_08005E48();   /* BL -> 0x08005E48 FUN_08005E48 */
    /* 0x080071B2: 0e48     ldr      r0, [pc, #0x38]  ; =0x0000C350 */
    r0 = *(uint32_t *)0x080071EC;   /* = 0x0000C350 */
    /* 0x080071B4: fef7befe bl       #0x8005f34 */
    FUN_08005F34();   /* BL -> 0x08005F34 FUN_08005F34 */
    /* 0x080071B8: 0646     mov      r6, r0 */
    r6 = r0;
    /* 0x080071BA: 0d48     ldr      r0, [pc, #0x34]  ; =FLASH_IF.ACR */
    r0 = *(uint32_t *)0x080071F0;   /* = MMIO FLASH+0x00 */
    /* 0x080071BC: 0269     ldr      r2, [r0, #0x10] */
    r2 = *(uint32_t *)(r0 + 16);  /* MMIO: FLASH_IF.CR(r) */
    /* 0x080071BE: 0221     movs     r1, #2 */
    r1 = 2;
    /* 0x080071C0: 8a43     bics     r2, r1 */
    r2 = r2 & ~r1;
    /* 0x080071C2: 0261     str      r2, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r2;   /* store */  /* MMIO: FLASH_IF.CR(w) */
    /* 0x080071C4: 002e     cmp      r6, #0 */
    /* flags = cmp(r6, #0) */
    /* 0x080071C6: 01d0     beq      #0x80071cc */
    if (r6 == 0) goto L_080071CC;
    /* 0x080071C8: 3c60     str      r4, [r7] */
    *(uint32_t *)(r7) = r4;   /* store */  /* MMIO: DMA1.ISR(w) */
    /* 0x080071CA: 08e0     b        #0x80071de */
    goto L_080071DE;

    L_080071CC:
    /* 0x080071CC: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x080071CE: c002     lsls     r0, r0, #0xb */
    r0 = r0 << 0xb;
    /* 0x080071D0: 2418     adds     r4, r4, r0 */
    r4 = r4 + r0;

    L_080071D2:
    /* 0x080071D2: a868     ldr      r0, [r5, #8] */
    r0 = *(uint32_t *)(r5 + 8);
    /* 0x080071D4: c102     lsls     r1, r0, #0xb */
    r1 = r0 << 0xb;
    /* 0x080071D6: 6868     ldr      r0, [r5, #4] */
    r0 = *(uint32_t *)(r5 + 4);
    /* 0x080071D8: 0818     adds     r0, r1, r0 */
    r0 = r1 + r0;
    /* 0x080071DA: a042     cmp      r0, r4 */
    /* flags = cmp(r0, r4) */
    /* 0x080071DC: e6d8     bhi      #0x80071ac */
    if (r0 > r4) goto L_080071AC;   /* unsigned */

    L_080071DE:
    /* 0x080071DE: 0248     ldr      r0, [pc, #8]  ; =RAM_20000240 */
    r0 = *(uint32_t *)0x080071E8;   /* = RAM 0x20000240 */
    /* 0x080071E0: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x080071E2: 0176     strb     r1, [r0, #0x18] */
    *(uint8_t *)(r0 + 24) = r1;   /* store */
    /* 0x080071E4: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x080071E6: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
