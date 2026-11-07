/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005354
 * Name      : FUN_08005354
 * End       : 0x080053B4
 * Size      : 96 bytes, 48 instructions decoded in body (functions.json insns=48, blocks=3)
 * Class     : function
 * Callers   : 1  -> 0x08005A18 FUN_08005A18
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005354(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005354: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x08005356: 174a     ldr      r2, [pc, #0x5c]  ; =RAM_200004CC */
    r2 = *(uint32_t *)0x080053B4;   /* = RAM 0x200004CC */
    /* 0x08005358: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x0800535A: c001     lsls     r0, r0, #7 */
    r0 = r0 << 7;
    /* 0x0800535C: 8418     adds     r4, r0, r2 */
    r4 = r0 + r2;

    L_0800535E:
    /* 0x0800535E: 0625     movs     r5, #6 */
    r5 = 6;
    /* 0x08005360: 154e     ldr      r6, [pc, #0x54]  ; =flash_0800F142 */
    r6 = *(uint32_t *)0x080053B8;   /* = flash data/const 0x0800F142 */
    /* 0x08005362: c818     adds     r0, r1, r3 */
    r0 = r1 + r3;
    /* 0x08005364: 5d43     muls     r5, r3, r5 */
    r5 = r3 * r5;
    /* 0x08005366: 225c     ldrb     r2, [r4, r0] */
    r2 = *(uint8_t *)(r4 + r0);
    /* 0x08005368: 775d     ldrb     r7, [r6, r5] */
    r7 = *(uint8_t *)(r6 + r5);
    /* 0x0800536A: ad19     adds     r5, r5, r6 */
    r5 = r5 + r6;
    /* 0x0800536C: 3a43     orrs     r2, r7 */
    r2 = r2 | r7;
    /* 0x0800536E: 2254     strb     r2, [r4, r0] */
    *(uint8_t *)(r4 + r0) = r2;   /* store */
    /* 0x08005370: 2218     adds     r2, r4, r0 */
    r2 = r4 + r0;
    /* 0x08005372: 1046     mov      r0, r2 */
    r0 = r2;
    /* 0x08005374: 8030     adds     r0, #0x80 */
    r0 = r0 + 0x80;
    /* 0x08005376: 0778     ldrb     r7, [r0] */
    r7 = *(uint8_t *)(r0);
    /* 0x08005378: 6e78     ldrb     r6, [r5, #1] */
    r6 = *(uint8_t *)(r5 + 1);
    /* 0x0800537A: 5b1c     adds     r3, r3, #1 */
    r3 = r3 + 1;
    /* 0x0800537C: 3743     orrs     r7, r6 */
    r7 = r7 | r6;
    /* 0x0800537E: 0770     strb     r7, [r0] */
    *(uint8_t *)(r0) = r7;   /* store */
    /* 0x08005380: 8030     adds     r0, #0x80 */
    r0 = r0 + 0x80;
    /* 0x08005382: 0678     ldrb     r6, [r0] */
    r6 = *(uint8_t *)(r0);
    /* 0x08005384: af78     ldrb     r7, [r5, #2] */
    r7 = *(uint8_t *)(r5 + 2);
    /* 0x08005386: dbb2     uxtb     r3, r3 */
    r3 = uxtb(r3);
    /* 0x08005388: 3e43     orrs     r6, r7 */
    r6 = r6 | r7;
    /* 0x0800538A: 0670     strb     r6, [r0] */
    *(uint8_t *)(r0) = r6;   /* store */
    /* 0x0800538C: 8030     adds     r0, #0x80 */
    r0 = r0 + 0x80;
    /* 0x0800538E: 0678     ldrb     r6, [r0] */
    r6 = *(uint8_t *)(r0);
    /* 0x08005390: ef78     ldrb     r7, [r5, #3] */
    r7 = *(uint8_t *)(r5 + 3);
    /* 0x08005392: 3e43     orrs     r6, r7 */
    r6 = r6 | r7;
    /* 0x08005394: 0670     strb     r6, [r0] */
    *(uint8_t *)(r0) = r6;   /* store */
    /* 0x08005396: 8030     adds     r0, #0x80 */
    r0 = r0 + 0x80;
    /* 0x08005398: 0678     ldrb     r6, [r0] */
    r6 = *(uint8_t *)(r0);
    /* 0x0800539A: 2f79     ldrb     r7, [r5, #4] */
    r7 = *(uint8_t *)(r5 + 4);
    /* 0x0800539C: 3e43     orrs     r6, r7 */
    r6 = r6 | r7;
    /* 0x0800539E: 0670     strb     r6, [r0] */
    *(uint8_t *)(r0) = r6;   /* store */
    /* 0x080053A0: 0520     movs     r0, #5 */
    r0 = 5;
    /* 0x080053A2: c001     lsls     r0, r0, #7 */
    r0 = r0 << 7;
    /* 0x080053A4: 1018     adds     r0, r2, r0 */
    r0 = r2 + r0;
    /* 0x080053A6: 0278     ldrb     r2, [r0] */
    r2 = *(uint8_t *)(r0);
    /* 0x080053A8: 6d79     ldrb     r5, [r5, #5] */
    r5 = *(uint8_t *)(r5 + 5);
    /* 0x080053AA: 2a43     orrs     r2, r5 */
    r2 = r2 | r5;
    /* 0x080053AC: 0270     strb     r2, [r0] */
    *(uint8_t *)(r0) = r2;   /* store */
    /* 0x080053AE: 302b     cmp      r3, #0x30 */
    /* flags = cmp(r3, #0x30) */
    /* 0x080053B0: d5d3     blo      #0x800535e */
    if (r3 < 0x30) goto L_0800535E;   /* unsigned */
    /* 0x080053B2: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */
}
