// 0x0802ef1c FUN_0802ef1c size=182

int FUN_0802ef1c(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  undefined8 uVar9;
  
  iVar1 = FUN_0802f7ec(param_1,0,param_3,param_4,param_4);
  if (iVar1 == 1) {
    iVar2 = FUN_0802f968(param_1,0);
    iVar3 = FUN_0802f93e(param_1,0);
    iVar4 = FUN_0802f86a(param_1,0);
    iVar1 = 0x1fffffff;
    uVar5 = FUN_0802ebf4(param_1);
    for (uVar8 = 0; uVar8 < uVar5; uVar8 = uVar8 + 1) {
      iVar7 = *(int *)(**(int **)(param_1 + 8) + uVar8 * 4);
      iVar6 = FUN_0802fcac(iVar7,DAT_0802efd4);
      if ((iVar6 == 0) && (iVar6 = *(int *)(iVar7 + 0x14), iVar6 <= iVar1)) {
        iVar1 = iVar6;
      }
    }
    if (iVar1 == 0x1fffffff) {
      iVar1 = DAT_0802efd8;
    }
    else {
      iVar1 = (*(int *)(param_1 + 0x14) + iVar3 + iVar4) - iVar1;
    }
    iVar6 = FUN_0802f79c(param_1);
    iVar7 = FUN_0802fc5e(param_1);
    uVar9 = FUN_0802f0cc(param_1,iVar6 - (((iVar7 - iVar2) - iVar3) + iVar4 * -2));
    iVar2 = (int)uVar9 + (int)((ulonglong)uVar9 >> 0x20);
    if (iVar2 < iVar1) {
      iVar2 = iVar1;
    }
  }
  else if (*(int *)(param_1 + 8) == 0) {
    iVar2 = 0;
  }
  else {
    iVar2 = -*(int *)(*(int *)(param_1 + 8) + 0x10);
  }
  return iVar2;
}

