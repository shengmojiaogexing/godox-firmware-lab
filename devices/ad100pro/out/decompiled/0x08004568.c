/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004568
 * Name      : FUN_08004568
 * End       : 0x080045D4
 * Size      : 108 bytes, 53 instructions decoded in body (functions.json insns=53, blocks=15)
 * Class     : function
 * Callers   : 1  -> 0x0800642C FUN_0800642C
 * Callees   : 1  -> 0x0800741C FUN_0800741C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004568(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004568: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800456A: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x0800456C: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800456E: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x08004570: 02f054ff bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x08004574: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x08004576: 1749     ldr      r1, [pc, #0x5c]  ; =RAM_2000014A */
    r1 = *(uint32_t *)0x080045D4;   /* = RAM 0x2000014A */
    /* 0x08004578: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800457A: 0ad1     bne      #0x8004592 */
    if (r0 != 0) goto L_08004592;
    /* 0x0800457C: 8878     ldrb     r0, [r1, #2] */
    r0 = *(uint8_t *)(r1 + 2);
    /* 0x0800457E: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08004580: 07d1     bne      #0x8004592 */
    if (r0 != 1) goto L_08004592;
    /* 0x08004582: 154c     ldr      r4, [pc, #0x54]  ; =RAM_200001CF */
    r4 = *(uint32_t *)0x080045D8;   /* = RAM 0x200001CF */
    /* 0x08004584: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x08004586: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08004588: 03d1     bne      #0x8004592 */
    if (r0 != 0) goto L_08004592;
    /* 0x0800458A: 1448     ldr      r0, [pc, #0x50]  ; =RAM_200001B8 */
    r0 = *(uint32_t *)0x080045DC;   /* = RAM 0x200001B8 */
    /* 0x0800458C: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800458E: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08004590: 01d0     beq      #0x8004596 */
    if (r0 == 0) goto L_08004596;

    L_08004592:
    /* 0x08004592: 4b72     strb     r3, [r1, #9] */
    *(uint8_t *)(r1 + 9) = r3;   /* store */
    /* 0x08004594: 1ce0     b        #0x80045d0 */
    goto L_080045D0;

    L_08004596:
    /* 0x08004596: 4d7a     ldrb     r5, [r1, #9] */
    r5 = *(uint8_t *)(r1 + 9);
    /* 0x08004598: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800459A: 114a     ldr      r2, [pc, #0x44]  ; =RAM_200001B6 */
    r2 = *(uint32_t *)0x080045E0;   /* = RAM 0x200001B6 */
    /* 0x0800459C: 002d     cmp      r5, #0 */
    /* flags = cmp(r5, #0) */
    /* 0x0800459E: 11d0     beq      #0x80045c4 */
    if (r5 == 0) goto L_080045C4;
    /* 0x080045A0: 6d1c     adds     r5, r5, #1 */
    r5 = r5 + 1;
    /* 0x080045A2: edb2     uxtb     r5, r5 */
    r5 = uxtb(r5);
    /* 0x080045A4: 4d72     strb     r5, [r1, #9] */
    *(uint8_t *)(r1 + 9) = r5;   /* store */
    /* 0x080045A6: 642d     cmp      r5, #0x64 */
    /* flags = cmp(r5, #0x64) */
    /* 0x080045A8: 0bd9     bls      #0x80045c2 */
    if (r5 <= 0x64) goto L_080045C2;   /* unsigned */
    /* 0x080045AA: 0d8b     ldrh     r5, [r1, #0x18] */
    r5 = *(uint16_t *)(r1 + 24);
    /* 0x080045AC: 1288     ldrh     r2, [r2] */
    r2 = *(uint16_t *)(r2);
    /* 0x080045AE: 0a35     adds     r5, #0xa */
    r5 = r5 + 0xa;
    /* 0x080045B0: aa42     cmp      r2, r5 */
    /* flags = cmp(r2, r5) */
    /* 0x080045B2: 0bd2     bhs      #0x80045cc */
    if (r2 >= r5) goto L_080045CC;   /* unsigned */
    /* 0x080045B4: 8b7a     ldrb     r3, [r1, #0xa] */
    r3 = *(uint8_t *)(r1 + 10);
    /* 0x080045B6: 5b1c     adds     r3, r3, #1 */
    r3 = r3 + 1;
    /* 0x080045B8: dbb2     uxtb     r3, r3 */
    r3 = uxtb(r3);
    /* 0x080045BA: 8b72     strb     r3, [r1, #0xa] */
    *(uint8_t *)(r1 + 10) = r3;   /* store */
    /* 0x080045BC: 042b     cmp      r3, #4 */
    /* flags = cmp(r3, #4) */
    /* 0x080045BE: 02d3     blo      #0x80045c6 */
    if (r3 < 4) goto L_080045C6;   /* unsigned */
    /* 0x080045C0: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */

    L_080045C2:
    /* 0x080045C2: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_080045C4:
    /* 0x080045C4: 1288     ldrh     r2, [r2] */
    r2 = *(uint16_t *)(r2);

    L_080045C6:
    /* 0x080045C6: 0a83     strh     r2, [r1, #0x18] */
    *(uint16_t *)(r1 + 24) = r2;   /* store */
    /* 0x080045C8: 4872     strb     r0, [r1, #9] */
    *(uint8_t *)(r1 + 9) = r0;   /* store */
    /* 0x080045CA: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_080045CC:
    /* 0x080045CC: 0a83     strh     r2, [r1, #0x18] */
    *(uint16_t *)(r1 + 24) = r2;   /* store */
    /* 0x080045CE: 4872     strb     r0, [r1, #9] */
    *(uint8_t *)(r1 + 9) = r0;   /* store */

    L_080045D0:
    /* 0x080045D0: 8b72     strb     r3, [r1, #0xa] */
    *(uint8_t *)(r1 + 10) = r3;   /* store */
    /* 0x080045D2: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
