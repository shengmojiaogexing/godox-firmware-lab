/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C6F4
 * Name      : FUN_0800C6F4
 * End       : 0x0800C798
 * Size      : 164 bytes, 76 instructions decoded in body (functions.json insns=76, blocks=19)
 * Class     : function
 * Callers   : 7  -> 0x08004AA8 FUN_08004AA8, 0x0800AA74 FUN_0800AA74, 0x0800AAFC FUN_0800AAFC, 0x0800B03C FUN_0800B03C, 0x0800B0BE FUN_0800B0BE, 0x0800B24A FUN_0800B24A, 0x0800B24E FUN_0800B24E
 * Callees   : 4  -> 0x08005F84 FUN_08005F84, 0x0800C4E8 FUN_0800C4E8, 0x0800C674 FUN_0800C674, 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C6F4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C6F4: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800C6F6: 2848     ldr      r0, [pc, #0xa0]  ; =RAM_2000015E */
    r0 = *(uint32_t *)0x0800C798;   /* = RAM 0x2000015E */
    /* 0x0800C6F8: 0088     ldrh     r0, [r0] */
    r0 = *(uint16_t *)(r0);
    /* 0x0800C6FA: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800C6FC: 02d0     beq      #0x800c704 */
    if (r0 == 0) goto L_0800C704;
    /* 0x0800C6FE: f9f741fc bl       #0x8005f84 */
    FUN_08005F84();   /* BL -> 0x08005F84 FUN_08005F84 */
    /* 0x0800C702: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800C704:
    /* 0x0800C704: 2548     ldr      r0, [pc, #0x94]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800C79C;   /* = RAM 0x20000175 */
    /* 0x0800C706: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800C708: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x0800C70A: 0bd0     beq      #0x800c724 */
    if (r0 == 0) goto L_0800C724;
    /* 0x0800C70C: 2449     ldr      r1, [pc, #0x90]  ; =RAM_20000480 */
    r1 = *(uint32_t *)0x0800C7A0;   /* = RAM 0x20000480 */
    /* 0x0800C70E: c978     ldrb     r1, [r1, #3] */
    r1 = *(uint8_t *)(r1 + 3);
    /* 0x0800C710: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x0800C712: 07d0     beq      #0x800c724 */
    if (r1 == 0) goto L_0800C724;
    /* 0x0800C714: 234a     ldr      r2, [pc, #0x8c]  ; =RAM_200001AB */
    r2 = *(uint32_t *)0x0800C7A4;   /* = RAM 0x200001AB */
    /* 0x0800C716: 1278     ldrb     r2, [r2] */
    r2 = *(uint8_t *)(r2);
    /* 0x0800C718: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x0800C71A: 03d1     bne      #0x800c724 */
    if (r2 != 0) goto L_0800C724;
    /* 0x0800C71C: 224a     ldr      r2, [pc, #0x88]  ; =RAM_200001B3 */
    r2 = *(uint32_t *)0x0800C7A8;   /* = RAM 0x200001B3 */
    /* 0x0800C71E: 1278     ldrb     r2, [r2] */
    r2 = *(uint8_t *)(r2);
    /* 0x0800C720: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x0800C722: 04d0     beq      #0x800c72e */
    if (r2 == 0) goto L_0800C72E;

    L_0800C724:
    /* 0x0800C724: 2149     ldr      r1, [pc, #0x84]  ; =RAM_200001B8 */
    r1 = *(uint32_t *)0x0800C7AC;   /* = RAM 0x200001B8 */
    /* 0x0800C726: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800C728: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x0800C72A: 17d0     beq      #0x800c75c */
    if (r1 == 1) goto L_0800C75C;
    /* 0x0800C72C: 27e0     b        #0x800c77e */
    goto L_0800C77E;

    L_0800C72E:
    /* 0x0800C72E: 2048     ldr      r0, [pc, #0x80]  ; =flash_0800F2AA */
    r0 = *(uint32_t *)0x0800C7B0;   /* = flash data/const 0x0800F2AA */
    /* 0x0800C730: 4900     lsls     r1, r1, #1 */
    r1 = r1 << 1;
    /* 0x0800C732: 405a     ldrh     r0, [r0, r1] */
    r0 = *(uint16_t *)(r0 + r1);
    /* 0x0800C734: 3221     movs     r1, #0x32 */
    r1 = 0x32;
    /* 0x0800C736: 4143     muls     r1, r0, r1 */
    r1 = r0 * r1;
    /* 0x0800C738: 1e4a     ldr      r2, [pc, #0x78]  ; =RAM_20000198 */
    r2 = *(uint32_t *)0x0800C7B4;   /* = RAM 0x20000198 */
    /* 0x0800C73A: 3228     cmp      r0, #0x32 */
    /* flags = cmp(r0, #0x32) */
    /* 0x0800C73C: 1160     str      r1, [r2] */
    *(uint32_t *)(r2) = r1;   /* store */
    /* 0x0800C73E: 02d3     blo      #0x800c746 */
    if (r0 < 0x32) goto L_0800C746;   /* unsigned */
    /* 0x0800C740: 1d49     ldr      r1, [pc, #0x74]  ; =RAM_20000164 */
    r1 = *(uint32_t *)0x0800C7B8;   /* = RAM 0x20000164 */
    /* 0x0800C742: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800C744: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */

    L_0800C746:
    /* 0x0800C746: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800C748: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800C74C: 7d20     movs     r0, #0x7d */
    r0 = 0x7d;
    /* 0x0800C74E: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x0800C750: 00f072f9 bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800C754: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C756: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800C75A: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800C75C:
    /* 0x0800C75C: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x0800C75E: 18d0     beq      #0x800c792 */
    if (r0 == 0x32) goto L_0800C792;
    /* 0x0800C760: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x0800C762: 1648     ldr      r0, [pc, #0x58]  ; =RAM_2000017D */
    r0 = *(uint32_t *)0x0800C7BC;   /* = RAM 0x2000017D */
    /* 0x0800C764: 890f     lsrs     r1, r1, #0x1e */
    r1 = r1 >> 0x1e;
    /* 0x0800C766: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800C768: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x0800C76A: 0ed0     beq      #0x800c78a */
    if (r1 == 1) goto L_0800C78A;
    /* 0x0800C76C: 1449     ldr      r1, [pc, #0x50]  ; =RAM_200001A2 */
    r1 = *(uint32_t *)0x0800C7C0;   /* = RAM 0x200001A2 */
    /* 0x0800C76E: 085c     ldrb     r0, [r1, r0] */
    r0 = *(uint8_t *)(r1 + r0);
    /* 0x0800C770: ff28     cmp      r0, #0xff */
    /* flags = cmp(r0, #0xff) */
    /* 0x0800C772: 01d0     beq      #0x800c778 */
    if (r0 == 0xff) goto L_0800C778;
    /* 0x0800C774: fff77eff bl       #0x800c674 */
    FUN_0800C674();   /* BL -> 0x0800C674 FUN_0800C674 */

    L_0800C778:
    /* 0x0800C778: 1249     ldr      r1, [pc, #0x48]  ; =RAM_200001CC */
    r1 = *(uint32_t *)0x0800C7C4;   /* = RAM 0x200001CC */
    /* 0x0800C77A: 1e20     movs     r0, #0x1e */
    r0 = 0x1e;
    /* 0x0800C77C: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */

    L_0800C77E:
    /* 0x0800C77E: 1249     ldr      r1, [pc, #0x48]  ; =RAM_200001B2 */
    r1 = *(uint32_t *)0x0800C7C8;   /* = RAM 0x200001B2 */
    /* 0x0800C780: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C782: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x0800C784: 1149     ldr      r1, [pc, #0x44]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x0800C7CC;   /* = RAM 0x20000134 */
    /* 0x0800C786: 4872     strb     r0, [r1, #9] */
    *(uint8_t *)(r1 + 9) = r0;   /* store */
    /* 0x0800C788: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800C78A:
    /* 0x0800C78A: 1149     ldr      r1, [pc, #0x44]  ; =RAM_2000019C */
    r1 = *(uint32_t *)0x0800C7D0;   /* = RAM 0x2000019C */
    /* 0x0800C78C: 085c     ldrb     r0, [r1, r0] */
    r0 = *(uint8_t *)(r1 + r0);
    /* 0x0800C78E: ff28     cmp      r0, #0xff */
    /* flags = cmp(r0, #0xff) */
    /* 0x0800C790: f2d0     beq      #0x800c778 */
    if (r0 == 0xff) goto L_0800C778;

    L_0800C792:
    /* 0x0800C792: fff7a9fe bl       #0x800c4e8 */
    FUN_0800C4E8();   /* BL -> 0x0800C4E8 FUN_0800C4E8 */
    /* 0x0800C796: efe7     b        #0x800c778 */
    goto L_0800C778;   /* loop */
}
