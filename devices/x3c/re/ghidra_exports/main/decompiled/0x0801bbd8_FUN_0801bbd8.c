// 0x0801bbd8 FUN_0801bbd8 size=12

void FUN_0801bbd8(uint param_1)

{
  *DAT_0801bbe4 = *DAT_0801bbe4 & 0xffff7fff | (param_1 & 1) << 0xf;
  return;
}

