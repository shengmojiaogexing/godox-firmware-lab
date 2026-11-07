// 0x0802ec02 FUN_0802ec02 size=62

void FUN_0802ec02(int param_1,int *param_2)

{
  FUN_080273fa(param_2,param_1 + 0x14);
  if (*(int *)(param_1 + 8) != 0) {
    *param_2 = *param_2 - *(int *)(*(int *)(param_1 + 8) + 0x18);
    param_2[2] = param_2[2] + *(int *)(*(int *)(param_1 + 8) + 0x18);
    param_2[1] = param_2[1] - *(int *)(*(int *)(param_1 + 8) + 0x18);
    param_2[3] = param_2[3] + *(int *)(*(int *)(param_1 + 8) + 0x18);
  }
  return;
}

