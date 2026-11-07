// 0x08037110 FUN_08037110 size=76

void FUN_08037110(int param_1,int *param_2,int *param_3)

{
  if (param_3 != (int *)0x0) {
    if (*(int *)(param_1 + 0x30) + *param_3 < *(int *)(param_1 + 0x58)) {
      *param_3 = *(int *)(param_1 + 0x58) - *(int *)(param_1 + 0x30);
    }
    if (*(int *)(param_1 + 0x60) < *(int *)(param_1 + 0x30) + *param_3) {
      *param_3 = *(int *)(param_1 + 0x60) - *(int *)(param_1 + 0x30);
    }
  }
  if (param_2 != (int *)0x0) {
    if (*(int *)(param_1 + 0x2c) + *param_2 < *(int *)(param_1 + 0x54)) {
      *param_2 = *(int *)(param_1 + 0x54) - *(int *)(param_1 + 0x2c);
    }
    if (*(int *)(param_1 + 0x5c) < *param_2 + *(int *)(param_1 + 0x2c)) {
      *param_2 = *(int *)(param_1 + 0x5c) - *(int *)(param_1 + 0x2c);
    }
  }
  return;
}

