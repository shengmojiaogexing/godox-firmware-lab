/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080098F8
 * Name      : FUN_080098F8
 * End       : 0x0800998A
 * Size      : 146 bytes, 68 instructions decoded in body (functions.json insns=68, blocks=16)
 * Class     : function
 * Callers   : 2  -> 0x0800CF74 main, 0x0800D014 FUN_0800D014
 * Callees   : 4  -> 0x0800311C FUN_0800311C, 0x08006D58 FUN_08006D58, 0x08006D74 FUN_08006D74, 0x08006DC8 FUN_08006DC8
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080098F8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080098F8: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x080098FA: 2448     ldr      r0, [pc, #0x90]  ; =RAM_2000016A */
    r0 = *(uint32_t *)0x0800998C;   /* = RAM 0x2000016A */
    /* 0x080098FC: ff25     movs     r5, #0xff */
    r5 = 0xff;
    /* 0x080098FE: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08009900: 234c     ldr      r4, [pc, #0x8c]  ; =RAM_200001E0 */
    r4 = *(uint32_t *)0x08009990;   /* = RAM 0x200001E0 */
    /* 0x08009902: f535     adds     r5, #0xf5 */
    r5 = r5 + 0xf5;
    /* 0x08009904: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08009906: 03d0     beq      #0x8009910 */
    if (r0 == 1) goto L_08009910;
    /* 0x08009908: a08a     ldrh     r0, [r4, #0x14] */
    r0 = *(uint16_t *)(r4 + 20);
    /* 0x0800990A: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x0800990C: 37d1     bne      #0x800997e */
    if (r0 != r5) goto L_0800997E;
    /* 0x0800990E: 2de0     b        #0x800996c */
    goto L_0800996C;

    L_08009910:
    /* 0x08009910: 2048     ldr      r0, [pc, #0x80]  ; =RAM_2000016C */
    r0 = *(uint32_t *)0x08009994;   /* = RAM 0x2000016C */
    /* 0x08009912: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08009914: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009916: 06d0     beq      #0x8009926 */
    if (r0 == 0) goto L_08009926;
    /* 0x08009918: 1f48     ldr      r0, [pc, #0x7c]  ; =RAM_200001A8 */
    r0 = *(uint32_t *)0x08009998;   /* = RAM 0x200001A8 */
    /* 0x0800991A: 0088     ldrh     r0, [r0] */
    r0 = *(uint16_t *)(r0);
    /* 0x0800991C: 6083     strh     r0, [r4, #0x1a] */
    *(uint16_t *)(r4 + 26) = r0;   /* store */
    /* 0x0800991E: 1f48     ldr      r0, [pc, #0x7c]  ; =RAM_2000018E */
    r0 = *(uint32_t *)0x0800999C;   /* = RAM 0x2000018E */
    /* 0x08009920: 0388     ldrh     r3, [r0] */
    r3 = *(uint16_t *)(r0);
    /* 0x08009922: a383     strh     r3, [r4, #0x1c] */
    *(uint16_t *)(r4 + 28) = r3;   /* store */
    /* 0x08009924: 18e0     b        #0x8009958 */
    goto L_08009958;

    L_08009926:
    /* 0x08009926: 1e48     ldr      r0, [pc, #0x78]  ; =RAM_2000016B */
    r0 = *(uint32_t *)0x080099A0;   /* = RAM 0x2000016B */
    /* 0x08009928: 0521     movs     r1, #5 */
    r1 = 5;
    /* 0x0800992A: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800992C: 1e38     subs     r0, #0x1e */
    r0 = r0 - 0x1e;
    /* 0x0800992E: f9f7f5fb bl       #0x800311c */
    FUN_0800311C();   /* BL -> 0x0800311C FUN_0800311C */
    /* 0x08009932: e083     strh     r0, [r4, #0x1e] */
    *(uint16_t *)(r4 + 30) = r0;   /* store */
    /* 0x08009934: 1b48     ldr      r0, [pc, #0x6c]  ; =RAM_20000169 */
    r0 = *(uint32_t *)0x080099A4;   /* = RAM 0x20000169 */
    /* 0x08009936: 1c49     ldr      r1, [pc, #0x70]  ; =flash_0800DA86 */
    r1 = *(uint32_t *)0x080099A8;   /* = flash data/const 0x0800DA86 */
    /* 0x08009938: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800993A: 4000     lsls     r0, r0, #1 */
    r0 = r0 << 1;
    /* 0x0800993C: 095a     ldrh     r1, [r1, r0] */
    r1 = *(uint16_t *)(r1 + r0);
    /* 0x0800993E: a183     strh     r1, [r4, #0x1c] */
    *(uint16_t *)(r4 + 28) = r1;   /* store */
    /* 0x08009940: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x08009942: 13d0     beq      #0x800996c */
    if (r1 == 0) goto L_0800996C;
    /* 0x08009944: e08a     ldrh     r0, [r4, #0x16] */
    r0 = *(uint16_t *)(r4 + 22);
    /* 0x08009946: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x08009948: 0fd0     beq      #0x800996a */
    if (r1 == r0) goto L_0800996A;
    /* 0x0800994A: 8142     cmp      r1, r0 */
    /* flags = cmp(r1, r0) */
    /* 0x0800994C: 01d9     bls      #0x8009952 */
    if (r1 <= r0) goto L_08009952;   /* unsigned */
    /* 0x0800994E: 401d     adds     r0, r0, #5 */
    r0 = r0 + 5;
    /* 0x08009950: 00e0     b        #0x8009954 */
    goto L_08009954;

    L_08009952:
    /* 0x08009952: 401f     subs     r0, r0, #5 */
    r0 = r0 - 5;

    L_08009954:
    /* 0x08009954: e082     strh     r0, [r4, #0x16] */
    *(uint16_t *)(r4 + 22) = r0;   /* store */
    /* 0x08009956: 83b2     uxth     r3, r0 */
    r3 = uxth(r0);

    L_08009958:
    /* 0x08009958: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800995A: 1021     movs     r1, #0x10 */
    r1 = 0x10;
    /* 0x0800995C: 1348     ldr      r0, [pc, #0x4c]  ; =RAM_20000260 */
    r0 = *(uint32_t *)0x080099AC;   /* = RAM 0x20000260 */
    /* 0x0800995E: fdf7fbf9 bl       #0x8006d58 */
    FUN_08006D58();   /* BL -> 0x08006D58 FUN_08006D58 */
    /* 0x08009962: 1021     movs     r1, #0x10 */
    r1 = 0x10;
    /* 0x08009964: 1148     ldr      r0, [pc, #0x44]  ; =RAM_20000260 */
    r0 = *(uint32_t *)0x080099AC;   /* = RAM 0x20000260 */
    /* 0x08009966: fdf705fa bl       #0x8006d74 */
    FUN_08006D74();   /* BL -> 0x08006D74 FUN_08006D74 */

    L_0800996A:
    /* 0x0800996A: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_0800996C:
    /* 0x0800996C: e08a     ldrh     r0, [r4, #0x16] */
    r0 = *(uint16_t *)(r4 + 22);
    /* 0x0800996E: a842     cmp      r0, r5 */
    /* flags = cmp(r0, r5) */
    /* 0x08009970: fbd0     beq      #0x800996a */
    if (r0 == r5) goto L_0800996A;
    /* 0x08009972: e582     strh     r5, [r4, #0x16] */
    *(uint16_t *)(r4 + 22) = r5;   /* store */
    /* 0x08009974: 1021     movs     r1, #0x10 */
    r1 = 0x10;
    /* 0x08009976: 0d48     ldr      r0, [pc, #0x34]  ; =RAM_20000260 */
    r0 = *(uint32_t *)0x080099AC;   /* = RAM 0x20000260 */
    /* 0x08009978: fdf726fa bl       #0x8006dc8 */
    FUN_08006DC8();   /* BL -> 0x08006DC8 FUN_08006DC8 */
    /* 0x0800997C: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_0800997E:
    /* 0x0800997E: a582     strh     r5, [r4, #0x14] */
    *(uint16_t *)(r4 + 20) = r5;   /* store */
    /* 0x08009980: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x08009982: 0a48     ldr      r0, [pc, #0x28]  ; =RAM_20000260 */
    r0 = *(uint32_t *)0x080099AC;   /* = RAM 0x20000260 */
    /* 0x08009984: fdf720fa bl       #0x8006dc8 */
    FUN_08006DC8();   /* BL -> 0x08006DC8 FUN_08006DC8 */
    /* 0x08009988: f0e7     b        #0x800996c */
    goto L_0800996C;   /* loop */
}
