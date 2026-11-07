/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800E29A
 * Name      : FUN_0800E29A
 * End       : 0x0800E70C
 * Size      : 1138 bytes, 327 instructions decoded in body (functions.json insns=52, blocks=2)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800E29A(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800E29A: 00f80000 strb.w   r0, [r0, r0] */
    *(uint8_t *)(r0 + r0) = r0;   /* store */
    /* 0x0800E29E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E2A0: 0700     movs     r7, r0 */
    r7 = r0;
    /* 0x0800E2A2: f800     lsls     r0, r7, #3 */
    r0 = r7 << 3;
    /* 0x0800E2A4: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800E2A6: 0600     movs     r6, r0 */
    r6 = r0;
    /* 0x0800E2A8: 7800     lsls     r0, r7, #1 */
    r0 = r7 << 1;
    /* 0x0800E2AA: 0600     movs     r6, r0 */
    r6 = r0;
    /* 0x0800E2AC: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800E2AE: f800     lsls     r0, r7, #3 */
    r0 = r7 << 3;
    /* 0x0800E2B0: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800E2B2: 80c6     stm      r6!, {r7} */
    /* stm r6!, {r7} */
    /* 0x0800E2B4: 40ee2038 cdp      p8, #4, c3, c0, c0, #1 */
    /* TODO: cdp p8, #4, c3, c0, c0, #1 */
    /* 0x0800E2B8: 1038     subs     r0, #0x10 */
    r0 = r0 - 0x10;
    /* 0x0800E2BA: 08ee04c6 cdp      p6, #0, c12, c8, c4, #0 */
    /* TODO: cdp p6, #0, c12, c8, c4, #0 */
    /* 0x0800E2BE: 0200     movs     r2, r0 */
    r2 = r0;
    /* 0x0800E2C0: 0100     movs     r1, r0 */
    r1 = r0;
    /* 0x0800E2C2: 0680     strh     r6, [r0] */
    *(uint16_t *)(r0) = r6;   /* store */
    /* 0x0800E2C4: fc40     lsrs     r4, r7 */
    r4 = r4 >> r7;
    /* 0x0800E2C6: fe20     movs     r0, #0xfe */
    r0 = 0xfe;
    /* 0x0800E2C8: 0210     asrs     r2, r0, #0x20 */
    r2 = r0 >> 0x20;
    /* 0x0800E2CA: 0308     lsrs     r3, r0, #0x20 */
    r3 = r0 >> 0x20;
    /* 0x0800E2CC: ff04     lsls     r7, r7, #0x13 */
    r7 = r7 << 0x13;
    /* 0x0800E2CE: fc02     lsls     r4, r7, #0xb */
    r4 = r7 << 0xb;
    /* 0x0800E2D0: 0001     lsls     r0, r0, #4 */
    r0 = r0 << 4;
    /* 0x0800E2D2: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800E2D4: 0086     strh     r0, [r0, #0x30] */
    *(uint16_t *)(r0 + 48) = r0;   /* store */
    /* 0x0800E2D6: 008e     ldrh     r0, [r0, #0x30] */
    r0 = *(uint16_t *)(r0 + 48);
    /* 0x0800E2D8: 009a     ldr      r2, [sp] */
    r2 = *(uint32_t *)(sp);
    /* 0x0800E2DA: 00b2     sxth     r0, r0 */
    r0 = sxth(r0);
    /* 0x0800E2DC: 00e2     b        #0x800e6e0 */
    goto L_0800E6E0;
    /* 0x0800E38E: 0200     movs     r2, r0 */
    r2 = r0;
    /* 0x0800E390: 6600     lsls     r6, r4, #1 */
    r6 = r4 << 1;
    /* 0x0800E392: 6600     lsls     r6, r4, #1 */
    r6 = r4 << 1;
    /* 0x0800E394: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E396: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E398: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E39A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E39C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E39E: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800E3A0: 01fe0201 cdp2     p1, #0, c0, c1, c2, #0 */
    /* TODO: cdp2 p1, #0, c0, c1, c2, #0 */
    /* 0x0800E3A4: 0201     lsls     r2, r0, #4 */
    r2 = r0 << 4;
    /* 0x0800E3A6: 0201     lsls     r2, r0, #4 */
    r2 = r0 << 4;
    /* 0x0800E3A8: 0201     lsls     r2, r0, #4 */
    r2 = r0 << 4;
    /* 0x0800E3AA: 01fe0000 cdp2     p0, #0, c0, c1, c0, #0 */
    /* TODO: cdp2 p0, #0, c0, c1, c0, #0 */
    /* 0x0800E3AE: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3B0: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3B2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3B4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3B6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3B8: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3BA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3BC: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3BE: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3C0: 0600     movs     r6, r0 */
    r6 = r0;
    /* 0x0800E3C2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3C4: 8001     lsls     r0, r0, #6 */
    r0 = r0 << 6;
    /* 0x0800E3C6: 0003     lsls     r0, r0, #0xc */
    r0 = r0 << 0xc;
    /* 0x0800E3C8: ff00     lsls     r7, r7, #3 */
    r7 = r7 << 3;
    /* 0x0800E3CA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3CC: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3CE: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3D0: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3D2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3D4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3D6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3D8: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3DA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3DC: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3DE: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E3E0: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800E3E2: 0101     lsls     r1, r0, #4 */
    r1 = r0 << 4;
    /* 0x0800E3E4: 0203     lsls     r2, r0, #0xc */
    r2 = r0 << 0xc;
    /* 0x0800E3E6: 0205     lsls     r2, r0, #0x14 */
    r2 = r0 << 0x14;
    /* 0x0800E3E8: 0209     lsrs     r2, r0, #4 */
    r2 = r0 >> 4;
    /* 0x0800E3EA: 0211     asrs     r2, r0, #4 */
    r2 = r0 >> 4;
    /* 0x0800E3EC: 01e1     b        #0x800e5f2 */
    goto L_0800E5F2;
    /* 0x0800E4B4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4B6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4B8: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4BA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4BC: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4BE: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4C0: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4C2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4C4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4C6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4C8: 0601     lsls     r6, r0, #4 */
    r6 = r0 << 4;
    /* 0x0800E4CA: e202     lsls     r2, r4, #0xb */
    r2 = r4 << 0xb;
    /* 0x0800E4CC: 1102     lsls     r1, r2, #8 */
    r1 = r2 << 8;
    /* 0x0800E4CE: 1102     lsls     r1, r2, #8 */
    r1 = r2 << 8;
    /* 0x0800E4D0: 1102     lsls     r1, r2, #8 */
    r1 = r2 << 8;
    /* 0x0800E4D2: 2201     lsls     r2, r4, #4 */
    r2 = r4 << 4;
    /* 0x0800E4D4: fc00     lsls     r4, r7, #3 */
    r4 = r7 << 3;
    /* 0x0800E4D6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4D8: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4DA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4DC: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4DE: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4E0: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4E2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4E4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4E6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4E8: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800E4EA: 0003     lsls     r0, r0, #0xc */
    r0 = r0 << 0xc;
    /* 0x0800E4EC: 001c     adds     r0, r0, #0 */
    r0 = r0 + 0;
    /* 0x0800E4EE: 00e0     b        #0x800e4f2 */
    goto L_0800E4F2;

    L_0800E4F2:
    /* 0x0800E4F2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4F4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4F6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4F8: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4FA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4FC: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E4FE: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E500: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E502: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E504: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E506: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E508: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E50A: 0600     movs     r6, r0 */
    r6 = r0;
    /* 0x0800E50C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E50E: 0100     movs     r1, r0 */
    r1 = r0;
    /* 0x0800E510: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E512: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E514: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E516: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E518: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E51A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E51C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E51E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E520: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E522: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E524: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E526: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E528: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E52A: 0008     lsrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x0800E52C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E52E: 0010     asrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x0800E530: 0010     asrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x0800E532: 0010     asrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x0800E534: 00fe0010 cdp2     p0, #0, c1, c0, c0, #0 */
    /* TODO: cdp2 p0, #0, c1, c0, c0, #0 */
    /* 0x0800E538: 0010     asrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x0800E53A: 0010     asrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x0800E53C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E53E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E540: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E542: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E544: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E546: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E548: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E54A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E54C: 0800     movs     r0, r1 */
    r0 = r1;
    /* 0x0800E54E: 0003     lsls     r0, r0, #0xc */
    r0 = r0 << 0xc;
    /* 0x0800E550: fc04     lsls     r4, r7, #0x13 */
    r4 = r7 << 0x13;
    /* 0x0800E552: 0208     lsrs     r2, r0, #0x20 */
    r2 = r0 >> 0x20;
    /* 0x0800E554: 0108     lsrs     r1, r0, #0x20 */
    r1 = r0 >> 0x20;
    /* 0x0800E556: 0108     lsrs     r1, r0, #0x20 */
    r1 = r0 >> 0x20;
    /* 0x0800E558: 0104     lsls     r1, r0, #0x10 */
    r1 = r0 << 0x10;
    /* 0x0800E55A: 0203     lsls     r2, r0, #0xc */
    r2 = r0 << 0xc;
    /* 0x0800E55C: fc00     lsls     r4, r7, #3 */
    r4 = r7 << 3;
    /* 0x0800E55E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E560: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E562: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E564: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E566: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E568: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E56A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E56C: 0008     lsrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x0800E56E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E570: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E572: 0100     movs     r1, r0 */
    r1 = r0;
    /* 0x0800E574: 0200     movs     r2, r0 */
    r2 = r0;
    /* 0x0800E576: 0400     movs     r4, r0 */
    r4 = r0;
    /* 0x0800E578: 0fff0000 vhadd.u8 d0, d15, d0 */
    /* TODO: vhadd.u8 d0, d15, d0 */
    /* 0x0800E57C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E57E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E580: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E582: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E584: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E586: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E588: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E58A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E58C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E58E: 0800     movs     r0, r1 */
    r0 = r1;
    /* 0x0800E590: 0002     lsls     r0, r0, #8 */
    r0 = r0 << 8;
    /* 0x0800E592: 0104     lsls     r1, r0, #0x10 */
    r1 = r0 << 0x10;
    /* 0x0800E594: 0308     lsrs     r3, r0, #0x20 */
    r3 = r0 >> 0x20;
    /* 0x0800E596: 0508     lsrs     r5, r0, #0x20 */
    r5 = r0 >> 0x20;
    /* 0x0800E598: 0908     lsrs     r1, r1, #0x20 */
    r1 = r1 >> 0x20;
    /* 0x0800E59A: 110c     lsrs     r1, r2, #0x10 */
    r1 = r2 >> 0x10;
    /* 0x0800E59C: 6103     lsls     r1, r4, #0xd */
    r1 = r4 << 0xd;
    /* 0x0800E59E: 8100     lsls     r1, r0, #2 */
    r1 = r0 << 2;
    /* 0x0800E5A0: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5A2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5A4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5A6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5A8: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5AA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5AC: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5AE: 0008     lsrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x0800E5B0: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5B2: 0206     lsls     r2, r0, #0x18 */
    r2 = r0 << 0x18;
    /* 0x0800E5B4: 0402     lsls     r4, r0, #8 */
    r4 = r0 << 8;
    /* 0x0800E5B6: 0841     asrs     r0, r1 */
    r0 = r0 >> r1;
    /* 0x0800E5B8: 0841     asrs     r0, r1 */
    r0 = r0 >> r1;
    /* 0x0800E5BA: 08c1     stm      r1!, {r3} */
    /* stm r1!, {r3} */
    /* 0x0800E5BC: 07a2     adr      r2, #0x1c */
    r2 = 0x00000000;   /* address */
    /* 0x0800E5BE: 001c     adds     r0, r0, #0 */
    r0 = r0 + 0;
    /* 0x0800E5C0: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5C2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5C4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5C6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5C8: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5CA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5CC: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5CE: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5D0: 0800     movs     r0, r1 */
    r0 = r1;
    /* 0x0800E5D2: 1800     movs     r0, r3 */
    r0 = r3;
    /* 0x0800E5D4: 2800     movs     r0, r5 */
    r0 = r5;
    /* 0x0800E5D6: 4801     lsls     r0, r1, #5 */
    r0 = r1 << 5;
    /* 0x0800E5D8: 8802     lsls     r0, r1, #0xa */
    r0 = r1 << 0xa;
    /* 0x0800E5DA: 0804     lsls     r0, r1, #0x10 */
    r0 = r1 << 0x10;
    /* 0x0800E5DC: 080f     lsrs     r0, r1, #0x1c */
    r0 = r1 >> 0x1c;
    /* 0x0800E5DE: ff00     lsls     r7, r7, #3 */
    r7 = r7 << 3;
    /* 0x0800E5E0: 0800     movs     r0, r1 */
    r0 = r1;
    /* 0x0800E5E2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5E4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5E6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5E8: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5EA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5EC: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5EE: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5F0: 0008     lsrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;

    L_0800E5F2:
    /* 0x0800E5F2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E5F4: 01c4     stm      r4!, {r0} */
    /* stm r4!, {r0} */
    /* 0x0800E5F6: 0e82     strh     r6, [r1, #0x10] */
    *(uint16_t *)(r1 + 16) = r6;   /* store */
    /* 0x0800E5F8: 0881     strh     r0, [r1, #8] */
    *(uint16_t *)(r1 + 8) = r0;   /* store */
    /* 0x0800E5FA: 0881     strh     r0, [r1, #8] */
    *(uint16_t *)(r1 + 8) = r0;   /* store */
    /* 0x0800E5FC: 0881     strh     r0, [r1, #8] */
    *(uint16_t *)(r1 + 8) = r0;   /* store */
    /* 0x0800E5FE: 0842     tst      r0, r1 */
    /* flags = tst(r0, r1) */
    /* 0x0800E600: 083c     subs     r4, #8 */
    r4 = r4 - 8;
    /* 0x0800E602: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E604: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E606: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E608: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E60A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E60C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E60E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E610: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E612: 0800     movs     r0, r1 */
    r0 = r1;
    /* 0x0800E614: 0003     lsls     r0, r0, #0xc */
    r0 = r0 << 0xc;
    /* 0x0800E616: fc04     lsls     r4, r7, #0x13 */
    r4 = r7 << 0x13;
    /* 0x0800E618: 4208     lsrs     r2, r0, #1 */
    r2 = r0 >> 1;
    /* 0x0800E61A: 8108     lsrs     r1, r0, #2 */
    r1 = r0 >> 2;
    /* 0x0800E61C: 8108     lsrs     r1, r0, #2 */
    r1 = r0 >> 2;
    /* 0x0800E61E: 8104     lsls     r1, r0, #0x12 */
    r1 = r0 << 0x12;
    /* 0x0800E620: 4202     lsls     r2, r0, #9 */
    r2 = r0 << 9;
    /* 0x0800E622: 3c00     movs     r4, r7 */
    r4 = r7;
    /* 0x0800E624: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E626: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E628: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E62A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E62C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E62E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E630: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E632: 0008     lsrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x0800E634: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E636: 0800     movs     r0, r1 */
    r0 = r1;
    /* 0x0800E638: 0800     movs     r0, r1 */
    r0 = r1;
    /* 0x0800E63A: 0807     lsls     r0, r1, #0x1c */
    r0 = r1 << 0x1c;
    /* 0x0800E63C: 0878     ldrb     r0, [r1] */
    r0 = *(uint8_t *)(r1);
    /* 0x0800E63E: 0980     strh     r1, [r1] */
    *(uint16_t *)(r1) = r1;   /* store */
    /* 0x0800E640: 0e00     movs     r6, r1 */
    r6 = r1;
    /* 0x0800E642: 0800     movs     r0, r1 */
    r0 = r1;
    /* 0x0800E644: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E646: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E648: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E64A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E64C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E64E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E650: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E652: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E654: 0800     movs     r0, r1 */
    r0 = r1;
    /* 0x0800E656: 0003     lsls     r0, r0, #0xc */
    r0 = r0 << 0xc;
    /* 0x0800E658: 1c04     lsls     r4, r3, #0x10 */
    r4 = r3 << 0x10;
    /* 0x0800E65A: a208     lsrs     r2, r4, #2 */
    r2 = r4 >> 2;
    /* 0x0800E65C: 4108     lsrs     r1, r0, #1 */
    r1 = r0 >> 1;
    /* 0x0800E65E: 4108     lsrs     r1, r0, #1 */
    r1 = r0 >> 1;
    /* 0x0800E660: 4104     lsls     r1, r0, #0x11 */
    r1 = r0 << 0x11;
    /* 0x0800E662: a203     lsls     r2, r4, #0xe */
    r2 = r4 << 0xe;
    /* 0x0800E664: 1c00     movs     r4, r3 */
    r4 = r3;
    /* 0x0800E666: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E668: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E66A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E66C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E66E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E670: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E672: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E674: 0008     lsrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x0800E676: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E678: 03c4     stm      r4!, {r0, r1} */
    /* stm r4!, {r0, r1} */
    /* 0x0800E67A: 0422     movs     r2, #4 */
    r2 = 4;
    /* 0x0800E67C: 0811     asrs     r0, r1, #4 */
    r0 = r1 >> 4;
    /* 0x0800E67E: 0811     asrs     r0, r1, #4 */
    r0 = r1 >> 4;
    /* 0x0800E680: 0811     asrs     r0, r1, #4 */
    r0 = r1 >> 4;
    /* 0x0800E682: 0422     movs     r2, #4 */
    r2 = 4;

    L_0800E6E0:
    /* 0x0800E6E0: 0007     lsls     r0, r0, #0x1c */
    r0 = r0 << 0x1c;
    /* 0x0800E6E2: e000     lsls     r0, r4, #3 */
    r0 = r4 << 3;
    /* 0x0800E6E4: fc00     lsls     r4, r7, #3 */
    r4 = r7 << 3;
    /* 0x0800E6E6: 1e00     movs     r6, r3 */
    r6 = r3;
    /* 0x0800E6E8: fc07     lsls     r4, r7, #0x1f */
    r4 = r7 << 0x1f;
    /* 0x0800E6EA: e01e     subs     r0, r4, #3 */
    r0 = r4 - 3;
    /* 0x0800E6EC: 001f     subs     r0, r0, #4 */
    r0 = r0 - 4;
    /* 0x0800E6EE: fe1f     subs     r6, r7, #7 */
    r6 = r7 - 7;
    /* 0x0800E6F0: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800E6F2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E6F4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E6F6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E6F8: 000b     lsrs     r0, r0, #0xc */
    r0 = r0 >> 0xc;
    /* 0x0800E6FA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800E6FC: 03f00ffc bl       #0x8011f1e */
    FUN_08011F1E();   /* BL -> 0x08011F1E FUN_08011F1E */
    /* 0x0800E700: 0c0c     lsrs     r4, r1, #0x10 */
    r4 = r1 >> 0x10;
    /* 0x0800E702: 1806     lsls     r0, r3, #0x18 */
    r0 = r3 << 0x18;
    /* 0x0800E704: 1806     lsls     r0, r3, #0x18 */
    r0 = r3 << 0x18;
    /* 0x0800E706: 1806     lsls     r0, r3, #0x18 */
    r0 = r3 << 0x18;
    /* 0x0800E708: 1806     lsls     r0, r3, #0x18 */
    r0 = r3 << 0x18;
    /* 0x0800E70A: 0c0c     lsrs     r4, r1, #0x10 */
    r4 = r1 >> 0x10;
}
