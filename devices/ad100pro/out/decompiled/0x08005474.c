/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005474
 * Name      : FUN_08005474
 * End       : 0x080054D6
 * Size      : 98 bytes, 45 instructions decoded in body (functions.json insns=45, blocks=9)
 * Class     : function
 * Callers   : 2  -> 0x0800C2B8 FUN_0800C2B8, 0x0800C338 FUN_0800C338
 * Callees   : 2  -> 0x08004DC0 FUN_08004DC0, 0x08005190 FUN_08005190
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005474(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005474: 30b5     push     {r4, r5, lr} */
    push({r4, r5, lr});
    /* 0x08005476: 1848     ldr      r0, [pc, #0x60]  ; =RAM_2000016A */
    r0 = *(uint32_t *)0x080054D8;   /* = RAM 0x2000016A */
    /* 0x08005478: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800547A: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800547C: 1cd0     beq      #0x80054b8 */
    if (r0 == 0) goto L_080054B8;
    /* 0x0800547E: 1748     ldr      r0, [pc, #0x5c]  ; =RAM_20000169 */
    r0 = *(uint32_t *)0x080054DC;   /* = RAM 0x20000169 */
    /* 0x08005480: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08005482: 0928     cmp      r0, #9 */
    /* flags = cmp(r0, #9) */
    /* 0x08005484: 1fd0     beq      #0x80054c6 */
    if (r0 == 9) goto L_080054C6;
    /* 0x08005486: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08005488: c2b2     uxtb     r2, r0 */
    r2 = uxtb(r0);
    /* 0x0800548A: 3b21     movs     r1, #0x3b */
    r1 = 0x3b;

    L_0800548C:
    /* 0x0800548C: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x0800548E: fff797fc bl       #0x8004dc0 */
    FUN_08004DC0();   /* BL -> 0x08004DC0 FUN_08004DC0 */
    /* 0x08005492: 6d21     movs     r1, #0x6d */
    r1 = 0x6d;
    /* 0x08005494: 0620     movs     r0, #6 */
    r0 = 6;
    /* 0x08005496: fff77bfe bl       #0x8005190 */
    FUN_08005190();   /* BL -> 0x08005190 FUN_08005190 */
    /* 0x0800549A: 0323     movs     r3, #3 */
    r3 = 3;
    /* 0x0800549C: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800549E: 1049     ldr      r1, [pc, #0x40]  ; =RAM_200004CC */
    r1 = *(uint32_t *)0x080054E0;   /* = RAM 0x200004CC */
    /* 0x080054A0: 1b02     lsls     r3, r3, #8 */
    r3 = r3 << 8;
    /* 0x080054A2: 8024     movs     r4, #0x80 */
    r4 = 0x80;

    L_080054A4:
    /* 0x080054A4: 0a18     adds     r2, r1, r0 */
    r2 = r1 + r0;
    /* 0x080054A6: d218     adds     r2, r2, r3 */
    r2 = r2 + r3;
    /* 0x080054A8: 1578     ldrb     r5, [r2] */
    r5 = *(uint8_t *)(r2);
    /* 0x080054AA: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x080054AC: 2543     orrs     r5, r4 */
    r5 = r5 | r4;
    /* 0x080054AE: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x080054B0: 1570     strb     r5, [r2] */
    *(uint8_t *)(r2) = r5;   /* store */
    /* 0x080054B2: 8028     cmp      r0, #0x80 */
    /* flags = cmp(r0, #0x80) */
    /* 0x080054B4: f6d3     blo      #0x80054a4 */
    if (r0 < 0x80) goto L_080054A4;   /* unsigned */
    /* 0x080054B6: 30bd     pop      {r4, r5, pc} */
    pop({r4, r5, pc});   /* return */

    L_080054B8:
    /* 0x080054B8: 0a22     movs     r2, #0xa */
    r2 = 0xa;
    /* 0x080054BA: 3221     movs     r1, #0x32 */
    r1 = 0x32;
    /* 0x080054BC: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x080054BE: fff77ffc bl       #0x8004dc0 */
    FUN_08004DC0();   /* BL -> 0x08004DC0 FUN_08004DC0 */
    /* 0x080054C2: 0a22     movs     r2, #0xa */
    r2 = 0xa;
    /* 0x080054C4: 05e0     b        #0x80054d2 */
    goto L_080054D2;

    L_080054C6:
    /* 0x080054C6: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x080054C8: 3221     movs     r1, #0x32 */
    r1 = 0x32;
    /* 0x080054CA: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x080054CC: fff778fc bl       #0x8004dc0 */
    FUN_08004DC0();   /* BL -> 0x08004DC0 FUN_08004DC0 */
    /* 0x080054D0: 0022     movs     r2, #0 */
    r2 = 0;

    L_080054D2:
    /* 0x080054D2: 4121     movs     r1, #0x41 */
    r1 = 0x41;
    /* 0x080054D4: dae7     b        #0x800548c */
    goto L_0800548C;   /* loop */
}
