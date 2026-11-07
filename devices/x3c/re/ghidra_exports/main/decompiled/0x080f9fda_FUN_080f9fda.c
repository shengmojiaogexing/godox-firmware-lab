// 0x080f9fda FUN_080f9fda size=8

void FUN_080f9fda(int param_1,uint param_2)

{
  *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) | param_2;
  return;
}

