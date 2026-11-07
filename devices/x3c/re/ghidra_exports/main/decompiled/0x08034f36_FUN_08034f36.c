// 0x08034f36 FUN_08034f36 size=202

void FUN_08034f36(undefined4 param_1,int *param_2,undefined4 param_3,int param_4,int param_5,
                 int param_6,int param_7,int param_8)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  piVar1 = (int *)FUN_08018528();
  iVar5 = (param_2[2] - *param_2) + 1;
  iVar2 = (param_2[3] - param_2[1]) + 1;
  if (param_7 == 0) {
    for (iVar4 = 0; iVar4 < iVar2; iVar4 = iVar4 + 1) {
      for (iVar3 = 0; iVar3 < iVar5; iVar3 = iVar3 + 1) {
        (**(code **)(*piVar1 + 0x28))
                  (*piVar1,param_1,param_3,*param_2 + iVar3,param_2[1] + iVar4,
                   *(undefined2 *)(param_4 + iVar3 * 2),param_6);
      }
      param_4 = param_4 + param_5 * 2;
    }
  }
  else {
    for (iVar4 = 0; iVar4 < iVar2; iVar4 = iVar4 + 1) {
      for (iVar3 = 0; iVar3 < iVar5; iVar3 = iVar3 + 1) {
        if (*(byte *)(param_7 + iVar3) != 0) {
          (**(code **)(*piVar1 + 0x28))
                    (*piVar1,param_1,param_3,*param_2 + iVar3,param_2[1] + iVar4,
                     *(undefined2 *)(param_4 + iVar3 * 2),
                     (uint)*(byte *)(param_7 + iVar3) * param_6 >> 8);
        }
      }
      param_7 = param_7 + param_8;
      param_4 = param_4 + param_5 * 2;
    }
  }
  return;
}

