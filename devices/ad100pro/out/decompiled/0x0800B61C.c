/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B61C
 * Name      : FUN_0800B61C
 * End       : 0x0800B67A
 * Size      : 94 bytes, 44 instructions decoded in body (functions.json insns=44, blocks=12)
 * Class     : function
 * Callers   : 1  -> 0x0800BA8C IRQ15_Handler
 * Callees   : 2  -> 0x08009BCC FUN_08009BCC, 0x08009D28 FUN_08009D28
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B61C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B61C: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800B61E: 1748     ldr      r0, [pc, #0x5c]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x0800B67C;   /* = RAM 0x20000175 */
    /* 0x0800B620: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800B622: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x0800B624: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x0800B626: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x0800B628: 05d0     beq      #0x800b636 */
    if (r0 == 2) goto L_0800B636;
    /* 0x0800B62A: 1548     ldr      r0, [pc, #0x54]  ; =RAM_2000017F */
    r0 = *(uint32_t *)0x0800B680;   /* = RAM 0x2000017F */
    /* 0x0800B62C: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800B62E: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800B630: 01d1     bne      #0x800b636 */
    if (r0 != 0) goto L_0800B636;
    /* 0x0800B632: fef7cbfa bl       #0x8009bcc */
    FUN_08009BCC();   /* BL -> 0x08009BCC FUN_08009BCC */

    L_0800B636:
    /* 0x0800B636: 1348     ldr      r0, [pc, #0x4c]  ; =RAM_200001AF */
    r0 = *(uint32_t *)0x0800B684;   /* = RAM 0x200001AF */
    /* 0x0800B638: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800B63A: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800B63C: 1cd1     bne      #0x800b678 */
    if (r0 != 1) goto L_0800B678;
    /* 0x0800B63E: 1248     ldr      r0, [pc, #0x48]  ; =RAM_200001B8 */
    r0 = *(uint32_t *)0x0800B688;   /* = RAM 0x200001B8 */
    /* 0x0800B640: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800B642: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800B644: 16d1     bne      #0x800b674 */
    if (r0 != 1) goto L_0800B674;
    /* 0x0800B646: 1148     ldr      r0, [pc, #0x44]  ; =RAM_200001B3 */
    r0 = *(uint32_t *)0x0800B68C;   /* = RAM 0x200001B3 */
    /* 0x0800B648: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800B64A: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800B64C: 03d0     beq      #0x800b656 */
    if (r0 == 1) goto L_0800B656;
    /* 0x0800B64E: 1048     ldr      r0, [pc, #0x40]  ; =RAM_20000134 */
    r0 = *(uint32_t *)0x0800B690;   /* = RAM 0x20000134 */
    /* 0x0800B650: c078     ldrb     r0, [r0, #3] */
    r0 = *(uint8_t *)(r0 + 3);
    /* 0x0800B652: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800B654: 0ed1     bne      #0x800b674 */
    if (r0 != 1) goto L_0800B674;

    L_0800B656:
    /* 0x0800B656: fef767fb bl       #0x8009d28 */
    FUN_08009D28();   /* BL -> 0x08009D28 FUN_08009D28 */
    /* 0x0800B65A: 0e48     ldr      r0, [pc, #0x38]  ; =RAM_20000165 */
    r0 = *(uint32_t *)0x0800B694;   /* = RAM 0x20000165 */
    /* 0x0800B65C: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800B65E: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800B660: 01d1     bne      #0x800b666 */
    if (r0 != 0) goto L_0800B666;
    /* 0x0800B662: 0d48     ldr      r0, [pc, #0x34]  ; =RAM_200001AE */
    r0 = *(uint32_t *)0x0800B698;   /* = RAM 0x200001AE */
    /* 0x0800B664: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);

    L_0800B666:
    /* 0x0800B666: 0d4a     ldr      r2, [pc, #0x34]  ; =RAM_200001AD */
    r2 = *(uint32_t *)0x0800B69C;   /* = RAM 0x200001AD */
    /* 0x0800B668: 1178     ldrb     r1, [r2] */
    r1 = *(uint8_t *)(r2);
    /* 0x0800B66A: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x0800B66C: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x0800B66E: 1170     strb     r1, [r2] */
    *(uint8_t *)(r2) = r1;   /* store */
    /* 0x0800B670: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x0800B672: 01d3     blo      #0x800b678 */
    if (r1 < r0) goto L_0800B678;   /* unsigned */

    L_0800B674:
    /* 0x0800B674: fef7aafa bl       #0x8009bcc */
    FUN_08009BCC();   /* BL -> 0x08009BCC FUN_08009BCC */

    L_0800B678:
    /* 0x0800B678: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
