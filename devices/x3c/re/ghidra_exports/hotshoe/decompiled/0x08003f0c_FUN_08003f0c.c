// 0x08003f0c FUN_08003f0c size=8

void FUN_08003f0c(int param_1,uint param_2)

{
  *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & ~param_2;
  return;
}

