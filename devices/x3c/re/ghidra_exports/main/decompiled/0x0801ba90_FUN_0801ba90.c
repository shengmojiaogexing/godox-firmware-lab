// 0x0801ba90 FUN_0801ba90 size=12

void FUN_0801ba90(uint param_1)

{
  *DAT_0801ba9c = *DAT_0801ba9c & 0xffff1fff | (param_1 & 7) << 0xd;
  return;
}

