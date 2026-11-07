/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005D7C
 * Name      : FUN_08005D7C
 * End       : 0x08005DE2
 * Size      : 102 bytes, 44 instructions decoded in body (functions.json insns=44, blocks=6)
 * Class     : function
 * Callers   : 4  -> 0x0800AFC4 FUN_0800AFC4, 0x0800B03C FUN_0800B03C, 0x0800B0BE FUN_0800B0BE, 0x0800B16A FUN_0800B16A
 * Callees   : 2  -> 0x08007426 FUN_08007426, 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005D7C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005D7C: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08005D7E: 1a49     ldr      r1, [pc, #0x68]  ; =RAM_2000017D */
    r1 = *(uint32_t *)0x08005DE8;   /* = RAM 0x2000017D */
    /* 0x08005D80: 1848     ldr      r0, [pc, #0x60]  ; =RAM_200001BD */
    r0 = *(uint32_t *)0x08005DE4;   /* = RAM 0x200001BD */
    /* 0x08005D82: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08005D84: 405c     ldrb     r0, [r0, r1] */
    r0 = *(uint8_t *)(r0 + r1);
    /* 0x08005D86: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x08005D88: 00d9     bls      #0x8005d8c */
    if (r0 <= 0x64) goto L_08005D8C;   /* unsigned */
    /* 0x08005D8A: 6420     movs     r0, #0x64 */
    r0 = 0x64;

    L_08005D8C:
    /* 0x08005D8C: 1749     ldr      r1, [pc, #0x5c]  ; =flash_0800D35A */
    r1 = *(uint32_t *)0x08005DEC;   /* = flash data/const 0x0800D35A */
    /* 0x08005D8E: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x08005D90: 0c5a     ldrh     r4, [r1, r0] */
    r4 = *(uint16_t *)(r1 + r0);
    /* 0x08005D92: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08005D94: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x08005D98: 1548     ldr      r0, [pc, #0x54]  ; =RAM_200001B8 */
    r0 = *(uint32_t *)0x08005DF0;   /* = RAM 0x200001B8 */
    /* 0x08005D9A: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08005D9C: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08005D9E: 17d1     bne      #0x8005dd0 */
    if (r0 != 1) goto L_08005DD0;
    /* 0x08005DA0: 1448     ldr      r0, [pc, #0x50]  ; =RAM_200001B2 */
    r0 = *(uint32_t *)0x08005DF4;   /* = RAM 0x200001B2 */
    /* 0x08005DA2: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08005DA4: 0528     cmp      r0, #5 */
    /* flags = cmp(r0, #5) */
    /* 0x08005DA6: 13d1     bne      #0x8005dd0 */
    if (r0 != 5) goto L_08005DD0;
    /* 0x08005DA8: 0925     movs     r5, #9 */
    r5 = 9;
    /* 0x08005DAA: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08005DAC: ed06     lsls     r5, r5, #0x1b */
    r5 = r5 << 0x1b;
    /* 0x08005DAE: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x08005DB0: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08005DB2: 01f038fb bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08005DB6: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08005DB8: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08005DBA: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08005DBC: 01f033fb bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08005DC0: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08005DC2: 06f039fe bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08005DC6: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08005DC8: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08005DCA: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08005DCC: 01f02bfb bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */

    L_08005DD0:
    /* 0x08005DD0: 9620     movs     r0, #0x96 */
    r0 = 0x96;
    /* 0x08005DD2: 06f031fe bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08005DD6: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005DD8: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x08005DDC: 0649     ldr      r1, [pc, #0x18]  ; =RAM_200001BC */
    r1 = *(uint32_t *)0x08005DF8;   /* = RAM 0x200001BC */
    /* 0x08005DDE: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08005DE0: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
