/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009FC0
 * Name      : FUN_08009FC0
 * End       : 0x0800A022
 * Size      : 98 bytes, 43 instructions decoded in body (functions.json insns=43, blocks=9)
 * Class     : function
 * Callers   : 4  -> 0x0800AFC4 FUN_0800AFC4, 0x0800B03C FUN_0800B03C, 0x0800B0BE FUN_0800B0BE, 0x0800B114 FUN_0800B114
 * Callees   : 3  -> 0x08007426 FUN_08007426, 0x08009E14 FUN_08009E14, 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009FC0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009FC0: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08009FC2: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x08009FC4: 1748     ldr      r0, [pc, #0x5c]  ; =RAM_200001B2 */
    r0 = *(uint32_t *)0x0800A024;   /* = RAM 0x200001B2 */
    /* 0x08009FC6: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08009FC8: 0528     cmp      r0, #5 */
    /* flags = cmp(r0, #5) */
    /* 0x08009FCA: 1ed1     bne      #0x800a00a */
    if (r0 != 5) goto L_0800A00A;
    /* 0x08009FCC: 1648     ldr      r0, [pc, #0x58]  ; =RAM_200001B8 */
    r0 = *(uint32_t *)0x0800A028;   /* = RAM 0x200001B8 */
    /* 0x08009FCE: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08009FD0: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08009FD2: 1ad1     bne      #0x800a00a */
    if (r0 != 1) goto L_0800A00A;
    /* 0x08009FD4: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08009FD6: fff71dff bl       #0x8009e14 */
    FUN_08009E14();   /* BL -> 0x08009E14 FUN_08009E14 */
    /* 0x08009FDA: 0926     movs     r6, #9 */
    r6 = 9;
    /* 0x08009FDC: 0546     mov      r5, r0 */
    r5 = r0;
    /* 0x08009FDE: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08009FE0: f606     lsls     r6, r6, #0x1b */
    r6 = r6 << 0x1b;
    /* 0x08009FE2: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x08009FE4: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x08009FE6: fdf71efa bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009FEA: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08009FEC: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08009FEE: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x08009FF0: fdf719fa bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009FF4: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08009FF6: 02f01ffd bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08009FFA: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08009FFC: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08009FFE: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x0800A000: fdf711fa bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A004: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800A006: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */

    L_0800A00A:
    /* 0x0800A00A: 0848     ldr      r0, [pc, #0x20]  ; =RAM_200001DF */
    r0 = *(uint32_t *)0x0800A02C;   /* = RAM 0x200001DF */
    /* 0x0800A00C: 092c     cmp      r4, #9 */
    /* flags = cmp(r4, #9) */
    /* 0x0800A00E: 01d2     bhs      #0x800a014 */
    if (r4 >= 9) goto L_0800A014;   /* unsigned */
    /* 0x0800A010: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800A012: 04e0     b        #0x800a01e */
    goto L_0800A01E;

    L_0800A014:
    /* 0x0800A014: 182c     cmp      r4, #0x18 */
    /* flags = cmp(r4, #0x18) */
    /* 0x0800A016: 01d2     bhs      #0x800a01c */
    if (r4 >= 0x18) goto L_0800A01C;   /* unsigned */
    /* 0x0800A018: 0c21     movs     r1, #0xc */
    r1 = 0xc;
    /* 0x0800A01A: 00e0     b        #0x800a01e */
    goto L_0800A01E;

    L_0800A01C:
    /* 0x0800A01C: 2421     movs     r1, #0x24 */
    r1 = 0x24;

    L_0800A01E:
    /* 0x0800A01E: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x0800A020: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
