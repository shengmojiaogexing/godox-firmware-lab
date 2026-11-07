// 0x08026a90 FUN_08026a90 size=26

uint FUN_08026a90(uint param_1,int param_2,int param_3)

{
  int iVar1;
  
  iVar1 = param_2 - param_3;
  if (iVar1 < 1) {
    iVar1 = param_3 - param_2;
  }
  param_1 = (uint)(iVar1 * 1000) / param_1;
  if (param_1 == 0) {
    param_1 = 1;
  }
  return param_1;
}

