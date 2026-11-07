/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008164
 * Name      : FUN_08008164
 * End       : 0x0800818A
 * Size      : 38 bytes, 18 instructions decoded in body (functions.json insns=18, blocks=3)
 * Class     : function
 * Callers   : 3  -> 0x080075EC FUN_080075EC, 0x080075FA FUN_080075FA, 0x0800B8F0 FUN_0800B8F0
 * Callees   : 1  -> 0x0800A2C8 FUN_0800A2C8
 * MMIO      : SYSTICK.LOAD
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08008164(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08008164: 0949     ldr      r1, [pc, #0x24]  ; =0x00FFFFFF */
    r1 = *(uint32_t *)0x0800818C;   /* = 0x00FFFFFF */
    /* 0x08008166: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08008168: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x0800816A: 8842     cmp      r0, r1 */
    /* flags = cmp(r0, r1) */
    /* 0x0800816C: 01d9     bls      #0x8008172 */
    if (r0 <= r1) goto L_08008172;   /* unsigned */
    /* 0x0800816E: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08008170: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_08008172:
    /* 0x08008172: 074c     ldr      r4, [pc, #0x1c]  ; =SYS.0x0 */
    r4 = *(uint32_t *)0x08008190;   /* = MMIO SCS+0x00 */
    /* 0x08008174: 6061     str      r0, [r4, #0x14] */
    *(uint32_t *)(r4 + 20) = r0;   /* store */  /* MMIO: SYSTICK.LOAD(w) */
    /* 0x08008176: 0321     movs     r1, #3 */
    r1 = 3;
    /* 0x08008178: 081f     subs     r0, r1, #4 */
    r0 = r1 - 4;
    /* 0x0800817A: 02f0a5f8 bl       #0x800a2c8 */
    FUN_0800A2C8();   /* BL -> 0x0800A2C8 FUN_0800A2C8 */
    /* 0x0800817E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08008180: a061     str      r0, [r4, #0x18] */
    *(uint32_t *)(r4 + 24) = r0;   /* store */  /* MMIO: SYSTICK.VAL(w) */
    /* 0x08008182: 0720     movs     r0, #7 */
    r0 = 7;
    /* 0x08008184: 2061     str      r0, [r4, #0x10] */
    *(uint32_t *)(r4 + 16) = r0;   /* store */  /* MMIO: SYSTICK.CTRL(w) */
    /* 0x08008186: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08008188: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
