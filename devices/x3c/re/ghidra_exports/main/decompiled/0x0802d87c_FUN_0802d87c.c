// 0x0802d87c FUN_0802d87c size=90

void FUN_0802d87c(int *param_1,int param_2,uint param_3)

{
  int iVar1;
  
  if (((uint)param_1 & 3) != 0) {
    iVar1 = 4 - ((uint)param_1 & 3);
    for (; (iVar1 != 0 && (param_3 != 0)); param_3 = param_3 - 1) {
      *(char *)param_1 = (char)param_2;
      iVar1 = iVar1 + -1;
      param_1 = (int *)((int)param_1 + 1);
    }
  }
  iVar1 = param_2 * 0x1010101;
  for (; 0x20 < param_3; param_3 = param_3 - 0x20) {
    *param_1 = iVar1;
    param_1[1] = iVar1;
    param_1[2] = iVar1;
    param_1[3] = iVar1;
    param_1[4] = iVar1;
    param_1[5] = iVar1;
    param_1[6] = iVar1;
    param_1[7] = iVar1;
    param_1 = param_1 + 8;
  }
  for (; 4 < param_3; param_3 = param_3 - 4) {
    *param_1 = iVar1;
    param_1 = param_1 + 1;
  }
  for (; param_3 != 0; param_3 = param_3 - 1) {
    *(char *)param_1 = (char)param_2;
    param_1 = (int *)((int)param_1 + 1);
  }
  return;
}

