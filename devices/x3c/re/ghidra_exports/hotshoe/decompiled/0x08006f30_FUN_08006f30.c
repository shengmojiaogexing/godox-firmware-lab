// 0x08006f30 FUN_08006f30 size=6

void FUN_08006f30(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0x10) = ~param_2;
  return;
}

