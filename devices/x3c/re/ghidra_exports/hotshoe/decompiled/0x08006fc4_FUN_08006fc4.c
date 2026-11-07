// 0x08006fc4 FUN_08006fc4 size=8

void FUN_08006fc4(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & ~param_2;
  return;
}

