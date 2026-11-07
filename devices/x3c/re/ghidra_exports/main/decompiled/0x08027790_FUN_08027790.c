// 0x08027790 FUN_08027790 size=82

void FUN_08027790(int param_1,int param_2,int param_3)

{
  int iVar1;
  int extraout_r2;
  
  if ((*(int *)(param_1 + 0x30) == param_2) && (*(int *)(param_1 + 0x34) == param_3)) {
    return;
  }
  *(int *)(param_1 + 0x34) = param_3;
  *(int *)(param_1 + 0x30) = param_2;
  iVar1 = FUN_0802774c(param_1);
  if (iVar1 != 2) {
    *(int *)(param_1 + 0x38) = param_2;
  }
  if (extraout_r2 < *(int *)(param_1 + 0x2c)) {
    *(int *)(param_1 + 0x2c) = extraout_r2;
    FUN_0802782e(param_1,extraout_r2,0);
  }
  if (*(int *)(param_1 + 0x2c) < param_2) {
    *(int *)(param_1 + 0x2c) = param_2;
    FUN_0802782e(param_1,param_2,0);
  }
  FUN_080302d0(param_1);
  return;
}

