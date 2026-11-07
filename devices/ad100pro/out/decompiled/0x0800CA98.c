/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CA98
 * Name      : FUN_0800CA98
 * End       : 0x0800CAB2
 * Size      : 26 bytes, 12 instructions decoded in body (functions.json insns=12, blocks=4)
 * Class     : hub
 * Callers   : 14  -> 0x0800A934 FUN_0800A934, 0x0800A9BC FUN_0800A9BC, 0x0800A9E2 FUN_0800A9E2, 0x0800AA12 FUN_0800AA12, 0x0800B9C0 FUN_0800B9C0, 0x0800B9CC FUN_0800B9CC, 0x0800C080 FUN_0800C080, 0x0800C0B4 FUN_0800C0B4, ... +6
 * Callees   : 1  -> 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CA98(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CA98: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800CA9A: 0346     mov      r3, r0 */
    r3 = r0;
    /* 0x0800CA9C: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800CA9E: 054c     ldr      r4, [pc, #0x14]  ; =0x000003DE */
    r4 = *(uint32_t *)0x0800CAB4;   /* = 0x000003DE */
    /* 0x0800CAA0: 04e0     b        #0x800caac */
    goto L_0800CAAC;

    L_0800CAA2:
    /* 0x0800CAA2: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800CAA4: fff7c8ff bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800CAA8: 521c     adds     r2, r2, #1 */
    r2 = r2 + 1;
    /* 0x0800CAAA: 92b2     uxth     r2, r2 */
    r2 = uxth(r2);

    L_0800CAAC:
    /* 0x0800CAAC: 9a42     cmp      r2, r3 */
    /* flags = cmp(r2, r3) */
    /* 0x0800CAAE: f8d3     blo      #0x800caa2 */
    if (r2 < r3) goto L_0800CAA2;   /* unsigned */
    /* 0x0800CAB0: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
