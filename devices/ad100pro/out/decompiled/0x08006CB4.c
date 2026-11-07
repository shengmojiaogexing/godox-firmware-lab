/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006CB4
 * Name      : FUN_08006CB4
 * End       : 0x08006CE8
 * Size      : 52 bytes, 26 instructions decoded in body (functions.json insns=26, blocks=3)
 * Class     : function
 * Callers   : 3  -> 0x080093A8 FUN_080093A8, 0x080093BC FUN_080093BC, 0x080093D4 FUN_080093D4
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006CB4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006CB4: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08006CB6: 4379     ldrb     r3, [r0, #5] */
    r3 = *(uint8_t *)(r0 + 5);
    /* 0x08006CB8: 012b     cmp      r3, #1 */
    /* flags = cmp(r3, #1) */
    /* 0x08006CBA: 13d0     beq      #0x8006ce4 */
    if (r3 == 1) goto L_08006CE4;
    /* 0x08006CBC: 0125     movs     r5, #1 */
    r5 = 1;
    /* 0x08006CBE: 4571     strb     r5, [r0, #5] */
    *(uint8_t *)(r0 + 5) = r5;   /* store */
    /* 0x08006CC0: 0223     movs     r3, #2 */
    r3 = 2;
    /* 0x08006CC2: 0371     strb     r3, [r0, #4] */
    *(uint8_t *)(r0 + 4) = r3;   /* store */
    /* 0x08006CC4: 0668     ldr      r6, [r0] */
    r6 = *(uint32_t *)(r0);
    /* 0x08006CC6: 3468     ldr      r4, [r6] */
    r4 = *(uint32_t *)(r6);
    /* 0x08006CC8: 074b     ldr      r3, [pc, #0x1c]  ; =0x00000FFE */
    r3 = *(uint32_t *)0x08006CE8;   /* = 0x00000FFE */
    /* 0x08006CCA: 9340     lsls     r3, r2 */
    r3 = r3 << r2;
    /* 0x08006CCC: 9c43     bics     r4, r3 */
    r4 = r4 & ~r3;
    /* 0x08006CCE: 0b68     ldr      r3, [r1] */
    r3 = *(uint32_t *)(r1);
    /* 0x08006CD0: 4968     ldr      r1, [r1, #4] */
    r1 = *(uint32_t *)(r1 + 4);
    /* 0x08006CD2: 0b43     orrs     r3, r1 */
    r3 = r3 | r1;
    /* 0x08006CD4: 9340     lsls     r3, r2 */
    r3 = r3 << r2;
    /* 0x08006CD6: 2343     orrs     r3, r4 */
    r3 = r3 | r4;
    /* 0x08006CD8: 3360     str      r3, [r6] */
    *(uint32_t *)(r6) = r3;   /* store */
    /* 0x08006CDA: 0571     strb     r5, [r0, #4] */
    *(uint8_t *)(r0 + 4) = r5;   /* store */
    /* 0x08006CDC: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x08006CDE: 4171     strb     r1, [r0, #5] */
    *(uint8_t *)(r0 + 5) = r1;   /* store */
    /* 0x08006CE0: 0846     mov      r0, r1 */
    r0 = r1;
    /* 0x08006CE2: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08006CE4:
    /* 0x08006CE4: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08006CE6: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
