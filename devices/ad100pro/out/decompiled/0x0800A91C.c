/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A91C
 * Name      : FUN_0800A91C
 * End       : 0x0800A930
 * Size      : 20 bytes, 9 instructions decoded in body (functions.json insns=9, blocks=3)
 * Class     : function
 * Callers   : 2  -> 0x08008D00 dispatcher_large, 0x08008F4E FUN_08008F4E
 * Callees   : 1  -> 0x0800A934 FUN_0800A934
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A91C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A91C: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800A91E: 044c     ldr      r4, [pc, #0x10]  ; =RAM_20000184 */
    r4 = *(uint32_t *)0x0800A930;   /* = RAM 0x20000184 */
    /* 0x0800A920: e078     ldrb     r0, [r4, #3] */
    r0 = *(uint8_t *)(r4 + 3);
    /* 0x0800A922: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800A924: 03d1     bne      #0x800a92e */
    if (r0 != 0) goto L_0800A92E;
    /* 0x0800A926: 00f005f8 bl       #0x800a934 */
    FUN_0800A934();   /* BL -> 0x0800A934 FUN_0800A934 */
    /* 0x0800A92A: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800A92C: e070     strb     r0, [r4, #3] */
    *(uint8_t *)(r4 + 3) = r0;   /* store */

    L_0800A92E:
    /* 0x0800A92E: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
