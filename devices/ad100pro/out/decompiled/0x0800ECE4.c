/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800ECE4
 * Name      : FUN_0800ECE4
 * End       : 0x0800ED78
 * Size      : 148 bytes, 55 instructions decoded in body (functions.json insns=55, blocks=2)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800ECE4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800ECE4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800ECE6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800ECE8: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800ECEA: 0380     strh     r3, [r0] */
    *(uint16_t *)(r0) = r3;   /* store */
    /* 0x0800ECEC: 0fe0     b        #0x800ed0e */
    goto L_0800ED0E;

    L_0800ED0E:
    /* 0x0800ED0E: 1041     asrs     r0, r2 */
    r0 = r0 >> r2;
    /* 0x0800ED10: 1001     lsls     r0, r2, #4 */
    r0 = r2 << 4;
    /* 0x0800ED12: 1001     lsls     r0, r2, #4 */
    r0 = r2 << 4;
    /* 0x0800ED14: 0802     lsls     r0, r1, #8 */
    r0 = r1 << 8;
    /* 0x0800ED16: 0c06     lsls     r4, r1, #0x18 */
    r4 = r1 << 0x18;
    /* 0x0800ED18: 060c     lsrs     r6, r0, #0x10 */
    r6 = r0 >> 0x10;
    /* 0x0800ED1A: 01f00000 and      r0, r1, #0 */
    r0 = r1 & 0;
    /* 0x0800ED1E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800ED20: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800ED22: 01f0060c and      ip, r1, #6 */
    ip = r1 & 6;
    /* 0x0800ED26: 0c06     lsls     r4, r1, #0x18 */
    r4 = r1 << 0x18;
    /* 0x0800ED28: 0802     lsls     r0, r1, #8 */
    r0 = r1 << 8;
    /* 0x0800ED2A: 0001     lsls     r0, r0, #4 */
    r0 = r0 << 4;
    /* 0x0800ED2C: 0001     lsls     r0, r0, #4 */
    r0 = r0 << 4;
    /* 0x0800ED2E: 1f81     strh     r7, [r3, #8] */
    *(uint16_t *)(r3 + 8) = r7;   /* store */
    /* 0x0800ED30: 0001     lsls     r0, r0, #4 */
    r0 = r0 << 4;
    /* 0x0800ED32: 0001     lsls     r0, r0, #4 */
    r0 = r0 << 4;
    /* 0x0800ED34: 0802     lsls     r0, r1, #8 */
    r0 = r1 << 8;
    /* 0x0800ED36: 0c06     lsls     r4, r1, #0x18 */
    r4 = r1 << 0x18;
    /* 0x0800ED38: 060c     lsrs     r6, r0, #0x10 */
    r6 = r0 >> 0x10;
    /* 0x0800ED3A: 01f00000 and      r0, r1, #0 */
    r0 = r1 & 0;
    /* 0x0800ED3E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800ED40: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800ED42: 07c0     stm      r0!, {r0, r1, r2} */
    /* stm r0!, {r0, r1, r2} */
    /* 0x0800ED44: 1830     adds     r0, #0x18 */
    r0 = r0 + 0x18;
    /* 0x0800ED46: 3018     adds     r0, r6, r0 */
    r0 = r6 + r0;
    /* 0x0800ED48: 2008     lsrs     r0, r4, #0x20 */
    r0 = r4 >> 0x20;
    /* 0x0800ED4A: 4384     strh     r3, [r0, #0x22] */
    *(uint16_t *)(r0 + 34) = r3;   /* store */
    /* 0x0800ED4C: 4c64     str      r4, [r1, #0x44] */
    *(uint32_t *)(r1 + 68) = r4;   /* store */
    /* 0x0800ED4E: 4824     movs     r4, #0x48 */
    r4 = 0x48;
    /* 0x0800ED50: 4824     movs     r4, #0x48 */
    r4 = 0x48;
    /* 0x0800ED52: 4824     movs     r4, #0x48 */
    r4 = 0x48;
    /* 0x0800ED54: 4444     add      r4, r8 */
    r4 = r4 + r8;
    /* 0x0800ED56: 2008     lsrs     r0, r4, #0x20 */
    r0 = r4 >> 0x20;
    /* 0x0800ED58: 3018     adds     r0, r6, r0 */
    r0 = r6 + r0;
    /* 0x0800ED5A: 1830     adds     r0, #0x18 */
    r0 = r0 + 0x18;
    /* 0x0800ED5C: 07c0     stm      r0!, {r0, r1, r2} */
    /* stm r0!, {r0, r1, r2} */
    /* 0x0800ED5E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800ED60: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800ED62: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x0800ED64: 03cc     ldm      r4!, {r0, r1} */
    /* ldm r4!, {r0, r1} */
    /* 0x0800ED66: 0244     add      r2, r0 */
    r2 = r2 + r0;
    /* 0x0800ED68: 0264     str      r2, [r0, #0x40] */
    *(uint32_t *)(r0 + 64) = r2;   /* store */
    /* 0x0800ED6A: 0224     movs     r4, #2 */
    r4 = 2;
    /* 0x0800ED6C: 033c     subs     r4, #3 */
    r4 = r4 - 3;
    /* 0x0800ED6E: 0198     ldr      r0, [sp, #4] */
    r0 = *(uint32_t *)(sp + 4);
    /* 0x0800ED70: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800ED72: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800ED74: 0200     movs     r2, r0 */
    r2 = r0;
    /* 0x0800ED76: 0600     movs     r6, r0 */
    r6 = r0;
}
