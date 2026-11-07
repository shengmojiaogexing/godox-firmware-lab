/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004872
 * Name      : FUN_08004872
 * End       : 0x08004880
 * Size      : 14 bytes, 6 instructions decoded in body (functions.json insns=6, blocks=1)
 * Class     : nested_block
 * Parent    : 0x080047CC FUN_080047CC  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004872(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004874: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
    /* 0x08004876: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x08004878: 0008     lsrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x0800487A: 0048     ldr      r0, [pc, #0]  ; =RAM_20000184 */
    r0 = *(uint32_t *)0x0800487C;   /* = RAM 0x20000184 */
    /* 0x0800487C: 8401     lsls     r4, r0, #6 */
    r4 = r0 << 6;
    /* 0x0800487E: 0020     movs     r0, #0 */
    r0 = 0;
}
