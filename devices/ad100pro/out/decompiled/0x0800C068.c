/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C068
 * Name      : FUN_0800C068
 * End       : 0x0800C07C
 * Size      : 20 bytes, 9 instructions decoded in body (functions.json insns=9, blocks=3)
 * Class     : function
 * Callers   : 2  -> 0x08008D00 dispatcher_large, 0x08008F9E FUN_08008F9E
 * Callees   : 1  -> 0x08006014 FUN_08006014
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C068(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C068: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800C06A: 044c     ldr      r4, [pc, #0x10]  ; =RAM_20000164 */
    r4 = *(uint32_t *)0x0800C07C;   /* = RAM 0x20000164 */
    /* 0x0800C06C: e07e     ldrb     r0, [r4, #0x1b] */
    r0 = *(uint8_t *)(r4 + 27);
    /* 0x0800C06E: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800C070: 03d1     bne      #0x800c07a */
    if (r0 != 0) goto L_0800C07A;
    /* 0x0800C072: f9f7cfff bl       #0x8006014 */
    FUN_08006014();   /* BL -> 0x08006014 FUN_08006014 */
    /* 0x0800C076: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C078: a077     strb     r0, [r4, #0x1e] */
    *(uint8_t *)(r4 + 30) = r0;   /* store */

    L_0800C07A:
    /* 0x0800C07A: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
