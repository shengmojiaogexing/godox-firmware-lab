/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800BD7C
 * Name      : FUN_0800BD7C
 * End       : 0x0800BD9E
 * Size      : 34 bytes, 17 instructions decoded in body (functions.json insns=17, blocks=1)
 * Class     : function
 * Callers   : 2  -> 0x080082F8 FUN_080082F8, 0x080083AC FUN_080083AC
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800BD7C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800BD7C: 30b5     push     {r4, r5, lr} */
    push({r4, r5, lr});
    /* 0x0800BD7E: 036a     ldr      r3, [r0, #0x20] */
    r3 = *(uint32_t *)(r0 + 32);
    /* 0x0800BD80: 046a     ldr      r4, [r0, #0x20] */
    r4 = *(uint32_t *)(r0 + 32);
    /* 0x0800BD82: 6408     lsrs     r4, r4, #1 */
    r4 = r4 >> 1;
    /* 0x0800BD84: 6400     lsls     r4, r4, #1 */
    r4 = r4 << 1;
    /* 0x0800BD86: 0462     str      r4, [r0, #0x20] */
    *(uint32_t *)(r0 + 32) = r4;   /* store */
    /* 0x0800BD88: 8469     ldr      r4, [r0, #0x18] */
    r4 = *(uint32_t *)(r0 + 24);
    /* 0x0800BD8A: f025     movs     r5, #0xf0 */
    r5 = 0xf0;
    /* 0x0800BD8C: ac43     bics     r4, r5 */
    r4 = r4 & ~r5;
    /* 0x0800BD8E: 1201     lsls     r2, r2, #4 */
    r2 = r2 << 4;
    /* 0x0800BD90: 2243     orrs     r2, r4 */
    r2 = r2 | r4;
    /* 0x0800BD92: 0a24     movs     r4, #0xa */
    r4 = 0xa;
    /* 0x0800BD94: a343     bics     r3, r4 */
    r3 = r3 & ~r4;
    /* 0x0800BD96: 0b43     orrs     r3, r1 */
    r3 = r3 | r1;
    /* 0x0800BD98: 8261     str      r2, [r0, #0x18] */
    *(uint32_t *)(r0 + 24) = r2;   /* store */
    /* 0x0800BD9A: 0362     str      r3, [r0, #0x20] */
    *(uint32_t *)(r0 + 32) = r3;   /* store */
    /* 0x0800BD9C: 30bd     pop      {r4, r5, pc} */
    pop({r4, r5, pc});   /* return */
}
