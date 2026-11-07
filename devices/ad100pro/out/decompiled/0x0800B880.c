/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B880
 * Name      : FUN_0800B880
 * End       : 0x0800B8DC
 * Size      : 92 bytes, 46 instructions decoded in body (functions.json insns=46, blocks=12)
 * Class     : function
 * Callers   : 2  -> 0x08008D00 dispatcher_large, 0x08008FDA FUN_08008FDA
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B880(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B880: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800B882: 1648     ldr      r0, [pc, #0x58]  ; =RAM_20000164 */
    r0 = *(uint32_t *)0x0800B8DC;   /* = RAM 0x20000164 */
    /* 0x0800B884: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800B886: 427d     ldrb     r2, [r0, #0x15] */
    r2 = *(uint8_t *)(r0 + 21);
    /* 0x0800B888: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800B88A: 0ed0     beq      #0x800b8aa */
    if (r2 == 1) goto L_0800B8AA;
    /* 0x0800B88C: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x0800B88E: 2032     adds     r2, #0x20 */
    r2 = r2 + 0x20;
    /* 0x0800B890: 9378     ldrb     r3, [r2, #2] */
    r3 = *(uint8_t *)(r2 + 2);
    /* 0x0800B892: 012b     cmp      r3, #1 */
    /* flags = cmp(r3, #1) */
    /* 0x0800B894: 0bd0     beq      #0x800b8ae */
    if (r3 == 1) goto L_0800B8AE;
    /* 0x0800B896: 0278     ldrb     r2, [r0] */
    r2 = *(uint8_t *)(r0);
    /* 0x0800B898: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x0800B89A: 02d0     beq      #0x800b8a2 */
    if (r2 == 1) goto L_0800B8A2;
    /* 0x0800B89C: 827c     ldrb     r2, [r0, #0x12] */
    r2 = *(uint8_t *)(r0 + 18);
    /* 0x0800B89E: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x0800B8A0: 07d0     beq      #0x800b8b2 */
    if (r2 == 0) goto L_0800B8B2;

    L_0800B8A2:
    /* 0x0800B8A2: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */
    /* 0x0800B8A4: 8174     strb     r1, [r0, #0x12] */
    *(uint8_t *)(r0 + 18) = r1;   /* store */
    /* 0x0800B8A6: 4163     str      r1, [r0, #0x34] */
    *(uint32_t *)(r0 + 52) = r1;   /* store */
    /* 0x0800B8A8: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800B8AA:
    /* 0x0800B8AA: 4175     strb     r1, [r0, #0x15] */
    *(uint8_t *)(r0 + 21) = r1;   /* store */
    /* 0x0800B8AC: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800B8AE:
    /* 0x0800B8AE: 9170     strb     r1, [r2, #2] */
    *(uint8_t *)(r2 + 2) = r1;   /* store */
    /* 0x0800B8B0: 0de0     b        #0x800b8ce */
    goto L_0800B8CE;

    L_0800B8B2:
    /* 0x0800B8B2: 427c     ldrb     r2, [r0, #0x11] */
    r2 = *(uint8_t *)(r0 + 17);
    /* 0x0800B8B4: 9307     lsls     r3, r2, #0x1e */
    r3 = r2 << 0x1e;
    /* 0x0800B8B6: 9b0f     lsrs     r3, r3, #0x1e */
    r3 = r3 >> 0x1e;
    /* 0x0800B8B8: 022b     cmp      r3, #2 */
    /* flags = cmp(r3, #2) */
    /* 0x0800B8BA: 08d0     beq      #0x800b8ce */
    if (r3 == 2) goto L_0800B8CE;
    /* 0x0800B8BC: d206     lsls     r2, r2, #0x1b */
    r2 = r2 << 0x1b;
    /* 0x0800B8BE: 940f     lsrs     r4, r2, #0x1e */
    r4 = r2 >> 0x1e;
    /* 0x0800B8C0: 064a     ldr      r2, [pc, #0x18]  ; =RAM_20000164 */
    r2 = *(uint32_t *)0x0800B8DC;   /* = RAM 0x20000164 */
    /* 0x0800B8C2: e723     movs     r3, #0xe7 */
    r3 = 0xe7;
    /* 0x0800B8C4: 527c     ldrb     r2, [r2, #0x11] */
    r2 = *(uint8_t *)(r2 + 17);
    /* 0x0800B8C6: 1a40     ands     r2, r3 */
    r2 = r2 & r3;
    /* 0x0800B8C8: 002c     cmp      r4, #0 */
    /* flags = cmp(r4, #0) */
    /* 0x0800B8CA: 02d0     beq      #0x800b8d2 */
    if (r4 == 0) goto L_0800B8D2;
    /* 0x0800B8CC: 4274     strb     r2, [r0, #0x11] */
    *(uint8_t *)(r0 + 17) = r2;   /* store */

    L_0800B8CE:
    /* 0x0800B8CE: c174     strb     r1, [r0, #0x13] */
    *(uint8_t *)(r0 + 19) = r1;   /* store */
    /* 0x0800B8D0: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */

    L_0800B8D2:
    /* 0x0800B8D2: 1032     adds     r2, #0x10 */
    r2 = r2 + 0x10;
    /* 0x0800B8D4: 4274     strb     r2, [r0, #0x11] */
    *(uint8_t *)(r0 + 17) = r2;   /* store */
    /* 0x0800B8D6: 024a     ldr      r2, [pc, #8]  ; =RAM_20000480 */
    r2 = *(uint32_t *)0x0800B8E0;   /* = RAM 0x20000480 */
    /* 0x0800B8D8: 9171     strb     r1, [r2, #6] */
    *(uint8_t *)(r2 + 6) = r1;   /* store */
    /* 0x0800B8DA: f8e7     b        #0x800b8ce */
    goto L_0800B8CE;   /* loop */
}
