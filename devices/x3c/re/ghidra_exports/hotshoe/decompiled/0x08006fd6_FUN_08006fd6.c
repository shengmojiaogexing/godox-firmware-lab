// 0x08006fd6 FUN_08006fd6 size=8

void FUN_08006fd6(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) | param_2;
  return;
}

