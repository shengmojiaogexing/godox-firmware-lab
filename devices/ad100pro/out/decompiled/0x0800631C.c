/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800631C
 * Name      : FUN_0800631C
 * End       : 0x08006366
 * Size      : 74 bytes, 37 instructions decoded in body (functions.json insns=37, blocks=3)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800631C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800631C: 0a05     lsls     r2, r1, #0x14 */
    r2 = r1 << 0x14;
    /* 0x0800631E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x08006320: 0802     lsls     r0, r1, #8 */
    r0 = r1 << 8;
    /* 0x08006322: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006324: b601     lsls     r6, r6, #6 */
    r6 = r6 << 6;
    /* 0x08006326: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006328: 7501     lsls     r5, r6, #5 */
    r5 = r6 << 5;
    /* 0x0800632A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800632C: 4a01     lsls     r2, r1, #5 */
    r2 = r1 << 5;
    /* 0x0800632E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006330: 8004     lsls     r0, r0, #0x12 */
    r0 = r0 << 0x12;
    /* 0x08006332: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006334: 9c01     lsls     r4, r3, #6 */
    r4 = r3 << 6;
    /* 0x08006336: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006338: 7d01     lsls     r5, r7, #5 */
    r5 = r7 << 5;
    /* 0x0800633A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800633C: aa01     lsls     r2, r5, #6 */
    r2 = r5 << 6;
    /* 0x0800633E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006340: 16d2     bhs      #0x8006370 */
    if (/* bhs */) goto L_08006370;   /* unsigned */
    /* 0x08006342: 0008     lsrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x08006344: 6701     lsls     r7, r4, #5 */
    r7 = r4 << 5;
    /* 0x08006346: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006348: 8c01     lsls     r4, r1, #6 */
    r4 = r1 << 6;
    /* 0x0800634A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800634C: 4e62     str      r6, [r1, #0x24] */
    *(uint32_t *)(r1 + 36) = r6;   /* store */
    /* 0x0800634E: 1058     ldr      r0, [r2, r0] */
    r0 = *(uint32_t *)(r2 + r0);
    /* 0x08006350: 39b4     push     {r0, r3, r4, r5} */
    push({r0, r3, r4, r5});
    /* 0x08006352: f83f     subs     r7, #0xf8 */
    r7 = r7 - 0xf8;
    /* 0x08006354: 8501     lsls     r5, r0, #6 */
    r5 = r0 << 6;
    /* 0x08006356: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006358: 3d01     lsls     r5, r7, #4 */
    r5 = r7 << 4;
    /* 0x0800635A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800635C: 7b14     asrs     r3, r7, #0x11 */
    r3 = r7 >> 0x11;
    /* 0x0800635E: ae47     blxns    r5 */
    /* TODO: blxns r5 */
    /* 0x08006360: e17a     ldrb     r1, [r4, #0xb] */
    r1 = *(uint8_t *)(r4 + 11);
    /* 0x08006362: 943f     subs     r7, #0x94 */
    r7 = r7 - 0x94;
    /* 0x08006364: b91e     subs     r1, r7, #2 */
    r1 = r7 - 2;
}
