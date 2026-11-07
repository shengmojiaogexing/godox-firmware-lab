/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006014
 * Name      : FUN_08006014
 * End       : 0x080060C6
 * Size      : 178 bytes, 82 instructions decoded in body (functions.json insns=82, blocks=20)
 * Class     : function
 * Callers   : 6  -> 0x08004AA8 FUN_08004AA8, 0x08005A78 IRQ5_Handler, 0x08005AB4 FUN_08005AB4, 0x08005B0C IRQ7_Handler, 0x08005B42 FUN_08005B42, 0x0800C068 FUN_0800C068
 * Callees   : 5  -> 0x08005A24 FUN_08005A24, 0x08005F84 FUN_08005F84, 0x080091C4 FUN_080091C4, 0x08009CB4 FUN_08009CB4, 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006014(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006014: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08006016: 2c48     ldr      r0, [pc, #0xb0]  ; =RAM_20000134 */
    r0 = *(uint32_t *)0x080060C8;   /* = RAM 0x20000134 */
    /* 0x08006018: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x0800601A: 4472     strb     r4, [r0, #9] */
    *(uint8_t *)(r0 + 9) = r4;   /* store */
    /* 0x0800601C: 2b48     ldr      r0, [pc, #0xac]  ; =RAM_2000015E */
    r0 = *(uint32_t *)0x080060CC;   /* = RAM 0x2000015E */
    /* 0x0800601E: 0088     ldrh     r0, [r0] */
    r0 = *(uint16_t *)(r0);
    /* 0x08006020: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08006022: 02d0     beq      #0x800602a */
    if (r0 == 0) goto L_0800602A;
    /* 0x08006024: fff7aeff bl       #0x8005f84 */
    FUN_08005F84();   /* BL -> 0x08005F84 FUN_08005F84 */
    /* 0x08006028: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800602A:
    /* 0x0800602A: 2948     ldr      r0, [pc, #0xa4]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x080060D0;   /* = RAM 0x20000175 */
    /* 0x0800602C: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800602E: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x08006030: 0bd0     beq      #0x800604a */
    if (r0 == 0) goto L_0800604A;
    /* 0x08006032: 2849     ldr      r1, [pc, #0xa0]  ; =RAM_20000480 */
    r1 = *(uint32_t *)0x080060D4;   /* = RAM 0x20000480 */
    /* 0x08006034: c978     ldrb     r1, [r1, #3] */
    r1 = *(uint8_t *)(r1 + 3);
    /* 0x08006036: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x08006038: 07d0     beq      #0x800604a */
    if (r1 == 0) goto L_0800604A;
    /* 0x0800603A: 274a     ldr      r2, [pc, #0x9c]  ; =RAM_200001AB */
    r2 = *(uint32_t *)0x080060D8;   /* = RAM 0x200001AB */
    /* 0x0800603C: 1278     ldrb     r2, [r2] */
    r2 = *(uint8_t *)(r2);
    /* 0x0800603E: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x08006040: 03d1     bne      #0x800604a */
    if (r2 != 0) goto L_0800604A;
    /* 0x08006042: 264a     ldr      r2, [pc, #0x98]  ; =RAM_200001B3 */
    r2 = *(uint32_t *)0x080060DC;   /* = RAM 0x200001B3 */
    /* 0x08006044: 1278     ldrb     r2, [r2] */
    r2 = *(uint8_t *)(r2);
    /* 0x08006046: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x08006048: 04d0     beq      #0x8006054 */
    if (r2 == 0) goto L_08006054;

    L_0800604A:
    /* 0x0800604A: 2549     ldr      r1, [pc, #0x94]  ; =RAM_200001B8 */
    r1 = *(uint32_t *)0x080060E0;   /* = RAM 0x200001B8 */
    /* 0x0800604C: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800604E: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x08006050: 17d0     beq      #0x8006082 */
    if (r1 == 1) goto L_08006082;
    /* 0x08006052: 2be0     b        #0x80060ac */
    goto L_080060AC;

    L_08006054:
    /* 0x08006054: 2348     ldr      r0, [pc, #0x8c]  ; =flash_0800F2AA */
    r0 = *(uint32_t *)0x080060E4;   /* = flash data/const 0x0800F2AA */
    /* 0x08006056: 4900     lsls     r1, r1, #1 */
    r1 = r1 << 1;
    /* 0x08006058: 405a     ldrh     r0, [r0, r1] */
    r0 = *(uint16_t *)(r0 + r1);
    /* 0x0800605A: 3221     movs     r1, #0x32 */
    r1 = 0x32;
    /* 0x0800605C: 4143     muls     r1, r0, r1 */
    r1 = r0 * r1;
    /* 0x0800605E: 224a     ldr      r2, [pc, #0x88]  ; =RAM_20000198 */
    r2 = *(uint32_t *)0x080060E8;   /* = RAM 0x20000198 */
    /* 0x08006060: 3228     cmp      r0, #0x32 */
    /* flags = cmp(r0, #0x32) */
    /* 0x08006062: 1160     str      r1, [r2] */
    *(uint32_t *)(r2) = r1;   /* store */
    /* 0x08006064: 02d3     blo      #0x800606c */
    if (r0 < 0x32) goto L_0800606C;   /* unsigned */
    /* 0x08006066: 2149     ldr      r1, [pc, #0x84]  ; =RAM_20000164 */
    r1 = *(uint32_t *)0x080060EC;   /* = RAM 0x20000164 */
    /* 0x08006068: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800606A: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */

    L_0800606C:
    /* 0x0800606C: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800606E: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x08006072: 7d20     movs     r0, #0x7d */
    r0 = 0x7d;
    /* 0x08006074: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x08006076: 06f0dffc bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800607A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800607C: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x08006080: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_08006082:
    /* 0x08006082: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x08006084: 15d0     beq      #0x80060b2 */
    if (r0 == 0x32) goto L_080060B2;
    /* 0x08006086: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x08006088: 1948     ldr      r0, [pc, #0x64]  ; =RAM_2000017D */
    r0 = *(uint32_t *)0x080060F0;   /* = RAM 0x2000017D */
    /* 0x0800608A: 890f     lsrs     r1, r1, #0x1e */
    r1 = r1 >> 0x1e;
    /* 0x0800608C: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800608E: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x08006090: 12d0     beq      #0x80060b8 */
    if (r1 == 1) goto L_080060B8;
    /* 0x08006092: 1849     ldr      r1, [pc, #0x60]  ; =RAM_200001A2 */
    r1 = *(uint32_t *)0x080060F4;   /* = RAM 0x200001A2 */
    /* 0x08006094: 085c     ldrb     r0, [r1, r0] */
    r0 = *(uint8_t *)(r1 + r0);
    /* 0x08006096: ff28     cmp      r0, #0xff */
    /* flags = cmp(r0, #0xff) */
    /* 0x08006098: 01d0     beq      #0x800609e */
    if (r0 == 0xff) goto L_0800609E;
    /* 0x0800609A: 03f00bfe bl       #0x8009cb4 */
    FUN_08009CB4();   /* BL -> 0x08009CB4 FUN_08009CB4 */

    L_0800609E:
    /* 0x0800609E: 1649     ldr      r1, [pc, #0x58]  ; =RAM_200001CC */
    r1 = *(uint32_t *)0x080060F8;   /* = RAM 0x200001CC */
    /* 0x080060A0: 1e20     movs     r0, #0x1e */
    r0 = 0x1e;
    /* 0x080060A2: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x080060A4: 1548     ldr      r0, [pc, #0x54]  ; =RAM_200001BC */
    r0 = *(uint32_t *)0x080060FC;   /* = RAM 0x200001BC */
    /* 0x080060A6: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */
    /* 0x080060A8: 1548     ldr      r0, [pc, #0x54]  ; =RAM_200001B2 */
    r0 = *(uint32_t *)0x08006100;   /* = RAM 0x200001B2 */
    /* 0x080060AA: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */

    L_080060AC:
    /* 0x080060AC: 1548     ldr      r0, [pc, #0x54]  ; =RAM_20000182 */
    r0 = *(uint32_t *)0x08006104;   /* = RAM 0x20000182 */
    /* 0x080060AE: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */
    /* 0x080060B0: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_080060B2:
    /* 0x080060B2: fff7b7fc bl       #0x8005a24 */
    FUN_08005A24();   /* BL -> 0x08005A24 FUN_08005A24 */
    /* 0x080060B6: f2e7     b        #0x800609e */
    goto L_0800609E;   /* loop */

    L_080060B8:
    /* 0x080060B8: 1349     ldr      r1, [pc, #0x4c]  ; =RAM_2000019C */
    r1 = *(uint32_t *)0x08006108;   /* = RAM 0x2000019C */
    /* 0x080060BA: 085c     ldrb     r0, [r1, r0] */
    r0 = *(uint8_t *)(r1 + r0);
    /* 0x080060BC: ff28     cmp      r0, #0xff */
    /* flags = cmp(r0, #0xff) */
    /* 0x080060BE: eed0     beq      #0x800609e */
    if (r0 == 0xff) goto L_0800609E;
    /* 0x080060C0: 03f080f8 bl       #0x80091c4 */
    FUN_080091C4();   /* BL -> 0x080091C4 FUN_080091C4 */
    /* 0x080060C4: ebe7     b        #0x800609e */
    goto L_0800609E;   /* loop */
}
