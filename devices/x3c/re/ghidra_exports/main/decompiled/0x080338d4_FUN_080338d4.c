// 0x080338d4 FUN_080338d4 size=90

int FUN_080338d4(int param_1)

{
  param_1 = param_1 % 0x168;
  if (param_1 < 0) {
    param_1 = (int)(short)((short)param_1 + 0x168);
  }
  if (param_1 < 0x5a) {
    return (int)*(short *)(DAT_08033930 + param_1 * 2);
  }
  if (param_1 - 0x5aU < 0x5a) {
    return (int)*(short *)(DAT_08033930 + (short)(0xb4 - (short)param_1) * 2);
  }
  if (param_1 - 0xb4U < 0x5a) {
    return (int)-*(short *)(DAT_08033930 + (short)(param_1 - 0xb4U) * 2);
  }
  return (int)-*(short *)(DAT_08033930 + (short)(0x168 - (short)param_1) * 2);
}

