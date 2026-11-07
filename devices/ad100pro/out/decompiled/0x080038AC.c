/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080038AC
 * Name      : FUN_080038AC
 * End       : 0x080038B8
 * Size      : 12 bytes, 6 instructions decoded in body (functions.json insns=6, blocks=1)
 * Class     : function
 * Callers   : 4  -> 0x08004880 FUN_08004880, 0x08008A5C FUN_08008A5C, 0x08008D00 dispatcher_large, 0x08008E24 FUN_08008E24
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080038AC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080038AC: 0348     ldr      r0, [pc, #0xc]  ; =RAM_200001E0 */
    r0 = *(uint32_t *)0x080038BC;   /* = RAM 0x200001E0 */
    /* 0x080038AE: 0249     ldr      r1, [pc, #8]  ; =0x00001770 */
    r1 = *(uint32_t *)0x080038B8;   /* = 0x00001770 */
    /* 0x080038B0: c184     strh     r1, [r0, #0x26] */
    *(uint16_t *)(r0 + 38) = r1;   /* store */
    /* 0x080038B2: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x080038B4: 0173     strb     r1, [r0, #0xc] */
    *(uint8_t *)(r0 + 12) = r1;   /* store */
    /* 0x080038B6: 7047     bx       lr */
    return;
}
