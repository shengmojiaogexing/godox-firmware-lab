// 0x08028008 FUN_08028008 size=52

int FUN_08028008(undefined4 param_1,int param_2)

{
  int iVar1;
  
  if (param_2 == 0) {
    return 0;
  }
  iVar1 = FUN_08028228(param_1);
  if (param_2 * iVar1 + 0x50 < 0x140) {
    return 1;
  }
  iVar1 = FUN_08028228(param_1);
  return (param_2 * iVar1 + 0x50) / 0xa0;
}

