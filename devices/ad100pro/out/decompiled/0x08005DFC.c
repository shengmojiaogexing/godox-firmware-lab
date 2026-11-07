/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005DFC
 * Name      : FUN_08005DFC
 * End       : 0x08005E18
 * Size      : 28 bytes, 12 instructions decoded in body (functions.json insns=12, blocks=1)
 * Class     : function
 * Callers   : 4  -> 0x080097F8 FUN_080097F8, 0x0800980E FUN_0800980E, 0x0800AD44 FUN_0800AD44, 0x0800AD64 FUN_0800AD64
 * Callees   : 2  -> 0x08007154 FUN_08007154, 0x08007284 FUN_08007284
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005DFC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005DFC: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08005DFE: 01f041fa bl       #0x8007284 */
    FUN_08007284();   /* BL -> 0x08007284 FUN_08007284 */
    /* 0x08005E02: 0548     ldr      r0, [pc, #0x14]  ; =RAM_20000474 */
    r0 = *(uint32_t *)0x08005E18;   /* = RAM 0x20000474 */
    /* 0x08005E04: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x08005E06: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */
    /* 0x08005E08: 0449     ldr      r1, [pc, #0x10]  ; =0x0801F000 */
    r1 = *(uint32_t *)0x08005E1C;   /* = 0x0801F000 */
    /* 0x08005E0A: 4160     str      r1, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r1;   /* store */
    /* 0x08005E0C: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08005E0E: 8160     str      r1, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r1;   /* store */
    /* 0x08005E10: 0349     ldr      r1, [pc, #0xc]  ; =RAM_2000012C */
    r1 = *(uint32_t *)0x08005E20;   /* = RAM 0x2000012C */
    /* 0x08005E12: 01f09ff9 bl       #0x8007154 */
    FUN_08007154();   /* BL -> 0x08007154 FUN_08007154 */
    /* 0x08005E16: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
