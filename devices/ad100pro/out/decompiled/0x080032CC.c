/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080032CC
 * Name      : FUN_080032CC
 * End       : 0x08003396
 * Size      : 202 bytes, 100 instructions decoded in body (functions.json insns=100, blocks=13)
 * Class     : function
 * Callers   : 1  -> 0x0800610C FUN_0800610C
 * Callees   : 1  -> 0x080034A4 FUN_080034A4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080032CC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080032CC: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x080032CE: 1fb4     push     {r0, r1, r2, r3, r4} */
    push({r0, r1, r2, r3, r4});
    /* 0x080032D0: 88b0     sub      sp, #0x20 */
    sp = sp - 0x20;
    /* 0x080032D2: 0b99     ldr      r1, [sp, #0x2c] */
    r1 = *(uint32_t *)(sp + 44);
    /* 0x080032D4: 0998     ldr      r0, [sp, #0x24] */
    r0 = *(uint32_t *)(sp + 36);
    /* 0x080032D6: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x080032D8: 4840     eors     r0, r1 */
    r0 = r0 ^ r1;
    /* 0x080032DA: 0999     ldr      r1, [sp, #0x24] */
    r1 = *(uint32_t *)(sp + 36);
    /* 0x080032DC: c00f     lsrs     r0, r0, #0x1f */
    r0 = r0 >> 0x1f;
    /* 0x080032DE: 4900     lsls     r1, r1, #1 */
    r1 = r1 << 1;
    /* 0x080032E0: 4908     lsrs     r1, r1, #1 */
    r1 = r1 >> 1;
    /* 0x080032E2: 0991     str      r1, [sp, #0x24] */
    *(uint32_t *)(sp + 36) = r1;   /* store */
    /* 0x080032E4: c007     lsls     r0, r0, #0x1f */
    r0 = r0 << 0x1f;
    /* 0x080032E6: 0b99     ldr      r1, [sp, #0x2c] */
    r1 = *(uint32_t *)(sp + 44);
    /* 0x080032E8: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x080032EA: 4900     lsls     r1, r1, #1 */
    r1 = r1 << 1;
    /* 0x080032EC: 4908     lsrs     r1, r1, #1 */
    r1 = r1 >> 1;
    /* 0x080032EE: 0b91     str      r1, [sp, #0x2c] */
    *(uint32_t *)(sp + 44) = r1;   /* store */
    /* 0x080032F0: 0999     ldr      r1, [sp, #0x24] */
    r1 = *(uint32_t *)(sp + 36);
    /* 0x080032F2: 0898     ldr      r0, [sp, #0x20] */
    r0 = *(uint32_t *)(sp + 32);
    /* 0x080032F4: a646     mov      lr, r4 */
    lr = r4;
    /* 0x080032F6: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x080032F8: 1cd0     beq      #0x8003334 */
    if (/* beq */) goto L_08003334;
    /* 0x080032FA: 0b99     ldr      r1, [sp, #0x2c] */
    r1 = *(uint32_t *)(sp + 44);
    /* 0x080032FC: 0a98     ldr      r0, [sp, #0x28] */
    r0 = *(uint32_t *)(sp + 40);
    /* 0x080032FE: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08003300: 18d0     beq      #0x8003334 */
    if (/* beq */) goto L_08003334;
    /* 0x08003302: 0998     ldr      r0, [sp, #0x24] */
    r0 = *(uint32_t *)(sp + 36);
    /* 0x08003304: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x08003306: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x08003308: 420d     lsrs     r2, r0, #0x15 */
    r2 = r0 >> 0x15;
    /* 0x0800330A: 4800     lsls     r0, r1, #1 */
    r0 = r1 << 1;
    /* 0x0800330C: 410d     lsrs     r1, r0, #0x15 */
    r1 = r0 >> 0x15;
    /* 0x0800330E: 0998     ldr      r0, [sp, #0x24] */
    r0 = *(uint32_t *)(sp + 36);
    /* 0x08003310: 2405     lsls     r4, r4, #0x14 */
    r4 = r4 << 0x14;
    /* 0x08003312: 0003     lsls     r0, r0, #0xc */
    r0 = r0 << 0xc;
    /* 0x08003314: 000b     lsrs     r0, r0, #0xc */
    r0 = r0 >> 0xc;
    /* 0x08003316: 2043     orrs     r0, r4 */
    r0 = r0 | r4;
    /* 0x08003318: 0990     str      r0, [sp, #0x24] */
    *(uint32_t *)(sp + 36) = r0;   /* store */
    /* 0x0800331A: 0b98     ldr      r0, [sp, #0x2c] */
    r0 = *(uint32_t *)(sp + 44);
    /* 0x0800331C: 0003     lsls     r0, r0, #0xc */
    r0 = r0 << 0xc;
    /* 0x0800331E: 000b     lsrs     r0, r0, #0xc */
    r0 = r0 >> 0xc;
    /* 0x08003320: 2043     orrs     r0, r4 */
    r0 = r0 | r4;
    /* 0x08003322: 0b90     str      r0, [sp, #0x2c] */
    *(uint32_t *)(sp + 44) = r0;   /* store */
    /* 0x08003324: 5018     adds     r0, r2, r1 */
    r0 = r2 + r1;
    /* 0x08003326: 1c49     ldr      r1, [pc, #0x70]  ; =0xFFFFFC0D */
    r1 = *(uint32_t *)0x08003398;   /* = 0xFFFFFC0D */
    /* 0x08003328: 4018     adds     r0, r0, r1 */
    r0 = r0 + r1;
    /* 0x0800332A: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800332C: 0190     str      r0, [sp, #4] */
    *(uint32_t *)(sp + 4) = r0;   /* store */
    /* 0x0800332E: 1446     mov      r4, r2 */
    r4 = r2;
    /* 0x08003330: 1046     mov      r0, r2 */
    r0 = r2;
    /* 0x08003332: 05e0     b        #0x8003340 */
    goto L_08003340;

    L_08003334:
    /* 0x08003334: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003336: 0146     mov      r1, r0 */
    r1 = r0;

    L_08003338:
    /* 0x08003338: 0db0     add      sp, #0x34 */
    sp = sp + 0x34;
    /* 0x0800333A: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */

    L_0800333C:
    /* 0x0800333C: 0328     cmp      r0, #3 */
    /* flags = cmp(r0, #3) */
    /* 0x0800333E: 01da     bge      #0x8003344 */
    if (r0 >= 3) goto L_08003344;   /* signed */

    L_08003340:
    /* 0x08003340: 0346     mov      r3, r0 */
    r3 = r0;
    /* 0x08003342: 00e0     b        #0x8003346 */
    goto L_08003346;

    L_08003344:
    /* 0x08003344: 0323     movs     r3, #3 */
    r3 = 3;

    L_08003346:
    /* 0x08003346: 9c46     mov      ip, r3 */
    ip = r3;
    /* 0x08003348: c11a     subs     r1, r0, r3 */
    r1 = r0 - r3;
    /* 0x0800334A: 0aae     add      r6, sp, #0x28 */
    r6 = sp + 0x28;
    /* 0x0800334C: 0ae0     b        #0x8003364 */
    goto L_08003364;

    L_0800334E:
    /* 0x0800334E: 5f00     lsls     r7, r3, #1 */
    r7 = r3 << 1;
    /* 0x08003350: 08ad     add      r5, sp, #0x20 */
    r5 = sp + 0x20;
    /* 0x08003352: ed5b     ldrh     r5, [r5, r7] */
    r5 = *(uint16_t *)(r5 + r7);
    /* 0x08003354: 4f00     lsls     r7, r1, #1 */
    r7 = r1 << 1;
    /* 0x08003356: f75b     ldrh     r7, [r6, r7] */
    r7 = *(uint16_t *)(r6 + r7);
    /* 0x08003358: 7d43     muls     r5, r7, r5 */
    r5 = r7 * r5;
    /* 0x0800335A: 0027     movs     r7, #0 */
    r7 = 0;
    /* 0x0800335C: aa18     adds     r2, r5, r2 */
    r2 = r5 + r2;
    /* 0x0800335E: 7c41     adcs     r4, r7 */
    r4 = r4 + r7;
    /* 0x08003360: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x08003362: 5b1e     subs     r3, r3, #1 */
    r3 = r3 - 1;

    L_08003364:
    /* 0x08003364: 6145     cmp      r1, ip */
    /* flags = cmp(r1, ip) */
    /* 0x08003366: f2dd     ble      #0x800334e */
    if (r1 <= ip) goto L_0800334E;   /* signed */
    /* 0x08003368: 4100     lsls     r1, r0, #1 */
    r1 = r0 << 1;
    /* 0x0800336A: 04ab     add      r3, sp, #0x10 */
    r3 = sp + 0x10;
    /* 0x0800336C: 5a52     strh     r2, [r3, r1] */
    *(uint16_t *)(r3 + r1) = r2;   /* store */
    /* 0x0800336E: 2104     lsls     r1, r4, #0x10 */
    r1 = r4 << 0x10;
    /* 0x08003370: 120c     lsrs     r2, r2, #0x10 */
    r2 = r2 >> 0x10;
    /* 0x08003372: 0a43     orrs     r2, r1 */
    r2 = r2 | r1;
    /* 0x08003374: 240c     lsrs     r4, r4, #0x10 */
    r4 = r4 >> 0x10;
    /* 0x08003376: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08003378: 0828     cmp      r0, #8 */
    /* flags = cmp(r0, #8) */
    /* 0x0800337A: dfdb     blt      #0x800333c */
    if (r0 < 8) goto L_0800333C;   /* signed */
    /* 0x0800337C: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x0800337E: 0290     str      r0, [sp, #8] */
    *(uint32_t *)(sp + 8) = r0;   /* store */
    /* 0x08003380: 0099     ldr      r1, [sp] */
    r1 = *(uint32_t *)(sp);
    /* 0x08003382: 7046     mov      r0, lr */
    r0 = lr;
    /* 0x08003384: 0191     str      r1, [sp, #4] */
    *(uint32_t *)(sp + 4) = r1;   /* store */
    /* 0x08003386: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08003388: 059b     ldr      r3, [sp, #0x14] */
    r3 = *(uint32_t *)(sp + 20);
    /* 0x0800338A: 049a     ldr      r2, [sp, #0x10] */
    r2 = *(uint32_t *)(sp + 16);
    /* 0x0800338C: 0799     ldr      r1, [sp, #0x1c] */
    r1 = *(uint32_t *)(sp + 28);
    /* 0x0800338E: 0698     ldr      r0, [sp, #0x18] */
    r0 = *(uint32_t *)(sp + 24);
    /* 0x08003390: 00f088f8 bl       #0x80034a4 */
    FUN_080034A4();   /* BL -> 0x080034A4 FUN_080034A4 */
    /* 0x08003394: d0e7     b        #0x8003338 */
    goto L_08003338;   /* loop */
}
