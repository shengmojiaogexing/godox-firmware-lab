/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080032BC
 * Name      : FUN_080032BC
 * End       : 0x080032C8
 * Size      : 12 bytes, 6 instructions decoded in body (functions.json insns=165, blocks=18)
 * Class     : function
 * Callers   : 2  -> 0x0800610C FUN_0800610C, 0x08006282 FUN_08006282
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080032BC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080032BC: 30b4     push     {r4, r5} */
    push({r4, r5});
    /* 0x080032BE: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x080032C0: e407     lsls     r4, r4, #0x1f */
    r4 = r4 << 0x1f;
    /* 0x080032C2: 6140     eors     r1, r4 */
    r1 = r1 ^ r4;
    /* 0x080032C4: 30bc     pop      {r4, r5} */
    pop({r4, r5});
    /* 0x080032C6: 4fe7     b        #0x8003168 */
    goto L_08003168;   /* loop */
}
