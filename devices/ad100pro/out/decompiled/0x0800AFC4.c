/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800AFC4
 * Name      : FUN_0800AFC4
 * End       : 0x0800B24A
 * Size      : 646 bytes, 279 instructions decoded in body (functions.json insns=27, blocks=5)
 * Class     : hub
 * Callers   : 2  -> 0x0800AA74 FUN_0800AA74, 0x0800AB40 FUN_0800AB40
 * Callees   : 11  -> 0x08003E2C FUN_08003E2C, 0x08003EB4 FUN_08003EB4, 0x08005D7C FUN_08005D7C, 0x0800655C FUN_0800655C, 0x08009FC0 FUN_08009FC0, 0x0800A620 FUN_0800A620, 0x0800A818 FUN_0800A818, 0x0800B4F8 FUN_0800B4F8, ... +3
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800AFC4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800AFC4: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x0800AFC6: cb4b     ldr      r3, [pc, #0x32c]  ; =RAM_2000017D */
    r3 = *(uint32_t *)0x0800B2F4;   /* = RAM 0x2000017D */
    /* 0x0800AFC8: 5028     cmp      r0, #0x50 */
    /* flags = cmp(r0, #0x50) */
    /* 0x0800AFCA: 1e78     ldrb     r6, [r3] */
    r6 = *(uint8_t *)(r3);
    /* 0x0800AFCC: 03d0     beq      #0x800afd6 */
    if (r0 == 0x50) goto L_0800AFD6;
    /* 0x0800AFCE: 3346     mov      r3, r6 */
    r3 = r6;
    /* 0x0800AFD0: 0933     adds     r3, #9 */
    r3 = r3 + 9;
    /* 0x0800AFD2: 9842     cmp      r0, r3 */
    /* flags = cmp(r0, r3) */
    /* 0x0800AFD4: 72d1     bne      #0x800b0bc */
    if (r0 != r3) goto L_0800B0BC;

    L_0800AFD6:
    /* 0x0800AFD6: c848     ldr      r0, [pc, #0x320]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800B2F8;   /* = RAM 0x20000175 */
    /* 0x0800AFD8: b039     subs     r1, #0xb0 */
    r1 = r1 - 0xb0;
    /* 0x0800AFDA: 0578     ldrb     r5, [r0] */
    r5 = *(uint8_t *)(r0);
    /* 0x0800AFDC: c748     ldr      r0, [pc, #0x31c]  ; =RAM_20000185 */
    r0 = *(uint32_t *)0x0800B2FC;   /* = RAM 0x20000185 */
    /* 0x0800AFDE: af07     lsls     r7, r5, #0x1e */
    r7 = r5 << 0x1e;
    /* 0x0800AFE0: bf0f     lsrs     r7, r7, #0x1e */
    r7 = r7 >> 0x1e;
    /* 0x0800AFE2: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800AFE4: c64c     ldr      r4, [pc, #0x318]  ; =RAM_200001B2 */
    r4 = *(uint32_t *)0x0800B300;   /* = RAM 0x200001B2 */
    /* 0x0800AFE6: 0b00     movs     r3, r1 */
    r3 = r1;
    /* 0x0800AFE8: 01f083fc bl       #0x800c8f2 */
    FUN_0800C8F2();   /* BL -> 0x0800C8F2 FUN_0800C8F2 */
    /* 0x0800AFEC: 261e     subs     r6, r4, #0 */
    r6 = r4 - 0;
    /* 0x0800AFEE: 1428     cmp      r0, #0x14 */
    /* flags = cmp(r0, #0x14) */
    /* 0x0800AFF0: 3b69     ldr      r3, [r7, #0x10] */
    r3 = *(uint32_t *)(r7 + 16);
    /* 0x0800AFF2: 1e1e     subs     r6, r3, #0 */
    r6 = r3 - 0;
    /* 0x0800AFF4: 1e1e     subs     r6, r3, #0 */
    r6 = r3 - 0;
    /* 0x0800AFF6: f21e     subs     r2, r6, #3 */
    r2 = r6 - 3;
    /* 0x0800B012: e91e     subs     r1, r5, #3 */
    r1 = r5 - 3;
    /* 0x0800B014: b848     ldr      r0, [pc, #0x2e0]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800B2F8;   /* = RAM 0x20000175 */
    /* 0x0800B016: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800B018: 8008     lsrs     r0, r0, #2 */
    r0 = r0 >> 2;
    /* 0x0800B01A: 8000     lsls     r0, r0, #2 */
    r0 = r0 << 2;
    /* 0x0800B01C: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x0800B01E: 04d0     beq      #0x800b02a */
    if (r2 == 0) goto L_0800B02A;
    /* 0x0800B020: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800B022: 04d0     beq      #0x800b02e */
    if (r2 == 1) goto L_0800B02E;
    /* 0x0800B024: 022a     cmp      r2, #2 */
    /* flags = cmp(r2, #2) */
    /* 0x0800B026: 05d0     beq      #0x800b034 */
    if (r2 == 2) goto L_0800B034;
    /* 0x0800B028: 57e1     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */

    L_0800B02A:
    /* 0x0800B02A: b349     ldr      r1, [pc, #0x2cc]  ; =RAM_20000175 */
    r1 = *(uint32_t *)0x0800B2F8;   /* = RAM 0x20000175 */
    /* 0x0800B02C: 04e0     b        #0x800b038 */
    goto L_0800B038;

    L_0800B02E:
    /* 0x0800B02E: b249     ldr      r1, [pc, #0x2c8]  ; =RAM_20000175 */
    r1 = *(uint32_t *)0x0800B2F8;   /* = RAM 0x20000175 */
    /* 0x0800B030: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800B032: 01e0     b        #0x800b038 */
    goto L_0800B038;

    L_0800B034:
    /* 0x0800B034: b049     ldr      r1, [pc, #0x2c0]  ; =RAM_20000175 */
    r1 = *(uint32_t *)0x0800B2F8;   /* = RAM 0x20000175 */
    /* 0x0800B036: 801c     adds     r0, r0, #2 */
    r0 = r0 + 2;

    L_0800B038:
    /* 0x0800B038: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x0800B03A: 4ee1     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */
    /* 0x0800B03C: b148     ldr      r0, [pc, #0x2c4]  ; =RAM_20000231 */
    r0 = *(uint32_t *)0x0800B304;   /* = RAM 0x20000231 */
    /* 0x0800B03E: 552a     cmp      r2, #0x55 */
    /* flags = cmp(r2, #0x55) */
    /* 0x0800B040: 01d3     blo      #0x800b046 */
    if (r2 < 0x55) goto L_0800B046;   /* unsigned */
    /* 0x0800B042: 0421     movs     r1, #4 */
    r1 = 4;
    /* 0x0800B044: 28e1     b        #0x800b298 */
    FUN_0800B298();   /* tail call 0x0800B298 */

    L_0800B046:
    /* 0x0800B046: 462a     cmp      r2, #0x46 */
    /* flags = cmp(r2, #0x46) */
    /* 0x0800B048: 01d3     blo      #0x800b04e */
    if (r2 < 0x46) goto L_0800B04E;   /* unsigned */
    /* 0x0800B04A: 0321     movs     r1, #3 */
    r1 = 3;
    /* 0x0800B04C: 24e1     b        #0x800b298 */
    FUN_0800B298();   /* tail call 0x0800B298 */

    L_0800B04E:
    /* 0x0800B04E: 322a     cmp      r2, #0x32 */
    /* flags = cmp(r2, #0x32) */
    /* 0x0800B050: 01d3     blo      #0x800b056 */
    if (r2 < 0x32) goto L_0800B056;   /* unsigned */
    /* 0x0800B052: 0221     movs     r1, #2 */
    r1 = 2;
    /* 0x0800B054: 20e1     b        #0x800b298 */
    FUN_0800B298();   /* tail call 0x0800B298 */

    L_0800B056:
    /* 0x0800B056: 232a     cmp      r2, #0x23 */
    /* flags = cmp(r2, #0x23) */
    /* 0x0800B058: 01d3     blo      #0x800b05e */
    if (r2 < 0x23) goto L_0800B05E;   /* unsigned */
    /* 0x0800B05A: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800B05C: 1ce1     b        #0x800b298 */
    FUN_0800B298();   /* tail call 0x0800B298 */

    L_0800B05E:
    /* 0x0800B05E: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800B060: 1ae1     b        #0x800b298 */
    FUN_0800B298();   /* tail call 0x0800B298 */
    /* 0x0800B062: a549     ldr      r1, [pc, #0x294]  ; =RAM_20000175 */
    r1 = *(uint32_t *)0x0800B2F8;   /* = RAM 0x20000175 */
    /* 0x0800B064: e723     movs     r3, #0xe7 */
    r3 = 0xe7;
    /* 0x0800B066: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800B068: 1940     ands     r1, r3 */
    r1 = r1 & r3;
    /* 0x0800B06A: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x0800B06C: 02d0     beq      #0x800b074 */
    if (r2 == 0) goto L_0800B074;
    /* 0x0800B06E: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800B070: 12d0     beq      #0x800b098 */
    if (r2 == 1) goto L_0800B098;
    /* 0x0800B072: 32e1     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */

    L_0800B074:
    /* 0x0800B074: 0428     cmp      r0, #4 */
    /* flags = cmp(r0, #4) */
    /* 0x0800B076: 05d0     beq      #0x800b084 */
    if (r0 == 4) goto L_0800B084;
    /* 0x0800B078: 0528     cmp      r0, #5 */
    /* flags = cmp(r0, #5) */
    /* 0x0800B07A: 03d0     beq      #0x800b084 */
    if (r0 == 5) goto L_0800B084;
    /* 0x0800B07C: 0328     cmp      r0, #3 */
    /* flags = cmp(r0, #3) */
    /* 0x0800B07E: 01d0     beq      #0x800b084 */
    if (r0 == 3) goto L_0800B084;
    /* 0x0800B080: 0728     cmp      r0, #7 */
    /* flags = cmp(r0, #7) */
    /* 0x0800B082: 04d1     bne      #0x800b08e */
    if (r0 != 7) goto L_0800B08E;

    L_0800B084:
    /* 0x0800B084: e806     lsls     r0, r5, #0x1b */
    r0 = r5 << 0x1b;
    /* 0x0800B086: 02d5     bpl      #0x800b08e */
    if (r0 >= 7) goto L_0800B08E;   /* result non-negative */
    /* 0x0800B088: 9f48     ldr      r0, [pc, #0x27c]  ; =RAM_2000020E */
    r0 = *(uint32_t *)0x0800B308;   /* = RAM 0x2000020E */
    /* 0x0800B08A: 1e22     movs     r2, #0x1e */
    r2 = 0x1e;
    /* 0x0800B08C: 0271     strb     r2, [r0, #4] */
    *(uint8_t *)(r0 + 4) = r2;   /* store */

    L_0800B08E:
    /* 0x0800B08E: 9a48     ldr      r0, [pc, #0x268]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800B2F8;   /* = RAM 0x20000175 */
    /* 0x0800B090: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x0800B092: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800B094: 9c49     ldr      r1, [pc, #0x270]  ; =RAM_2000020E */
    r1 = *(uint32_t *)0x0800B308;   /* = RAM 0x2000020E */
    /* 0x0800B096: 0be0     b        #0x800b0b0 */
    goto L_0800B0B0;

    L_0800B098:
    /* 0x0800B098: 9748     ldr      r0, [pc, #0x25c]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800B2F8;   /* = RAM 0x20000175 */
    /* 0x0800B09A: 012f     cmp      r7, #1 */
    /* flags = cmp(r7, #1) */
    /* 0x0800B09C: 03d1     bne      #0x800b0a6 */
    if (r7 != 1) goto L_0800B0A6;
    /* 0x0800B09E: 9b4a     ldr      r2, [pc, #0x26c]  ; =RAM_20000480 */
    r2 = *(uint32_t *)0x0800B30C;   /* = RAM 0x20000480 */
    /* 0x0800B0A0: 9279     ldrb     r2, [r2, #6] */
    r2 = *(uint8_t *)(r2 + 6);
    /* 0x0800B0A2: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800B0A4: 06d0     beq      #0x800b0b4 */
    if (r2 == 1) goto L_0800B0B4;

    L_0800B0A6:
    /* 0x0800B0A6: 1031     adds     r1, #0x10 */
    r1 = r1 + 0x10;
    /* 0x0800B0A8: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x0800B0AA: 9749     ldr      r1, [pc, #0x25c]  ; =RAM_2000020E */
    r1 = *(uint32_t *)0x0800B308;   /* = RAM 0x2000020E */
    /* 0x0800B0AC: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800B0AE: 4870     strb     r0, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r0;   /* store */

    L_0800B0B0:
    /* 0x0800B0B0: 8870     strb     r0, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r0;   /* store */
    /* 0x0800B0B2: 12e1     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */

    L_0800B0B4:
    /* 0x0800B0B4: 9449     ldr      r1, [pc, #0x250]  ; =RAM_2000020E */
    r1 = *(uint32_t *)0x0800B308;   /* = RAM 0x2000020E */
    /* 0x0800B0B6: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800B0B8: 4870     strb     r0, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r0;   /* store */
    /* 0x0800B0BA: 0ee1     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */

    L_0800B0BC:
    /* 0x0800B0BC: c4e0     b        #0x800b248 */
    goto L_0800B248;
    /* 0x0800B0BE: 1a21     movs     r1, #0x1a */
    r1 = 0x1a;
    /* 0x0800B0C0: 0a23     movs     r3, #0xa */
    r3 = 0xa;
    /* 0x0800B0C2: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x0800B0C4: 10d0     beq      #0x800b0e8 */
    if (r0 == 2) goto L_0800B0E8;
    /* 0x0800B0C6: 0428     cmp      r0, #4 */
    /* flags = cmp(r0, #4) */
    /* 0x0800B0C8: 39d0     beq      #0x800b13e */
    if (r0 == 4) goto L_0800B13E;
    /* 0x0800B0CA: 0528     cmp      r0, #5 */
    /* flags = cmp(r0, #5) */
    /* 0x0800B0CC: 46d0     beq      #0x800b15c */
    if (r0 == 5) goto L_0800B15C;
    /* 0x0800B0CE: 0728     cmp      r0, #7 */
    /* flags = cmp(r0, #7) */
    /* 0x0800B0D0: 44d0     beq      #0x800b15c */
    if (r0 == 7) goto L_0800B15C;
    /* 0x0800B0D2: 0328     cmp      r0, #3 */
    /* flags = cmp(r0, #3) */
    /* 0x0800B0D4: 5fd0     beq      #0x800b196 */
    if (r0 == 3) goto L_0800B196;
    /* 0x0800B0D6: 0628     cmp      r0, #6 */
    /* flags = cmp(r0, #6) */
    /* 0x0800B0D8: 6dd0     beq      #0x800b1b6 */
    if (r0 == 6) goto L_0800B1B6;
    /* 0x0800B0DA: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800B0DC: 6cd0     beq      #0x800b1b8 */
    if (r2 == 1) goto L_0800B1B8;
    /* 0x0800B0DE: 022a     cmp      r2, #2 */
    /* flags = cmp(r2, #2) */
    /* 0x0800B0E0: 6bd0     beq      #0x800b1ba */
    if (r2 == 2) goto L_0800B1BA;
    /* 0x0800B0E2: 072a     cmp      r2, #7 */
    /* flags = cmp(r2, #7) */
    /* 0x0800B0E4: 6ad0     beq      #0x800b1bc */
    if (r2 == 7) goto L_0800B1BC;
    /* 0x0800B0E6: 76e0     b        #0x800b1d6 */
    goto L_0800B1D6;

    L_0800B0E8:
    /* 0x0800B0E8: 8948     ldr      r0, [pc, #0x224]  ; =0x00000226 */
    r0 = *(uint32_t *)0x0800B310;   /* = 0x00000226 */
    /* 0x0800B0EA: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800B0EC: 02d0     beq      #0x800b0f4 */
    if (r2 == 1) goto L_0800B0F4;
    /* 0x0800B0EE: 042a     cmp      r2, #4 */
    /* flags = cmp(r2, #4) */
    /* 0x0800B0F0: 0cd0     beq      #0x800b10c */
    if (r2 == 4) goto L_0800B10C;
    /* 0x0800B0F2: 1ae0     b        #0x800b12a */
    goto L_0800B12A;

    L_0800B0F4:
    /* 0x0800B0F4: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800B0F6: 81f31088 msr      primask, r1 */
    /* TODO: msr primask, r1 */
    /* 0x0800B0FA: 01f09dfc bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800B0FE: 0520     movs     r0, #5 */
    r0 = 5;
    /* 0x0800B100: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800B102: 7d48     ldr      r0, [pc, #0x1f4]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800B2F8;   /* = RAM 0x20000175 */
    /* 0x0800B104: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800B106: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x0800B108: 0cd0     beq      #0x800b124 */
    if (r2 == 4) goto L_0800B124;
    /* 0x0800B10A: 99e0     b        #0x800b240 */
    goto L_0800B240;

    L_0800B10C:
    /* 0x0800B10C: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800B10E: 81f31088 msr      primask, r1 */
    /* TODO: msr primask, r1 */
    /* 0x0800B112: 01f091fc bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800B116: 0520     movs     r0, #5 */
    r0 = 5;
    /* 0x0800B118: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800B11A: 7748     ldr      r0, [pc, #0x1dc]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800B2F8;   /* = RAM 0x20000175 */
    /* 0x0800B11C: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800B11E: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x0800B120: 02d1     bne      #0x800b128 */
    if (r2 != 4) goto L_0800B128;
    /* 0x0800B122: 1820     movs     r0, #0x18 */
    r0 = 0x18;

    L_0800B124:
    /* 0x0800B124: fef74cff bl       #0x8009fc0 */
    FUN_08009FC0();   /* BL -> 0x08009FC0 FUN_08009FC0 */

    L_0800B128:
    /* 0x0800B128: 8ae0     b        #0x800b240 */
    goto L_0800B240;

    L_0800B12A:
    /* 0x0800B12A: 022a     cmp      r2, #2 */
    /* flags = cmp(r2, #2) */
    /* 0x0800B12C: 25d0     beq      #0x800b17a */
    if (r2 == 2) goto L_0800B17A;
    /* 0x0800B12E: 52e0     b        #0x800b1d6 */
    goto L_0800B1D6;

    L_0800B130:
    /* 0x0800B130: e806     lsls     r0, r5, #0x1b */
    r0 = r5 << 0x1b;
    /* 0x0800B132: 25d4     bmi      #0x800b180 */
    if (r2 < 2) goto L_0800B180;   /* result negative */

    L_0800B134:
    /* 0x0800B134: 7448     ldr      r0, [pc, #0x1d0]  ; =RAM_2000020E */
    r0 = *(uint32_t *)0x0800B308;   /* = RAM 0x2000020E */
    /* 0x0800B136: 4078     ldrb     r0, [r0, #1] */
    r0 = *(uint8_t *)(r0 + 1);
    /* 0x0800B138: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800B13A: 21d0     beq      #0x800b180 */
    if (r0 == 1) goto L_0800B180;
    /* 0x0800B13C: 54e0     b        #0x800b1e8 */
    goto L_0800B1E8;

    L_0800B13E:
    /* 0x0800B13E: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800B140: 0ed1     bne      #0x800b160 */
    if (r2 != 1) goto L_0800B160;
    /* 0x0800B142: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800B144: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800B148: 0520     movs     r0, #5 */
    r0 = 5;
    /* 0x0800B14A: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800B14C: a807     lsls     r0, r5, #0x1e */
    r0 = r5 << 0x1e;
    /* 0x0800B14E: 77d1     bne      #0x800b240 */
    if (r2 != 1) goto L_0800B240;
    /* 0x0800B150: fff766fa bl       #0x800a620 */
    FUN_0800A620();   /* BL -> 0x0800A620 FUN_0800A620 */
    /* 0x0800B154: 74e0     b        #0x800b240 */
    goto L_0800B240;

    L_0800B156:
    /* 0x0800B156: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x0800B158: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800B15A: c1e0     b        #0x800b2e0 */
    FUN_0800B2E0();   /* tail call 0x0800B2E0 */

    L_0800B15C:
    /* 0x0800B15C: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800B15E: 02d0     beq      #0x800b166 */
    if (r2 == 1) goto L_0800B166;

    L_0800B160:
    /* 0x0800B160: 032a     cmp      r2, #3 */
    /* flags = cmp(r2, #3) */
    /* 0x0800B162: f8d0     beq      #0x800b156 */
    if (r2 == 3) goto L_0800B156;
    /* 0x0800B164: e1e7     b        #0x800b12a */
    goto L_0800B12A;   /* loop */

    L_0800B166:
    /* 0x0800B166: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800B168: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800B16C: 0520     movs     r0, #5 */
    r0 = 5;
    /* 0x0800B16E: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800B170: a807     lsls     r0, r5, #0x1e */
    r0 = r5 << 0x1e;
    /* 0x0800B172: 65d1     bne      #0x800b240 */
    if (r2 != 3) goto L_0800B240;
    /* 0x0800B174: faf702fe bl       #0x8005d7c */
    FUN_08005D7C();   /* BL -> 0x08005D7C FUN_08005D7C */
    /* 0x0800B178: 62e0     b        #0x800b240 */
    goto L_0800B240;

    L_0800B17A:
    /* 0x0800B17A: 022f     cmp      r7, #2 */
    /* flags = cmp(r7, #2) */
    /* 0x0800B17C: dad0     beq      #0x800b134 */
    if (r7 == 2) goto L_0800B134;
    /* 0x0800B17E: d7e7     b        #0x800b130 */
    goto L_0800B130;   /* loop */

    L_0800B180:
    /* 0x0800B180: 2170     strb     r1, [r4] */
    *(uint8_t *)(r4) = r1;   /* store */
    /* 0x0800B182: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800B184: 0620     movs     r0, #6 */
    r0 = 6;
    /* 0x0800B186: 01f0d4fd bl       #0x800cd32 */
    FUN_0800CD32();   /* BL -> 0x0800CD32 FUN_0800CD32 */
    /* 0x0800B18A: 6249     ldr      r1, [pc, #0x188]  ; =RAM_200001DE */
    r1 = *(uint32_t *)0x0800B314;   /* = RAM 0x200001DE */
    /* 0x0800B18C: 6420     movs     r0, #0x64 */
    r0 = 0x64;
    /* 0x0800B18E: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x0800B190: fbf7e4f9 bl       #0x800655c */
    FUN_0800655C();   /* BL -> 0x0800655C FUN_0800655C */
    /* 0x0800B194: a1e0     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */

    L_0800B196:
    /* 0x0800B196: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800B198: 1bd1     bne      #0x800b1d2 */
    if (r2 != 1) goto L_0800B1D2;
    /* 0x0800B19A: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800B19C: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800B1A0: 0520     movs     r0, #5 */
    r0 = 5;
    /* 0x0800B1A2: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800B1A4: a807     lsls     r0, r5, #0x1e */
    r0 = r5 << 0x1e;
    /* 0x0800B1A6: 4bd1     bne      #0x800b240 */
    if (r2 != 1) goto L_0800B240;
    /* 0x0800B1A8: 5b48     ldr      r0, [pc, #0x16c]  ; =RAM_200001BD */
    r0 = *(uint32_t *)0x0800B318;   /* = RAM 0x200001BD */
    /* 0x0800B1AA: 805d     ldrb     r0, [r0, r6] */
    r0 = *(uint8_t *)(r0 + r6);
    /* 0x0800B1AC: ff28     cmp      r0, #0xff */
    /* flags = cmp(r0, #0xff) */
    /* 0x0800B1AE: 47d0     beq      #0x800b240 */
    if (r0 == 0xff) goto L_0800B240;
    /* 0x0800B1B0: 00f0a2f9 bl       #0x800b4f8 */
    FUN_0800B4F8();   /* BL -> 0x0800B4F8 FUN_0800B4F8 */
    /* 0x0800B1B4: 44e0     b        #0x800b240 */
    goto L_0800B240;

    L_0800B1B6:
    /* 0x0800B1B6: 19e0     b        #0x800b1ec */
    goto L_0800B1EC;

    L_0800B1B8:
    /* 0x0800B1B8: 28e0     b        #0x800b20c */
    goto L_0800B20C;

    L_0800B1BA:
    /* 0x0800B1BA: 31e0     b        #0x800b220 */
    goto L_0800B220;

    L_0800B1BC:
    /* 0x0800B1BC: 37e0     b        #0x800b22e */
    goto L_0800B22E;

    L_0800B1D2:
    /* 0x0800B1D2: 022a     cmp      r2, #2 */
    /* flags = cmp(r2, #2) */
    /* 0x0800B1D4: 24d0     beq      #0x800b220 */
    if (r2 == 2) goto L_0800B220;

    L_0800B1D6:
    /* 0x0800B1D6: 092a     cmp      r2, #9 */
    /* flags = cmp(r2, #9) */
    /* 0x0800B1D8: 37d0     beq      #0x800b24a */
    if (r2 == 9) goto L_0800B24A;
    /* 0x0800B1DA: 7ee0     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */

    L_0800B1DC:
    /* 0x0800B1DC: 2170     strb     r1, [r4] */
    *(uint8_t *)(r4) = r1;   /* store */
    /* 0x0800B1DE: 7fe0     b        #0x800b2e0 */
    FUN_0800B2E0();   /* tail call 0x0800B2E0 */

    L_0800B1E0:
    /* 0x0800B1E0: 4948     ldr      r0, [pc, #0x124]  ; =RAM_2000020E */
    r0 = *(uint32_t *)0x0800B308;   /* = RAM 0x2000020E */
    /* 0x0800B1E2: 4078     ldrb     r0, [r0, #1] */
    r0 = *(uint8_t *)(r0 + 1);
    /* 0x0800B1E4: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800B1E6: 20d0     beq      #0x800b22a */
    if (r0 == 1) goto L_0800B22A;

    L_0800B1E8:
    /* 0x0800B1E8: 2370     strb     r3, [r4] */
    *(uint8_t *)(r4) = r3;   /* store */
    /* 0x0800B1EA: 76e0     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */

    L_0800B1EC:
    /* 0x0800B1EC: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800B1EE: f0d1     bne      #0x800b1d2 */
    if (r2 != 1) goto L_0800B1D2;
    /* 0x0800B1F0: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800B1F2: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800B1F6: 0520     movs     r0, #5 */
    r0 = 5;
    /* 0x0800B1F8: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800B1FA: a807     lsls     r0, r5, #0x1e */
    r0 = r5 << 0x1e;
    /* 0x0800B1FC: 20d1     bne      #0x800b240 */
    if (r2 != 1) goto L_0800B240;
    /* 0x0800B1FE: 4648     ldr      r0, [pc, #0x118]  ; =RAM_200001BD */
    r0 = *(uint32_t *)0x0800B318;   /* = RAM 0x200001BD */
    /* 0x0800B200: 805d     ldrb     r0, [r0, r6] */
    r0 = *(uint8_t *)(r0 + r6);
    /* 0x0800B202: ff28     cmp      r0, #0xff */
    /* flags = cmp(r0, #0xff) */
    /* 0x0800B204: 1cd0     beq      #0x800b240 */
    if (r0 == 0xff) goto L_0800B240;
    /* 0x0800B206: fff707fb bl       #0x800a818 */
    FUN_0800A818();   /* BL -> 0x0800A818 FUN_0800A818 */
    /* 0x0800B20A: 19e0     b        #0x800b240 */
    goto L_0800B240;

    L_0800B20C:
    /* 0x0800B20C: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800B20E: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800B212: 0520     movs     r0, #5 */
    r0 = 5;
    /* 0x0800B214: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800B216: a807     lsls     r0, r5, #0x1e */
    r0 = r5 << 0x1e;
    /* 0x0800B218: 12d1     bne      #0x800b240 */
    if (r0 != 0xff) goto L_0800B240;
    /* 0x0800B21A: f8f707fe bl       #0x8003e2c */
    FUN_08003E2C();   /* BL -> 0x08003E2C FUN_08003E2C */
    /* 0x0800B21E: 0fe0     b        #0x800b240 */
    goto L_0800B240;

    L_0800B220:
    /* 0x0800B220: 022f     cmp      r7, #2 */
    /* flags = cmp(r7, #2) */
    /* 0x0800B222: ddd0     beq      #0x800b1e0 */
    if (r7 == 2) goto L_0800B1E0;
    /* 0x0800B224: e806     lsls     r0, r5, #0x1b */
    r0 = r5 << 0x1b;
    /* 0x0800B226: dbd5     bpl      #0x800b1e0 */
    if (r7 >= 2) goto L_0800B1E0;   /* result non-negative */
    /* 0x0800B228: d8e7     b        #0x800b1dc */
    goto L_0800B1DC;   /* loop */

    L_0800B22A:
    /* 0x0800B22A: 2170     strb     r1, [r4] */
    *(uint8_t *)(r4) = r1;   /* store */
    /* 0x0800B22C: 55e0     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */

    L_0800B22E:
    /* 0x0800B22E: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800B230: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800B234: 0520     movs     r0, #5 */
    r0 = 5;
    /* 0x0800B236: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800B238: a807     lsls     r0, r5, #0x1e */
    r0 = r5 << 0x1e;
    /* 0x0800B23A: 01d1     bne      #0x800b240 */
    if (r7 != 2) goto L_0800B240;
    /* 0x0800B23C: f8f73afe bl       #0x8003eb4 */
    FUN_08003EB4();   /* BL -> 0x08003EB4 FUN_08003EB4 */

    L_0800B240:
    /* 0x0800B240: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800B242: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800B244: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */

    L_0800B248:
    /* 0x0800B248: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
