// 0x08019e48 FUN_08019e48 size=374

int FUN_08019e48(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  
  FUN_080311f0(param_1,0);
  iVar1 = FUN_0802f864(param_1,0);
  iVar2 = FUN_0802f962(param_1,0);
  iVar2 = iVar2 + iVar1;
  iVar3 = FUN_0802f938(param_1,0);
  iVar3 = iVar3 + iVar1;
  iVar4 = FUN_0802f79c(param_1);
  iVar1 = DAT_08019fc0;
  iVar4 = iVar4 + iVar3 + iVar2;
  uVar5 = FUN_0802ebf4(param_1);
  iVar6 = FUN_0802f7e0(param_1,0);
  if (iVar6 == 1) {
    for (uVar8 = 0; uVar8 < uVar5; uVar8 = uVar8 + 1) {
      iVar7 = *(int *)(**(int **)(param_1 + 8) + uVar8 * 4);
      iVar6 = FUN_0802fcac(iVar7,DAT_08019fc4);
      if (iVar6 == 0) {
        iVar6 = FUN_08030384(iVar7);
        if (iVar6 == 0) {
          iVar6 = FUN_0802f7c8(iVar7,0);
          if ((((iVar6 == 0) || (iVar6 == 3)) || (iVar6 == 6)) || (iVar6 == 8)) {
            iVar6 = *(int *)(param_1 + 0x1c) - *(int *)(iVar7 + 0x14);
            if (iVar1 <= iVar6 + 1) {
              iVar1 = iVar6 + 1;
            }
          }
          else {
            iVar6 = FUN_0802faf4(iVar7,0);
            if (iVar6 == 0) {
              iVar6 = (*(int *)(iVar7 + 0x1c) - *(int *)(iVar7 + 0x14)) + 1 + iVar2;
              if (iVar1 <= iVar6) {
                iVar1 = iVar6;
              }
            }
          }
        }
        else {
          iVar6 = *(int *)(param_1 + 0x1c) - *(int *)(iVar7 + 0x14);
          if (iVar1 <= iVar6 + 1) {
            iVar1 = iVar6 + 1;
          }
        }
      }
    }
    if (iVar1 != -0x1fffffff) {
      iVar1 = iVar1 + iVar3;
    }
  }
  else {
    for (uVar8 = 0; uVar8 < uVar5; uVar8 = uVar8 + 1) {
      iVar7 = *(int *)(**(int **)(param_1 + 8) + uVar8 * 4);
      iVar6 = FUN_0802fcac(iVar7,DAT_08019fc4);
      if (iVar6 == 0) {
        iVar6 = FUN_08030384(iVar7);
        if (iVar6 == 0) {
          iVar6 = FUN_0802f7c8(iVar7,0);
          if (((iVar6 == 0) || (iVar6 == 1)) || ((iVar6 == 4 || (iVar6 == 7)))) {
            iVar6 = *(int *)(iVar7 + 0x1c) - *(int *)(param_1 + 0x14);
            if (iVar1 <= iVar6 + 1) {
              iVar1 = iVar6 + 1;
            }
          }
          else {
            iVar6 = FUN_0802fafa(iVar7,0);
            if (iVar6 == 0) {
              iVar6 = (*(int *)(iVar7 + 0x1c) - *(int *)(iVar7 + 0x14)) + 1 + iVar3;
              if (iVar1 <= iVar6) {
                iVar1 = iVar6;
              }
            }
          }
        }
        else {
          iVar6 = *(int *)(iVar7 + 0x1c) - *(int *)(param_1 + 0x14);
          if (iVar1 <= iVar6 + 1) {
            iVar1 = iVar6 + 1;
          }
        }
      }
    }
    if (iVar1 != -0x1fffffff) {
      iVar1 = iVar1 + iVar2;
    }
  }
  if (iVar1 == -0x1fffffff) {
  }
  else if (iVar4 < iVar1) {
    iVar4 = iVar1;
  }
  return iVar4;
}

