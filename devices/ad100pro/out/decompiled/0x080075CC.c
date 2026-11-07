/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080075CC
 * Name      : FUN_080075CC
 * End       : 0x080075E6
 * Size      : 26 bytes, 11 instructions decoded in body (functions.json insns=11, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x0800CF74 main
 * Callees   : 2  -> 0x080075EC FUN_080075EC, 0x08007610 FUN_08007610
 * MMIO      : FLASH_IF.ACR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080075CC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080075CC: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x080075CE: 0648     ldr      r0, [pc, #0x18]  ; =FLASH_IF.ACR */
    r0 = *(uint32_t *)0x080075E8;   /* = MMIO FLASH+0x00 */
    /* 0x080075D0: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: FLASH_IF.ACR(r) */
    /* 0x080075D2: 1022     movs     r2, #0x10 */
    r2 = 0x10;
    /* 0x080075D4: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x080075D6: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: FLASH_IF.ACR(w) */
    /* 0x080075D8: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080075DA: 00f007f8 bl       #0x80075ec */
    FUN_080075EC();   /* BL -> 0x080075EC FUN_080075EC */
    /* 0x080075DE: 00f017f8 bl       #0x8007610 */
    FUN_08007610();   /* BL -> 0x08007610 FUN_08007610 */
    /* 0x080075E2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080075E4: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
