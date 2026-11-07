/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080064E8
 * Name      : FUN_080064E8
 * End       : 0x08006554
 * Size      : 108 bytes, 53 instructions decoded in body (functions.json insns=53, blocks=17)
 * Class     : function
 * Callers   : 2  -> 0x080047CC FUN_080047CC, 0x0800486E FUN_0800486E
 * Callees   : 1  -> 0x0800AB7C FUN_0800AB7C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080064E8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080064E8: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x080064EA: 1a48     ldr      r0, [pc, #0x68]  ; =RAM_20000480 */
    r0 = *(uint32_t *)0x08006554;   /* = RAM 0x20000480 */
    /* 0x080064EC: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x080064EE: 0179     ldrb     r1, [r0, #4] */
    r1 = *(uint8_t *)(r0 + 4);
    /* 0x080064F0: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x080064F2: 00d9     bls      #0x80064f6 */
    if (r1 <= 1) goto L_080064F6;   /* unsigned */
    /* 0x080064F4: 0271     strb     r2, [r0, #4] */
    *(uint8_t *)(r0 + 4) = r2;   /* store */

    L_080064F6:
    /* 0x080064F6: 0379     ldrb     r3, [r0, #4] */
    r3 = *(uint8_t *)(r0 + 4);
    /* 0x080064F8: 1748     ldr      r0, [pc, #0x5c]  ; =RAM_20000164 */
    r0 = *(uint32_t *)0x08006558;   /* = RAM 0x20000164 */
    /* 0x080064FA: 017e     ldrb     r1, [r0, #0x18] */
    r1 = *(uint8_t *)(r0 + 24);
    /* 0x080064FC: 8b42     cmp      r3, r1 */
    /* flags = cmp(r3, r1) */
    /* 0x080064FE: 28d0     beq      #0x8006552 */
    if (r3 == r1) goto L_08006552;
    /* 0x08006500: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x08006502: 002b     cmp      r3, #0 */
    /* flags = cmp(r3, #0) */
    /* 0x08006504: 04d0     beq      #0x8006510 */
    if (r3 == 0) goto L_08006510;
    /* 0x08006506: 0276     strb     r2, [r0, #0x18] */
    *(uint8_t *)(r0 + 24) = r2;   /* store */
    /* 0x08006508: 437e     ldrb     r3, [r0, #0x19] */
    r3 = *(uint8_t *)(r0 + 25);
    /* 0x0800650A: 002b     cmp      r3, #0 */
    /* flags = cmp(r3, #0) */
    /* 0x0800650C: 0ad0     beq      #0x8006524 */
    if (r3 == 0) goto L_08006524;
    /* 0x0800650E: 0ae0     b        #0x8006526 */
    goto L_08006526;

    L_08006510:
    /* 0x08006510: 0176     strb     r1, [r0, #0x18] */
    *(uint8_t *)(r0 + 24) = r1;   /* store */
    /* 0x08006512: 4176     strb     r1, [r0, #0x19] */
    *(uint8_t *)(r0 + 25) = r1;   /* store */
    /* 0x08006514: 427c     ldrb     r2, [r0, #0x11] */
    r2 = *(uint8_t *)(r0 + 17);
    /* 0x08006516: 9307     lsls     r3, r2, #0x1e */
    r3 = r2 << 0x1e;
    /* 0x08006518: 05d1     bne      #0x8006526 */
    if (r3 != 0) goto L_08006526;
    /* 0x0800651A: 9208     lsrs     r2, r2, #2 */
    r2 = r2 >> 2;
    /* 0x0800651C: 9200     lsls     r2, r2, #2 */
    r2 = r2 << 2;
    /* 0x0800651E: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x08006520: 4274     strb     r2, [r0, #0x11] */
    *(uint8_t *)(r0 + 17) = r2;   /* store */
    /* 0x08006522: 00e0     b        #0x8006526 */
    goto L_08006526;

    L_08006524:
    /* 0x08006524: 4276     strb     r2, [r0, #0x19] */
    *(uint8_t *)(r0 + 25) = r2;   /* store */

    L_08006526:
    /* 0x08006526: 0c4b     ldr      r3, [pc, #0x30]  ; =RAM_20000164 */
    r3 = *(uint32_t *)0x08006558;   /* = RAM 0x20000164 */
    /* 0x08006528: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800652A: 3833     adds     r3, #0x38 */
    r3 = r3 + 0x38;
    /* 0x0800652C: 5024     movs     r4, #0x50 */
    r4 = 0x50;
    /* 0x0800652E: 9a1d     adds     r2, r3, #6 */
    r2 = r3 + 6;

    L_08006530:
    /* 0x08006530: 1d5c     ldrb     r5, [r3, r0] */
    r5 = *(uint8_t *)(r3 + r0);
    /* 0x08006532: ff2d     cmp      r5, #0xff */
    /* flags = cmp(r5, #0xff) */
    /* 0x08006534: 00d1     bne      #0x8006538 */
    if (r5 != 0xff) goto L_08006538;
    /* 0x08006536: 1c54     strb     r4, [r3, r0] */
    *(uint8_t *)(r3 + r0) = r4;   /* store */

    L_08006538:
    /* 0x08006538: 155c     ldrb     r5, [r2, r0] */
    r5 = *(uint8_t *)(r2 + r0);
    /* 0x0800653A: ff2d     cmp      r5, #0xff */
    /* flags = cmp(r5, #0xff) */
    /* 0x0800653C: 00d1     bne      #0x8006540 */
    if (r5 != 0xff) goto L_08006540;
    /* 0x0800653E: 1454     strb     r4, [r2, r0] */
    *(uint8_t *)(r2 + r0) = r4;   /* store */

    L_08006540:
    /* 0x08006540: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08006542: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08006544: 0628     cmp      r0, #6 */
    /* flags = cmp(r0, #6) */
    /* 0x08006546: f3d3     blo      #0x8006530 */
    if (r0 < 6) goto L_08006530;   /* unsigned */
    /* 0x08006548: 0348     ldr      r0, [pc, #0xc]  ; =RAM_20000164 */
    r0 = *(uint32_t *)0x08006558;   /* = RAM 0x20000164 */
    /* 0x0800654A: 2030     adds     r0, #0x20 */
    r0 = r0 + 0x20;
    /* 0x0800654C: 4170     strb     r1, [r0, #1] */
    *(uint8_t *)(r0 + 1) = r1;   /* store */
    /* 0x0800654E: 04f015fb bl       #0x800ab7c */
    FUN_0800AB7C();   /* BL -> 0x0800AB7C FUN_0800AB7C */

    L_08006552:
    /* 0x08006552: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
