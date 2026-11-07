// 0x080279d6 FUN_080279d6 size=102

uint FUN_080279d6(uint param_1,uint param_2,uint param_3,int param_4)

{
  uint uVar1;
  
  if (((param_2 & 0x7fffffff) >> 0x1d == 1) && (uVar1 = param_2 & 0x9fffffff, (int)uVar1 < 0x7d1)) {
    if (1000 < (int)uVar1) {
      uVar1 = 1000 - uVar1;
    }
    param_2 = (int)(param_4 * uVar1) / 100;
  }
  if (((param_3 & 0x7fffffff) >> 0x1d == 1) && (uVar1 = param_3 & 0x9fffffff, (int)uVar1 < 0x7d1)) {
    if (1000 < (int)uVar1) {
      uVar1 = 1000 - uVar1;
    }
    param_3 = (int)(param_4 * uVar1) / 100;
  }
  if ((int)param_1 < (int)param_3) {
    uVar1 = param_1;
  }
  else {
    uVar1 = param_3;
  }
  if ((int)uVar1 < (int)param_2) {
    return param_2;
  }
  if ((int)param_1 < (int)param_3) {
    return param_1;
  }
  return param_3;
}

