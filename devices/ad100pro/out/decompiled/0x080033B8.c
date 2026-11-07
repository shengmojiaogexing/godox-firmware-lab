/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080033B8
 * Name      : FUN_080033B8
 * End       : 0x080033EA
 * Size      : 50 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=5)
 * Class     : function
 * Callers   : 1  -> 0x0800610C FUN_0800610C
 * Callees   : 1  -> 0x08003144 FUN_08003144
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080033B8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080033B8: 4a00     lsls     r2, r1, #1 */
    r2 = r1 << 1;
    /* 0x080033BA: 0903     lsls     r1, r1, #0xc */
    r1 = r1 << 0xc;
    /* 0x080033BC: 0123     movs     r3, #1 */
    r3 = 1;
    /* 0x080033BE: 090b     lsrs     r1, r1, #0xc */
    r1 = r1 >> 0xc;
    /* 0x080033C0: 1b05     lsls     r3, r3, #0x14 */
    r3 = r3 << 0x14;
    /* 0x080033C2: 1943     orrs     r1, r3 */
    r1 = r1 | r3;
    /* 0x080033C4: 094b     ldr      r3, [pc, #0x24]  ; =0x000003FF */
    r3 = *(uint32_t *)0x080033EC;   /* = 0x000003FF */
    /* 0x080033C6: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x080033C8: 520d     lsrs     r2, r2, #0x15 */
    r2 = r2 >> 0x15;
    /* 0x080033CA: 9a42     cmp      r2, r3 */
    /* flags = cmp(r2, r3) */
    /* 0x080033CC: 01da     bge      #0x80033d2 */
    if (r2 >= r3) goto L_080033D2;   /* signed */
    /* 0x080033CE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080033D0: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_080033D2:
    /* 0x080033D2: 064b     ldr      r3, [pc, #0x18]  ; =0x000003FF */
    r3 = *(uint32_t *)0x080033EC;   /* = 0x000003FF */
    /* 0x080033D4: 3433     adds     r3, #0x34 */
    r3 = r3 + 0x34;
    /* 0x080033D6: 9a42     cmp      r2, r3 */
    /* flags = cmp(r2, r3) */
    /* 0x080033D8: 03dc     bgt      #0x80033e2 */
    if (r2 > r3) goto L_080033E2;   /* signed */
    /* 0x080033DA: 9a1a     subs     r2, r3, r2 */
    r2 = r3 - r2;
    /* 0x080033DC: fff7b2fe bl       #0x8003144 */
    FUN_08003144();   /* BL -> 0x08003144 FUN_08003144 */
    /* 0x080033E0: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_080033E2:
    /* 0x080033E2: 0349     ldr      r1, [pc, #0xc]  ; =0xFFFFFBCD */
    r1 = *(uint32_t *)0x080033F0;   /* = 0xFFFFFBCD */
    /* 0x080033E4: 5118     adds     r1, r2, r1 */
    r1 = r2 + r1;
    /* 0x080033E6: 8840     lsls     r0, r1 */
    r0 = r0 << r1;
    /* 0x080033E8: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
