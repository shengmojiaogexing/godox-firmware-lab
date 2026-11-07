/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B578
 * Name      : FUN_0800B578
 * End       : 0x0800B580
 * Size      : 8 bytes, 4 instructions decoded in body (functions.json insns=4, blocks=1)
 * Class     : function
 * Callers   : 5  -> 0x08004880 FUN_08004880, 0x08008A5C FUN_08008A5C, 0x08008A6C FUN_08008A6C, 0x08008D00 dispatcher_large, 0x08008EC6 FUN_08008EC6
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B578(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B578: 0149     ldr      r1, [pc, #4]  ; =RAM_20000120 */
    r1 = *(uint32_t *)0x0800B580;   /* = RAM 0x20000120 */
    /* 0x0800B57A: c820     movs     r0, #0xc8 */
    r0 = 0xc8;
    /* 0x0800B57C: 8880     strh     r0, [r1, #4] */
    *(uint16_t *)(r1 + 4) = r0;   /* store */
    /* 0x0800B57E: 7047     bx       lr */
    return;
}
