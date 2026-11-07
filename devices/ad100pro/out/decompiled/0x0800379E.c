/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800379E
 * Name      : FUN_0800379E
 * End       : 0x080037B8
 * Size      : 26 bytes, 12 instructions decoded in body (functions.json insns=12, blocks=1)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x080069A8 FUN_080069A8
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800379E(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800379E: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x080037A0: 406a     ldr      r0, [r0, #0x24] */
    r0 = *(uint32_t *)(r0 + 36);
    /* 0x080037A2: 416c     ldr      r1, [r0, #0x44] */
    r1 = *(uint32_t *)(r0 + 68);
    /* 0x080037A4: 4022     movs     r2, #0x40 */
    r2 = 0x40;
    /* 0x080037A6: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x080037A8: 4164     str      r1, [r0, #0x44] */
    *(uint32_t *)(r0 + 68) = r1;   /* store */
    /* 0x080037AA: 816c     ldr      r1, [r0, #0x48] */
    r1 = *(uint32_t *)(r0 + 72);
    /* 0x080037AC: 0422     movs     r2, #4 */
    r2 = 4;
    /* 0x080037AE: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x080037B0: 8164     str      r1, [r0, #0x48] */
    *(uint32_t *)(r0 + 72) = r1;   /* store */
    /* 0x080037B2: 03f0f9f8 bl       #0x80069a8 */
    FUN_080069A8();   /* BL -> 0x080069A8 FUN_080069A8 */
    /* 0x080037B6: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
