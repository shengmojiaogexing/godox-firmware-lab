// 0x0802ee28 FUN_0802ee28 size=152

int FUN_0802ee28(int param_1)

{
  undefined4 uVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  undefined8 uVar10;
  
  iVar6 = DAT_0802eec0;
  uVar2 = FUN_0802ebf4(param_1);
  uVar1 = DAT_0802eec4;
  iVar8 = iVar6;
  for (uVar9 = 0; uVar9 < uVar2; uVar9 = uVar9 + 1) {
    iVar4 = *(int *)(**(int **)(param_1 + 8) + uVar9 * 4);
    iVar3 = FUN_0802fcac(iVar4,uVar1);
    if ((iVar3 == 0) && (iVar3 = *(int *)(iVar4 + 0x20), iVar8 <= iVar3)) {
      iVar8 = iVar3;
    }
  }
  iVar3 = FUN_0802f99e(param_1,0);
  iVar4 = FUN_0802f908(param_1,0);
  iVar5 = FUN_0802f86a(param_1,0);
  if (iVar8 != iVar6) {
    iVar8 = iVar8 - ((*(int *)(param_1 + 0x20) - iVar4) - iVar5);
  }
  iVar6 = FUN_0802f780(param_1);
  iVar7 = FUN_0802ed88(param_1);
  uVar10 = FUN_0802f0dc(param_1,iVar6 - (((iVar7 - iVar3) - iVar4) + iVar5 * -2));
  iVar6 = (int)((ulonglong)uVar10 >> 0x20) - (int)uVar10;
  if (iVar6 < iVar8) {
    iVar6 = iVar8;
  }
  return iVar6;
}

