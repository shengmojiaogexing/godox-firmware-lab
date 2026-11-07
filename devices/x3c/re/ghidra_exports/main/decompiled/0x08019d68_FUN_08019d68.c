// 0x08019d68 FUN_08019d68 size=216

int FUN_08019d68(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  
  FUN_0803121c(param_1,0);
  iVar1 = FUN_0802f864(param_1,0);
  iVar2 = FUN_0802f998(param_1,0);
  iVar3 = FUN_0802f902(param_1,0);
  iVar4 = FUN_0802f780(param_1);
  iVar7 = DAT_08019e40;
  iVar4 = iVar4 + iVar2 + iVar1 + iVar3 + iVar1;
  uVar5 = FUN_0802ebf4(param_1);
  for (uVar9 = 0; uVar9 < uVar5; uVar9 = uVar9 + 1) {
    iVar8 = *(int *)(**(int **)(param_1 + 8) + uVar9 * 4);
    iVar6 = FUN_0802fcac(iVar8,DAT_08019e44);
    if (iVar6 == 0) {
      iVar6 = FUN_08030384(iVar8);
      if (iVar6 == 0) {
        iVar6 = FUN_0802f7c8(iVar8,0);
        if ((((iVar6 == 0) || (iVar6 == 1)) || (iVar6 == 2)) || (iVar6 == 3)) {
          iVar6 = *(int *)(iVar8 + 0x20) - *(int *)(param_1 + 0x18);
          if (iVar7 <= iVar6 + 1) {
            iVar7 = iVar6 + 1;
          }
        }
        else {
          iVar6 = FUN_0802fafa(iVar8,0);
          if ((iVar6 == 0) &&
             (iVar6 = (*(int *)(iVar8 + 0x20) - *(int *)(iVar8 + 0x18)) + 1 + iVar2 + iVar1,
             iVar7 <= iVar6)) {
            iVar7 = iVar6;
          }
        }
      }
      else {
        iVar6 = *(int *)(iVar8 + 0x20) - *(int *)(param_1 + 0x18);
        if (iVar7 <= iVar6 + 1) {
          iVar7 = iVar6 + 1;
        }
      }
    }
  }
  if (iVar7 == -0x1fffffff) {
    iVar7 = iVar4;
  }
  else {
    iVar7 = iVar7 + iVar3 + iVar1;
    if (iVar7 <= iVar4) {
      iVar7 = iVar4;
    }
  }
  return iVar7;
}

