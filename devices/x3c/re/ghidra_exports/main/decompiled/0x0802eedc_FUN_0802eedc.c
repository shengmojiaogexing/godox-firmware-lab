// 0x0802eedc FUN_0802eedc size=56

void FUN_0802eedc(undefined4 param_1,int *param_2)

{
  int iVar1;
  
  iVar1 = FUN_08026998(param_1,DAT_0802ef14);
  if (iVar1 == 0) {
    iVar1 = FUN_0802f0cc(param_1);
  }
  else {
    iVar1 = -*(int *)(iVar1 + 0x28);
  }
  *param_2 = iVar1;
  iVar1 = FUN_08026998(param_1,DAT_0802ef18);
  if (iVar1 == 0) {
    iVar1 = FUN_0802f0dc(param_1);
  }
  else {
    iVar1 = -*(int *)(iVar1 + 0x28);
  }
  param_2[1] = iVar1;
  return;
}

