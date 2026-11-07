/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A8D0
 * Name      : FUN_0800A8D0
 * End       : 0x0800A8FE
 * Size      : 46 bytes, 20 instructions decoded in body (functions.json insns=20, blocks=3)
 * Class     : function
 * Callers   : 4  -> 0x080091C4 FUN_080091C4, 0x08009224 FUN_08009224, 0x0800C4E8 FUN_0800C4E8, 0x0800C54C FUN_0800C54C
 * Callees   : 3  -> 0x08005C40 FUN_08005C40, 0x0800854C FUN_0800854C, 0x0800868C FUN_0800868C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A8D0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A8D0: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800A8D2: 0b4a     ldr      r2, [pc, #0x2c]  ; =RAM_2000030C */
    r2 = *(uint32_t *)0x0800A900;   /* = RAM 0x2000030C */
    /* 0x0800A8D4: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x0800A8D6: 1368     ldr      r3, [r2] */
    r3 = *(uint32_t *)(r2);
    /* 0x0800A8D8: d862     str      r0, [r3, #0x2c] */
    *(uint32_t *)(r3 + 44) = r0;   /* store */
    /* 0x0800A8DA: 1368     ldr      r3, [r2] */
    r3 = *(uint32_t *)(r2);
    /* 0x0800A8DC: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800A8DE: 5862     str      r0, [r3, #0x24] */
    *(uint32_t *)(r3 + 36) = r0;   /* store */
    /* 0x0800A8E0: 1068     ldr      r0, [r2] */
    r0 = *(uint32_t *)(r2);
    /* 0x0800A8E2: 491e     subs     r1, r1, #1 */
    r1 = r1 - 1;
    /* 0x0800A8E4: 8163     str      r1, [r0, #0x38] */
    *(uint32_t *)(r0 + 56) = r1;   /* store */
    /* 0x0800A8E6: 1046     mov      r0, r2 */
    r0 = r2;
    /* 0x0800A8E8: fdf730fe bl       #0x800854c */
    FUN_0800854C();   /* BL -> 0x0800854C FUN_0800854C */
    /* 0x0800A8EC: 0421     movs     r1, #4 */
    r1 = 4;
    /* 0x0800A8EE: 0448     ldr      r0, [pc, #0x10]  ; =RAM_2000030C */
    r0 = *(uint32_t *)0x0800A900;   /* = RAM 0x2000030C */
    /* 0x0800A8F0: fdf7ccfe bl       #0x800868c */
    FUN_0800868C();   /* BL -> 0x0800868C FUN_0800868C */
    /* 0x0800A8F4: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800A8F6: 01d0     beq      #0x800a8fc */
    if (r0 == 0) goto L_0800A8FC;
    /* 0x0800A8F8: fbf7a2f9 bl       #0x8005c40 */
    FUN_08005C40();   /* BL -> 0x08005C40 FUN_08005C40 */

    L_0800A8FC:
    /* 0x0800A8FC: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
