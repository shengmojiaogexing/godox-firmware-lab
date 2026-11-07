/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080053BC
 * Name      : FUN_080053BC
 * End       : 0x080053E0
 * Size      : 36 bytes, 17 instructions decoded in body (functions.json insns=17, blocks=5)
 * Class     : function
 * Callers   : 2  -> 0x0800C2B8 FUN_0800C2B8, 0x0800C3D4 FUN_0800C3D4
 * Callees   : 1  -> 0x08005200 FUN_08005200
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080053BC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080053BC: 00b5     push     {lr} */
    push({lr});
    /* 0x080053BE: 0849     ldr      r1, [pc, #0x20]  ; =RAM_20000175 */
    r1 = *(uint32_t *)0x080053E0;   /* = RAM 0x20000175 */
    /* 0x080053C0: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x080053C2: 8907     lsls     r1, r1, #0x1e */
    r1 = r1 << 0x1e;
    /* 0x080053C4: 890f     lsrs     r1, r1, #0x1e */
    r1 = r1 >> 0x1e;
    /* 0x080053C6: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x080053C8: 09d1     bne      #0x80053de */
    if (r1 != 1) goto L_080053DE;
    /* 0x080053CA: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080053CC: 07d0     beq      #0x80053de */
    if (r0 == 0) goto L_080053DE;
    /* 0x080053CE: 0328     cmp      r0, #3 */
    /* flags = cmp(r0, #3) */
    /* 0x080053D0: 05d2     bhs      #0x80053de */
    if (r0 >= 3) goto L_080053DE;   /* unsigned */
    /* 0x080053D2: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x080053D4: c2b2     uxtb     r2, r0 */
    r2 = uxtb(r0);
    /* 0x080053D6: 3721     movs     r1, #0x37 */
    r1 = 0x37;
    /* 0x080053D8: 0620     movs     r0, #6 */
    r0 = 6;
    /* 0x080053DA: fff711ff bl       #0x8005200 */
    FUN_08005200();   /* BL -> 0x08005200 FUN_08005200 */

    L_080053DE:
    /* 0x080053DE: 00bd     pop      {pc} */
    pop({pc});   /* return */
}
