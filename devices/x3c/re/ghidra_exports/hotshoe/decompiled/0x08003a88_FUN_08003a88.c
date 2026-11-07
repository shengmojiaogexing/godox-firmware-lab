// 0x08003a88 FUN_08003a88 size=12

void FUN_08003a88(uint param_1)

{
  *(uint *)(DAT_08003a94 + 0xc) = *(uint *)(DAT_08003a94 + 0xc) & ~(param_1 >> 4);
  return;
}

