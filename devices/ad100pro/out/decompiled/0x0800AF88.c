/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800AF88
 * Name      : FUN_0800AF88
 * End       : 0x0800AFB8
 * Size      : 48 bytes, 23 instructions decoded in body (functions.json insns=23, blocks=6)
 * Class     : hub
 * Callers   : 13  -> 0x08003C50 FUN_08003C50, 0x08003C66 FUN_08003C66, 0x08005C44 FUN_08005C44, 0x08005C58 FUN_08005C58, 0x08009060 FUN_08009060, 0x08009E30 FUN_08009E30, 0x08009E44 FUN_08009E44, 0x0800A4E8 FUN_0800A4E8, ... +5
 * Callees   : 1  -> 0x080030F0 FUN_080030F0
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800AF88(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800AF88: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800AF8A: 0b4b     ldr      r3, [pc, #0x2c]  ; =RAM_20000175 */
    r3 = *(uint32_t *)0x0800AFB8;   /* = RAM 0x20000175 */
    /* 0x0800AF8C: 1b78     ldrb     r3, [r3] */
    r3 = *(uint8_t *)(r3);
    /* 0x0800AF8E: 9b07     lsls     r3, r3, #0x1e */
    r3 = r3 << 0x1e;
    /* 0x0800AF90: 9b0f     lsrs     r3, r3, #0x1e */
    r3 = r3 >> 0x1e;
    /* 0x0800AF92: 012b     cmp      r3, #1 */
    /* flags = cmp(r3, #1) */
    /* 0x0800AF94: 03d1     bne      #0x800af9e */
    if (r3 != 1) goto L_0800AF9E;
    /* 0x0800AF96: 094b     ldr      r3, [pc, #0x24]  ; =RAM_20000480 */
    r3 = *(uint32_t *)0x0800AFBC;   /* = RAM 0x20000480 */
    /* 0x0800AF98: 9b79     ldrb     r3, [r3, #6] */
    r3 = *(uint8_t *)(r3 + 6);
    /* 0x0800AF9A: 012b     cmp      r3, #1 */
    /* flags = cmp(r3, #1) */
    /* 0x0800AF9C: 0bd0     beq      #0x800afb6 */
    if (r3 == 1) goto L_0800AFB6;

    L_0800AF9E:
    /* 0x0800AF9E: 9142     cmp      r1, r2 */
    /* flags = cmp(r1, r2) */
    /* 0x0800AFA0: 09d2     bhs      #0x800afb6 */
    if (r1 >= r2) goto L_0800AFB6;   /* unsigned */
    /* 0x0800AFA2: 5243     muls     r2, r2, r2 */
    r2 = r2 * r2;
    /* 0x0800AFA4: 5043     muls     r0, r2, r0 */
    r0 = r2 * r0;
    /* 0x0800AFA6: 4943     muls     r1, r1, r1 */
    r1 = r1 * r1;
    /* 0x0800AFA8: f8f7a2f8 bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x0800AFAC: 0146     mov      r1, r0 */
    r1 = r0;
    /* 0x0800AFAE: 0448     ldr      r0, [pc, #0x10]  ; =0x0000157C */
    r0 = *(uint32_t *)0x0800AFC0;   /* = 0x0000157C */
    /* 0x0800AFB0: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x0800AFB2: 00d2     bhs      #0x800afb6 */
    if (r1 >= r0) goto L_0800AFB6;   /* unsigned */
    /* 0x0800AFB4: 88b2     uxth     r0, r1 */
    r0 = uxth(r1);

    L_0800AFB6:
    /* 0x0800AFB6: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
