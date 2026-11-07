// 0x08034c2c FUN_08034c2c size=776

void FUN_08034c2c(int param_1,int *param_2,int param_3,int param_4,int param_5,uint param_6,
                 int *param_7,int param_8)

{
  undefined2 uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  undefined2 *puVar8;
  int iVar9;
  int local_44;
  
  iVar4 = *param_2;
  iVar2 = param_2[2];
  iVar7 = (iVar2 - iVar4) + 1;
  iVar9 = (param_2[3] - param_2[1]) + 1;
  if (param_7 == (int *)0x0) {
    if (param_6 < 0xfd) {
      for (iVar2 = 0; iVar2 < iVar9; iVar2 = iVar2 + 1) {
        for (iVar4 = 0; iVar4 < iVar7; iVar4 = iVar4 + 1) {
          puVar8 = (undefined2 *)(param_1 + iVar4 * 2);
          uVar1 = FUN_08027e60(*(undefined2 *)(param_4 + iVar4 * 2),*puVar8,param_6);
          *puVar8 = uVar1;
        }
        param_1 = param_1 + param_3 * 2;
        param_4 = param_4 + param_5 * 2;
      }
    }
    else {
      for (iVar2 = 0; iVar2 < iVar9; iVar2 = iVar2 + 1) {
        FUN_0802d764(param_1,param_4,iVar7 * 2);
        param_1 = param_1 + param_3 * 2;
        param_4 = param_4 + param_5 * 2;
      }
    }
  }
  else if (param_6 < 0xfe) {
    for (iVar2 = 0; iVar2 < iVar9; iVar2 = iVar2 + 1) {
      for (iVar4 = 0; iVar4 < iVar7; iVar4 = iVar4 + 1) {
        uVar3 = (uint)*(byte *)((int)param_7 + iVar4);
        if (uVar3 != 0) {
          if (uVar3 < 0xfd) {
            uVar3 = param_6 * uVar3 >> 8;
          }
          else {
            uVar3 = param_6;
          }
          puVar8 = (undefined2 *)(param_1 + iVar4 * 2);
          uVar1 = FUN_08027e60(*(undefined2 *)(param_4 + iVar4 * 2),*puVar8,uVar3);
          *puVar8 = uVar1;
        }
      }
      param_1 = param_1 + param_3 * 2;
      param_4 = param_4 + param_5 * 2;
      param_7 = (int *)((int)param_7 + param_8);
    }
  }
  else {
    for (local_44 = 0; local_44 < iVar9; local_44 = local_44 + 1) {
      iVar5 = 0;
      for (piVar6 = param_7; (iVar5 < iVar7 && (((uint)piVar6 & 3) != 0));
          piVar6 = (int *)((int)piVar6 + 1)) {
        if ((char)*piVar6 != '\0') {
          if ((char)*piVar6 == -1) {
            *(undefined2 *)(param_1 + iVar5 * 2) = *(undefined2 *)(param_4 + iVar5 * 2);
          }
          else {
            puVar8 = (undefined2 *)(param_1 + iVar5 * 2);
            uVar1 = FUN_08027e60(*(undefined2 *)(param_4 + iVar5 * 2),*puVar8);
            *puVar8 = uVar1;
          }
        }
        iVar5 = iVar5 + 1;
      }
      for (; iVar5 < (iVar2 - iVar4) + -3; iVar5 = iVar5 + 4) {
        if (*piVar6 != 0) {
          if (*piVar6 == -1) {
            *(undefined2 *)(param_1 + iVar5 * 2) = *(undefined2 *)(param_4 + iVar5 * 2);
            *(undefined2 *)(param_1 + (iVar5 + 1) * 2) = *(undefined2 *)(param_4 + (iVar5 + 1) * 2);
            *(undefined2 *)(param_1 + (iVar5 + 2) * 2) = *(undefined2 *)(param_4 + (iVar5 + 2) * 2);
            *(undefined2 *)(param_1 + (iVar5 + 3) * 2) = *(undefined2 *)(param_4 + (iVar5 + 3) * 2);
          }
          else {
            if ((char)*piVar6 != '\0') {
              if ((char)*piVar6 == -1) {
                *(undefined2 *)(param_1 + iVar5 * 2) = *(undefined2 *)(param_4 + iVar5 * 2);
              }
              else {
                puVar8 = (undefined2 *)(param_1 + iVar5 * 2);
                uVar1 = FUN_08027e60(*(undefined2 *)(param_4 + iVar5 * 2),*puVar8);
                *puVar8 = uVar1;
              }
            }
            if (*(char *)((int)piVar6 + 1) != '\0') {
              if (*(char *)((int)piVar6 + 1) == -1) {
                *(undefined2 *)(param_1 + (iVar5 + 1) * 2) =
                     *(undefined2 *)(param_4 + (iVar5 + 1) * 2);
              }
              else {
                puVar8 = (undefined2 *)(param_1 + (iVar5 + 1) * 2);
                uVar1 = FUN_08027e60(*(undefined2 *)(param_4 + (iVar5 + 1) * 2),*puVar8);
                *puVar8 = uVar1;
              }
            }
            if (*(char *)((int)piVar6 + 2) != '\0') {
              if (*(char *)((int)piVar6 + 2) == -1) {
                *(undefined2 *)(param_1 + (iVar5 + 2) * 2) =
                     *(undefined2 *)(param_4 + (iVar5 + 2) * 2);
              }
              else {
                puVar8 = (undefined2 *)(param_1 + (iVar5 + 2) * 2);
                uVar1 = FUN_08027e60(*(undefined2 *)(param_4 + (iVar5 + 2) * 2),*puVar8);
                *puVar8 = uVar1;
              }
            }
            if (*(char *)((int)piVar6 + 3) != '\0') {
              if (*(char *)((int)piVar6 + 3) == -1) {
                *(undefined2 *)(param_1 + (iVar5 + 3) * 2) =
                     *(undefined2 *)(param_4 + (iVar5 + 3) * 2);
              }
              else {
                uVar1 = FUN_08027e60();
                *(undefined2 *)(param_1 + (iVar5 + 3) * 2) = uVar1;
              }
            }
          }
        }
        piVar6 = piVar6 + 1;
      }
      for (; iVar5 < iVar7; iVar5 = iVar5 + 1) {
        if ((char)*piVar6 != '\0') {
          if ((char)*piVar6 == -1) {
            *(undefined2 *)(param_1 + iVar5 * 2) = *(undefined2 *)(param_4 + iVar5 * 2);
          }
          else {
            puVar8 = (undefined2 *)(param_1 + iVar5 * 2);
            uVar1 = FUN_08027e60(*(undefined2 *)(param_4 + iVar5 * 2),*puVar8);
            *puVar8 = uVar1;
          }
        }
        piVar6 = (int *)((int)piVar6 + 1);
      }
      param_1 = param_1 + param_3 * 2;
      param_4 = param_4 + param_5 * 2;
      param_7 = (int *)((int)param_7 + param_8);
    }
  }
  return;
}

