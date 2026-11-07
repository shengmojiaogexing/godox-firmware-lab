// 0x0801755e FUN_0801755e size=40

undefined4 FUN_0801755e(int *param_1,int *param_2)

{
  if ((((param_2[2] < *param_1) || (param_1[2] < *param_2)) || (param_2[3] < param_1[1])) ||
     (param_1[3] < param_2[1])) {
    return 0;
  }
  return 1;
}

