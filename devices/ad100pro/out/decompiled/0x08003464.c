/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08003464
 * Name      : FUN_08003464
 * End       : 0x0800348A
 * Size      : 38 bytes, 19 instructions decoded in body (functions.json insns=19, blocks=4)
 * Class     : function
 * Callers   : 1  -> 0x08003168 lookup_or_pad
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08003464(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08003464: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x08003466: 202a     cmp      r2, #0x20 */
    /* flags = cmp(r2, #0x20) */
    /* 0x08003468: 06db     blt      #0x8003478 */
    if (r2 < 0x20) goto L_08003478;   /* signed */
    /* 0x0800346A: cb17     asrs     r3, r1, #0x1f */
    r3 = r1 >> 0x1f;
    /* 0x0800346C: 0846     mov      r0, r1 */
    r0 = r1;
    /* 0x0800346E: 203a     subs     r2, #0x20 */
    r2 = r2 - 0x20;
    /* 0x08003470: 1041     asrs     r0, r2 */
    r0 = r0 >> r2;
    /* 0x08003472: c117     asrs     r1, r0, #0x1f */
    r1 = r0 >> 0x1f;
    /* 0x08003474: 0b43     orrs     r3, r1 */
    r3 = r3 | r1;
    /* 0x08003476: 06e0     b        #0x8003486 */
    goto L_08003486;

    L_08003478:
    /* 0x08003478: 0b46     mov      r3, r1 */
    r3 = r1;
    /* 0x0800347A: 1341     asrs     r3, r2 */
    r3 = r3 >> r2;
    /* 0x0800347C: d040     lsrs     r0, r2 */
    r0 = r0 >> r2;
    /* 0x0800347E: 2024     movs     r4, #0x20 */
    r4 = 0x20;
    /* 0x08003480: a21a     subs     r2, r4, r2 */
    r2 = r4 - r2;
    /* 0x08003482: 9140     lsls     r1, r2 */
    r1 = r1 << r2;
    /* 0x08003484: 0843     orrs     r0, r1 */
    r0 = r0 | r1;

    L_08003486:
    /* 0x08003486: 1946     mov      r1, r3 */
    r1 = r3;
    /* 0x08003488: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
