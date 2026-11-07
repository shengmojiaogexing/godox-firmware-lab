// 0x08003930 FUN_08003930 size=10

void FUN_08003930(uint param_1)

{
  *(uint *)(DAT_0800393c + 0x18) = *(uint *)(DAT_0800393c + 0x18) & ~param_1;
  return;
}

