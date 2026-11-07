// 0x080212e8 FUN_080212e8 size=242

int FUN_080212e8(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  
  iVar1 = FUN_0802f0a8(param_1);
  if (iVar1 == 0) {
    iVar7 = 0;
  }
  else {
    iVar7 = 0x1fffffff;
    iVar2 = FUN_0802f992(param_1,0);
    iVar3 = FUN_0802f8fc(param_1,0);
    uVar4 = FUN_0802ebf4(param_1);
    for (uVar9 = 0; uVar9 < uVar4; uVar9 = uVar9 + 1) {
      iVar8 = *(int *)(**(int **)(param_1 + 8) + uVar9 * 4);
      iVar5 = FUN_0802fcac(iVar8,DAT_080213dc);
      if ((iVar5 == 0) && (iVar5 = FUN_0802fc9e(iVar8,0x1000), iVar5 != 0)) {
        if (iVar1 == 1) {
          iVar8 = *(int *)(iVar8 + 0x18);
          iVar5 = *(int *)(param_1 + 0x18) + iVar2;
        }
        else if (iVar1 == 2) {
          iVar8 = *(int *)(iVar8 + 0x20);
          iVar5 = *(int *)(param_1 + 0x20) - iVar3;
        }
        else {
          if (iVar1 != 3) goto LAB_080213c4;
          iVar8 = *(int *)(iVar8 + 0x18) +
                  ((*(int *)(iVar8 + 0x20) - *(int *)(iVar8 + 0x18)) + 1) / 2;
          iVar5 = *(int *)(param_1 + 0x18) +
                  iVar2 + ((((*(int *)(param_1 + 0x20) - *(int *)(param_1 + 0x18)) + 1) - iVar2) -
                          iVar3) / 2;
        }
        iVar8 = iVar8 + param_4;
        if ((param_2 <= iVar8) && (iVar8 <= param_3)) {
          iVar8 = iVar8 - iVar5;
          if (iVar8 < 1) {
            iVar5 = -iVar8;
          }
          else {
            iVar5 = iVar8;
          }
          if (iVar7 < 1) {
            iVar6 = -iVar7;
          }
          else {
            iVar6 = iVar7;
          }
          if (iVar5 < iVar6) {
            iVar7 = iVar8;
          }
        }
      }
LAB_080213c4:
    }
    if (iVar7 == 0x1fffffff) {
      iVar7 = 0;
    }
    else {
      iVar7 = -iVar7;
    }
  }
  return iVar7;
}

