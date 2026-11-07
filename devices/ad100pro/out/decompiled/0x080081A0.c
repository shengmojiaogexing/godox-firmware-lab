/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080081A0
 * Name      : FUN_080081A0
 * End       : 0x080081E8
 * Size      : 72 bytes, 36 instructions decoded in body (functions.json insns=36, blocks=3)
 * Class     : function
 * Callers   : 5  -> 0x080096A8 FUN_080096A8, 0x08009708 FUN_08009708, 0x08009768 FUN_08009768, 0x08009D84 FUN_08009D84, 0x08009DCE FUN_08009DCE
 * Callees   : 0  -> -
 * MMIO      : SYS.+0x4, SYS.+0x8
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080081A0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080081A0: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x080081A2: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x080081A4: 2032     adds     r2, #0x20 */
    r2 = r2 + 0x20;
    /* 0x080081A6: 137e     ldrb     r3, [r2, #0x18] */
    r3 = *(uint8_t *)(r2 + 24);
    /* 0x080081A8: 012b     cmp      r3, #1 */
    /* flags = cmp(r3, #1) */
    /* 0x080081AA: 1bd0     beq      #0x80081e4 */
    if (r3 == 1) goto L_080081E4;
    /* 0x080081AC: 0123     movs     r3, #1 */
    r3 = 1;
    /* 0x080081AE: 1376     strb     r3, [r2, #0x18] */
    *(uint8_t *)(r2 + 24) = r3;   /* store */
    /* 0x080081B0: 0224     movs     r4, #2 */
    r4 = 2;
    /* 0x080081B2: 5476     strb     r4, [r2, #0x19] */
    *(uint8_t *)(r2 + 25) = r4;   /* store */
    /* 0x080081B4: 0468     ldr      r4, [r0] */
    r4 = *(uint32_t *)(r0);
    /* 0x080081B6: 6568     ldr      r5, [r4, #4] */
    r5 = *(uint32_t *)(r4 + 4);  /* MMIO: SYS.+0x4(r) */
    /* 0x080081B8: 7026     movs     r6, #0x70 */
    r6 = 0x70;
    /* 0x080081BA: b543     bics     r5, r6 */
    r5 = r5 & ~r6;
    /* 0x080081BC: 6560     str      r5, [r4, #4] */
    *(uint32_t *)(r4 + 4) = r5;   /* store */  /* MMIO: SYS.+0x4(w) */
    /* 0x080081BE: 0468     ldr      r4, [r0] */
    r4 = *(uint32_t *)(r0);
    /* 0x080081C0: 6568     ldr      r5, [r4, #4] */
    r5 = *(uint32_t *)(r4 + 4);  /* MMIO: SYS.+0x4(r) */
    /* 0x080081C2: 0e68     ldr      r6, [r1] */
    r6 = *(uint32_t *)(r1);
    /* 0x080081C4: 3543     orrs     r5, r6 */
    r5 = r5 | r6;
    /* 0x080081C6: 6560     str      r5, [r4, #4] */
    *(uint32_t *)(r4 + 4) = r5;   /* store */  /* MMIO: SYS.+0x4(w) */
    /* 0x080081C8: 0468     ldr      r4, [r0] */
    r4 = *(uint32_t *)(r0);
    /* 0x080081CA: a568     ldr      r5, [r4, #8] */
    r5 = *(uint32_t *)(r4 + 8);  /* MMIO: SYS.+0x8(r) */
    /* 0x080081CC: 8026     movs     r6, #0x80 */
    r6 = 0x80;
    /* 0x080081CE: b543     bics     r5, r6 */
    r5 = r5 & ~r6;
    /* 0x080081D0: a560     str      r5, [r4, #8] */
    *(uint32_t *)(r4 + 8) = r5;   /* store */  /* MMIO: SYS.+0x8(w) */
    /* 0x080081D2: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);
    /* 0x080081D4: 8468     ldr      r4, [r0, #8] */
    r4 = *(uint32_t *)(r0 + 8);
    /* 0x080081D6: 4968     ldr      r1, [r1, #4] */
    r1 = *(uint32_t *)(r1 + 4);
    /* 0x080081D8: 0c43     orrs     r4, r1 */
    r4 = r4 | r1;
    /* 0x080081DA: 8460     str      r4, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r4;   /* store */
    /* 0x080081DC: 5376     strb     r3, [r2, #0x19] */
    *(uint8_t *)(r2 + 25) = r3;   /* store */
    /* 0x080081DE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080081E0: 1076     strb     r0, [r2, #0x18] */
    *(uint8_t *)(r2 + 24) = r0;   /* store */
    /* 0x080081E2: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_080081E4:
    /* 0x080081E4: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x080081E6: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
