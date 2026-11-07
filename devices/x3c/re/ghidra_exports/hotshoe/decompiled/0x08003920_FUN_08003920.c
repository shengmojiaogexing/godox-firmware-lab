// 0x08003920 FUN_08003920 size=10

void FUN_08003920(uint param_1)

{
  *(uint *)(DAT_0800392c + 0x10) = *(uint *)(DAT_0800392c + 0x10) & ~param_1;
  return;
}

