/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A620
 * Name      : FUN_0800A620
 * End       : 0x0800A68A
 * Size      : 106 bytes, 46 instructions decoded in body (functions.json insns=46, blocks=6)
 * Class     : function
 * Callers   : 4  -> 0x0800AFC4 FUN_0800AFC4, 0x0800B03C FUN_0800B03C, 0x0800B0BE FUN_0800B0BE, 0x0800B146 FUN_0800B146
 * Callees   : 2  -> 0x08007426 FUN_08007426, 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A620(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A620: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x0800A622: 1b49     ldr      r1, [pc, #0x6c]  ; =RAM_2000017D */
    r1 = *(uint32_t *)0x0800A690;   /* = RAM 0x2000017D */
    /* 0x0800A624: 1948     ldr      r0, [pc, #0x64]  ; =RAM_200001BD */
    r0 = *(uint32_t *)0x0800A68C;   /* = RAM 0x200001BD */
    /* 0x0800A626: 0978     ldrb     r1, [r1] */
    r1 = *(uint8_t *)(r1);
    /* 0x0800A628: 405c     ldrb     r0, [r0, r1] */
    r0 = *(uint8_t *)(r0 + r1);
    /* 0x0800A62A: 001d     adds     r0, r0, #4 */
    r0 = r0 + 4;
    /* 0x0800A62C: 6428     cmp      r0, #0x64 */
    /* flags = cmp(r0, #0x64) */
    /* 0x0800A62E: 00d9     bls      #0x800a632 */
    if (r0 <= 0x64) goto L_0800A632;   /* unsigned */
    /* 0x0800A630: 6420     movs     r0, #0x64 */
    r0 = 0x64;

    L_0800A632:
    /* 0x0800A632: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800A634: 1749     ldr      r1, [pc, #0x5c]  ; =flash_0800D35A */
    r1 = *(uint32_t *)0x0800A694;   /* = flash data/const 0x0800D35A */
    /* 0x0800A636: c00d     lsrs     r0, r0, #0x17 */
    r0 = r0 >> 0x17;
    /* 0x0800A638: 0c5a     ldrh     r4, [r1, r0] */
    r4 = *(uint16_t *)(r1 + r0);
    /* 0x0800A63A: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800A63C: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800A640: 1548     ldr      r0, [pc, #0x54]  ; =RAM_200001B8 */
    r0 = *(uint32_t *)0x0800A698;   /* = RAM 0x200001B8 */
    /* 0x0800A642: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800A644: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800A646: 17d1     bne      #0x800a678 */
    if (r0 != 1) goto L_0800A678;
    /* 0x0800A648: 1448     ldr      r0, [pc, #0x50]  ; =RAM_200001B2 */
    r0 = *(uint32_t *)0x0800A69C;   /* = RAM 0x200001B2 */
    /* 0x0800A64A: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800A64C: 0528     cmp      r0, #5 */
    /* flags = cmp(r0, #5) */
    /* 0x0800A64E: 13d1     bne      #0x800a678 */
    if (r0 != 5) goto L_0800A678;
    /* 0x0800A650: 0925     movs     r5, #9 */
    r5 = 9;
    /* 0x0800A652: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800A654: ed06     lsls     r5, r5, #0x1b */
    r5 = r5 << 0x1b;
    /* 0x0800A656: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x0800A658: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800A65A: fcf7e4fe bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A65E: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800A660: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800A662: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800A664: fcf7dffe bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A668: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800A66A: 02f0e5f9 bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800A66E: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800A670: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800A672: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800A674: fcf7d7fe bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */

    L_0800A678:
    /* 0x0800A678: 9620     movs     r0, #0x96 */
    r0 = 0x96;
    /* 0x0800A67A: 02f0ddf9 bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800A67E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800A680: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800A684: 0649     ldr      r1, [pc, #0x18]  ; =RAM_200001BC */
    r1 = *(uint32_t *)0x0800A6A0;   /* = RAM 0x200001BC */
    /* 0x0800A686: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x0800A688: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
