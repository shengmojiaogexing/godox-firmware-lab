/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005CCC
 * Name      : FUN_08005CCC
 * End       : 0x08005D56
 * Size      : 138 bytes, 68 instructions decoded in body (functions.json insns=68, blocks=17)
 * Class     : function
 * Callers   : 2  -> 0x080066C0 FUN_080066C0, 0x080066F4 FUN_080066F4
 * Callees   : 1  -> 0x08006730 FUN_08006730
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005CCC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005CCC: 30b5     push     {r4, r5, lr} */
    push({r4, r5, lr});
    /* 0x08005CCE: 2248     ldr      r0, [pc, #0x88]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x08005D58;   /* = RAM 0x20000175 */
    /* 0x08005CD0: 224d     ldr      r5, [pc, #0x88]  ; =RAM_200001D8 */
    r5 = *(uint32_t *)0x08005D5C;   /* = RAM 0x200001D8 */
    /* 0x08005CD2: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08005CD4: 224c     ldr      r4, [pc, #0x88]  ; =RAM_200001C8 */
    r4 = *(uint32_t *)0x08005D60;   /* = RAM 0x200001C8 */
    /* 0x08005CD6: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x08005CD8: 08d0     beq      #0x8005cec */
    if (/* beq */) goto L_08005CEC;
    /* 0x08005CDA: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08005CDC: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x08005CDE: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08005CE0: 25d0     beq      #0x8005d2e */
    if (r0 == 1) goto L_08005D2E;
    /* 0x08005CE2: 2820     movs     r0, #0x28 */
    r0 = 0x28;
    /* 0x08005CE4: 2880     strh     r0, [r5] */
    *(uint16_t *)(r5) = r0;   /* store */
    /* 0x08005CE6: 1420     movs     r0, #0x14 */
    r0 = 0x14;
    /* 0x08005CE8: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */
    /* 0x08005CEA: 27e0     b        #0x8005d3c */
    goto L_08005D3C;

    L_08005CEC:
    /* 0x08005CEC: 1e49     ldr      r1, [pc, #0x78]  ; =RAM_2000017D */
    r1 = *(uint32_t *)0x08005D68;   /* = RAM 0x2000017D */
    /* 0x08005CEE: 1d48     ldr      r0, [pc, #0x74]  ; =RAM_200001BD */
    r0 = *(uint32_t *)0x08005D64;   /* = RAM 0x200001BD */
    /* 0x08005CF0: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08005CF2: 1822     movs     r2, #0x18 */
    r2 = 0x18;
    /* 0x08005CF4: 405c     ldrb     r0, [r0, r1] */
    r0 = *(uint8_t *)(r0 + r1);
    /* 0x08005CF6: 1d49     ldr      r1, [pc, #0x74]  ; =RAM_200001B1 */
    r1 = *(uint32_t *)0x08005D6C;   /* = RAM 0x200001B1 */
    /* 0x08005CF8: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08005CFA: 511a     subs     r1, r2, r1 */
    r1 = r2 - r1;
    /* 0x08005CFC: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x08005CFE: 8a00     lsls     r2, r1, #2 */
    r2 = r1 << 2;
    /* 0x08005D00: 8918     adds     r1, r1, r2 */
    r1 = r1 + r2;
    /* 0x08005D02: 8908     lsrs     r1, r1, #2 */
    r1 = r1 >> 2;
    /* 0x08005D04: 0818     adds     r0, r1, r0 */
    r0 = r1 + r0;
    /* 0x08005D06: 1128     cmp      r0, #0x11 */
    /* flags = cmp(r0, #0x11) */
    /* 0x08005D08: 00d2     bhs      #0x8005d0c */
    if (r0 >= 0x11) goto L_08005D0C;   /* unsigned */
    /* 0x08005D0A: 1120     movs     r0, #0x11 */
    r0 = 0x11;

    L_08005D0C:
    /* 0x08005D0C: 1138     subs     r0, #0x11 */
    r0 = r0 - 0x11;
    /* 0x08005D0E: 80b2     uxth     r0, r0 */
    r0 = uxth(r0);
    /* 0x08005D10: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x08005D12: 00d9     bls      #0x8005d16 */
    if (r0 <= 0x64) goto L_08005D16;   /* unsigned */
    /* 0x08005D14: 6420     movs     r0, #0x64 */
    r0 = 0x64;

    L_08005D16:
    /* 0x08005D16: 1649     ldr      r1, [pc, #0x58]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x08005D70;   /* = RAM 0x20000134 */
    /* 0x08005D18: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08005D1A: 8870     strb     r0, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r0;   /* store */
    /* 0x08005D1C: 1549     ldr      r1, [pc, #0x54]  ; =flash_0800D4C6 */
    r1 = *(uint32_t *)0x08005D74;   /* = flash data/const 0x0800D4C6 */
    /* 0x08005D1E: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x08005D20: 085a     ldrh     r0, [r1, r0] */
    r0 = *(uint16_t *)(r1 + r0);
    /* 0x08005D22: 8105     lsls     r1, r0, #0x16 */
    r1 = r0 << 0x16;
    /* 0x08005D24: 890d     lsrs     r1, r1, #0x16 */
    r1 = r1 >> 0x16;
    /* 0x08005D26: 2980     strh     r1, [r5] */
    *(uint16_t *)(r5) = r1;   /* store */
    /* 0x08005D28: 800a     lsrs     r0, r0, #0xa */
    r0 = r0 >> 0xa;
    /* 0x08005D2A: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */
    /* 0x08005D2C: 01e0     b        #0x8005d32 */
    goto L_08005D32;

    L_08005D2E:
    /* 0x08005D2E: 00f0fffc bl       #0x8006730 */
    FUN_08006730();   /* BL -> 0x08006730 FUN_08006730 */

    L_08005D32:
    /* 0x08005D32: 2888     ldrh     r0, [r5] */
    r0 = *(uint16_t *)(r5);
    /* 0x08005D34: 0c28     cmp      r0, #0xc */
    /* flags = cmp(r0, #0xc) */
    /* 0x08005D36: 01d2     bhs      #0x8005d3c */
    if (r0 >= 0xc) goto L_08005D3C;   /* unsigned */
    /* 0x08005D38: 0c20     movs     r0, #0xc */
    r0 = 0xc;
    /* 0x08005D3A: 2880     strh     r0, [r5] */
    *(uint16_t *)(r5) = r0;   /* store */

    L_08005D3C:
    /* 0x08005D3C: 0e49     ldr      r1, [pc, #0x38]  ; =RAM_200001C6 */
    r1 = *(uint32_t *)0x08005D78;   /* = RAM 0x200001C6 */
    /* 0x08005D3E: 2888     ldrh     r0, [r5] */
    r0 = *(uint16_t *)(r5);
    /* 0x08005D40: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */
    /* 0x08005D42: 2088     ldrh     r0, [r4] */
    r0 = *(uint16_t *)(r4);
    /* 0x08005D44: 3228     cmp      r0, #0x32 */
    /* flags = cmp(r0, #0x32) */
    /* 0x08005D46: 01d9     bls      #0x8005d4c */
    if (r0 <= 0x32) goto L_08005D4C;   /* unsigned */
    /* 0x08005D48: 3220     movs     r0, #0x32 */
    r0 = 0x32;
    /* 0x08005D4A: 02e0     b        #0x8005d52 */
    goto L_08005D52;

    L_08005D4C:
    /* 0x08005D4C: 0f28     cmp      r0, #0xf */
    /* flags = cmp(r0, #0xf) */
    /* 0x08005D4E: 01d2     bhs      #0x8005d54 */
    if (r0 >= 0xf) goto L_08005D54;   /* unsigned */
    /* 0x08005D50: 0f20     movs     r0, #0xf */
    r0 = 0xf;

    L_08005D52:
    /* 0x08005D52: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */

    L_08005D54:
    /* 0x08005D54: 30bd     pop      {r4, r5, pc} */
    pop({r4, r5, pc});   /* return */
}
