// 0x0801ba70 FUN_0801ba70 size=12

void FUN_0801ba70(uint param_1)

{
  *DAT_0801ba7c = *DAT_0801ba7c & 0xffffff0f | (param_1 & 0xf) << 4;
  return;
}

