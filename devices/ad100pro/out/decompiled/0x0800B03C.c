/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B03C
 * Name      : FUN_0800B03C
 * End       : 0x0800B29C
 * Size      : 608 bytes, 272 instructions decoded in body (functions.json insns=21, blocks=11)
 * Class     : hub
 * Parent    : 0x0800AFC4 FUN_0800AFC4  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 11  -> 0x08003E2C FUN_08003E2C, 0x08003EB4 FUN_08003EB4, 0x08005D7C FUN_08005D7C, 0x0800655C FUN_0800655C, 0x08009FC0 FUN_08009FC0, 0x0800A620 FUN_0800A620, 0x0800A818 FUN_0800A818, 0x0800B4F8 FUN_0800B4F8, ... +3
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B03C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B03C: b148     ldr      r0, [pc, #0x2c4]  ; =RAM_20000231 */
    r0 = *(uint32_t *)0x0800B304;   /* = RAM 0x20000231 */
    /* 0x0800B03E: 552a     cmp      r2, #0x55 */
    /* flags = cmp(r2, #0x55) */
    /* 0x0800B040: 01d3     blo      #0x800b046 */
    if (r2 < 0x55) goto L_0800B046;   /* unsigned */
    /* 0x0800B042: 0421     movs     r1, #4 */
    r1 = 4;
    /* 0x0800B044: 28e1     b        #0x800b298 */
    goto L_0800B298;

    L_0800B046:
    /* 0x0800B046: 462a     cmp      r2, #0x46 */
    /* flags = cmp(r2, #0x46) */
    /* 0x0800B048: 01d3     blo      #0x800b04e */
    if (r2 < 0x46) goto L_0800B04E;   /* unsigned */
    /* 0x0800B04A: 0321     movs     r1, #3 */
    r1 = 3;
    /* 0x0800B04C: 24e1     b        #0x800b298 */
    goto L_0800B298;

    L_0800B04E:
    /* 0x0800B04E: 322a     cmp      r2, #0x32 */
    /* flags = cmp(r2, #0x32) */
    /* 0x0800B050: 01d3     blo      #0x800b056 */
    if (r2 < 0x32) goto L_0800B056;   /* unsigned */
    /* 0x0800B052: 0221     movs     r1, #2 */
    r1 = 2;
    /* 0x0800B054: 20e1     b        #0x800b298 */
    goto L_0800B298;

    L_0800B056:
    /* 0x0800B056: 232a     cmp      r2, #0x23 */
    /* flags = cmp(r2, #0x23) */
    /* 0x0800B058: 01d3     blo      #0x800b05e */
    if (r2 < 0x23) goto L_0800B05E;   /* unsigned */
    /* 0x0800B05A: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800B05C: 1ce1     b        #0x800b298 */
    goto L_0800B298;

    L_0800B05E:
    /* 0x0800B05E: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800B060: 1ae1     b        #0x800b298 */
    goto L_0800B298;
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

    L_0800B24A:
    /* 0x0800B24A: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800B24C: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800B250: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800B252: 1843     orrs     r0, r3 */
    r0 = r0 | r3;
    /* 0x0800B254: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800B256: a807     lsls     r0, r5, #0x1e */
    r0 = r5 << 0x1e;
    /* 0x0800B258: 02d1     bne      #0x800b260 */
    if (r7 != 2) goto L_0800B260;
    /* 0x0800B25A: 3049     ldr      r1, [pc, #0xc0]  ; =RAM_200001C4 */
    r1 = *(uint32_t *)0x0800B31C;   /* = RAM 0x200001C4 */
    /* 0x0800B25C: 6420     movs     r0, #0x64 */
    r0 = 0x64;
    /* 0x0800B25E: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */

    L_0800B260:
    /* 0x0800B260: 01f048fa bl       #0x800c6f4 */
    FUN_0800C6F4();   /* BL -> 0x0800C6F4 FUN_0800C6F4 */
    /* 0x0800B264: ece7     b        #0x800b240 */
    goto L_0800B240;   /* loop */
    /* 0x0800B266: 2c48     ldr      r0, [pc, #0xb0]  ; =RAM_200001BD */
    r0 = *(uint32_t *)0x0800B318;   /* = RAM 0x200001BD */
    /* 0x0800B268: 8255     strb     r2, [r0, r6] */
    *(uint8_t *)(r0 + r6) = r2;   /* store */
    /* 0x0800B26A: 36e0     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */
    /* 0x0800B26C: 2c48     ldr      r0, [pc, #0xb0]  ; =RAM_2000019C */
    r0 = *(uint32_t *)0x0800B320;   /* = RAM 0x2000019C */
    /* 0x0800B26E: 00e0     b        #0x800b272 */
    goto L_0800B272;
    /* 0x0800B270: 2c48     ldr      r0, [pc, #0xb0]  ; =RAM_200001A2 */
    r0 = *(uint32_t *)0x0800B324;   /* = RAM 0x200001A2 */

    L_0800B272:
    /* 0x0800B272: 502a     cmp      r2, #0x50 */
    /* flags = cmp(r2, #0x50) */
    /* 0x0800B274: 8255     strb     r2, [r0, r6] */
    *(uint8_t *)(r0 + r6) = r2;   /* store */
    /* 0x0800B276: 30d9     bls      #0x800b2da */
    if (r2 <= 0x50) goto L_0800B2DA;   /* unsigned */
    /* 0x0800B278: ff2a     cmp      r2, #0xff */
    /* flags = cmp(r2, #0xff) */
    /* 0x0800B27A: 2ed0     beq      #0x800b2da */
    if (r2 == 0xff) goto L_0800B2DA;
    /* 0x0800B27C: 5021     movs     r1, #0x50 */
    r1 = 0x50;
    /* 0x0800B27E: 8155     strb     r1, [r0, r6] */
    *(uint8_t *)(r0 + r6) = r1;   /* store */
    /* 0x0800B280: 2be0     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */
    /* 0x0800B282: 2948     ldr      r0, [pc, #0xa4]  ; =RAM_20000165 */
    r0 = *(uint32_t *)0x0800B328;   /* = RAM 0x20000165 */
    /* 0x0800B284: 5a2a     cmp      r2, #0x5a */
    /* flags = cmp(r2, #0x5a) */
    /* 0x0800B286: 0270     strb     r2, [r0] */
    *(uint8_t *)(r0) = r2;   /* store */
    /* 0x0800B288: 08d9     bls      #0x800b29c */
    if (r2 <= 0x5a) goto L_0800B29C;   /* unsigned */
    /* 0x0800B28A: 5a21     movs     r1, #0x5a */
    r1 = 0x5a;
    /* 0x0800B28C: 04e0     b        #0x800b298 */
    goto L_0800B298;
    /* 0x0800B28E: 2748     ldr      r0, [pc, #0x9c]  ; =RAM_20000166 */
    r0 = *(uint32_t *)0x0800B32C;   /* = RAM 0x20000166 */
    /* 0x0800B290: 632a     cmp      r2, #0x63 */
    /* flags = cmp(r2, #0x63) */
    /* 0x0800B292: 0270     strb     r2, [r0] */
    *(uint8_t *)(r0) = r2;   /* store */
    /* 0x0800B294: 02d9     bls      #0x800b29c */
    if (r2 <= 0x63) goto L_0800B29C;   /* unsigned */
    /* 0x0800B296: 6321     movs     r1, #0x63 */
    r1 = 0x63;

    L_0800B298:
    /* 0x0800B298: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x0800B29A: 1ee0     b        #0x800b2da */
    FUN_0800B2DA();   /* tail call 0x0800B2DA */
}
