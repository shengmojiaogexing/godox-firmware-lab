/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080030C8
 * Name      : __start_call_main
 * End       : 0x080030C8
 * Size      : 0 bytes, 1 instructions decoded in body (functions.json insns=0, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x080030C2 __start_mov_sp  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 2  -> 0x08003548 __libc_init_array, 0x0800355A FUN_0800355A
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void __start_call_main(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080030C8: 0048     ldr      r0, [pc, #0]  ; =func_0800CF74 */
    r0 = *(uint32_t *)0x080030CC;   /* = flash data/const 0x0800CF75 */
}
