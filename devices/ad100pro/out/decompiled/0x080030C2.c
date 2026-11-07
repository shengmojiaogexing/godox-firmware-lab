/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080030C2
 * Name      : __start_mov_sp
 * End       : 0x080030CC
 * Size      : 10 bytes, 4 instructions decoded in body (functions.json insns=4, blocks=1)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x08003548 __libc_init_array
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void __start_mov_sp(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080030C2: 8546     mov      sp, r0 */
    sp = r0;
    /* 0x080030C4: 00f040fa bl       #0x8003548 */
    __libc_init_array();   /* BL -> 0x08003548 __libc_init_array */
    /* 0x080030C8: 0048     ldr      r0, [pc, #0]  ; =func_0800CF74 */
    r0 = *(uint32_t *)0x080030CC;   /* = flash data/const 0x0800CF75 */
    /* 0x080030CA: 0047     bx       r0 */
    goto *r0;   /* tail branch  */
}
