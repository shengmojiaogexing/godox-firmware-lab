/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080044D4
 * Name      : FUN_080044D4
 * End       : 0x08004514
 * Size      : 64 bytes, 32 instructions decoded in body (functions.json insns=32, blocks=4)
 * Class     : function
 * Callers   : 6  -> 0x08003F0C FUN_08003F0C, 0x0800400A FUN_0800400A, 0x080040A2 FUN_080040A2, 0x08004156 FUN_08004156, 0x0800429A FUN_0800429A, 0x080042E8 FUN_080042E8
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080044D4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080044D4: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x080044D6: 0f49     ldr      r1, [pc, #0x3c]  ; =RAM_2000017A */
    r1 = *(uint32_t *)0x08004514;   /* = RAM 0x2000017A */
    /* 0x080044D8: 4623     movs     r3, #0x46 */
    r3 = 0x46;
    /* 0x080044DA: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x080044DC: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x080044DE: 18d1     bne      #0x8004512 */
    if (r1 != 1) goto L_08004512;
    /* 0x080044E0: 0d4c     ldr      r4, [pc, #0x34]  ; =RAM_200004CC */
    r4 = *(uint32_t *)0x08004518;   /* = RAM 0x200004CC */
    /* 0x080044E2: 2a22     movs     r2, #0x2a */
    r2 = 0x2a;
    /* 0x080044E4: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x080044E6: 3f25     movs     r5, #0x3f */
    r5 = 0x3f;
    /* 0x080044E8: 0002     lsls     r0, r0, #8 */
    r0 = r0 << 8;
    /* 0x080044EA: 0419     adds     r4, r0, r4 */
    r4 = r0 + r4;

    L_080044EC:
    /* 0x080044EC: 5818     adds     r0, r3, r1 */
    r0 = r3 + r1;
    /* 0x080044EE: 2618     adds     r6, r4, r0 */
    r6 = r4 + r0;
    /* 0x080044F0: f01d     adds     r0, r6, #7 */
    r0 = r6 + 7;
    /* 0x080044F2: ff30     adds     r0, #0xff */
    r0 = r0 + 0xff;
    /* 0x080044F4: fa30     adds     r0, #0xfa */
    r0 = r0 + 0xfa;
    /* 0x080044F6: 0778     ldrb     r7, [r0] */
    r7 = *(uint8_t *)(r0);
    /* 0x080044F8: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x080044FA: 6f40     eors     r7, r5 */
    r7 = r7 ^ r5;
    /* 0x080044FC: 0770     strb     r7, [r0] */
    *(uint8_t *)(r0) = r7;   /* store */
    /* 0x080044FE: 0520     movs     r0, #5 */
    r0 = 5;
    /* 0x08004500: c001     lsls     r0, r0, #7 */
    r0 = r0 << 7;
    /* 0x08004502: 3018     adds     r0, r6, r0 */
    r0 = r6 + r0;
    /* 0x08004504: 0678     ldrb     r6, [r0] */
    r6 = *(uint8_t *)(r0);
    /* 0x08004506: ff27     movs     r7, #0xff */
    r7 = 0xff;
    /* 0x08004508: 7e40     eors     r6, r7 */
    r6 = r6 ^ r7;
    /* 0x0800450A: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x0800450C: 0670     strb     r6, [r0] */
    *(uint8_t *)(r0) = r6;   /* store */
    /* 0x0800450E: 9142     cmp      r1, r2 */
    /* flags = cmp(r1, r2) */
    /* 0x08004510: ecd3     blo      #0x80044ec */
    if (r1 < r2) goto L_080044EC;   /* unsigned */

    L_08004512:
    /* 0x08004512: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
