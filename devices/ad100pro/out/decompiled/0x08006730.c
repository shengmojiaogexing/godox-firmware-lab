/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006730
 * Name      : FUN_08006730
 * End       : 0x08006758
 * Size      : 40 bytes, 20 instructions decoded in body (functions.json insns=20, blocks=1)
 * Class     : function
 * Callers   : 7  -> 0x08003D2C FUN_08003D2C, 0x08005CCC FUN_08005CCC, 0x080090E8 FUN_080090E8, 0x08009EE0 FUN_08009EE0, 0x0800A570 FUN_0800A570, 0x0800A768 FUN_0800A768, 0x0800B448 FUN_0800B448
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006730(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006730: 0a49     ldr      r1, [pc, #0x28]  ; =RAM_2000017D */
    r1 = *(uint32_t *)0x0800675C;   /* = RAM 0x2000017D */
    /* 0x08006732: 0948     ldr      r0, [pc, #0x24]  ; =RAM_2000019C */
    r0 = *(uint32_t *)0x08006758;   /* = RAM 0x2000019C */
    /* 0x08006734: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x08006736: 5022     movs     r2, #0x50 */
    r2 = 0x50;
    /* 0x08006738: 405c     ldrb     r0, [r0, r1] */
    r0 = *(uint8_t *)(r0 + r1);
    /* 0x0800673A: 0949     ldr      r1, [pc, #0x24]  ; =RAM_200001AA */
    r1 = *(uint32_t *)0x08006760;   /* = RAM 0x200001AA */
    /* 0x0800673C: 101a     subs     r0, r2, r0 */
    r0 = r2 - r0;
    /* 0x0800673E: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08006740: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08006742: 0849     ldr      r1, [pc, #0x20]  ; =flash_0800D424 */
    r1 = *(uint32_t *)0x08006764;   /* = flash data/const 0x0800D424 */
    /* 0x08006744: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x08006746: 085a     ldrh     r0, [r1, r0] */
    r0 = *(uint16_t *)(r1 + r0);
    /* 0x08006748: 074a     ldr      r2, [pc, #0x1c]  ; =RAM_200001D8 */
    r2 = *(uint32_t *)0x08006768;   /* = RAM 0x200001D8 */
    /* 0x0800674A: 8305     lsls     r3, r0, #0x16 */
    r3 = r0 << 0x16;
    /* 0x0800674C: 9b0d     lsrs     r3, r3, #0x16 */
    r3 = r3 >> 0x16;
    /* 0x0800674E: 0749     ldr      r1, [pc, #0x1c]  ; =RAM_200001C8 */
    r1 = *(uint32_t *)0x0800676C;   /* = RAM 0x200001C8 */
    /* 0x08006750: 1380     strh     r3, [r2] */
    *(uint16_t *)(r2) = r3;   /* store */
    /* 0x08006752: 800a     lsrs     r0, r0, #0xa */
    r0 = r0 >> 0xa;
    /* 0x08006754: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */
    /* 0x08006756: 7047     bx       lr */
    return;
}
