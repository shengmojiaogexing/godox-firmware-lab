/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008148
 * Name      : FUN_08008148
 * End       : 0x0800815C
 * Size      : 20 bytes, 10 instructions decoded in body (functions.json insns=10, blocks=4)
 * Class     : function
 * Callers   : 1  -> 0x0800B8F0 FUN_0800B8F0
 * Callees   : 0  -> -
 * MMIO      : SYSTICK.CTRL
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08008148(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08008148: 0422     movs     r2, #4 */
    r2 = 4;
    /* 0x0800814A: 0449     ldr      r1, [pc, #0x10]  ; =SYS.0x0 */
    r1 = *(uint32_t *)0x0800815C;   /* = MMIO SCS+0x00 */
    /* 0x0800814C: 0428     cmp      r0, #4 */
    /* flags = cmp(r0, #4) */
    /* 0x0800814E: 0869     ldr      r0, [r1, #0x10] */
    r0 = *(uint32_t *)(r1 + 16);  /* MMIO: SYSTICK.CTRL(r) */
    /* 0x08008150: 02d0     beq      #0x8008158 */
    if (r0 == 4) goto L_08008158;
    /* 0x08008152: 9043     bics     r0, r2 */
    r0 = r0 & ~r2;

    L_08008154:
    /* 0x08008154: 0861     str      r0, [r1, #0x10] */
    *(uint32_t *)(r1 + 16) = r0;   /* store */  /* MMIO: SYSTICK.CTRL(w) */
    /* 0x08008156: 7047     bx       lr */
    return;

    L_08008158:
    /* 0x08008158: 1043     orrs     r0, r2 */
    r0 = r0 | r2;
    /* 0x0800815A: fbe7     b        #0x8008154 */
    goto L_08008154;   /* loop */
}
