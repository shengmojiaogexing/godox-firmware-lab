// 0x080038f0 FUN_080038f0 size=20

void FUN_080038f0(uint param_1)

{
  *(uint *)(DAT_08003904 + 0x20) = *(uint *)(DAT_08003904 + 0x20) & 0xfffffcff | (param_1 & 3) << 8;
  return;
}

