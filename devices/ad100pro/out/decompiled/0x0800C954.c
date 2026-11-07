/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C954
 * Name      : FUN_0800C954
 * End       : 0x0800CA38
 * Size      : 228 bytes, 86 instructions decoded in body (functions.json insns=86, blocks=1)
 * Class     : function
 * Callers   : 1  -> 0x0800D088 FUN_0800D088
 * Callees   : 1  -> 0x080047BE FUN_080047BE
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C954(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C954: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800C956: ae21     movs     r1, #0xae */
    r1 = 0xae;
    /* 0x0800C958: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C95A: f7f730ff bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C95E: 0221     movs     r1, #2 */
    r1 = 2;
    /* 0x0800C960: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C962: f7f72cff bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C966: 1021     movs     r1, #0x10 */
    r1 = 0x10;
    /* 0x0800C968: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C96A: f7f728ff bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C96E: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x0800C970: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C972: f7f724ff bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C976: b021     movs     r1, #0xb0 */
    r1 = 0xb0;
    /* 0x0800C978: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C97A: f7f720ff bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C97E: 8121     movs     r1, #0x81 */
    r1 = 0x81;
    /* 0x0800C980: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C982: f7f71cff bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C986: ff21     movs     r1, #0xff */
    r1 = 0xff;
    /* 0x0800C988: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C98A: f7f718ff bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C98E: a121     movs     r1, #0xa1 */
    r1 = 0xa1;
    /* 0x0800C990: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C992: f7f714ff bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C996: a621     movs     r1, #0xa6 */
    r1 = 0xa6;
    /* 0x0800C998: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C99A: f7f710ff bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C99E: a821     movs     r1, #0xa8 */
    r1 = 0xa8;
    /* 0x0800C9A0: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C9A2: f7f70cff bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C9A6: 3f21     movs     r1, #0x3f */
    r1 = 0x3f;
    /* 0x0800C9A8: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C9AA: f7f708ff bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C9AE: ad21     movs     r1, #0xad */
    r1 = 0xad;
    /* 0x0800C9B0: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C9B2: f7f704ff bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C9B6: 8b21     movs     r1, #0x8b */
    r1 = 0x8b;
    /* 0x0800C9B8: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C9BA: f7f700ff bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C9BE: 3321     movs     r1, #0x33 */
    r1 = 0x33;
    /* 0x0800C9C0: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C9C2: f7f7fcfe bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C9C6: c821     movs     r1, #0xc8 */
    r1 = 0xc8;
    /* 0x0800C9C8: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C9CA: f7f7f8fe bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C9CE: d321     movs     r1, #0xd3 */
    r1 = 0xd3;
    /* 0x0800C9D0: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C9D2: f7f7f4fe bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C9D6: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800C9D8: 0846     mov      r0, r1 */
    r0 = r1;
    /* 0x0800C9DA: f7f7f0fe bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C9DE: d521     movs     r1, #0xd5 */
    r1 = 0xd5;
    /* 0x0800C9E0: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C9E2: f7f7ecfe bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C9E6: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800C9E8: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C9EA: f7f7e8fe bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C9EE: d921     movs     r1, #0xd9 */
    r1 = 0xd9;
    /* 0x0800C9F0: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C9F2: f7f7e4fe bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C9F6: 1f21     movs     r1, #0x1f */
    r1 = 0x1f;
    /* 0x0800C9F8: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C9FA: f7f7e0fe bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800C9FE: da21     movs     r1, #0xda */
    r1 = 0xda;
    /* 0x0800CA00: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800CA02: f7f7dcfe bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800CA06: 1221     movs     r1, #0x12 */
    r1 = 0x12;
    /* 0x0800CA08: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800CA0A: f7f7d8fe bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800CA0E: db21     movs     r1, #0xdb */
    r1 = 0xdb;
    /* 0x0800CA10: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800CA12: f7f7d4fe bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800CA16: 3021     movs     r1, #0x30 */
    r1 = 0x30;
    /* 0x0800CA18: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800CA1A: f7f7d0fe bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800CA1E: a021     movs     r1, #0xa0 */
    r1 = 0xa0;
    /* 0x0800CA20: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800CA22: f7f7ccfe bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800CA26: c021     movs     r1, #0xc0 */
    r1 = 0xc0;
    /* 0x0800CA28: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800CA2A: f7f7c8fe bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800CA2E: af21     movs     r1, #0xaf */
    r1 = 0xaf;
    /* 0x0800CA30: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800CA32: f7f7c4fe bl       #0x80047be */
    FUN_080047BE();   /* BL -> 0x080047BE FUN_080047BE */
    /* 0x0800CA36: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
