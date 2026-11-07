/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004B74
 * Name      : FUN_08004B74
 * End       : 0x08004BD4
 * Size      : 96 bytes, 48 instructions decoded in body (functions.json insns=48, blocks=3)
 * Class     : function
 * Callers   : 4  -> 0x08008D00 dispatcher_large, 0x08008F9E FUN_08008F9E, 0x0800AD44 FUN_0800AD44, 0x0800AD64 FUN_0800AD64
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004B74(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004B74: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08004B76: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08004B78: 1649     ldr      r1, [pc, #0x58]  ; =RAM_20000175 */
    r1 = *(uint32_t *)0x08004BD4;   /* = RAM 0x20000175 */
    /* 0x08004B7A: 174c     ldr      r4, [pc, #0x5c]  ; =RAM_2000019C */
    r4 = *(uint32_t *)0x08004BD8;   /* = RAM 0x2000019C */
    /* 0x08004B7C: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08004B7E: 174b     ldr      r3, [pc, #0x5c]  ; =RAM_200001A2 */
    r3 = *(uint32_t *)0x08004BDC;   /* = RAM 0x200001A2 */
    /* 0x08004B80: 0146     mov      r1, r0 */
    r1 = r0;
    /* 0x08004B82: 5022     movs     r2, #0x50 */
    r2 = 0x50;

    L_08004B84:
    /* 0x08004B84: 6254     strb     r2, [r4, r1] */
    *(uint8_t *)(r4 + r1) = r2;   /* store */
    /* 0x08004B86: 5a54     strb     r2, [r3, r1] */
    *(uint8_t *)(r3 + r1) = r2;   /* store */
    /* 0x08004B88: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x08004B8A: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x08004B8C: 0629     cmp      r1, #6 */
    /* flags = cmp(r1, #6) */
    /* 0x08004B8E: f9d3     blo      #0x8004b84 */
    if (r1 < 6) goto L_08004B84;   /* unsigned */
    /* 0x08004B90: 1349     ldr      r1, [pc, #0x4c]  ; =RAM_20000165 */
    r1 = *(uint32_t *)0x08004BE0;   /* = RAM 0x20000165 */
    /* 0x08004B92: 0522     movs     r2, #5 */
    r2 = 5;
    /* 0x08004B94: 0a70     strb     r2, [r1] */
    *(uint8_t *)(r1) = r2;   /* store */
    /* 0x08004B96: 1349     ldr      r1, [pc, #0x4c]  ; =RAM_20000166 */
    r1 = *(uint32_t *)0x08004BE4;   /* = RAM 0x20000166 */
    /* 0x08004B98: 0123     movs     r3, #1 */
    r3 = 1;
    /* 0x08004B9A: 0a70     strb     r2, [r1] */
    *(uint8_t *)(r1) = r2;   /* store */
    /* 0x08004B9C: 1249     ldr      r1, [pc, #0x48]  ; =RAM_200001B1 */
    r1 = *(uint32_t *)0x08004BE8;   /* = RAM 0x200001B1 */
    /* 0x08004B9E: 0224     movs     r4, #2 */
    r4 = 2;
    /* 0x08004BA0: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08004BA2: 1249     ldr      r1, [pc, #0x48]  ; =RAM_20000480 */
    r1 = *(uint32_t *)0x08004BEC;   /* = RAM 0x20000480 */
    /* 0x08004BA4: 0b70     strb     r3, [r1] */
    *(uint8_t *)(r1) = r3;   /* store */
    /* 0x08004BA6: 4870     strb     r0, [r1, #1] */
    *(uint8_t *)(r1 + 1) = r0;   /* store */
    /* 0x08004BA8: 8c70     strb     r4, [r1, #2] */
    *(uint8_t *)(r1 + 2) = r4;   /* store */
    /* 0x08004BAA: c870     strb     r0, [r1, #3] */
    *(uint8_t *)(r1 + 3) = r0;   /* store */
    /* 0x08004BAC: 0b71     strb     r3, [r1, #4] */
    *(uint8_t *)(r1 + 4) = r3;   /* store */
    /* 0x08004BAE: 4871     strb     r0, [r1, #5] */
    *(uint8_t *)(r1 + 5) = r0;   /* store */
    /* 0x08004BB0: 8871     strb     r0, [r1, #6] */
    *(uint8_t *)(r1 + 6) = r0;   /* store */
    /* 0x08004BB2: 0f49     ldr      r1, [pc, #0x3c]  ; =RAM_2000017C */
    r1 = *(uint32_t *)0x08004BF0;   /* = RAM 0x2000017C */
    /* 0x08004BB4: 0b70     strb     r3, [r1] */
    *(uint8_t *)(r1) = r3;   /* store */
    /* 0x08004BB6: 0f4b     ldr      r3, [pc, #0x3c]  ; =RAM_20000183 */
    r3 = *(uint32_t *)0x08004BF4;   /* = RAM 0x20000183 */
    /* 0x08004BB8: 1421     movs     r1, #0x14 */
    r1 = 0x14;
    /* 0x08004BBA: 1970     strb     r1, [r3] */
    *(uint8_t *)(r3) = r1;   /* store */
    /* 0x08004BBC: 0e49     ldr      r1, [pc, #0x38]  ; =RAM_2000017D */
    r1 = *(uint32_t *)0x08004BF8;   /* = RAM 0x2000017D */
    /* 0x08004BBE: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08004BC0: 0e49     ldr      r1, [pc, #0x38]  ; =RAM_20000169 */
    r1 = *(uint32_t *)0x08004BFC;   /* = RAM 0x20000169 */
    /* 0x08004BC2: 0a70     strb     r2, [r1] */
    *(uint8_t *)(r1) = r2;   /* store */
    /* 0x08004BC4: 0e49     ldr      r1, [pc, #0x38]  ; =RAM_20000231 */
    r1 = *(uint32_t *)0x08004C00;   /* = RAM 0x20000231 */
    /* 0x08004BC6: 0f4a     ldr      r2, [pc, #0x3c]  ; =RAM_2000016B */
    r2 = *(uint32_t *)0x08004C04;   /* = RAM 0x2000016B */
    /* 0x08004BC8: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08004BCA: 3721     movs     r1, #0x37 */
    r1 = 0x37;
    /* 0x08004BCC: 1170     strb     r1, [r2] */
    *(uint8_t *)(r2) = r1;   /* store */
    /* 0x08004BCE: 0e49     ldr      r1, [pc, #0x38]  ; =RAM_2000016A */
    r1 = *(uint32_t *)0x08004C08;   /* = RAM 0x2000016A */
    /* 0x08004BD0: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08004BD2: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
