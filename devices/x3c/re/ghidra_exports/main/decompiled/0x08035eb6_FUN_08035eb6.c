// 0x08035eb6 FUN_08035eb6 size=116

void FUN_08035eb6(int param_1,int param_2,int param_3,int param_4,int *param_5,int *param_6)

{
  int iVar1;
  
  if ((param_4 < 2) && (((param_1 == 3 || (param_1 == 4)) || (param_1 == 5)))) {
    param_1 = 2;
  }
  param_2 = param_2 - param_3;
  switch(param_1) {
  default:
    *param_6 = 0;
    return;
  case 1:
    *param_6 = 0;
    *param_5 = *param_5 + param_2;
    return;
  case 2:
    *param_6 = 0;
    *param_5 = *param_5 + param_2 / 2;
    return;
  case 3:
    param_2 = param_2 / (param_4 + 1);
    *param_6 = param_2;
    *param_5 = param_2 + *param_5;
    return;
  case 4:
    iVar1 = param_2 / param_4 + *param_6;
    *param_6 = iVar1;
    *param_5 = *param_5 + iVar1 / 2;
    return;
  case 5:
    *param_6 = param_2 / (param_4 + -1);
    return;
  }
}

