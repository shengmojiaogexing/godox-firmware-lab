// 0x080277e2 FUN_080277e2 size=76

void FUN_080277e2(int param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  
  if ((*(byte *)(param_1 + 0x6c) & 3) == 2) {
    iVar1 = *(int *)(param_1 + 0x34);
    if (param_2 < iVar1) {
      iVar2 = param_2;
    }
    else {
      iVar2 = iVar1;
    }
    if (iVar2 < *(int *)(param_1 + 0x30)) {
      iVar1 = *(int *)(param_1 + 0x30);
    }
    else if (param_2 < iVar1) {
      iVar1 = param_2;
    }
    if (*(int *)(param_1 + 0x2c) < iVar1) {
      iVar1 = *(int *)(param_1 + 0x2c);
    }
    if (*(int *)(param_1 + 0x38) != iVar1) {
      FUN_08027870(param_1,iVar1,param_1 + 0x38,param_1 + 0x5c,param_3);
    }
  }
  return;
}

