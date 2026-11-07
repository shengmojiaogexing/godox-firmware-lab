/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A904
 * Name      : FUN_0800A904
 * End       : 0x0800A914
 * Size      : 16 bytes, 8 instructions decoded in body (functions.json insns=8, blocks=1)
 * Class     : function
 * Callers   : 4  -> 0x080091C4 FUN_080091C4, 0x08009260 FUN_08009260, 0x0800C4E8 FUN_0800C4E8, 0x0800C578 FUN_0800C578
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A904(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A904: 034a     ldr      r2, [pc, #0xc]  ; =RAM_2000030C */
    r2 = *(uint32_t *)0x0800A914;   /* = RAM 0x2000030C */
    /* 0x0800A906: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x0800A908: 1368     ldr      r3, [r2] */
    r3 = *(uint32_t *)(r2);
    /* 0x0800A90A: d862     str      r0, [r3, #0x2c] */
    *(uint32_t *)(r3 + 44) = r0;   /* store */
    /* 0x0800A90C: 1068     ldr      r0, [r2] */
    r0 = *(uint32_t *)(r2);
    /* 0x0800A90E: 491e     subs     r1, r1, #1 */
    r1 = r1 - 1;
    /* 0x0800A910: 8163     str      r1, [r0, #0x38] */
    *(uint32_t *)(r0 + 56) = r1;   /* store */
    /* 0x0800A912: 7047     bx       lr */
    return;
}
