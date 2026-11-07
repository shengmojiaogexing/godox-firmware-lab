/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008A2C
 * Name      : FUN_08008A2C
 * End       : 0x08008A56
 * Size      : 42 bytes, 19 instructions decoded in body (functions.json insns=19, blocks=1)
 * Class     : function
 * Callers   : 6  -> 0x080091C4 FUN_080091C4, 0x08009260 FUN_08009260, 0x0800B9C0 FUN_0800B9C0, 0x0800B9CC FUN_0800B9CC, 0x0800C4E8 FUN_0800C4E8, 0x0800C5A4 FUN_0800C5A4
 * Callees   : 2  -> 0x080072C4 FUN_080072C4, 0x08007426 FUN_08007426
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08008A2C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08008A2C: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08008A2E: 0924     movs     r4, #9 */
    r4 = 9;
    /* 0x08008A30: e406     lsls     r4, r4, #0x1b */
    r4 = r4 << 0x1b;
    /* 0x08008A32: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08008A34: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08008A36: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08008A38: fef7f5fc bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08008A3C: 0649     ldr      r1, [pc, #0x18]  ; =RAM_20000398 */
    r1 = *(uint32_t *)0x08008A58;   /* = RAM 0x20000398 */
    /* 0x08008A3E: 8020     movs     r0, #0x80 */
    r0 = 0x80;
    /* 0x08008A40: 0860     str      r0, [r1] */
    *(uint32_t *)(r1) = r0;   /* store */
    /* 0x08008A42: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08008A44: 4860     str      r0, [r1, #4] */
    *(uint32_t *)(r1 + 4) = r0;   /* store */
    /* 0x08008A46: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08008A48: 8860     str      r0, [r1, #8] */
    *(uint32_t *)(r1 + 8) = r0;   /* store */
    /* 0x08008A4A: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x08008A4C: c860     str      r0, [r1, #0xc] */
    *(uint32_t *)(r1 + 12) = r0;   /* store */
    /* 0x08008A4E: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08008A50: fef738fc bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08008A54: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
