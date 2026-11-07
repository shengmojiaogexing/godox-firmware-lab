/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CE6C
 * Name      : FUN_0800CE6C
 * End       : 0x0800CF32
 * Size      : 198 bytes, 99 instructions decoded in body (functions.json insns=99, blocks=1)
 * Class     : function
 * Callers   : 2  -> 0x0800AD44 FUN_0800AD44, 0x0800AD64 FUN_0800AD64
 * Callees   : 0  -> -
 * MMIO      : GPIOB.+0x1
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CE6C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CE6C: 3248     ldr      r0, [pc, #0xc8]  ; =RAM_20000130 */
    r0 = *(uint32_t *)0x0800CF38;   /* = RAM 0x20000130 */
    /* 0x0800CE6E: 3149     ldr      r1, [pc, #0xc4]  ; =RAM_20000434 */
    r1 = *(uint32_t *)0x0800CF34;   /* = RAM 0x20000434 */
    /* 0x0800CE70: 0346     mov      r3, r0 */
    r3 = r0;
    /* 0x0800CE72: 0a68     ldr      r2, [r1] */
    r2 = *(uint32_t *)(r1);
    /* 0x0800CE74: 0260     str      r2, [r0] */
    *(uint32_t *)(r0) = r2;   /* store */
    /* 0x0800CE76: 0278     ldrb     r2, [r0] */
    r2 = *(uint8_t *)(r0);
    /* 0x0800CE78: 103b     subs     r3, #0x10 */
    r3 = r3 - 0x10;
    /* 0x0800CE7A: 5a70     strb     r2, [r3, #1] */
    *(uint8_t *)(r3 + 1) = r2;   /* store */  /* MMIO: GPIOB.+0x1(w) */
    /* 0x0800CE7C: 2f4b     ldr      r3, [pc, #0xbc]  ; =RAM_20000175 */
    r3 = *(uint32_t *)0x0800CF3C;   /* = RAM 0x20000175 */
    /* 0x0800CE7E: 4278     ldrb     r2, [r0, #1] */
    r2 = *(uint8_t *)(r0 + 1);
    /* 0x0800CE80: 1a70     strb     r2, [r3] */
    *(uint8_t *)(r3) = r2;   /* store */
    /* 0x0800CE82: 2f4b     ldr      r3, [pc, #0xbc]  ; =RAM_2000017D */
    r3 = *(uint32_t *)0x0800CF40;   /* = RAM 0x2000017D */
    /* 0x0800CE84: 8278     ldrb     r2, [r0, #2] */
    r2 = *(uint8_t *)(r0 + 2);
    /* 0x0800CE86: 1a70     strb     r2, [r3] */
    *(uint8_t *)(r3) = r2;   /* store */
    /* 0x0800CE88: 2e4b     ldr      r3, [pc, #0xb8]  ; =RAM_20000165 */
    r3 = *(uint32_t *)0x0800CF44;   /* = RAM 0x20000165 */
    /* 0x0800CE8A: c278     ldrb     r2, [r0, #3] */
    r2 = *(uint8_t *)(r0 + 3);
    /* 0x0800CE8C: 1a70     strb     r2, [r3] */
    *(uint8_t *)(r3) = r2;   /* store */
    /* 0x0800CE8E: 4a68     ldr      r2, [r1, #4] */
    r2 = *(uint32_t *)(r1 + 4);
    /* 0x0800CE90: 0260     str      r2, [r0] */
    *(uint32_t *)(r0) = r2;   /* store */
    /* 0x0800CE92: 2d4b     ldr      r3, [pc, #0xb4]  ; =RAM_20000166 */
    r3 = *(uint32_t *)0x0800CF48;   /* = RAM 0x20000166 */
    /* 0x0800CE94: 0278     ldrb     r2, [r0] */
    r2 = *(uint8_t *)(r0);
    /* 0x0800CE96: 1a70     strb     r2, [r3] */
    *(uint8_t *)(r3) = r2;   /* store */
    /* 0x0800CE98: 2c4b     ldr      r3, [pc, #0xb0]  ; =RAM_200001B1 */
    r3 = *(uint32_t *)0x0800CF4C;   /* = RAM 0x200001B1 */
    /* 0x0800CE9A: 4278     ldrb     r2, [r0, #1] */
    r2 = *(uint8_t *)(r0 + 1);
    /* 0x0800CE9C: 1a70     strb     r2, [r3] */
    *(uint8_t *)(r3) = r2;   /* store */
    /* 0x0800CE9E: 2c4b     ldr      r3, [pc, #0xb0]  ; =RAM_2000017C */
    r3 = *(uint32_t *)0x0800CF50;   /* = RAM 0x2000017C */
    /* 0x0800CEA0: 8278     ldrb     r2, [r0, #2] */
    r2 = *(uint8_t *)(r0 + 2);
    /* 0x0800CEA2: 1a70     strb     r2, [r3] */
    *(uint8_t *)(r3) = r2;   /* store */
    /* 0x0800CEA4: 2b4b     ldr      r3, [pc, #0xac]  ; =RAM_20000183 */
    r3 = *(uint32_t *)0x0800CF54;   /* = RAM 0x20000183 */
    /* 0x0800CEA6: c278     ldrb     r2, [r0, #3] */
    r2 = *(uint8_t *)(r0 + 3);
    /* 0x0800CEA8: 1a70     strb     r2, [r3] */
    *(uint8_t *)(r3) = r2;   /* store */
    /* 0x0800CEAA: 8a68     ldr      r2, [r1, #8] */
    r2 = *(uint32_t *)(r1 + 8);
    /* 0x0800CEAC: 0260     str      r2, [r0] */
    *(uint32_t *)(r0) = r2;   /* store */
    /* 0x0800CEAE: 2a4a     ldr      r2, [pc, #0xa8]  ; =RAM_2000019C */
    r2 = *(uint32_t *)0x0800CF58;   /* = RAM 0x2000019C */
    /* 0x0800CEB0: 0378     ldrb     r3, [r0] */
    r3 = *(uint8_t *)(r0);
    /* 0x0800CEB2: 1370     strb     r3, [r2] */
    *(uint8_t *)(r2) = r3;   /* store */
    /* 0x0800CEB4: 4378     ldrb     r3, [r0, #1] */
    r3 = *(uint8_t *)(r0 + 1);
    /* 0x0800CEB6: 5370     strb     r3, [r2, #1] */
    *(uint8_t *)(r2 + 1) = r3;   /* store */
    /* 0x0800CEB8: 8378     ldrb     r3, [r0, #2] */
    r3 = *(uint8_t *)(r0 + 2);
    /* 0x0800CEBA: 9370     strb     r3, [r2, #2] */
    *(uint8_t *)(r2 + 2) = r3;   /* store */
    /* 0x0800CEBC: c378     ldrb     r3, [r0, #3] */
    r3 = *(uint8_t *)(r0 + 3);
    /* 0x0800CEBE: d370     strb     r3, [r2, #3] */
    *(uint8_t *)(r2 + 3) = r3;   /* store */
    /* 0x0800CEC0: cb68     ldr      r3, [r1, #0xc] */
    r3 = *(uint32_t *)(r1 + 12);
    /* 0x0800CEC2: 0360     str      r3, [r0] */
    *(uint32_t *)(r0) = r3;   /* store */
    /* 0x0800CEC4: 0378     ldrb     r3, [r0] */
    r3 = *(uint8_t *)(r0);
    /* 0x0800CEC6: 1371     strb     r3, [r2, #4] */
    *(uint8_t *)(r2 + 4) = r3;   /* store */
    /* 0x0800CEC8: 4378     ldrb     r3, [r0, #1] */
    r3 = *(uint8_t *)(r0 + 1);
    /* 0x0800CECA: 5371     strb     r3, [r2, #5] */
    *(uint8_t *)(r2 + 5) = r3;   /* store */
    /* 0x0800CECC: 234b     ldr      r3, [pc, #0x8c]  ; =RAM_20000169 */
    r3 = *(uint32_t *)0x0800CF5C;   /* = RAM 0x20000169 */
    /* 0x0800CECE: 8278     ldrb     r2, [r0, #2] */
    r2 = *(uint8_t *)(r0 + 2);
    /* 0x0800CED0: 1a70     strb     r2, [r3] */
    *(uint8_t *)(r3) = r2;   /* store */
    /* 0x0800CED2: 234b     ldr      r3, [pc, #0x8c]  ; =RAM_20000231 */
    r3 = *(uint32_t *)0x0800CF60;   /* = RAM 0x20000231 */
    /* 0x0800CED4: c278     ldrb     r2, [r0, #3] */
    r2 = *(uint8_t *)(r0 + 3);
    /* 0x0800CED6: 1a70     strb     r2, [r3] */
    *(uint8_t *)(r3) = r2;   /* store */
    /* 0x0800CED8: 0a69     ldr      r2, [r1, #0x10] */
    r2 = *(uint32_t *)(r1 + 16);
    /* 0x0800CEDA: 0260     str      r2, [r0] */
    *(uint32_t *)(r0) = r2;   /* store */
    /* 0x0800CEDC: 214a     ldr      r2, [pc, #0x84]  ; =RAM_200001A2 */
    r2 = *(uint32_t *)0x0800CF64;   /* = RAM 0x200001A2 */
    /* 0x0800CEDE: 0378     ldrb     r3, [r0] */
    r3 = *(uint8_t *)(r0);
    /* 0x0800CEE0: 1370     strb     r3, [r2] */
    *(uint8_t *)(r2) = r3;   /* store */
    /* 0x0800CEE2: 4378     ldrb     r3, [r0, #1] */
    r3 = *(uint8_t *)(r0 + 1);
    /* 0x0800CEE4: 5370     strb     r3, [r2, #1] */
    *(uint8_t *)(r2 + 1) = r3;   /* store */
    /* 0x0800CEE6: 8378     ldrb     r3, [r0, #2] */
    r3 = *(uint8_t *)(r0 + 2);
    /* 0x0800CEE8: 9370     strb     r3, [r2, #2] */
    *(uint8_t *)(r2 + 2) = r3;   /* store */
    /* 0x0800CEEA: c378     ldrb     r3, [r0, #3] */
    r3 = *(uint8_t *)(r0 + 3);
    /* 0x0800CEEC: d370     strb     r3, [r2, #3] */
    *(uint8_t *)(r2 + 3) = r3;   /* store */
    /* 0x0800CEEE: 4b69     ldr      r3, [r1, #0x14] */
    r3 = *(uint32_t *)(r1 + 20);
    /* 0x0800CEF0: 0360     str      r3, [r0] */
    *(uint32_t *)(r0) = r3;   /* store */
    /* 0x0800CEF2: 0378     ldrb     r3, [r0] */
    r3 = *(uint8_t *)(r0);
    /* 0x0800CEF4: 1371     strb     r3, [r2, #4] */
    *(uint8_t *)(r2 + 4) = r3;   /* store */
    /* 0x0800CEF6: 4378     ldrb     r3, [r0, #1] */
    r3 = *(uint8_t *)(r0 + 1);
    /* 0x0800CEF8: 5371     strb     r3, [r2, #5] */
    *(uint8_t *)(r2 + 5) = r3;   /* store */
    /* 0x0800CEFA: 1b4b     ldr      r3, [pc, #0x6c]  ; =RAM_2000016A */
    r3 = *(uint32_t *)0x0800CF68;   /* = RAM 0x2000016A */
    /* 0x0800CEFC: 8278     ldrb     r2, [r0, #2] */
    r2 = *(uint8_t *)(r0 + 2);
    /* 0x0800CEFE: 1a70     strb     r2, [r3] */
    *(uint8_t *)(r3) = r2;   /* store */
    /* 0x0800CF00: c278     ldrb     r2, [r0, #3] */
    r2 = *(uint8_t *)(r0 + 3);
    /* 0x0800CF02: 1a4b     ldr      r3, [pc, #0x68]  ; =RAM_2000016B */
    r3 = *(uint32_t *)0x0800CF6C;   /* = RAM 0x2000016B */
    /* 0x0800CF04: 1a70     strb     r2, [r3] */
    *(uint8_t *)(r3) = r2;   /* store */
    /* 0x0800CF06: 8a69     ldr      r2, [r1, #0x18] */
    r2 = *(uint32_t *)(r1 + 24);
    /* 0x0800CF08: 0260     str      r2, [r0] */
    *(uint32_t *)(r0) = r2;   /* store */
    /* 0x0800CF0A: 194a     ldr      r2, [pc, #0x64]  ; =RAM_20000480 */
    r2 = *(uint32_t *)0x0800CF70;   /* = RAM 0x20000480 */
    /* 0x0800CF0C: 0378     ldrb     r3, [r0] */
    r3 = *(uint8_t *)(r0);
    /* 0x0800CF0E: 1370     strb     r3, [r2] */
    *(uint8_t *)(r2) = r3;   /* store */
    /* 0x0800CF10: 4378     ldrb     r3, [r0, #1] */
    r3 = *(uint8_t *)(r0 + 1);
    /* 0x0800CF12: 5370     strb     r3, [r2, #1] */
    *(uint8_t *)(r2 + 1) = r3;   /* store */
    /* 0x0800CF14: 8378     ldrb     r3, [r0, #2] */
    r3 = *(uint8_t *)(r0 + 2);
    /* 0x0800CF16: 9370     strb     r3, [r2, #2] */
    *(uint8_t *)(r2 + 2) = r3;   /* store */
    /* 0x0800CF18: c378     ldrb     r3, [r0, #3] */
    r3 = *(uint8_t *)(r0 + 3);
    /* 0x0800CF1A: d370     strb     r3, [r2, #3] */
    *(uint8_t *)(r2 + 3) = r3;   /* store */
    /* 0x0800CF1C: c969     ldr      r1, [r1, #0x1c] */
    r1 = *(uint32_t *)(r1 + 28);
    /* 0x0800CF1E: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */
    /* 0x0800CF20: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x0800CF22: 1171     strb     r1, [r2, #4] */
    *(uint8_t *)(r2 + 4) = r1;   /* store */
    /* 0x0800CF24: 4178     ldrb     r1, [r0, #1] */
    r1 = *(uint8_t *)(r0 + 1);
    /* 0x0800CF26: 5171     strb     r1, [r2, #5] */
    *(uint8_t *)(r2 + 5) = r1;   /* store */
    /* 0x0800CF28: 8178     ldrb     r1, [r0, #2] */
    r1 = *(uint8_t *)(r0 + 2);
    /* 0x0800CF2A: 9171     strb     r1, [r2, #6] */
    *(uint8_t *)(r2 + 6) = r1;   /* store */
    /* 0x0800CF2C: c078     ldrb     r0, [r0, #3] */
    r0 = *(uint8_t *)(r0 + 3);
    /* 0x0800CF2E: d071     strb     r0, [r2, #7] */
    *(uint8_t *)(r2 + 7) = r0;   /* store */
    /* 0x0800CF30: 7047     bx       lr */
    return;
}
