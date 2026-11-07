/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005A24
 * Name      : FUN_08005A24
 * End       : 0x08005A6E
 * Size      : 74 bytes, 29 instructions decoded in body (functions.json insns=29, blocks=1)
 * Class     : function
 * Callers   : 2  -> 0x08006014 FUN_08006014, 0x0800609C FUN_0800609C
 * Callees   : 2  -> 0x08007426 FUN_08007426, 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005A24(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005A24: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08005A26: 0924     movs     r4, #9 */
    r4 = 9;
    /* 0x08005A28: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08005A2A: e406     lsls     r4, r4, #0x1b */
    r4 = r4 << 0x1b;
    /* 0x08005A2C: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x08005A2E: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08005A30: 01f0f9fc bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08005A34: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08005A36: 81f31088 msr      primask, r1 */
    /* TODO: msr primask, r1 */
    /* 0x08005A3A: 0a46     mov      r2, r1 */
    r2 = r1;
    /* 0x08005A3C: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08005A3E: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08005A40: 01f0f1fc bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08005A44: 6420     movs     r0, #0x64 */
    r0 = 0x64;
    /* 0x08005A46: 06f0f7ff bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08005A4A: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08005A4C: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08005A4E: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08005A50: 01f0e9fc bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08005A54: 6420     movs     r0, #0x64 */
    r0 = 0x64;
    /* 0x08005A56: 06f0efff bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08005A5A: 0549     ldr      r1, [pc, #0x14]  ; =RAM_200001DB */
    r1 = *(uint32_t *)0x08005A70;   /* = RAM 0x200001DB */
    /* 0x08005A5C: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x08005A5E: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08005A60: 0448     ldr      r0, [pc, #0x10]  ; =0x00000BB8 */
    r0 = *(uint32_t *)0x08005A74;   /* = 0x00000BB8 */
    /* 0x08005A62: 06f0e9ff bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08005A66: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005A68: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x08005A6C: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
