/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B588
 * Name      : FUN_0800B588
 * End       : 0x0800B5B6
 * Size      : 46 bytes, 21 instructions decoded in body (functions.json insns=21, blocks=3)
 * Class     : function
 * Callers   : 4  -> 0x080097F8 FUN_080097F8, 0x0800980E FUN_0800980E, 0x0800AD44 FUN_0800AD44, 0x0800AF32 FUN_0800AF32
 * Callees   : 2  -> 0x08004690 FUN_08004690, 0x08005EF4 FUN_08005EF4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B588(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B588: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800B58A: 0446     mov      r4, r0 */
    r4 = r0;
    /* 0x0800B58C: 0a48     ldr      r0, [pc, #0x28]  ; =RAM_20000454 */
    r0 = *(uint32_t *)0x0800B5B8;   /* = RAM 0x20000454 */
    /* 0x0800B58E: f9f77ff8 bl       #0x8004690 */
    FUN_08004690();   /* BL -> 0x08004690 FUN_08004690 */
    /* 0x0800B592: 0948     ldr      r0, [pc, #0x24]  ; =RAM_20000454 */
    r0 = *(uint32_t *)0x0800B5B8;   /* = RAM 0x20000454 */
    /* 0x0800B594: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800B596: 0346     mov      r3, r0 */
    r3 = r0;
    /* 0x0800B598: 203b     subs     r3, #0x20 */
    r3 = r3 - 0x20;

    L_0800B59A:
    /* 0x0800B59A: 8a00     lsls     r2, r1, #2 */
    r2 = r1 << 2;
    /* 0x0800B59C: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x0800B59E: 8558     ldr      r5, [r0, r2] */
    r5 = *(uint32_t *)(r0 + r2);
    /* 0x0800B5A0: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x0800B5A2: 9d50     str      r5, [r3, r2] */
    *(uint32_t *)(r3 + r2) = r5;   /* store */
    /* 0x0800B5A4: 0829     cmp      r1, #8 */
    /* flags = cmp(r1, #8) */
    /* 0x0800B5A6: f8d3     blo      #0x800b59a */
    if (r1 < 8) goto L_0800B59A;   /* unsigned */
    /* 0x0800B5A8: 6420     movs     r0, #0x64 */
    r0 = 0x64;
    /* 0x0800B5AA: 4443     muls     r4, r0, r4 */
    r4 = r0 * r4;
    /* 0x0800B5AC: 0348     ldr      r0, [pc, #0xc]  ; =0x0801F000 */
    r0 = *(uint32_t *)0x0800B5BC;   /* = 0x0801F000 */
    /* 0x0800B5AE: 2018     adds     r0, r4, r0 */
    r0 = r4 + r0;
    /* 0x0800B5B0: faf7a0fc bl       #0x8005ef4 */
    FUN_08005EF4();   /* BL -> 0x08005EF4 FUN_08005EF4 */
    /* 0x0800B5B4: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
