/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006770
 * Name      : FUN_08006770
 * End       : 0x080067A6
 * Size      : 54 bytes, 27 instructions decoded in body (functions.json insns=27, blocks=4)
 * Class     : function
 * Callers   : 7  -> 0x08003C50 FUN_08003C50, 0x08005C44 FUN_08005C44, 0x08009060 FUN_08009060, 0x08009E30 FUN_08009E30, 0x0800A4E8 FUN_0800A4E8, 0x0800A6E0 FUN_0800A6E0, 0x0800B3C0 FUN_0800B3C0
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006770(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006770: 0e49     ldr      r1, [pc, #0x38]  ; =RAM_2000017D */
    r1 = *(uint32_t *)0x080067AC;   /* = RAM 0x2000017D */
    /* 0x08006772: 0d48     ldr      r0, [pc, #0x34]  ; =RAM_2000019C */
    r0 = *(uint32_t *)0x080067A8;   /* = RAM 0x2000019C */
    /* 0x08006774: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08006776: 405c     ldrb     r0, [r0, r1] */
    r0 = *(uint8_t *)(r0 + r1);
    /* 0x08006778: 5021     movs     r1, #0x50 */
    r1 = 0x50;
    /* 0x0800677A: 081a     subs     r0, r1, r0 */
    r0 = r1 - r0;
    /* 0x0800677C: 0c49     ldr      r1, [pc, #0x30]  ; =RAM_200001AA */
    r1 = *(uint32_t *)0x080067B0;   /* = RAM 0x200001AA */
    /* 0x0800677E: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08006780: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08006782: 0c49     ldr      r1, [pc, #0x30]  ; =RAM_20000175 */
    r1 = *(uint32_t *)0x080067B4;   /* = RAM 0x20000175 */
    /* 0x08006784: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08006786: c906     lsls     r1, r1, #0x1b */
    r1 = r1 << 0x1b;
    /* 0x08006788: 03d4     bmi      #0x8006792 */
    if (/* bmi */) goto L_08006792;   /* result negative */
    /* 0x0800678A: 0b49     ldr      r1, [pc, #0x2c]  ; =RAM_20000480 */
    r1 = *(uint32_t *)0x080067B8;   /* = RAM 0x20000480 */
    /* 0x0800678C: 8979     ldrb     r1, [r1, #6] */
    r1 = *(uint8_t *)(r1 + 6);
    /* 0x0800678E: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x08006790: 03d0     beq      #0x800679a */
    if (r1 == 1) goto L_0800679A;

    L_08006792:
    /* 0x08006792: 0a49     ldr      r1, [pc, #0x28]  ; =flash_0800D174 */
    r1 = *(uint32_t *)0x080067BC;   /* = flash data/const 0x0800D174 */
    /* 0x08006794: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x08006796: 085a     ldrh     r0, [r1, r0] */
    r0 = *(uint16_t *)(r1 + r0);
    /* 0x08006798: 7047     bx       lr */
    return;

    L_0800679A:
    /* 0x0800679A: 0849     ldr      r1, [pc, #0x20]  ; =flash_0800D174 */
    r1 = *(uint32_t *)0x080067BC;   /* = flash data/const 0x0800D174 */
    /* 0x0800679C: 8000     lsls     r0, r0, #2 */
    r0 = r0 << 2;
    /* 0x0800679E: a231     adds     r1, #0xa2 */
    r1 = r1 + 0xa2;
    /* 0x080067A0: 4018     adds     r0, r0, r1 */
    r0 = r0 + r1;
    /* 0x080067A2: 4088     ldrh     r0, [r0, #2] */
    r0 = *(uint16_t *)(r0 + 2);
    /* 0x080067A4: 7047     bx       lr */
    return;
}
