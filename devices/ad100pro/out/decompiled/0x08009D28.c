/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009D28
 * Name      : FUN_08009D28
 * End       : 0x08009D76
 * Size      : 78 bytes, 31 instructions decoded in body (functions.json insns=31, blocks=3)
 * Class     : function
 * Callers   : 1  -> 0x0800B61C FUN_0800B61C
 * Callees   : 3  -> 0x08007426 FUN_08007426, 0x08008738 FUN_08008738, 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009D28(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009D28: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08009D2A: 134c     ldr      r4, [pc, #0x4c]  ; =RAM_200001C4 */
    r4 = *(uint32_t *)0x08009D78;   /* = RAM 0x200001C4 */
    /* 0x08009D2C: 2088     ldrh     r0, [r4] */
    r0 = *(uint16_t *)(r4);
    /* 0x08009D2E: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009D30: 20d0     beq      #0x8009d74 */
    if (r0 == 0) goto L_08009D74;
    /* 0x08009D32: 0925     movs     r5, #9 */
    r5 = 9;
    /* 0x08009D34: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08009D36: ed06     lsls     r5, r5, #0x1b */
    r5 = r5 << 0x1b;
    /* 0x08009D38: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x08009D3A: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08009D3C: fdf773fb bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009D40: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08009D42: 81f31088 msr      primask, r1 */
    /* TODO: msr primask, r1 */
    /* 0x08009D46: 0a46     mov      r2, r1 */
    r2 = r1;
    /* 0x08009D48: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08009D4A: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08009D4C: fdf76bfb bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009D50: 2088     ldrh     r0, [r4] */
    r0 = *(uint16_t *)(r4);
    /* 0x08009D52: 02f071fe bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08009D56: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08009D58: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08009D5A: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08009D5C: fdf763fb bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009D60: 0648     ldr      r0, [pc, #0x18]  ; =0x00000BB8 */
    r0 = *(uint32_t *)0x08009D7C;   /* = 0x00000BB8 */
    /* 0x08009D62: 02f069fe bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08009D66: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08009D68: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x08009D6C: 0449     ldr      r1, [pc, #0x10]  ; =RAM_200001BB */
    r1 = *(uint32_t *)0x08009D80;   /* = RAM 0x200001BB */
    /* 0x08009D6E: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08009D70: fef7e2fc bl       #0x8008738 */
    FUN_08008738();   /* BL -> 0x08008738 FUN_08008738 */

    L_08009D74:
    /* 0x08009D74: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
