/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004690
 * Name      : FUN_08004690
 * End       : 0x08004754
 * Size      : 196 bytes, 98 instructions decoded in body (functions.json insns=98, blocks=1)
 * Class     : function
 * Callers   : 3  -> 0x080097F8 FUN_080097F8, 0x080097FC FUN_080097FC, 0x0800B588 FUN_0800B588
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004690(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004690: 3049     ldr      r1, [pc, #0xc0]  ; =RAM_20000120 */
    r1 = *(uint32_t *)0x08004754;   /* = RAM 0x20000120 */
    /* 0x08004692: 5c22     movs     r2, #0x5c */
    r2 = 0x5c;
    /* 0x08004694: 4a70     strb     r2, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r2;   /* store */
    /* 0x08004696: 1031     adds     r1, #0x10 */
    r1 = r1 + 0x10;
    /* 0x08004698: 0a70     strb     r2, [r1] */
    *(uint8_t *)(r1) = r2;   /* store */
    /* 0x0800469A: 2f4a     ldr      r2, [pc, #0xbc]  ; =RAM_20000175 */
    r2 = *(uint32_t *)0x08004758;   /* = RAM 0x20000175 */
    /* 0x0800469C: 1278     ldrb     r2, [r2] */
    r2 = *(uint8_t *)(r2);
    /* 0x0800469E: 4a70     strb     r2, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r2;   /* store */
    /* 0x080046A0: 2e4a     ldr      r2, [pc, #0xb8]  ; =RAM_2000017D */
    r2 = *(uint32_t *)0x0800475C;   /* = RAM 0x2000017D */
    /* 0x080046A2: 1278     ldrb     r2, [r2] */
    r2 = *(uint8_t *)(r2);
    /* 0x080046A4: 8a70     strb     r2, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r2;   /* store */
    /* 0x080046A6: 2e4a     ldr      r2, [pc, #0xb8]  ; =RAM_20000165 */
    r2 = *(uint32_t *)0x08004760;   /* = RAM 0x20000165 */
    /* 0x080046A8: 1278     ldrb     r2, [r2] */
    r2 = *(uint8_t *)(r2);
    /* 0x080046AA: ca70     strb     r2, [r1, #3] */
    *(uint8_t *)(r1 + 3) = r2;   /* store */
    /* 0x080046AC: 0a68     ldr      r2, [r1] */
    r2 = *(uint32_t *)(r1);
    /* 0x080046AE: 0260     str      r2, [r0] */
    *(uint32_t *)(r0) = r2;   /* store */
    /* 0x080046B0: 2c4a     ldr      r2, [pc, #0xb0]  ; =RAM_20000166 */
    r2 = *(uint32_t *)0x08004764;   /* = RAM 0x20000166 */
    /* 0x080046B2: 1278     ldrb     r2, [r2] */
    r2 = *(uint8_t *)(r2);
    /* 0x080046B4: 0a70     strb     r2, [r1] */
    *(uint8_t *)(r1) = r2;   /* store */
    /* 0x080046B6: 2c4a     ldr      r2, [pc, #0xb0]  ; =RAM_200001B1 */
    r2 = *(uint32_t *)0x08004768;   /* = RAM 0x200001B1 */
    /* 0x080046B8: 1278     ldrb     r2, [r2] */
    r2 = *(uint8_t *)(r2);
    /* 0x080046BA: 4a70     strb     r2, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r2;   /* store */
    /* 0x080046BC: 2b4a     ldr      r2, [pc, #0xac]  ; =RAM_2000017C */
    r2 = *(uint32_t *)0x0800476C;   /* = RAM 0x2000017C */
    /* 0x080046BE: 1278     ldrb     r2, [r2] */
    r2 = *(uint8_t *)(r2);
    /* 0x080046C0: 8a70     strb     r2, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r2;   /* store */
    /* 0x080046C2: 2b4a     ldr      r2, [pc, #0xac]  ; =RAM_20000183 */
    r2 = *(uint32_t *)0x08004770;   /* = RAM 0x20000183 */
    /* 0x080046C4: 1278     ldrb     r2, [r2] */
    r2 = *(uint8_t *)(r2);
    /* 0x080046C6: ca70     strb     r2, [r1, #3] */
    *(uint8_t *)(r1 + 3) = r2;   /* store */
    /* 0x080046C8: 0a68     ldr      r2, [r1] */
    r2 = *(uint32_t *)(r1);
    /* 0x080046CA: 4260     str      r2, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r2;   /* store */
    /* 0x080046CC: 294a     ldr      r2, [pc, #0xa4]  ; =RAM_2000019C */
    r2 = *(uint32_t *)0x08004774;   /* = RAM 0x2000019C */
    /* 0x080046CE: 1378     ldrb     r3, [r2] */
    r3 = *(uint8_t *)(r2);
    /* 0x080046D0: 0b70     strb     r3, [r1] */
    *(uint8_t *)(r1) = r3;   /* store */
    /* 0x080046D2: 5378     ldrb     r3, [r2, #1] */
    r3 = *(uint8_t *)(r2 + 1);
    /* 0x080046D4: 4b70     strb     r3, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r3;   /* store */
    /* 0x080046D6: 9378     ldrb     r3, [r2, #2] */
    r3 = *(uint8_t *)(r2 + 2);
    /* 0x080046D8: 8b70     strb     r3, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r3;   /* store */
    /* 0x080046DA: d378     ldrb     r3, [r2, #3] */
    r3 = *(uint8_t *)(r2 + 3);
    /* 0x080046DC: cb70     strb     r3, [r1, #3] */
    *(uint8_t *)(r1 + 3) = r3;   /* store */
    /* 0x080046DE: 0b68     ldr      r3, [r1] */
    r3 = *(uint32_t *)(r1);
    /* 0x080046E0: 8360     str      r3, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r3;   /* store */
    /* 0x080046E2: 1379     ldrb     r3, [r2, #4] */
    r3 = *(uint8_t *)(r2 + 4);
    /* 0x080046E4: 0b70     strb     r3, [r1] */
    *(uint8_t *)(r1) = r3;   /* store */
    /* 0x080046E6: 5279     ldrb     r2, [r2, #5] */
    r2 = *(uint8_t *)(r2 + 5);
    /* 0x080046E8: 4a70     strb     r2, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r2;   /* store */
    /* 0x080046EA: 234a     ldr      r2, [pc, #0x8c]  ; =RAM_20000169 */
    r2 = *(uint32_t *)0x08004778;   /* = RAM 0x20000169 */
    /* 0x080046EC: 1278     ldrb     r2, [r2] */
    r2 = *(uint8_t *)(r2);
    /* 0x080046EE: 8a70     strb     r2, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r2;   /* store */
    /* 0x080046F0: 224a     ldr      r2, [pc, #0x88]  ; =RAM_20000231 */
    r2 = *(uint32_t *)0x0800477C;   /* = RAM 0x20000231 */
    /* 0x080046F2: 1278     ldrb     r2, [r2] */
    r2 = *(uint8_t *)(r2);
    /* 0x080046F4: ca70     strb     r2, [r1, #3] */
    *(uint8_t *)(r1 + 3) = r2;   /* store */
    /* 0x080046F6: 0a68     ldr      r2, [r1] */
    r2 = *(uint32_t *)(r1);
    /* 0x080046F8: c260     str      r2, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r2;   /* store */
    /* 0x080046FA: 214a     ldr      r2, [pc, #0x84]  ; =RAM_200001A2 */
    r2 = *(uint32_t *)0x08004780;   /* = RAM 0x200001A2 */
    /* 0x080046FC: 1378     ldrb     r3, [r2] */
    r3 = *(uint8_t *)(r2);
    /* 0x080046FE: 0b70     strb     r3, [r1] */
    *(uint8_t *)(r1) = r3;   /* store */
    /* 0x08004700: 5378     ldrb     r3, [r2, #1] */
    r3 = *(uint8_t *)(r2 + 1);
    /* 0x08004702: 4b70     strb     r3, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r3;   /* store */
    /* 0x08004704: 9378     ldrb     r3, [r2, #2] */
    r3 = *(uint8_t *)(r2 + 2);
    /* 0x08004706: 8b70     strb     r3, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r3;   /* store */
    /* 0x08004708: d378     ldrb     r3, [r2, #3] */
    r3 = *(uint8_t *)(r2 + 3);
    /* 0x0800470A: cb70     strb     r3, [r1, #3] */
    *(uint8_t *)(r1 + 3) = r3;   /* store */
    /* 0x0800470C: 0b68     ldr      r3, [r1] */
    r3 = *(uint32_t *)(r1);
    /* 0x0800470E: 0361     str      r3, [r0, #0x10] */
    *(uint32_t *)(r0 + 16) = r3;   /* store */
    /* 0x08004710: 1379     ldrb     r3, [r2, #4] */
    r3 = *(uint8_t *)(r2 + 4);
    /* 0x08004712: 0b70     strb     r3, [r1] */
    *(uint8_t *)(r1) = r3;   /* store */
    /* 0x08004714: 5279     ldrb     r2, [r2, #5] */
    r2 = *(uint8_t *)(r2 + 5);
    /* 0x08004716: 4a70     strb     r2, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r2;   /* store */
    /* 0x08004718: 1a4a     ldr      r2, [pc, #0x68]  ; =RAM_2000016A */
    r2 = *(uint32_t *)0x08004784;   /* = RAM 0x2000016A */
    /* 0x0800471A: 1278     ldrb     r2, [r2] */
    r2 = *(uint8_t *)(r2);
    /* 0x0800471C: 8a70     strb     r2, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r2;   /* store */
    /* 0x0800471E: 1a4a     ldr      r2, [pc, #0x68]  ; =RAM_2000016B */
    r2 = *(uint32_t *)0x08004788;   /* = RAM 0x2000016B */
    /* 0x08004720: 1278     ldrb     r2, [r2] */
    r2 = *(uint8_t *)(r2);
    /* 0x08004722: ca70     strb     r2, [r1, #3] */
    *(uint8_t *)(r1 + 3) = r2;   /* store */
    /* 0x08004724: 0a68     ldr      r2, [r1] */
    r2 = *(uint32_t *)(r1);
    /* 0x08004726: 4261     str      r2, [r0, #0x14] */
    *(uint32_t *)(r0 + 20) = r2;   /* store */
    /* 0x08004728: 184a     ldr      r2, [pc, #0x60]  ; =RAM_20000480 */
    r2 = *(uint32_t *)0x0800478C;   /* = RAM 0x20000480 */
    /* 0x0800472A: 1378     ldrb     r3, [r2] */
    r3 = *(uint8_t *)(r2);
    /* 0x0800472C: 0b70     strb     r3, [r1] */
    *(uint8_t *)(r1) = r3;   /* store */
    /* 0x0800472E: 5378     ldrb     r3, [r2, #1] */
    r3 = *(uint8_t *)(r2 + 1);
    /* 0x08004730: 4b70     strb     r3, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r3;   /* store */
    /* 0x08004732: 9378     ldrb     r3, [r2, #2] */
    r3 = *(uint8_t *)(r2 + 2);
    /* 0x08004734: 8b70     strb     r3, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r3;   /* store */
    /* 0x08004736: d378     ldrb     r3, [r2, #3] */
    r3 = *(uint8_t *)(r2 + 3);
    /* 0x08004738: cb70     strb     r3, [r1, #3] */
    *(uint8_t *)(r1 + 3) = r3;   /* store */
    /* 0x0800473A: 0b68     ldr      r3, [r1] */
    r3 = *(uint32_t *)(r1);
    /* 0x0800473C: 8361     str      r3, [r0, #0x18] */
    *(uint32_t *)(r0 + 24) = r3;   /* store */
    /* 0x0800473E: 1379     ldrb     r3, [r2, #4] */
    r3 = *(uint8_t *)(r2 + 4);
    /* 0x08004740: 0b70     strb     r3, [r1] */
    *(uint8_t *)(r1) = r3;   /* store */
    /* 0x08004742: 5379     ldrb     r3, [r2, #5] */
    r3 = *(uint8_t *)(r2 + 5);
    /* 0x08004744: 4b70     strb     r3, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r3;   /* store */
    /* 0x08004746: 9379     ldrb     r3, [r2, #6] */
    r3 = *(uint8_t *)(r2 + 6);
    /* 0x08004748: 8b70     strb     r3, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r3;   /* store */
    /* 0x0800474A: d279     ldrb     r2, [r2, #7] */
    r2 = *(uint8_t *)(r2 + 7);
    /* 0x0800474C: ca70     strb     r2, [r1, #3] */
    *(uint8_t *)(r1 + 3) = r2;   /* store */
    /* 0x0800474E: 0968     ldr      r1, [r1] */
    r1 = *(uint32_t *)(r1);
    /* 0x08004750: c161     str      r1, [r0, #0x1c] */
    *(uint32_t *)(r0 + 28) = r1;   /* store */
    /* 0x08004752: 7047     bx       lr */
    return;
}
