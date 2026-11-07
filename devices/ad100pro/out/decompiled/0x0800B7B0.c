/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B7B0
 * Name      : FUN_0800B7B0
 * End       : 0x0800B7E4
 * Size      : 52 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=8)
 * Class     : function
 * Callers   : 2  -> 0x0800CF74 main, 0x0800D014 FUN_0800D014
 * Callees   : 2  -> 0x0800D0E4 FUN_0800D0E4, 0x0800D108 FUN_0800D108
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B7B0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B7B0: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800B7B2: 0c48     ldr      r0, [pc, #0x30]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800B7E4;   /* = RAM 0x20000175 */
    /* 0x0800B7B4: 0c4c     ldr      r4, [pc, #0x30]  ; =RAM_200001E0 */
    r4 = *(uint32_t *)0x0800B7E8;   /* = RAM 0x200001E0 */
    /* 0x0800B7B6: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800B7B8: 8107     lsls     r1, r0, #0x1e */
    r1 = r0 << 0x1e;
    /* 0x0800B7BA: 890f     lsrs     r1, r1, #0x1e */
    r1 = r1 >> 0x1e;
    /* 0x0800B7BC: e079     ldrb     r0, [r4, #7] */
    r0 = *(uint8_t *)(r4 + 7);
    /* 0x0800B7BE: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x0800B7C0: 09d1     bne      #0x800b7d6 */
    if (r1 != 1) goto L_0800B7D6;
    /* 0x0800B7C2: 0a49     ldr      r1, [pc, #0x28]  ; =RAM_20000480 */
    r1 = *(uint32_t *)0x0800B7EC;   /* = RAM 0x20000480 */
    /* 0x0800B7C4: 4978     ldrb     r1, [r1, #1] */
    r1 = *(uint8_t *)(r1 + 1);
    /* 0x0800B7C6: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x0800B7C8: 05d0     beq      #0x800b7d6 */
    if (r1 == 0) goto L_0800B7D6;
    /* 0x0800B7CA: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800B7CC: 09d1     bne      #0x800b7e2 */
    if (r0 != 0) goto L_0800B7E2;
    /* 0x0800B7CE: 01f089fc bl       #0x800d0e4 */
    FUN_0800D0E4();   /* BL -> 0x0800D0E4 FUN_0800D0E4 */
    /* 0x0800B7D2: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800B7D4: 04e0     b        #0x800b7e0 */
    goto L_0800B7E0;

    L_0800B7D6:
    /* 0x0800B7D6: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800B7D8: 03d1     bne      #0x800b7e2 */
    if (r0 != 1) goto L_0800B7E2;
    /* 0x0800B7DA: 01f095fc bl       #0x800d108 */
    FUN_0800D108();   /* BL -> 0x0800D108 FUN_0800D108 */
    /* 0x0800B7DE: 0020     movs     r0, #0 */
    r0 = 0;

    L_0800B7E0:
    /* 0x0800B7E0: e071     strb     r0, [r4, #7] */
    *(uint8_t *)(r4 + 7) = r0;   /* store */

    L_0800B7E2:
    /* 0x0800B7E2: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
