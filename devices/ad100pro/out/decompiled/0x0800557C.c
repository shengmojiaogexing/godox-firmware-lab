/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800557C
 * Name      : FUN_0800557C
 * End       : 0x080055FE
 * Size      : 130 bytes, 60 instructions decoded in body (functions.json insns=60, blocks=10)
 * Class     : function
 * Callers   : 1  -> 0x080054E4 FUN_080054E4
 * Callees   : 3  -> 0x080030F0 FUN_080030F0, 0x08004D74 FUN_08004D74, 0x0800C8F2 FUN_0800C8F2
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800557C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800557C: f3b5     push     {r0, r1, r4, r5, r6, r7, lr} */
    push({r0, r1, r4, r5, r6, r7, lr});
    /* 0x0800557E: 5048     ldr      r0, [pc, #0x140]  ; =RAM_2000017C */
    r0 = *(uint32_t *)0x080056C0;   /* = RAM 0x2000017C */
    /* 0x08005580: 0c46     mov      r4, r1 */
    r4 = r1;
    /* 0x08005582: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x08005584: 4f48     ldr      r0, [pc, #0x13c]  ; =RAM_2000017D */
    r0 = *(uint32_t *)0x080056C4;   /* = RAM 0x2000017D */
    /* 0x08005586: 81b0     sub      sp, #4 */
    sp = sp - 4;
    /* 0x08005588: 0278     ldrb     r2, [r0] */
    r2 = *(uint8_t *)(r0);
    /* 0x0800558A: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x0800558C: 0dd0     beq      #0x80055aa */
    if (r1 == 1) goto L_080055AA;
    /* 0x0800558E: 0021     movs     r1, #0 */
    r1 = 0;

    L_08005590:
    /* 0x08005590: 4d48     ldr      r0, [pc, #0x134]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x080056C8;   /* = RAM 0x20000175 */
    /* 0x08005592: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08005594: c306     lsls     r3, r0, #0x1b */
    r3 = r0 << 0x1b;
    /* 0x08005596: 4d48     ldr      r0, [pc, #0x134]  ; =RAM_2000019C */
    r0 = *(uint32_t *)0x080056CC;   /* = RAM 0x2000019C */
    /* 0x08005598: 09d5     bpl      #0x80055ae */
    if (r1 >= 1) goto L_080055AE;   /* result non-negative */
    /* 0x0800559A: 435c     ldrb     r3, [r0, r1] */
    r3 = *(uint8_t *)(r0 + r1);
    /* 0x0800559C: 462b     cmp      r3, #0x46 */
    /* flags = cmp(r3, #0x46) */
    /* 0x0800559E: 09d9     bls      #0x80055b4 */
    if (r3 <= 0x46) goto L_080055B4;   /* unsigned */
    /* 0x080055A0: ff2b     cmp      r3, #0xff */
    /* flags = cmp(r3, #0xff) */
    /* 0x080055A2: 17d0     beq      #0x80055d4 */
    if (r3 == 0xff) goto L_080055D4;
    /* 0x080055A4: 4623     movs     r3, #0x46 */
    r3 = 0x46;
    /* 0x080055A6: 4354     strb     r3, [r0, r1] */
    *(uint8_t *)(r0 + r1) = r3;   /* store */
    /* 0x080055A8: 04e0     b        #0x80055b4 */
    goto L_080055B4;

    L_080055AA:
    /* 0x080055AA: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x080055AC: f0e7     b        #0x8005590 */
    goto L_08005590;   /* loop */

    L_080055AE:
    /* 0x080055AE: 415c     ldrb     r1, [r0, r1] */
    r1 = *(uint8_t *)(r0 + r1);
    /* 0x080055B0: ff29     cmp      r1, #0xff */
    /* flags = cmp(r1, #0xff) */
    /* 0x080055B2: 0fd0     beq      #0x80055d4 */
    if (r1 == 0xff) goto L_080055D4;

    L_080055B4:
    /* 0x080055B4: 805c     ldrb     r0, [r0, r2] */
    r0 = *(uint8_t *)(r0 + r2);
    /* 0x080055B6: 5021     movs     r1, #0x50 */
    r1 = 0x50;
    /* 0x080055B8: 081a     subs     r0, r1, r0 */
    r0 = r1 - r0;
    /* 0x080055BA: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x080055BC: 0a21     movs     r1, #0xa */
    r1 = 0xa;
    /* 0x080055BE: fdf797fd bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x080055C2: 0300     movs     r3, r0 */
    r3 = r0;
    /* 0x080055C4: 07f095f9 bl       #0x800c8f2 */
    FUN_0800C8F2();   /* BL -> 0x0800C8F2 FUN_0800C8F2 */
    /* 0x080055C8: 091b     subs     r1, r1, r4 */
    r1 = r1 - r4;
    /* 0x080055CA: 1f23     movs     r3, #0x1f */
    r3 = 0x1f;
    /* 0x080055CC: 266f     ldr      r6, [r4, #0x70] */
    r6 = *(uint32_t *)(r4 + 112);
    /* 0x080055CE: 7274     strb     r2, [r6, #0x11] */
    *(uint8_t *)(r6 + 17) = r2;   /* store */
    /* 0x080055D0: 7678     ldrb     r6, [r6, #1] */
    r6 = *(uint8_t *)(r6 + 1);
    /* 0x080055D2: 2900     movs     r1, r5 */
    r1 = r5;

    L_080055D4:
    /* 0x080055D4: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080055D6: 0d30     adds     r0, #0xd */
    r0 = r0 + 0xd;
    /* 0x080055D8: c1b2     uxtb     r1, r0 */
    r1 = uxtb(r0);
    /* 0x080055DA: 0a22     movs     r2, #0xa */
    r2 = 0xa;
    /* 0x080055DC: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x080055DE: fff7c9fb bl       #0x8004d74 */
    FUN_08004D74();   /* BL -> 0x08004D74 FUN_08004D74 */
    /* 0x080055E2: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x080055E4: 1b30     adds     r0, #0x1b */
    r0 = r0 + 0x1b;
    /* 0x080055E6: c1b2     uxtb     r1, r0 */
    r1 = uxtb(r0);
    /* 0x080055E8: 0b22     movs     r2, #0xb */
    r2 = 0xb;
    /* 0x080055EA: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x080055EC: fff7c2fb bl       #0x8004d74 */
    FUN_08004D74();   /* BL -> 0x08004D74 FUN_08004D74 */
    /* 0x080055F0: 2634     adds     r4, #0x26 */
    r4 = r4 + 0x26;
    /* 0x080055F2: e1b2     uxtb     r1, r4 */
    r1 = uxtb(r4);
    /* 0x080055F4: 0b22     movs     r2, #0xb */
    r2 = 0xb;
    /* 0x080055F6: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x080055F8: fff7bcfb bl       #0x8004d74 */
    FUN_08004D74();   /* BL -> 0x08004D74 FUN_08004D74 */
    /* 0x080055FC: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */
}
