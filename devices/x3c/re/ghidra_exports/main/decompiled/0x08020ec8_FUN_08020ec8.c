// 0x08020ec8 FUN_08020ec8 size=172

void FUN_08020ec8(undefined4 param_1,int *param_2,undefined4 param_3,undefined2 param_4,int param_5,
                 int param_6,int param_7)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  piVar1 = (int *)FUN_08018528();
  if (param_6 == 0) {
    for (iVar2 = param_2[1]; iVar2 <= param_2[3]; iVar2 = iVar2 + 1) {
      for (iVar3 = *param_2; iVar3 <= param_2[2]; iVar3 = iVar3 + 1) {
        (**(code **)(*piVar1 + 0x28))(*piVar1,param_1,param_3,iVar3,iVar2,param_4,param_5);
      }
    }
  }
  else {
    iVar4 = *param_2;
    iVar2 = param_2[2];
    iVar5 = param_2[1];
    iVar3 = param_2[3];
    for (iVar7 = 0; iVar7 < (iVar3 - iVar5) + 1; iVar7 = iVar7 + 1) {
      for (iVar6 = 0; iVar6 < (iVar2 - iVar4) + 1; iVar6 = iVar6 + 1) {
        if (*(byte *)(param_6 + iVar6) != 0) {
          (**(code **)(*piVar1 + 0x28))
                    (*piVar1,param_1,param_3,*param_2 + iVar6,param_2[1] + iVar7,param_4,
                     (uint)*(byte *)(param_6 + iVar6) * param_5 >> 8);
        }
      }
      param_6 = param_6 + param_7;
    }
  }
  return;
}

