/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004E98
 * Name      : FUN_08004E98
 * End       : 0x08004EDC
 * Size      : 68 bytes, 34 instructions decoded in body (functions.json insns=34, blocks=4)
 * Class     : hub
 * Callers   : 13  -> 0x08004C6C FUN_08004C6C, 0x08004C92 FUN_08004C92, 0x08005400 FUN_08005400, 0x08005424 FUN_08005424, 0x080054E4 FUN_080054E4, 0x08005536 FUN_08005536, 0x08005546 FUN_08005546, 0x08005552 FUN_08005552, ... +5
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004E98(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004E98: ffb5     push     {r0, r1, r2, r3, r4, r5, r6, r7, lr} */
    push({r0, r1, r2, r3, r4, r5, r6, r7, lr});
    /* 0x08004E9A: 8446     mov      ip, r0 */
    ip = r0;
    /* 0x08004E9C: 8e46     mov      lr, r1 */
    lr = r1;
    /* 0x08004E9E: d000     lsls     r0, r2, #3 */
    r0 = r2 << 3;
    /* 0x08004EA0: 1718     adds     r7, r2, r0 */
    r7 = r2 + r0;
    /* 0x08004EA2: 0e49     ldr      r1, [pc, #0x38]  ; =flash_0800DE6C */
    r1 = *(uint32_t *)0x08004EDC;   /* = flash data/const 0x0800DE6C */
    /* 0x08004EA4: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x08004EA6: ce5d     ldrb     r6, [r1, r7] */
    r6 = *(uint8_t *)(r1 + r7);
    /* 0x08004EA8: 15e0     b        #0x8004ed6 */
    goto L_08004ED6;

    L_08004EAA:
    /* 0x08004EAA: 7818     adds     r0, r7, r1 */
    r0 = r7 + r1;
    /* 0x08004EAC: c018     adds     r0, r0, r3 */
    r0 = r0 + r3;
    /* 0x08004EAE: 4278     ldrb     r2, [r0, #1] */
    r2 = *(uint8_t *)(r0 + 1);
    /* 0x08004EB0: 0398     ldr      r0, [sp, #0xc] */
    r0 = *(uint32_t *)(sp + 12);
    /* 0x08004EB2: 0b4c     ldr      r4, [pc, #0x2c]  ; =RAM_200004CC */
    r4 = *(uint32_t *)0x08004EE0;   /* = RAM 0x200004CC */
    /* 0x08004EB4: 8240     lsls     r2, r0 */
    r2 = r2 << r0;
    /* 0x08004EB6: 6046     mov      r0, ip */
    r0 = ip;
    /* 0x08004EB8: c501     lsls     r5, r0, #7 */
    r5 = r0 << 7;
    /* 0x08004EBA: 2c19     adds     r4, r5, r4 */
    r4 = r5 + r4;
    /* 0x08004EBC: 7046     mov      r0, lr */
    r0 = lr;
    /* 0x08004EBE: c518     adds     r5, r0, r3 */
    r5 = r0 + r3;
    /* 0x08004EC0: 605d     ldrb     r0, [r4, r5] */
    r0 = *(uint8_t *)(r4 + r5);
    /* 0x08004EC2: 1043     orrs     r0, r2 */
    r0 = r0 | r2;
    /* 0x08004EC4: 6055     strb     r0, [r4, r5] */
    *(uint8_t *)(r4 + r5) = r0;   /* store */
    /* 0x08004EC6: 6419     adds     r4, r4, r5 */
    r4 = r4 + r5;
    /* 0x08004EC8: 803c     subs     r4, #0x80 */
    r4 = r4 - 0x80;
    /* 0x08004ECA: 2578     ldrb     r5, [r4] */
    r5 = *(uint8_t *)(r4);
    /* 0x08004ECC: 120a     lsrs     r2, r2, #8 */
    r2 = r2 >> 8;
    /* 0x08004ECE: 1543     orrs     r5, r2 */
    r5 = r5 | r2;
    /* 0x08004ED0: 5b1c     adds     r3, r3, #1 */
    r3 = r3 + 1;
    /* 0x08004ED2: 2570     strb     r5, [r4] */
    *(uint8_t *)(r4) = r5;   /* store */
    /* 0x08004ED4: dbb2     uxtb     r3, r3 */
    r3 = uxtb(r3);

    L_08004ED6:
    /* 0x08004ED6: b342     cmp      r3, r6 */
    /* flags = cmp(r3, r6) */
    /* 0x08004ED8: e7d3     blo      #0x8004eaa */
    if (r3 < r6) goto L_08004EAA;   /* unsigned */
    /* 0x08004EDA: ffbd     pop      {r0, r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r0, r1, r2, r3, r4, r5, r6, r7, pc});   /* return */
}
