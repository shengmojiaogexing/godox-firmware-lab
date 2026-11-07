/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004F30
 * Name      : FUN_08004F30
 * End       : 0x08004F8C
 * Size      : 92 bytes, 46 instructions decoded in body (functions.json insns=46, blocks=4)
 * Class     : hub
 * Callers   : 54  -> 0x08003F0C FUN_08003F0C, 0x08003F3E FUN_08003F3E, 0x08003F4A FUN_08003F4A, 0x08003F56 FUN_08003F56, 0x08003F62 FUN_08003F62, 0x08003F6E FUN_08003F6E, 0x08003F7A FUN_08003F7A, 0x08003FB0 FUN_08003FB0, ... +46
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004F30(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004F30: ffb5     push     {r0, r1, r2, r3, r4, r5, r6, r7, lr} */
    push({r0, r1, r2, r3, r4, r5, r6, r7, lr});
    /* 0x08004F32: 1001     lsls     r0, r2, #4 */
    r0 = r2 << 4;
    /* 0x08004F34: 81b0     sub      sp, #4 */
    sp = sp - 4;
    /* 0x08004F36: 1018     adds     r0, r2, r0 */
    r0 = r2 + r0;
    /* 0x08004F38: 1449     ldr      r1, [pc, #0x50]  ; =flash_0800DEC6 */
    r1 = *(uint32_t *)0x08004F8C;   /* = flash data/const 0x0800DEC6 */
    /* 0x08004F3A: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08004F3C: 085c     ldrb     r0, [r1, r0] */
    r0 = *(uint8_t *)(r1 + r0);
    /* 0x08004F3E: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x08004F40: 8446     mov      ip, r0 */
    ip = r0;
    /* 0x08004F42: 1fe0     b        #0x8004f84 */
    goto L_08004F84;

    L_08004F44:
    /* 0x08004F44: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x08004F46: 009e     ldr      r6, [sp] */
    r6 = *(uint32_t *)(sp);
    /* 0x08004F48: c201     lsls     r2, r0, #7 */
    r2 = r0 << 7;
    /* 0x08004F4A: 1148     ldr      r0, [pc, #0x44]  ; =RAM_200004CC */
    r0 = *(uint32_t *)0x08004F90;   /* = RAM 0x200004CC */
    /* 0x08004F4C: 1518     adds     r5, r2, r0 */
    r5 = r2 + r0;
    /* 0x08004F4E: 0f4a     ldr      r2, [pc, #0x3c]  ; =flash_0800DEC6 */
    r2 = *(uint32_t *)0x08004F8C;   /* = flash data/const 0x0800DEC6 */
    /* 0x08004F50: 0298     ldr      r0, [sp, #8] */
    r0 = *(uint32_t *)(sp + 8);
    /* 0x08004F52: b218     adds     r2, r6, r2 */
    r2 = r6 + r2;
    /* 0x08004F54: 4e00     lsls     r6, r1, #1 */
    r6 = r1 << 1;
    /* 0x08004F56: 4418     adds     r4, r0, r1 */
    r4 = r0 + r1;
    /* 0x08004F58: 9619     adds     r6, r2, r6 */
    r6 = r2 + r6;
    /* 0x08004F5A: 285d     ldrb     r0, [r5, r4] */
    r0 = *(uint8_t *)(r5 + r4);
    /* 0x08004F5C: 7278     ldrb     r2, [r6, #1] */
    r2 = *(uint8_t *)(r6 + 1);
    /* 0x08004F5E: 2f19     adds     r7, r5, r4 */
    r7 = r5 + r4;
    /* 0x08004F60: 8037     adds     r7, #0x80 */
    r7 = r7 + 0x80;
    /* 0x08004F62: 1043     orrs     r0, r2 */
    r0 = r0 | r2;
    /* 0x08004F64: 3a78     ldrb     r2, [r7] */
    r2 = *(uint8_t *)(r7);
    /* 0x08004F66: b678     ldrb     r6, [r6, #2] */
    r6 = *(uint8_t *)(r6 + 2);
    /* 0x08004F68: be46     mov      lr, r7 */
    lr = r7;
    /* 0x08004F6A: 3243     orrs     r2, r6 */
    r2 = r2 | r6;
    /* 0x08004F6C: 9840     lsls     r0, r3 */
    r0 = r0 << r3;
    /* 0x08004F6E: 0826     movs     r6, #8 */
    r6 = 8;
    /* 0x08004F70: f61a     subs     r6, r6, r3 */
    r6 = r6 - r3;
    /* 0x08004F72: 1746     mov      r7, r2 */
    r7 = r2;
    /* 0x08004F74: f740     lsrs     r7, r6 */
    r7 = r7 >> r6;
    /* 0x08004F76: 3843     orrs     r0, r7 */
    r0 = r0 | r7;
    /* 0x08004F78: 9a40     lsls     r2, r3 */
    r2 = r2 << r3;
    /* 0x08004F7A: 2855     strb     r0, [r5, r4] */
    *(uint8_t *)(r5 + r4) = r0;   /* store */
    /* 0x08004F7C: 7746     mov      r7, lr */
    r7 = lr;
    /* 0x08004F7E: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x08004F80: 3a70     strb     r2, [r7] */
    *(uint8_t *)(r7) = r2;   /* store */
    /* 0x08004F82: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);

    L_08004F84:
    /* 0x08004F84: 6145     cmp      r1, ip */
    /* flags = cmp(r1, ip) */
    /* 0x08004F86: ddd3     blo      #0x8004f44 */
    if (r1 < ip) goto L_08004F44;   /* unsigned */
    /* 0x08004F88: 05b0     add      sp, #0x14 */
    sp = sp + 0x14;
    /* 0x08004F8A: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
