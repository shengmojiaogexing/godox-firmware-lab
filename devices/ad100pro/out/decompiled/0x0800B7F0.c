/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B7F0
 * Name      : FUN_0800B7F0
 * End       : 0x0800B800
 * Size      : 16 bytes, 8 instructions decoded in body (functions.json insns=8, blocks=1)
 * Class     : hub
 * Callers   : 11  -> 0x08004880 FUN_08004880, 0x08005A78 IRQ5_Handler, 0x08005AB4 FUN_08005AB4, 0x08005B0C IRQ7_Handler, 0x08005B42 FUN_08005B42, 0x08008A5C FUN_08008A5C, 0x08008A68 FUN_08008A68, 0x08008D00 dispatcher_large, ... +3
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B7F0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B7F0: 0349     ldr      r1, [pc, #0xc]  ; =RAM_200001D2 */
    r1 = *(uint32_t *)0x0800B800;   /* = RAM 0x200001D2 */
    /* 0x0800B7F2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800B7F4: 0880     strh     r0, [r1] */
    *(uint16_t *)(r1) = r0;   /* store */
    /* 0x0800B7F6: 0349     ldr      r1, [pc, #0xc]  ; =RAM_200001D5 */
    r1 = *(uint32_t *)0x0800B804;   /* = RAM 0x200001D5 */
    /* 0x0800B7F8: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x0800B7FA: 0349     ldr      r1, [pc, #0xc]  ; =RAM_200001D4 */
    r1 = *(uint32_t *)0x0800B808;   /* = RAM 0x200001D4 */
    /* 0x0800B7FC: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x0800B7FE: 7047     bx       lr */
    return;
}
