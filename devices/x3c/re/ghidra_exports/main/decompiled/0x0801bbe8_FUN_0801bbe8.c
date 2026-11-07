// 0x0801bbe8 FUN_0801bbe8 size=26

void FUN_0801bbe8(uint param_1)

{
  *DAT_0801bc04 = *DAT_0801bc04 & 0xffe0ffff | (param_1 >> 4 & 0x1f) << 0x10;
  DAT_0801bc04[0x1a] = DAT_0801bc04[0x1a] & 0xfffffcff | (param_1 & 3) << 8;
  return;
}

