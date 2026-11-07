/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009038
 * Name      : FUN_08009038
 * End       : 0x08009058
 * Size      : 32 bytes, 14 instructions decoded in body (functions.json insns=14, blocks=4)
 * Class     : function
 * Callers   : 2  -> 0x0800D134 FUN_0800D134, 0x0800D154 FUN_0800D154
 * Callees   : 2  -> 0x080074F0 FUN_080074F0, 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009038(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009038: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800903A: 0546     mov      r5, r0 */
    r5 = r0;
    /* 0x0800903C: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x0800903E: 064e     ldr      r6, [pc, #0x18]  ; =0x000003E3 */
    r6 = *(uint32_t *)0x08009058;   /* = 0x000003E3 */
    /* 0x08009040: 07e0     b        #0x8009052 */
    goto L_08009052;

    L_08009042:
    /* 0x08009042: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x08009044: 03f0f8fc bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08009048: 0448     ldr      r0, [pc, #0x10]  ; =RAM_20000384 */
    r0 = *(uint32_t *)0x0800905C;   /* = RAM 0x20000384 */
    /* 0x0800904A: fef751fa bl       #0x80074f0 */
    FUN_080074F0();   /* BL -> 0x080074F0 FUN_080074F0 */
    /* 0x0800904E: 641c     adds     r4, r4, #1 */
    r4 = r4 + 1;
    /* 0x08009050: a4b2     uxth     r4, r4 */
    r4 = uxth(r4);

    L_08009052:
    /* 0x08009052: ac42     cmp      r4, r5 */
    /* flags = cmp(r4, r5) */
    /* 0x08009054: f5d3     blo      #0x8009042 */
    if (r4 < r5) goto L_08009042;   /* unsigned */
    /* 0x08009056: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
