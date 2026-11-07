// 0x08003940 FUN_08003940 size=10

void FUN_08003940(uint param_1)

{
  *(uint *)(DAT_0800394c + 0xc) = *(uint *)(DAT_0800394c + 0xc) & ~param_1;
  return;
}

