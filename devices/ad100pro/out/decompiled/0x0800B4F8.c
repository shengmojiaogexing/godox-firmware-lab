/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B4F8
 * Name      : FUN_0800B4F8
 * End       : 0x0800B550
 * Size      : 88 bytes, 36 instructions decoded in body (functions.json insns=36, blocks=4)
 * Class     : function
 * Callers   : 4  -> 0x0800AFC4 FUN_0800AFC4, 0x0800B03C FUN_0800B03C, 0x0800B0BE FUN_0800B0BE, 0x0800B19E FUN_0800B19E
 * Callees   : 2  -> 0x08007426 FUN_08007426, 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B4F8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B4F8: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800B4FA: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800B4FC: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800B500: 3220     movs     r0, #0x32 */
    r0 = 0x32;
    /* 0x0800B502: 01f099fa bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800B506: 1248     ldr      r0, [pc, #0x48]  ; =RAM_200001B8 */
    r0 = *(uint32_t *)0x0800B550;   /* = RAM 0x200001B8 */
    /* 0x0800B508: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800B50A: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800B50C: 17d1     bne      #0x800b53e */
    if (r0 != 1) goto L_0800B53E;
    /* 0x0800B50E: 1148     ldr      r0, [pc, #0x44]  ; =RAM_200001B2 */
    r0 = *(uint32_t *)0x0800B554;   /* = RAM 0x200001B2 */
    /* 0x0800B510: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800B512: 0528     cmp      r0, #5 */
    /* flags = cmp(r0, #5) */
    /* 0x0800B514: 13d1     bne      #0x800b53e */
    if (r0 != 5) goto L_0800B53E;
    /* 0x0800B516: 0924     movs     r4, #9 */
    r4 = 9;
    /* 0x0800B518: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800B51A: e406     lsls     r4, r4, #0x1b */
    r4 = r4 << 0x1b;
    /* 0x0800B51C: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x0800B51E: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800B520: fbf781ff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800B524: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800B526: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800B528: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800B52A: fbf77cff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800B52E: c820     movs     r0, #0xc8 */
    r0 = 0xc8;
    /* 0x0800B530: 01f082fa bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800B534: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800B536: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800B538: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800B53A: fbf774ff bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */

    L_0800B53E:
    /* 0x0800B53E: 9620     movs     r0, #0x96 */
    r0 = 0x96;
    /* 0x0800B540: 01f07afa bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x0800B544: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800B546: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800B54A: 0349     ldr      r1, [pc, #0xc]  ; =RAM_200001BC */
    r1 = *(uint32_t *)0x0800B558;   /* = RAM 0x200001BC */
    /* 0x0800B54C: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x0800B54E: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
