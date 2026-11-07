// 0x0803c344 FUN_0803c344 size=56

uint FUN_0803c344(uint param_1)

{
  undefined2 *unaff_r4;
  
  if (param_1 >> 0x17 < 0x9f) {
    return (param_1 << 8 | 0x80000000) >> (0x9e - (param_1 >> 0x17) & 0xff);
  }
  if (((int)param_1 < 0) && (param_1 << 1 < 0x7f000000)) {
    return 0;
  }
  FUN_0803c3aa();
  do {
    *unaff_r4 = (short)unaff_r4;
  } while( true );
}

