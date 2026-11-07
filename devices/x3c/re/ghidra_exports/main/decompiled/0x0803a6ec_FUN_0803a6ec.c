// 0x0803a6ec FUN_0803a6ec size=20

void FUN_0803a6ec(int param_1,uint param_2,int param_3)

{
  if (param_3 == 1) {
    *(uint *)(param_1 + 0x18) = *(uint *)(param_1 + 0x18) | param_2;
    return;
  }
  *(uint *)(param_1 + 0x18) = *(uint *)(param_1 + 0x18) & ~param_2;
  return;
}

