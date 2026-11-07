// 0x08018c24 FUN_08018c24 size=24

void FUN_08018c24(int param_1,uint param_2,int param_3)

{
  if (param_3 == 1) {
    *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) | param_2;
    return;
  }
  if (param_3 == 0) {
    *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & ~param_2;
  }
  return;
}

