/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C674
 * Name      : FUN_0800C674
 * End       : 0x0800C6DA
 * Size      : 102 bytes, 42 instructions decoded in body (functions.json insns=42, blocks=5)
 * Class     : function
 * Callers   : 2  -> 0x0800C6F4 FUN_0800C6F4, 0x0800C758 FUN_0800C758
 * Callees   : 4  -> 0x08007426 FUN_08007426, 0x08008738 FUN_08008738, 0x0800B5EC FUN_0800B5EC, 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C674(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C674: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800C676: 194c     ldr      r4, [pc, #0x64]  ; =RAM_200001B2 */
    r4 = *(uint32_t *)0x0800C6DC;   /* = RAM 0x200001B2 */
    /* 0x0800C678: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800C67A: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x0800C67C: 2cd1     bne      #0x800c6d8 */
    if (r0 != 0xa) goto L_0800C6D8;
    /* 0x0800C67E: fef7b5ff bl       #0x800b5ec */
    FUN_0800B5EC();   /* BL -> 0x0800B5EC FUN_0800B5EC */
    /* 0x0800C682: 1748     ldr      r0, [pc, #0x5c]  ; =RAM_20000165 */
    r0 = *(uint32_t *)0x0800C6E0;   /* = RAM 0x20000165 */
    /* 0x0800C684: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800C686: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800C688: 02d9     bls      #0x800c690 */
    if (r0 <= 1) goto L_0800C690;   /* unsigned */
    /* 0x0800C68A: 1649     ldr      r1, [pc, #0x58]  ; =RAM_200001B3 */
    r1 = *(uint32_t *)0x0800C6E4;   /* = RAM 0x200001B3 */
    /* 0x0800C68C: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800C68E: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */

    L_0800C690:
    /* 0x0800C690: 0925     movs     r5, #9 */
    r5 = 9;
    /* 0x0800C692: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800C694: ed06     lsls     r5, r5, #0x1b */
    r5 = r5 << 0x1b;
    /* 0x0800C696: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x0800C698: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800C69A: faf7c4fe bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800C69E: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800C6A0: 81f31088 msr      primask, r1 */
    /* TODO: msr primask, r1 */
    /* 0x0800C6A4: 0a46     mov      r2, r1 */
    r2 = r1;
    /* 0x0800C6A6: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800C6A8: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800C6AA: faf7bcfe bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800C6AE: 0e48     ldr      r0, [pc, #0x38]  ; =RAM_200001C4 */
    r0 = *(uint32_t *)0x0800C6E8;   /* = RAM 0x200001C4 */
    /* 0x0800C6B0: 0088     ldrh     r0, [r0] */
    r0 = *(uint16_t *)(r0);
    /* 0x0800C6B2: 00f0c1f9 bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800C6B6: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800C6B8: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800C6BA: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800C6BC: faf7b3fe bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800C6C0: 0a48     ldr      r0, [pc, #0x28]  ; =0x00000BB8 */
    r0 = *(uint32_t *)0x0800C6EC;   /* = 0x00000BB8 */
    /* 0x0800C6C2: 00f0b9f9 bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800C6C6: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C6C8: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800C6CC: 0546     mov      r5, r0 */
    r5 = r0;
    /* 0x0800C6CE: 0848     ldr      r0, [pc, #0x20]  ; =RAM_200001BB */
    r0 = *(uint32_t *)0x0800C6F0;   /* = RAM 0x200001BB */
    /* 0x0800C6D0: 0570     strb     r5, [r0] */
    *(uint8_t *)(r0) = r5;   /* store */
    /* 0x0800C6D2: fcf731f8 bl       #0x8008738 */
    FUN_08008738();   /* BL -> 0x08008738 FUN_08008738 */
    /* 0x0800C6D6: 2570     strb     r5, [r4] */
    *(uint8_t *)(r4) = r5;   /* store */

    L_0800C6D8:
    /* 0x0800C6D8: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
