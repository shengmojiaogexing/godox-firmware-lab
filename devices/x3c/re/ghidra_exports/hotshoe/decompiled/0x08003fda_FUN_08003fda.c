// 0x08003fda FUN_08003fda size=8

void FUN_08003fda(int param_1,uint param_2)

{
  *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) | param_2;
  return;
}

