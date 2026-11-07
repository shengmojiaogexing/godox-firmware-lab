/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080067C0
 * Name      : FUN_080067C0
 * End       : 0x080067DA
 * Size      : 26 bytes, 13 instructions decoded in body (functions.json insns=13, blocks=1)
 * Class     : hub
 * Callers   : 14  -> 0x08003C50 FUN_08003C50, 0x08003C66 FUN_08003C66, 0x08005C44 FUN_08005C44, 0x08005C64 FUN_08005C64, 0x08009060 FUN_08009060, 0x08009080 FUN_08009080, 0x08009E30 FUN_08009E30, 0x08009E6A FUN_08009E6A, ... +6
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080067C0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080067C0: 0749     ldr      r1, [pc, #0x1c]  ; =RAM_2000017D */
    r1 = *(uint32_t *)0x080067E0;   /* = RAM 0x2000017D */
    /* 0x080067C2: 0648     ldr      r0, [pc, #0x18]  ; =RAM_200001A2 */
    r0 = *(uint32_t *)0x080067DC;   /* = RAM 0x200001A2 */
    /* 0x080067C4: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x080067C6: 405c     ldrb     r0, [r0, r1] */
    r0 = *(uint8_t *)(r0 + r1);
    /* 0x080067C8: 5021     movs     r1, #0x50 */
    r1 = 0x50;
    /* 0x080067CA: 081a     subs     r0, r1, r0 */
    r0 = r1 - r0;
    /* 0x080067CC: 0549     ldr      r1, [pc, #0x14]  ; =RAM_200001AA */
    r1 = *(uint32_t *)0x080067E4;   /* = RAM 0x200001AA */
    /* 0x080067CE: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x080067D0: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x080067D2: 0549     ldr      r1, [pc, #0x14]  ; =flash_0800D174 */
    r1 = *(uint32_t *)0x080067E8;   /* = flash data/const 0x0800D174 */
    /* 0x080067D4: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x080067D6: 085a     ldrh     r0, [r1, r0] */
    r0 = *(uint16_t *)(r1 + r0);
    /* 0x080067D8: 7047     bx       lr */
    return;
}
