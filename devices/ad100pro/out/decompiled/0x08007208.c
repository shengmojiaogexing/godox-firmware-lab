/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007208
 * Name      : FUN_08007208
 * End       : 0x08007276
 * Size      : 110 bytes, 51 instructions decoded in body (functions.json insns=51, blocks=11)
 * Class     : function
 * Callers   : 1  -> 0x08005EF4 FUN_08005EF4
 * Callees   : 3  -> 0x08003144 FUN_08003144, 0x08005E6C FUN_08005E6C, 0x08005F34 FUN_08005F34
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007208(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007208: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x0800720A: 1fb4     push     {r0, r1, r2, r3, r4} */
    push({r0, r1, r2, r3, r4});
    /* 0x0800720C: 0746     mov      r7, r0 */
    r7 = r0;
    /* 0x0800720E: 1a48     ldr      r0, [pc, #0x68]  ; =RAM_20000240 */
    r0 = *(uint32_t *)0x08007278;   /* = RAM 0x20000240 */
    /* 0x08007210: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x08007212: 017e     ldrb     r1, [r0, #0x18] */
    r1 = *(uint8_t *)(r0 + 24);
    /* 0x08007214: 1546     mov      r5, r2 */
    r5 = r2;
    /* 0x08007216: 1e46     mov      r6, r3 */
    r6 = r3;
    /* 0x08007218: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x0800721A: 07d0     beq      #0x800722c */
    if (r1 == 1) goto L_0800722C;
    /* 0x0800721C: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800721E: 0176     strb     r1, [r0, #0x18] */
    *(uint8_t *)(r0 + 24) = r1;   /* store */
    /* 0x08007220: 1648     ldr      r0, [pc, #0x58]  ; =0x0000C350 */
    r0 = *(uint32_t *)0x0800727C;   /* = 0x0000C350 */
    /* 0x08007222: fef787fe bl       #0x8005f34 */
    FUN_08005F34();   /* BL -> 0x08005F34 FUN_08005F34 */
    /* 0x08007226: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08007228: 03d0     beq      #0x8007232 */
    if (r0 == 0) goto L_08007232;
    /* 0x0800722A: 20e0     b        #0x800726e */
    goto L_0800726E;

    L_0800722C:
    /* 0x0800722C: 0220     movs     r0, #2 */
    r0 = 2;

    L_0800722E:
    /* 0x0800722E: 05b0     add      sp, #0x14 */
    sp = sp + 0x14;
    /* 0x08007230: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */

    L_08007232:
    /* 0x08007232: 012f     cmp      r7, #1 */
    /* flags = cmp(r7, #1) */
    /* 0x08007234: 02d0     beq      #0x800723c */
    if (r7 == 1) goto L_0800723C;
    /* 0x08007236: 022f     cmp      r7, #2 */
    /* flags = cmp(r7, #2) */
    /* 0x08007238: 00d0     beq      #0x800723c */
    if (r7 == 2) goto L_0800723C;
    /* 0x0800723A: 0427     movs     r7, #4 */
    r7 = 4;

    L_0800723C:
    /* 0x0800723C: 2201     lsls     r2, r4, #4 */
    r2 = r4 << 4;
    /* 0x0800723E: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08007240: 3146     mov      r1, r6 */
    r1 = r6;
    /* 0x08007242: fbf77fff bl       #0x8003144 */
    FUN_08003144();   /* BL -> 0x08003144 FUN_08003144 */
    /* 0x08007246: 81b2     uxth     r1, r0 */
    r1 = uxth(r0);
    /* 0x08007248: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x0800724A: 6200     lsls     r2, r4, #1 */
    r2 = r4 << 1;
    /* 0x0800724C: 1018     adds     r0, r2, r0 */
    r0 = r2 + r0;
    /* 0x0800724E: fef70dfe bl       #0x8005e6c */
    FUN_08005E6C();   /* BL -> 0x08005E6C FUN_08005E6C */
    /* 0x08007252: 0a48     ldr      r0, [pc, #0x28]  ; =0x0000C350 */
    r0 = *(uint32_t *)0x0800727C;   /* = 0x0000C350 */
    /* 0x08007254: fef76efe bl       #0x8005f34 */
    FUN_08005F34();   /* BL -> 0x08005F34 FUN_08005F34 */
    /* 0x08007258: 0949     ldr      r1, [pc, #0x24]  ; =FLASH_IF.ACR */
    r1 = *(uint32_t *)0x08007280;   /* = MMIO FLASH+0x00 */
    /* 0x0800725A: 0a69     ldr      r2, [r1, #0x10] */
    r2 = *(uint32_t *)(r1 + 16);  /* MMIO: FLASH_IF.CR(r) */
    /* 0x0800725C: 5208     lsrs     r2, r2, #1 */
    r2 = r2 >> 1;
    /* 0x0800725E: 5200     lsls     r2, r2, #1 */
    r2 = r2 << 1;
    /* 0x08007260: 0a61     str      r2, [r1, #0x10] */
    *(uint32_t *)(r1 + 16) = r2;   /* store */  /* MMIO: FLASH_IF.CR(w) */
    /* 0x08007262: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08007264: 03d1     bne      #0x800726e */
    if (r0 != 0) goto L_0800726E;
    /* 0x08007266: 641c     adds     r4, r4, #1 */
    r4 = r4 + 1;
    /* 0x08007268: e4b2     uxtb     r4, r4 */
    r4 = uxtb(r4);
    /* 0x0800726A: bc42     cmp      r4, r7 */
    /* flags = cmp(r4, r7) */
    /* 0x0800726C: e6d3     blo      #0x800723c */
    if (r4 < r7) goto L_0800723C;   /* unsigned */

    L_0800726E:
    /* 0x0800726E: 0249     ldr      r1, [pc, #8]  ; =RAM_20000240 */
    r1 = *(uint32_t *)0x08007278;   /* = RAM 0x20000240 */
    /* 0x08007270: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08007272: 0a76     strb     r2, [r1, #0x18] */
    *(uint8_t *)(r1 + 24) = r2;   /* store */
    /* 0x08007274: dbe7     b        #0x800722e */
    goto L_0800722E;   /* loop */
}
