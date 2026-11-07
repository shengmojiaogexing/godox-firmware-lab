/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007610
 * Name      : FUN_08007610
 * End       : 0x08007642
 * Size      : 50 bytes, 22 instructions decoded in body (functions.json insns=22, blocks=1)
 * Class     : function
 * Callers   : 2  -> 0x080075CC FUN_080075CC, 0x080075DC FUN_080075DC
 * Callees   : 1  -> 0x08007670 FUN_08007670
 * MMIO      : RCC.APB2ENR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007610(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007610: 08b5     push     {r3, lr} */
    push({r3, lr});
    /* 0x08007612: 0c48     ldr      r0, [pc, #0x30]  ; =RCC.CR */
    r0 = *(uint32_t *)0x08007644;   /* = MMIO RCC+0x00 */
    /* 0x08007614: 8169     ldr      r1, [r0, #0x18] */
    r1 = *(uint32_t *)(r0 + 24);  /* MMIO: RCC.APB2ENR(r) */
    /* 0x08007616: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08007618: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x0800761A: 8161     str      r1, [r0, #0x18] */
    *(uint32_t *)(r0 + 24) = r1;   /* store */  /* MMIO: RCC.APB2ENR(w) */
    /* 0x0800761C: 8069     ldr      r0, [r0, #0x18] */
    r0 = *(uint32_t *)(r0 + 24);  /* MMIO: RCC.APB2ENR(r) */
    /* 0x0800761E: 1040     ands     r0, r2 */
    r0 = r0 & r2;
    /* 0x08007620: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08007622: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08007624: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x08007626: 501f     subs     r0, r2, #5 */
    r0 = r2 - 5;
    /* 0x08007628: 00f022f8 bl       #0x8007670 */
    FUN_08007670();   /* BL -> 0x08007670 FUN_08007670 */
    /* 0x0800762C: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800762E: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x08007630: 901e     subs     r0, r2, #2 */
    r0 = r2 - 2;
    /* 0x08007632: 00f01df8 bl       #0x8007670 */
    FUN_08007670();   /* BL -> 0x08007670 FUN_08007670 */
    /* 0x08007636: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08007638: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x0800763A: 501e     subs     r0, r2, #1 */
    r0 = r2 - 1;
    /* 0x0800763C: 00f018f8 bl       #0x8007670 */
    FUN_08007670();   /* BL -> 0x08007670 FUN_08007670 */
    /* 0x08007640: 08bd     pop      {r3, pc} */
    pop({r3, pc});   /* return */
}
