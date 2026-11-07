// 0x08038224 FUN_08038224 size=20

void FUN_08038224(int param_1,uint param_2,int param_3)

{
  if (param_3 == 0) {
    *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & ~param_2;
    return;
  }
  *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) | param_2;
  return;
}

