/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08003548
 * Name      : __libc_init_array
 * End       : 0x08003564
 * Size      : 28 bytes, 13 instructions decoded in body (functions.json insns=12, blocks=4)
 * Class     : function
 * Callers   : 1  -> 0x080030C2 __start_mov_sp
 * Callees   : 1  -> 0x080030C8 __start_call_main
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void __libc_init_array(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08003548: 064c     ldr      r4, [pc, #0x18]  ; =flash_0800F3EC */
    r4 = *(uint32_t *)0x08003564;   /* = flash data/const 0x0800F3EC */
    /* 0x0800354A: 0125     movs     r5, #1 */
    r5 = 1;
    /* 0x0800354C: 064e     ldr      r6, [pc, #0x18]  ; =flash_0800F40C */
    r6 = *(uint32_t *)0x08003568;   /* = flash data/const 0x0800F40C */
    /* 0x0800354E: 05e0     b        #0x800355c */
    goto L_0800355C;

    L_08003550:
    /* 0x08003550: e368     ldr      r3, [r4, #0xc] */
    r3 = *(uint32_t *)(r4 + 12);
    /* 0x08003552: 07cc     ldm      r4!, {r0, r1, r2} */
    /* ldm r4!, {r0, r1, r2} */
    /* 0x08003554: 2b43     orrs     r3, r5 */
    r3 = r3 | r5;
    /* 0x08003556: 0c3c     subs     r4, #0xc */
    r4 = r4 - 0xc;
    /* 0x08003558: 9847     blx      r3 */
    ((void(*)())r3)();   /* indirect call */
    /* 0x0800355A: 1034     adds     r4, #0x10 */
    r4 = r4 + 0x10;

    L_0800355C:
    /* 0x0800355C: b442     cmp      r4, r6 */
    /* flags = cmp(r4, r6) */
    /* 0x0800355E: f7d3     blo      #0x8003550 */
    if (r4 < r6) goto L_08003550;   /* unsigned */
    /* 0x08003560: fff7b2fd bl       #0x80030c8 */
    __start_call_main();   /* BL -> 0x080030C8 __start_call_main */
}
