// 0x0801adec FUN_0801adec size=50

void FUN_0801adec(int *param_1,int *param_2)

{
  int iVar1;
  
  iVar1 = *param_2;
  if (iVar1 < 1) {
    *param_2 = param_1[1] * 2 + 3 + iVar1;
  }
  else {
    *param_2 = (param_1[1] - *param_1) * 2 + 5 + iVar1;
    *param_1 = *param_1 + -1;
  }
  param_1[1] = param_1[1] + 1;
  return;
}

