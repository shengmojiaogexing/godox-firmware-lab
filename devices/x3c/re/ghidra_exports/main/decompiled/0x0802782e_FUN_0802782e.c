// 0x0802782e FUN_0802782e size=66

void FUN_0802782e(int param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  
  if (*(int *)(param_1 + 0x2c) != param_2) {
    iVar1 = *(int *)(param_1 + 0x34);
    if (param_2 < iVar1) {
      iVar2 = param_2;
    }
    else {
      iVar2 = iVar1;
    }
    if (iVar2 < *(int *)(param_1 + 0x30)) {
      param_2 = *(int *)(param_1 + 0x30);
    }
    else if (iVar1 <= param_2) {
      param_2 = iVar1;
    }
    if (param_2 < *(int *)(param_1 + 0x38)) {
      param_2 = *(int *)(param_1 + 0x38);
    }
    if (*(int *)(param_1 + 0x2c) != param_2) {
      FUN_08027870(param_1,param_2,param_1 + 0x2c,param_1 + 0x4c,param_3);
    }
  }
  return;
}

