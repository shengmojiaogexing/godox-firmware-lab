/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B344
 * Name      : FUN_0800B344
 * End       : 0x0800B38A
 * Size      : 70 bytes, 28 instructions decoded in body (functions.json insns=28, blocks=3)
 * Class     : function
 * Callers   : 2  -> 0x0800AA74 FUN_0800AA74, 0x0800AB2E FUN_0800AB2E
 * Callees   : 3  -> 0x08007426 FUN_08007426, 0x080074F0 FUN_080074F0, 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B344(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B344: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800B346: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800B348: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800B34C: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x0800B34E: 0925     movs     r5, #9 */
    r5 = 9;
    /* 0x0800B350: 0e4e     ldr      r6, [pc, #0x38]  ; =0x00003A98 */
    r6 = *(uint32_t *)0x0800B38C;   /* = 0x00003A98 */
    /* 0x0800B352: ed06     lsls     r5, r5, #0x1b */
    r5 = r5 << 0x1b;

    L_0800B354:
    /* 0x0800B354: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800B356: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800B358: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800B35A: fcf764f8 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800B35E: 3c20     movs     r0, #0x3c */
    r0 = 0x3c;
    /* 0x0800B360: 01f06afb bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800B364: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800B366: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800B368: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800B36A: fcf75cf8 bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800B36E: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x0800B370: 01f062fb bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800B374: 0648     ldr      r0, [pc, #0x18]  ; =RAM_20000384 */
    r0 = *(uint32_t *)0x0800B390;   /* = RAM 0x20000384 */
    /* 0x0800B376: fcf7bbf8 bl       #0x80074f0 */
    FUN_080074F0();   /* BL -> 0x080074F0 FUN_080074F0 */
    /* 0x0800B37A: 641c     adds     r4, r4, #1 */
    r4 = r4 + 1;
    /* 0x0800B37C: e4b2     uxtb     r4, r4 */
    r4 = uxtb(r4);
    /* 0x0800B37E: 3c2c     cmp      r4, #0x3c */
    /* flags = cmp(r4, #0x3c) */
    /* 0x0800B380: e8d3     blo      #0x800b354 */
    if (r4 < 0x3c) goto L_0800B354;   /* unsigned */
    /* 0x0800B382: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800B384: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800B388: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
