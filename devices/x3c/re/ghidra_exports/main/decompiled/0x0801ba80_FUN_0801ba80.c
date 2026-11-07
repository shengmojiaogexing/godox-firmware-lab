// 0x0801ba80 FUN_0801ba80 size=12

void FUN_0801ba80(uint param_1)

{
  *DAT_0801ba8c = *DAT_0801ba8c & 0xffffe3ff | (param_1 & 7) << 10;
  return;
}

