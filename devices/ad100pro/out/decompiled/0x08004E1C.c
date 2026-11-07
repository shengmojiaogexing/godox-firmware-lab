/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004E1C
 * Name      : FUN_08004E1C
 * End       : 0x08004E44
 * Size      : 40 bytes, 20 instructions decoded in body (functions.json insns=20, blocks=4)
 * Class     : function
 * Callers   : 2  -> 0x08004C6C FUN_08004C6C, 0x08004C88 FUN_08004C88
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004E1C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004E1C: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08004E1E: d400     lsls     r4, r2, #3 */
    r4 = r2 << 3;
    /* 0x08004E20: 1419     adds     r4, r2, r4 */
    r4 = r2 + r4;
    /* 0x08004E22: 084a     ldr      r2, [pc, #0x20]  ; =flash_0800DB0C */
    r2 = *(uint32_t *)0x08004E44;   /* = flash data/const 0x0800DB0C */
    /* 0x08004E24: 084e     ldr      r6, [pc, #0x20]  ; =RAM_200004CC */
    r6 = *(uint32_t *)0x08004E48;   /* = RAM 0x200004CC */
    /* 0x08004E26: a518     adds     r5, r4, r2 */
    r5 = r4 + r2;
    /* 0x08004E28: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x08004E2A: c001     lsls     r0, r0, #7 */
    r0 = r0 << 7;
    /* 0x08004E2C: 125d     ldrb     r2, [r2, r4] */
    r2 = *(uint8_t *)(r2 + r4);
    /* 0x08004E2E: 8019     adds     r0, r0, r6 */
    r0 = r0 + r6;
    /* 0x08004E30: 05e0     b        #0x8004e3e */
    goto L_08004E3E;

    L_08004E32:
    /* 0x08004E32: ee18     adds     r6, r5, r3 */
    r6 = r5 + r3;
    /* 0x08004E34: cc18     adds     r4, r1, r3 */
    r4 = r1 + r3;
    /* 0x08004E36: 7678     ldrb     r6, [r6, #1] */
    r6 = *(uint8_t *)(r6 + 1);
    /* 0x08004E38: 5b1c     adds     r3, r3, #1 */
    r3 = r3 + 1;
    /* 0x08004E3A: 0655     strb     r6, [r0, r4] */
    *(uint8_t *)(r0 + r4) = r6;   /* store */
    /* 0x08004E3C: dbb2     uxtb     r3, r3 */
    r3 = uxtb(r3);

    L_08004E3E:
    /* 0x08004E3E: 9a42     cmp      r2, r3 */
    /* flags = cmp(r2, r3) */
    /* 0x08004E40: f7d8     bhi      #0x8004e32 */
    if (r2 > r3) goto L_08004E32;   /* unsigned */
    /* 0x08004E42: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
