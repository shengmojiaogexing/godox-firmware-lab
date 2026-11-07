// 0x080274fe FUN_080274fe size=26

void FUN_080274fe(int *param_1,int param_2,int param_3)

{
  *param_1 = *param_1 + param_2;
  param_1[2] = param_2 + param_1[2];
  param_1[1] = param_1[1] + param_3;
  param_1[3] = param_1[3] + param_3;
  return;
}

