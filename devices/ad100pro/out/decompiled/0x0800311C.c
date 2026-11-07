/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800311C
 * Name      : FUN_0800311C
 * End       : 0x08003144
 * Size      : 40 bytes, 19 instructions decoded in body (functions.json insns=19, blocks=9)
 * Class     : function
 * Callers   : 5  -> 0x0800610C FUN_0800610C, 0x080098F8 FUN_080098F8, 0x0800B0BE FUN_0800B0BE, 0x0800B2A4 FUN_0800B2A4, 0x0800B2A8 FUN_0800B2A8
 * Callees   : 1  -> 0x080030F0 FUN_080030F0
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800311C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800311C: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800311E: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x08003120: 2546     mov      r5, r4 */
    r5 = r4;
    /* 0x08003122: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08003124: 01da     bge      #0x800312a */
    if (r0 >= 0) goto L_0800312A;   /* signed */
    /* 0x08003126: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x08003128: 4042     rsbs     r0, r0, #0 */
    r0 = 0 - r0;

    L_0800312A:
    /* 0x0800312A: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x0800312C: 01da     bge      #0x8003132 */
    if (r1 >= 0) goto L_08003132;   /* signed */
    /* 0x0800312E: 0125     movs     r5, #1 */
    r5 = 1;
    /* 0x08003130: 4942     rsbs     r1, r1, #0 */
    r1 = 0 - r1;

    L_08003132:
    /* 0x08003132: fff7ddff bl       #0x80030f0 */
    FUN_080030F0();   /* BL -> 0x080030F0 FUN_080030F0 */
    /* 0x08003136: ac42     cmp      r4, r5 */
    /* flags = cmp(r4, r5) */
    /* 0x08003138: 00d0     beq      #0x800313c */
    if (r4 == r5) goto L_0800313C;
    /* 0x0800313A: 4042     rsbs     r0, r0, #0 */
    r0 = 0 - r0;

    L_0800313C:
    /* 0x0800313C: 002c     cmp      r4, #0 */
    /* flags = cmp(r4, #0) */
    /* 0x0800313E: 00d0     beq      #0x8003142 */
    if (r4 == 0) goto L_08003142;
    /* 0x08003140: 4942     rsbs     r1, r1, #0 */
    r1 = 0 - r1;

    L_08003142:
    /* 0x08003142: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
