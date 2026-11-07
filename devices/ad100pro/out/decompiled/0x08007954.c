/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007954
 * Name      : FUN_08007954
 * End       : 0x08007980
 * Size      : 44 bytes, 22 instructions decoded in body (functions.json insns=22, blocks=1)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007954(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007954: 0203     lsls     r2, r0, #0xc */
    r2 = r0 << 0xc;
    /* 0x08007956: 0405     lsls     r4, r0, #0x14 */
    r4 = r0 << 0x14;
    /* 0x08007958: 0607     lsls     r6, r0, #0x1c */
    r6 = r0 << 0x1c;
    /* 0x0800795A: 0809     lsrs     r0, r1, #4 */
    r0 = r1 >> 4;
    /* 0x0800795C: 0a0b     lsrs     r2, r1, #0xc */
    r2 = r1 >> 0xc;
    /* 0x0800795E: 0c0d     lsrs     r4, r1, #0x14 */
    r4 = r1 >> 0x14;
    /* 0x08007960: 0e0f     lsrs     r6, r1, #0x1c */
    r6 = r1 >> 0x1c;
    /* 0x08007962: 1010     asrs     r0, r2, #0x20 */
    r0 = r2 >> 0x20;
    /* 0x08007964: 0102     lsls     r1, r0, #8 */
    r1 = r0 << 8;
    /* 0x08007966: 0304     lsls     r3, r0, #0x10 */
    r3 = r0 << 0x10;
    /* 0x08007968: 0506     lsls     r5, r0, #0x18 */
    r5 = r0 << 0x18;
    /* 0x0800796A: 0708     lsrs     r7, r0, #0x20 */
    r7 = r0 >> 0x20;
    /* 0x0800796C: 090a     lsrs     r1, r1, #8 */
    r1 = r1 >> 8;
    /* 0x0800796E: 0b0c     lsrs     r3, r1, #0x10 */
    r3 = r1 >> 0x10;
    /* 0x08007970: 0d0e     lsrs     r5, r1, #0x18 */
    r5 = r1 >> 0x18;
    /* 0x08007972: 0f10     asrs     r7, r1, #0x20 */
    r7 = r1 >> 0x20;
    /* 0x08007974: 0010     asrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x08007976: 0240     ands     r2, r0 */
    r2 = r2 & r0;
    /* 0x08007978: 0012     asrs     r0, r0, #8 */
    r0 = r0 >> 8;
    /* 0x0800797A: 7a00     lsls     r2, r7, #1 */
    r2 = r7 << 1;
    /* 0x0800797C: 006c     ldr      r0, [r0, #0x40] */
    r0 = *(uint32_t *)(r0 + 64);
    /* 0x0800797E: dc02     lsls     r4, r3, #0xb */
    r4 = r3 << 0xb;
}
