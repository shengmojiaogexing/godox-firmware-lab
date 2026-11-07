/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080047A0
 * Name      : FUN_080047A0
 * End       : 0x080047BE
 * Size      : 30 bytes, 15 instructions decoded in body (functions.json insns=15, blocks=3)
 * Class     : function
 * Callers   : 1  -> 0x08007104 FUN_08007104
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080047A0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080047A0: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x080047A2: 0468     ldr      r4, [r0] */
    r4 = *(uint32_t *)(r0);
    /* 0x080047A4: 6360     str      r3, [r4, #4] */
    *(uint32_t *)(r4 + 4) = r3;   /* store */
    /* 0x080047A6: 4368     ldr      r3, [r0, #4] */
    r3 = *(uint32_t *)(r0 + 4);
    /* 0x080047A8: 102b     cmp      r3, #0x10 */
    /* flags = cmp(r3, #0x10) */
    /* 0x080047AA: 0368     ldr      r3, [r0] */
    r3 = *(uint32_t *)(r0);
    /* 0x080047AC: 03d0     beq      #0x80047b6 */
    if (r3 == 0x10) goto L_080047B6;
    /* 0x080047AE: 9960     str      r1, [r3, #8] */
    *(uint32_t *)(r3 + 8) = r1;   /* store */
    /* 0x080047B0: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);
    /* 0x080047B2: c260     str      r2, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r2;   /* store */
    /* 0x080047B4: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_080047B6:
    /* 0x080047B6: 9a60     str      r2, [r3, #8] */
    *(uint32_t *)(r3 + 8) = r2;   /* store */
    /* 0x080047B8: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);
    /* 0x080047BA: c160     str      r1, [r0, #0xc] */
    *(uint32_t *)(r0 + 12) = r1;   /* store */
    /* 0x080047BC: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
