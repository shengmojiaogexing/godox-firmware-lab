/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009BCC
 * Name      : FUN_08009BCC
 * End       : 0x08009BE6
 * Size      : 26 bytes, 12 instructions decoded in body (functions.json insns=12, blocks=1)
 * Class     : function
 * Callers   : 4  -> 0x0800AA74 FUN_0800AA74, 0x0800AB22 FUN_0800AB22, 0x0800B61C FUN_0800B61C, 0x0800B658 FUN_0800B658
 * Callees   : 1  -> 0x08009E04 FUN_08009E04
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009BCC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009BCC: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08009BCE: 0648     ldr      r0, [pc, #0x18]  ; =RAM_200001AF */
    r0 = *(uint32_t *)0x08009BE8;   /* = RAM 0x200001AF */
    /* 0x08009BD0: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x08009BD2: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */
    /* 0x08009BD4: 0548     ldr      r0, [pc, #0x14]  ; =RAM_200001B3 */
    r0 = *(uint32_t *)0x08009BEC;   /* = RAM 0x200001B3 */
    /* 0x08009BD6: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */
    /* 0x08009BD8: 00f014f9 bl       #0x8009e04 */
    FUN_08009E04();   /* BL -> 0x08009E04 FUN_08009E04 */
    /* 0x08009BDC: 0448     ldr      r0, [pc, #0x10]  ; =RAM_200001B0 */
    r0 = *(uint32_t *)0x08009BF0;   /* = RAM 0x200001B0 */
    /* 0x08009BDE: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */
    /* 0x08009BE0: 0448     ldr      r0, [pc, #0x10]  ; =RAM_200001AC */
    r0 = *(uint32_t *)0x08009BF4;   /* = RAM 0x200001AC */
    /* 0x08009BE2: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */
    /* 0x08009BE4: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
