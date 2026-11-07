/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009CB4
 * Name      : FUN_08009CB4
 * End       : 0x08009D14
 * Size      : 96 bytes, 39 instructions decoded in body (functions.json insns=39, blocks=5)
 * Class     : function
 * Callers   : 2  -> 0x08006014 FUN_08006014, 0x0800607E FUN_0800607E
 * Callees   : 4  -> 0x08007426 FUN_08007426, 0x08008738 FUN_08008738, 0x0800B5EC FUN_0800B5EC, 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009CB4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009CB4: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08009CB6: 174c     ldr      r4, [pc, #0x5c]  ; =RAM_200001C4 */
    r4 = *(uint32_t *)0x08009D14;   /* = RAM 0x200001C4 */
    /* 0x08009CB8: 2088     ldrh     r0, [r4] */
    r0 = *(uint16_t *)(r4);
    /* 0x08009CBA: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009CBC: 29d0     beq      #0x8009d12 */
    if (r0 == 0) goto L_08009D12;
    /* 0x08009CBE: 0925     movs     r5, #9 */
    r5 = 9;
    /* 0x08009CC0: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08009CC2: ed06     lsls     r5, r5, #0x1b */
    r5 = r5 << 0x1b;
    /* 0x08009CC4: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x08009CC6: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08009CC8: fdf7adfb bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009CCC: 01f08efc bl       #0x800b5ec */
    FUN_0800B5EC();   /* BL -> 0x0800B5EC FUN_0800B5EC */
    /* 0x08009CD0: 1148     ldr      r0, [pc, #0x44]  ; =RAM_20000165 */
    r0 = *(uint32_t *)0x08009D18;   /* = RAM 0x20000165 */
    /* 0x08009CD2: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08009CD4: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08009CD6: 02d9     bls      #0x8009cde */
    if (r0 <= 1) goto L_08009CDE;   /* unsigned */
    /* 0x08009CD8: 1049     ldr      r1, [pc, #0x40]  ; =RAM_20000134 */
    r1 = *(uint32_t *)0x08009D1C;   /* = RAM 0x20000134 */
    /* 0x08009CDA: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08009CDC: c870     strb     r0, [r1, #3] */
    *(uint8_t *)(r1 + 3) = r0;   /* store */

    L_08009CDE:
    /* 0x08009CDE: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08009CE0: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x08009CE4: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x08009CE6: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08009CE8: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08009CEA: fdf79cfb bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009CEE: 2088     ldrh     r0, [r4] */
    r0 = *(uint16_t *)(r4);
    /* 0x08009CF0: 02f0a2fe bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08009CF4: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08009CF6: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08009CF8: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08009CFA: fdf794fb bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009CFE: 0848     ldr      r0, [pc, #0x20]  ; =0x00000BB8 */
    r0 = *(uint32_t *)0x08009D20;   /* = 0x00000BB8 */
    /* 0x08009D00: 02f09afe bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08009D04: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08009D06: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x08009D0A: 0649     ldr      r1, [pc, #0x18]  ; =RAM_200001BB */
    r1 = *(uint32_t *)0x08009D24;   /* = RAM 0x200001BB */
    /* 0x08009D0C: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08009D0E: fef713fd bl       #0x8008738 */
    FUN_08008738();   /* BL -> 0x08008738 FUN_08008738 */

    L_08009D12:
    /* 0x08009D12: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
