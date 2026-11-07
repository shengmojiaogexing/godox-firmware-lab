/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A30C
 * Name      : FUN_0800A30C
 * End       : 0x0800A476
 * Size      : 362 bytes, 157 instructions decoded in body (functions.json insns=157, blocks=17)
 * Class     : function
 * Callers   : 1  -> 0x0800B5C0 FUN_0800B5C0
 * Callees   : 2  -> 0x08004670 FUN_08004670, 0x0800A480 FUN_0800A480
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A30C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A30C: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x0800A30E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800A310: faf7aef9 bl       #0x8004670 */
    FUN_08004670();   /* BL -> 0x08004670 FUN_08004670 */
    /* 0x0800A314: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800A316: b020     movs     r0, #0xb0 */
    r0 = 0xb0;
    /* 0x0800A318: 00f0b2f8 bl       #0x800a480 */
    FUN_0800A480();   /* BL -> 0x0800A480 FUN_0800A480 */
    /* 0x0800A31C: 564c     ldr      r4, [pc, #0x158]  ; =RAM_2000022E */
    r4 = *(uint32_t *)0x0800A478;   /* = RAM 0x2000022E */
    /* 0x0800A31E: 0026     movs     r6, #0 */
    r6 = 0;
    /* 0x0800A320: 564d     ldr      r5, [pc, #0x158]  ; =RAM_200004CC */
    r5 = *(uint32_t *)0x0800A47C;   /* = RAM 0x200004CC */
    /* 0x0800A322: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */

    L_0800A324:
    /* 0x0800A324: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800A326: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800A328: 285c     ldrb     r0, [r5, r0] */
    r0 = *(uint8_t *)(r5 + r0);
    /* 0x0800A32A: 00f0a9f8 bl       #0x800a480 */
    FUN_0800A480();   /* BL -> 0x0800A480 FUN_0800A480 */
    /* 0x0800A32E: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800A330: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A332: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A334: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800A336: 8028     cmp      r0, #0x80 */
    /* flags = cmp(r0, #0x80) */
    /* 0x0800A338: f4d3     blo      #0x800a324 */
    if (r0 < 0x80) goto L_0800A324;   /* unsigned */
    /* 0x0800A33A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800A33C: faf798f9 bl       #0x8004670 */
    FUN_08004670();   /* BL -> 0x08004670 FUN_08004670 */
    /* 0x0800A340: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800A342: b120     movs     r0, #0xb1 */
    r0 = 0xb1;
    /* 0x0800A344: 00f09cf8 bl       #0x800a480 */
    FUN_0800A480();   /* BL -> 0x0800A480 FUN_0800A480 */
    /* 0x0800A348: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */

    L_0800A34A:
    /* 0x0800A34A: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800A34C: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800A34E: 2818     adds     r0, r5, r0 */
    r0 = r5 + r0;
    /* 0x0800A350: 8030     adds     r0, #0x80 */
    r0 = r0 + 0x80;
    /* 0x0800A352: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);  /* MMIO: NVIC.+0x300(r) */
    /* 0x0800A354: 00f094f8 bl       #0x800a480 */
    FUN_0800A480();   /* BL -> 0x0800A480 FUN_0800A480 */
    /* 0x0800A358: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800A35A: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A35C: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A35E: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800A360: 8028     cmp      r0, #0x80 */
    /* flags = cmp(r0, #0x80) */
    /* 0x0800A362: f2d3     blo      #0x800a34a */
    if (r0 < 0x80) goto L_0800A34A;   /* unsigned */
    /* 0x0800A364: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800A366: faf783f9 bl       #0x8004670 */
    FUN_08004670();   /* BL -> 0x08004670 FUN_08004670 */
    /* 0x0800A36A: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800A36C: b220     movs     r0, #0xb2 */
    r0 = 0xb2;
    /* 0x0800A36E: 00f087f8 bl       #0x800a480 */
    FUN_0800A480();   /* BL -> 0x0800A480 FUN_0800A480 */
    /* 0x0800A372: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */

    L_0800A374:
    /* 0x0800A374: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800A376: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800A378: 2818     adds     r0, r5, r0 */
    r0 = r5 + r0;
    /* 0x0800A37A: ff30     adds     r0, #0xff */
    r0 = r0 + 0xff;
    /* 0x0800A37C: 0130     adds     r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A37E: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);  /* MMIO: NVIC.+0x300(r) */
    /* 0x0800A380: 00f07ef8 bl       #0x800a480 */
    FUN_0800A480();   /* BL -> 0x0800A480 FUN_0800A480 */
    /* 0x0800A384: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800A386: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A388: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A38A: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800A38C: 8028     cmp      r0, #0x80 */
    /* flags = cmp(r0, #0x80) */
    /* 0x0800A38E: f1d3     blo      #0x800a374 */
    if (r0 < 0x80) goto L_0800A374;   /* unsigned */
    /* 0x0800A390: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800A392: faf76df9 bl       #0x8004670 */
    FUN_08004670();   /* BL -> 0x08004670 FUN_08004670 */
    /* 0x0800A396: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800A398: b320     movs     r0, #0xb3 */
    r0 = 0xb3;
    /* 0x0800A39A: 00f071f8 bl       #0x800a480 */
    FUN_0800A480();   /* BL -> 0x0800A480 FUN_0800A480 */
    /* 0x0800A39E: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */

    L_0800A3A0:
    /* 0x0800A3A0: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800A3A2: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800A3A4: 2818     adds     r0, r5, r0 */
    r0 = r5 + r0;
    /* 0x0800A3A6: ff30     adds     r0, #0xff */
    r0 = r0 + 0xff;
    /* 0x0800A3A8: 8130     adds     r0, #0x81 */
    r0 = r0 + 0x81;
    /* 0x0800A3AA: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);  /* MMIO: NVIC.+0x300(r) */
    /* 0x0800A3AC: 00f068f8 bl       #0x800a480 */
    FUN_0800A480();   /* BL -> 0x0800A480 FUN_0800A480 */
    /* 0x0800A3B0: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800A3B2: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A3B4: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A3B6: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800A3B8: 8028     cmp      r0, #0x80 */
    /* flags = cmp(r0, #0x80) */
    /* 0x0800A3BA: f1d3     blo      #0x800a3a0 */
    if (r0 < 0x80) goto L_0800A3A0;   /* unsigned */
    /* 0x0800A3BC: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800A3BE: faf757f9 bl       #0x8004670 */
    FUN_08004670();   /* BL -> 0x08004670 FUN_08004670 */
    /* 0x0800A3C2: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800A3C4: b420     movs     r0, #0xb4 */
    r0 = 0xb4;
    /* 0x0800A3C6: 00f05bf8 bl       #0x800a480 */
    FUN_0800A480();   /* BL -> 0x0800A480 FUN_0800A480 */
    /* 0x0800A3CA: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */

    L_0800A3CC:
    /* 0x0800A3CC: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800A3CE: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800A3D0: 2818     adds     r0, r5, r0 */
    r0 = r5 + r0;
    /* 0x0800A3D2: ff30     adds     r0, #0xff */
    r0 = r0 + 0xff;
    /* 0x0800A3D4: ff30     adds     r0, #0xff */
    r0 = r0 + 0xff;
    /* 0x0800A3D6: 0230     adds     r0, #2 */
    r0 = r0 + 2;
    /* 0x0800A3D8: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);  /* MMIO: NVIC.+0x300(r) */
    /* 0x0800A3DA: 00f051f8 bl       #0x800a480 */
    FUN_0800A480();   /* BL -> 0x0800A480 FUN_0800A480 */
    /* 0x0800A3DE: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800A3E0: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A3E2: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A3E4: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800A3E6: 8028     cmp      r0, #0x80 */
    /* flags = cmp(r0, #0x80) */
    /* 0x0800A3E8: f0d3     blo      #0x800a3cc */
    if (r0 < 0x80) goto L_0800A3CC;   /* unsigned */
    /* 0x0800A3EA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800A3EC: faf740f9 bl       #0x8004670 */
    FUN_08004670();   /* BL -> 0x08004670 FUN_08004670 */
    /* 0x0800A3F0: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800A3F2: b520     movs     r0, #0xb5 */
    r0 = 0xb5;
    /* 0x0800A3F4: 00f044f8 bl       #0x800a480 */
    FUN_0800A480();   /* BL -> 0x0800A480 FUN_0800A480 */
    /* 0x0800A3F8: 0527     movs     r7, #5 */
    r7 = 5;
    /* 0x0800A3FA: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */
    /* 0x0800A3FC: ff01     lsls     r7, r7, #7 */
    r7 = r7 << 7;

    L_0800A3FE:
    /* 0x0800A3FE: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800A400: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800A402: 2818     adds     r0, r5, r0 */
    r0 = r5 + r0;
    /* 0x0800A404: c019     adds     r0, r0, r7 */
    r0 = r0 + r7;
    /* 0x0800A406: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);  /* MMIO: NVIC.+0x300(r) */
    /* 0x0800A408: 00f03af8 bl       #0x800a480 */
    FUN_0800A480();   /* BL -> 0x0800A480 FUN_0800A480 */
    /* 0x0800A40C: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800A40E: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A410: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A412: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800A414: 8028     cmp      r0, #0x80 */
    /* flags = cmp(r0, #0x80) */
    /* 0x0800A416: f2d3     blo      #0x800a3fe */
    if (r0 < 0x80) goto L_0800A3FE;   /* unsigned */
    /* 0x0800A418: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800A41A: faf729f9 bl       #0x8004670 */
    FUN_08004670();   /* BL -> 0x08004670 FUN_08004670 */
    /* 0x0800A41E: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800A420: b620     movs     r0, #0xb6 */
    r0 = 0xb6;
    /* 0x0800A422: 00f02df8 bl       #0x800a480 */
    FUN_0800A480();   /* BL -> 0x0800A480 FUN_0800A480 */
    /* 0x0800A426: 0327     movs     r7, #3 */
    r7 = 3;
    /* 0x0800A428: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */
    /* 0x0800A42A: 3f02     lsls     r7, r7, #8 */
    r7 = r7 << 8;

    L_0800A42C:
    /* 0x0800A42C: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800A42E: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800A430: 2818     adds     r0, r5, r0 */
    r0 = r5 + r0;
    /* 0x0800A432: c019     adds     r0, r0, r7 */
    r0 = r0 + r7;
    /* 0x0800A434: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);  /* MMIO: NVIC.+0x300(r) */
    /* 0x0800A436: 00f023f8 bl       #0x800a480 */
    FUN_0800A480();   /* BL -> 0x0800A480 FUN_0800A480 */
    /* 0x0800A43A: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800A43C: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A43E: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A440: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800A442: 8028     cmp      r0, #0x80 */
    /* flags = cmp(r0, #0x80) */
    /* 0x0800A444: f2d3     blo      #0x800a42c */
    if (r0 < 0x80) goto L_0800A42C;   /* unsigned */
    /* 0x0800A446: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800A448: faf712f9 bl       #0x8004670 */
    FUN_08004670();   /* BL -> 0x08004670 FUN_08004670 */
    /* 0x0800A44C: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800A44E: b720     movs     r0, #0xb7 */
    r0 = 0xb7;
    /* 0x0800A450: 00f016f8 bl       #0x800a480 */
    FUN_0800A480();   /* BL -> 0x0800A480 FUN_0800A480 */
    /* 0x0800A454: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */
    /* 0x0800A456: 0726     movs     r6, #7 */
    r6 = 7;
    /* 0x0800A458: f601     lsls     r6, r6, #7 */
    r6 = r6 << 7;

    L_0800A45A:
    /* 0x0800A45A: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800A45C: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800A45E: 2818     adds     r0, r5, r0 */
    r0 = r5 + r0;
    /* 0x0800A460: 8019     adds     r0, r0, r6 */
    r0 = r0 + r6;
    /* 0x0800A462: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);  /* MMIO: NVIC.+0x300(r) */
    /* 0x0800A464: 00f00cf8 bl       #0x800a480 */
    FUN_0800A480();   /* BL -> 0x0800A480 FUN_0800A480 */
    /* 0x0800A468: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800A46A: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A46C: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A46E: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x0800A470: 8028     cmp      r0, #0x80 */
    /* flags = cmp(r0, #0x80) */
    /* 0x0800A472: f2d3     blo      #0x800a45a */
    if (r0 < 0x80) goto L_0800A45A;   /* unsigned */
    /* 0x0800A474: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
