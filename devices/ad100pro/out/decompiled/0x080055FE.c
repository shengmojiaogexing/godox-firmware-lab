/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080055FE
 * Name      : FUN_080055FE
 * End       : 0x080056A6
 * Size      : 168 bytes, 75 instructions decoded in body (functions.json insns=65, blocks=10)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x08004D28 FUN_08004D28, 0x08004D74 FUN_08004D74
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080055FE(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080055FE: 0227     movs     r7, #2 */
    r7 = 2;
    /* 0x08005600: 0526     movs     r6, #5 */
    r6 = 5;
    /* 0x08005602: 0625     movs     r5, #6 */
    r5 = 6;
    /* 0x08005604: 09e0     b        #0x800561a */
    goto L_0800561A;
    /* 0x08005606: 0127     movs     r7, #1 */
    r7 = 1;
    /* 0x08005608: 0226     movs     r6, #2 */
    r6 = 2;
    /* 0x0800560A: 0825     movs     r5, #8 */
    r5 = 8;
    /* 0x0800560C: 05e0     b        #0x800561a */
    goto L_0800561A;
    /* 0x0800560E: 0627     movs     r7, #6 */
    r7 = 6;
    /* 0x08005610: 0426     movs     r6, #4 */
    r6 = 4;
    /* 0x08005612: 01e0     b        #0x8005618 */
    goto L_08005618;
    /* 0x08005614: 0327     movs     r7, #3 */
    r7 = 3;
    /* 0x08005616: 0226     movs     r6, #2 */
    r6 = 2;

    L_08005618:
    /* 0x08005618: 0025     movs     r5, #0 */
    r5 = 0;

    L_0800561A:
    /* 0x0800561A: 0091     str      r1, [sp] */
    *(uint32_t *)(sp) = r1;   /* store */
    /* 0x0800561C: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800561E: 2146     mov      r1, r4 */
    r1 = r4;
    /* 0x08005620: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x08005622: fff781fb bl       #0x8004d28 */
    FUN_08004D28();   /* BL -> 0x08004D28 FUN_08004D28 */
    /* 0x08005626: a01d     adds     r0, r4, #6 */
    r0 = r4 + 6;
    /* 0x08005628: c1b2     uxtb     r1, r0 */
    r1 = uxtb(r0);
    /* 0x0800562A: 0a22     movs     r2, #0xa */
    r2 = 0xa;
    /* 0x0800562C: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x0800562E: fff77bfb bl       #0x8004d28 */
    FUN_08004D28();   /* BL -> 0x08004D28 FUN_08004D28 */
    /* 0x08005632: 002f     cmp      r7, #0 */
    /* flags = cmp(r7, #0) */
    /* 0x08005634: 06d0     beq      #0x8005644 */
    if (r7 == 0) goto L_08005644;
    /* 0x08005636: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08005638: 0f30     adds     r0, #0xf */
    r0 = r0 + 0xf;
    /* 0x0800563A: c1b2     uxtb     r1, r0 */
    r1 = uxtb(r0);
    /* 0x0800563C: 3a46     mov      r2, r7 */
    r2 = r7;
    /* 0x0800563E: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x08005640: fff798fb bl       #0x8004d74 */
    FUN_08004D74();   /* BL -> 0x08004D74 FUN_08004D74 */

    L_08005644:
    /* 0x08005644: 002e     cmp      r6, #0 */
    /* flags = cmp(r6, #0) */
    /* 0x08005646: 06d0     beq      #0x8005656 */
    if (r6 == 0) goto L_08005656;
    /* 0x08005648: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800564A: 1a30     adds     r0, #0x1a */
    r0 = r0 + 0x1a;
    /* 0x0800564C: c1b2     uxtb     r1, r0 */
    r1 = uxtb(r0);
    /* 0x0800564E: 3246     mov      r2, r6 */
    r2 = r6;
    /* 0x08005650: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x08005652: fff78ffb bl       #0x8004d74 */
    FUN_08004D74();   /* BL -> 0x08004D74 FUN_08004D74 */

    L_08005656:
    /* 0x08005656: 002d     cmp      r5, #0 */
    /* flags = cmp(r5, #0) */
    /* 0x08005658: 06d0     beq      #0x8005668 */
    if (r5 == 0) goto L_08005668;
    /* 0x0800565A: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800565C: 2530     adds     r0, #0x25 */
    r0 = r0 + 0x25;
    /* 0x0800565E: c1b2     uxtb     r1, r0 */
    r1 = uxtb(r0);
    /* 0x08005660: 2a46     mov      r2, r5 */
    r2 = r5;
    /* 0x08005662: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x08005664: fff786fb bl       #0x8004d74 */
    FUN_08004D74();   /* BL -> 0x08004D74 FUN_08004D74 */

    L_08005668:
    /* 0x08005668: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800566A: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800566C: c6d0     beq      #0x80055fc */
    if (r0 == 0) goto L_080055FC;
    /* 0x0800566E: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08005670: 3230     adds     r0, #0x32 */
    r0 = r0 + 0x32;
    /* 0x08005672: c1b2     uxtb     r1, r0 */
    r1 = uxtb(r0);
    /* 0x08005674: 0c22     movs     r2, #0xc */
    r2 = 0xc;
    /* 0x08005676: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x08005678: fff756fb bl       #0x8004d28 */
    FUN_08004D28();   /* BL -> 0x08004D28 FUN_08004D28 */
    /* 0x0800567C: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800567E: 3c30     adds     r0, #0x3c */
    r0 = r0 + 0x3c;
    /* 0x08005680: c1b2     uxtb     r1, r0 */
    r1 = uxtb(r0);
    /* 0x08005682: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08005684: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x08005686: fff74ffb bl       #0x8004d28 */
    FUN_08004D28();   /* BL -> 0x08004D28 FUN_08004D28 */
    /* 0x0800568A: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800568C: 4330     adds     r0, #0x43 */
    r0 = r0 + 0x43;
    /* 0x0800568E: c1b2     uxtb     r1, r0 */
    r1 = uxtb(r0);
    /* 0x08005690: 0b22     movs     r2, #0xb */
    r2 = 0xb;
    /* 0x08005692: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x08005694: fff748fb bl       #0x8004d28 */
    FUN_08004D28();   /* BL -> 0x08004D28 FUN_08004D28 */
    /* 0x08005698: 4734     adds     r4, #0x47 */
    r4 = r4 + 0x47;
    /* 0x0800569A: e1b2     uxtb     r1, r4 */
    r1 = uxtb(r4);
    /* 0x0800569C: 009a     ldr      r2, [sp] */
    r2 = *(uint32_t *)(sp);
    /* 0x0800569E: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x080056A0: fff742fb bl       #0x8004d28 */
    FUN_08004D28();   /* BL -> 0x08004D28 FUN_08004D28 */
    /* 0x080056A4: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */
}
