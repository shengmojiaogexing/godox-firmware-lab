// 0x08028758 FUN_08028758 size=92

void FUN_08028758(int param_1,int param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  int iVar2;
  
  if (param_4 == 0) {
    FUN_08037ff4(param_1,param_3,s_No_data_080287b4);
    return;
  }
  if (2 < *(byte *)(param_2 + 0xf)) {
    iVar1 = 0;
    if (*(code **)(param_1 + 0x1c) != (code *)0x0) {
      iVar1 = (**(code **)(param_1 + 0x1c))(param_1,param_2,param_3,param_4);
    }
    iVar2 = 1;
    if (iVar1 != 1) {
      iVar2 = FUN_0801bf14(param_1,param_2,param_3,param_4);
    }
    if (iVar2 != 1) {
      FUN_08037ff4(param_1,param_3,s_No_data_080287b4);
      return;
    }
  }
  return;
}

