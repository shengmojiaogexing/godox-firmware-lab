/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800BD9E
 * Name      : FUN_0800BD9E
 * End       : 0x0800BDC4
 * Size      : 38 bytes, 19 instructions decoded in body (functions.json insns=19, blocks=1)
 * Class     : function
 * Callers   : 2  -> 0x080082F8 FUN_080082F8, 0x0800839C FUN_0800839C
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800BD9E(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800BD9E: 30b5     push     {r4, r5, lr} */
    push({r4, r5, lr});
    /* 0x0800BDA0: 036a     ldr      r3, [r0, #0x20] */
    r3 = *(uint32_t *)(r0 + 32);
    /* 0x0800BDA2: 1024     movs     r4, #0x10 */
    r4 = 0x10;
    /* 0x0800BDA4: a343     bics     r3, r4 */
    r3 = r3 & ~r4;
    /* 0x0800BDA6: 0362     str      r3, [r0, #0x20] */
    *(uint32_t *)(r0 + 32) = r3;   /* store */
    /* 0x0800BDA8: 8469     ldr      r4, [r0, #0x18] */
    r4 = *(uint32_t *)(r0 + 24);
    /* 0x0800BDAA: 036a     ldr      r3, [r0, #0x20] */
    r3 = *(uint32_t *)(r0 + 32);
    /* 0x0800BDAC: 0f25     movs     r5, #0xf */
    r5 = 0xf;
    /* 0x0800BDAE: 2d03     lsls     r5, r5, #0xc */
    r5 = r5 << 0xc;
    /* 0x0800BDB0: 1203     lsls     r2, r2, #0xc */
    r2 = r2 << 0xc;
    /* 0x0800BDB2: ac43     bics     r4, r5 */
    r4 = r4 & ~r5;
    /* 0x0800BDB4: 2243     orrs     r2, r4 */
    r2 = r2 | r4;
    /* 0x0800BDB6: a024     movs     r4, #0xa0 */
    r4 = 0xa0;
    /* 0x0800BDB8: a343     bics     r3, r4 */
    r3 = r3 & ~r4;
    /* 0x0800BDBA: 0901     lsls     r1, r1, #4 */
    r1 = r1 << 4;
    /* 0x0800BDBC: 1943     orrs     r1, r3 */
    r1 = r1 | r3;
    /* 0x0800BDBE: 8261     str      r2, [r0, #0x18] */
    *(uint32_t *)(r0 + 24) = r2;   /* store */
    /* 0x0800BDC0: 0162     str      r1, [r0, #0x20] */
    *(uint32_t *)(r0 + 32) = r1;   /* store */
    /* 0x0800BDC2: 30bd     pop      {r4, r5, pc} */
    pop({r4, r5, pc});   /* return */
}
