// 0x08017734 FUN_08017734 size=52

void FUN_08017734(int *param_1,int *param_2,int *param_3)

{
  int iVar1;
  
  iVar1 = *param_2;
  if (*param_3 <= *param_2) {
    iVar1 = *param_3;
  }
  *param_1 = iVar1;
  iVar1 = param_2[1];
  if (param_3[1] <= param_2[1]) {
    iVar1 = param_3[1];
  }
  param_1[1] = iVar1;
  iVar1 = param_3[2];
  if (param_3[2] < param_2[2]) {
    iVar1 = param_2[2];
  }
  param_1[2] = iVar1;
  iVar1 = param_2[3];
  if (param_2[3] <= param_3[3]) {
    iVar1 = param_3[3];
  }
  param_1[3] = iVar1;
  return;
}

