// 0x0802efdc FUN_0802efdc size=176

int FUN_0802efdc(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  undefined8 uVar10;
  
  iVar2 = FUN_0802f7ec(param_1,0);
  iVar7 = DAT_0802f08c;
  if (iVar2 == 1) {
    if (*(int *)(param_1 + 8) == 0) {
      iVar7 = 0;
    }
    else {
      iVar7 = *(int *)(*(int *)(param_1 + 8) + 0x10);
    }
  }
  else {
    uVar3 = FUN_0802ebf4(param_1);
    uVar1 = DAT_0802f090;
    iVar2 = iVar7;
    for (uVar9 = 0; uVar9 < uVar3; uVar9 = uVar9 + 1) {
      iVar5 = *(int *)(**(int **)(param_1 + 8) + uVar9 * 4);
      iVar4 = FUN_0802fcac(iVar5,uVar1);
      if ((iVar4 == 0) && (iVar4 = *(int *)(iVar5 + 0x1c), iVar2 <= iVar4)) {
        iVar2 = iVar4;
      }
    }
    iVar4 = FUN_0802f968(param_1,0);
    iVar5 = FUN_0802f93e(param_1,0);
    iVar6 = FUN_0802f86a(param_1,0);
    if (iVar2 != iVar7) {
      iVar2 = iVar2 - ((*(int *)(param_1 + 0x1c) - iVar4) - iVar6);
    }
    iVar7 = FUN_0802f79c(param_1);
    iVar8 = FUN_0802fc5e(param_1);
    uVar10 = FUN_0802f0cc(param_1,iVar7 - (((iVar8 - iVar4) - iVar5) + iVar6 * -2));
    iVar7 = (int)((ulonglong)uVar10 >> 0x20) - (int)uVar10;
    if (iVar7 < iVar2) {
      iVar7 = iVar2;
    }
  }
  return iVar7;
}

