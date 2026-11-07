// 0x08020b88 FUN_08020b88 size=832

void FUN_08020b88(ushort *param_1,int *param_2,int param_3,uint param_4,uint param_5,byte *param_6,
                 int param_7)

{
  short sVar1;
  ushort uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  ushort *puVar7;
  int iVar8;
  int iVar9;
  undefined4 uVar10;
  uint uVar11;
  int iVar12;
  uint local_40;
  uint local_3c;
  int local_38;
  ushort local_28;
  
  iVar6 = *param_2;
  iVar3 = param_2[2];
  iVar9 = (iVar3 - iVar6) + 1;
  iVar12 = (param_2[3] - param_2[1]) + 1;
  local_28 = (ushort)param_4;
  if (param_6 == (byte *)0x0) {
    if (param_5 < 0xfd) {
      local_3c = 0;
      local_40 = FUN_08027e60(param_4,0,param_5);
      uVar11 = param_5 + 4 >> 3 & 0x1f;
      sVar1 = (short)uVar11 * 8;
      iVar3 = uVar11 * -8 + 0xff;
      for (iVar6 = 0; iVar6 < iVar12; iVar6 = iVar6 + 1) {
        for (iVar8 = 0; iVar8 < iVar9; iVar8 = iVar8 + 1) {
          if (local_3c != param_1[iVar8]) {
            local_3c = (uint)param_1[iVar8];
            local_40 = (((uint)(param_1[iVar8] >> 0xb) * iVar3 +
                        (uint)(ushort)((local_28 >> 0xb) * sVar1)) * 0x80810 >> 0x1b) << 0xb |
                       ((((local_3c & 0x7ff) >> 5) * iVar3 +
                        (uint)(ushort)((short)((param_4 & 0x7ff) >> 5) * sVar1)) * 0x40408 >> 0x1a)
                       << 5 | ((local_3c & 0x1f) * iVar3 + (uint)(ushort)((local_28 & 0x1f) * sVar1)
                              ) * 0x80810 >> 0x1b;
          }
          param_1[iVar8] = (ushort)local_40;
        }
        param_1 = param_1 + param_3;
      }
    }
    else {
      for (iVar3 = 0; iVar3 < iVar12; iVar3 = iVar3 + 1) {
        FUN_08027b22(param_1,param_4,iVar9);
        param_1 = param_1 + param_3;
      }
    }
  }
  else {
    uVar10 = CONCAT22(local_28,local_28);
    if (param_5 < 0xfd) {
      uVar5 = 0;
      local_40 = (uint)*param_1;
      uVar11 = 0;
      local_3c = local_40;
      for (local_38 = 0; local_38 < iVar12; local_38 = local_38 + 1) {
        for (iVar3 = 0; iVar3 < iVar9; iVar3 = iVar3 + 1) {
          uVar4 = (uint)*param_6;
          if (uVar4 != 0) {
            if (uVar4 != uVar5) {
              if (uVar4 == 0xff) {
                uVar11 = param_5;
              }
              else {
                uVar11 = uVar4 * param_5 >> 8;
              }
            }
            if ((uVar4 != uVar5) || (local_3c != param_1[iVar3])) {
              if (uVar11 == 0xff) {
                local_40 = param_4;
              }
              else {
                local_40 = FUN_08027e60(param_4,param_1[iVar3],uVar11);
              }
              uVar5 = (uint)*param_6;
              local_3c = (uint)param_1[iVar3];
            }
            param_1[iVar3] = (ushort)local_40;
          }
          param_6 = param_6 + 1;
        }
        param_1 = param_1 + param_3;
        param_6 = param_6 + (param_7 - iVar9);
      }
    }
    else {
      for (local_38 = 0; local_38 < iVar12; local_38 = local_38 + 1) {
        iVar8 = 0;
        for (; (iVar8 < iVar9 && (((uint)param_6 & 3) != 0)); param_6 = param_6 + 1) {
          if (*param_6 == 0xff) {
            *param_1 = local_28;
          }
          else {
            uVar2 = FUN_08027e60(param_4,*param_1);
            *param_1 = uVar2;
          }
          param_1 = param_1 + 1;
          iVar8 = iVar8 + 1;
        }
        for (; iVar8 <= (iVar3 - iVar6) + -3; iVar8 = iVar8 + 4) {
          if (*(int *)param_6 == -1) {
            if (((uint)param_1 & 3) == 0) {
              *(undefined4 *)param_1 = uVar10;
              *(undefined4 *)(param_1 + 2) = uVar10;
            }
            else {
              *param_1 = local_28;
              *(undefined4 *)(param_1 + 1) = uVar10;
              param_1[3] = local_28;
            }
          }
          else if (*(int *)param_6 != 0) {
            if (*param_6 == 0xff) {
              *param_1 = local_28;
            }
            else {
              uVar2 = FUN_08027e60(param_4,*param_1);
              *param_1 = uVar2;
            }
            puVar7 = param_1 + 1;
            if (param_6[1] == 0xff) {
              *puVar7 = local_28;
            }
            else {
              uVar2 = FUN_08027e60(param_4,*puVar7);
              *puVar7 = uVar2;
            }
            puVar7 = param_1 + 2;
            if (param_6[2] == 0xff) {
              *puVar7 = local_28;
            }
            else {
              uVar2 = FUN_08027e60(param_4,*puVar7);
              *puVar7 = uVar2;
            }
            puVar7 = param_1 + 3;
            if (param_6[3] == 0xff) {
              *puVar7 = local_28;
            }
            else {
              uVar2 = FUN_08027e60(param_4,*puVar7);
              *puVar7 = uVar2;
            }
          }
          param_6 = param_6 + 4;
          param_1 = param_1 + 4;
        }
        for (; iVar8 < iVar9; iVar8 = iVar8 + 1) {
          if (*param_6 == 0xff) {
            *param_1 = local_28;
          }
          else {
            uVar2 = FUN_08027e60(param_4,*param_1);
            *param_1 = uVar2;
          }
          param_6 = param_6 + 1;
          param_1 = param_1 + 1;
        }
        param_1 = param_1 + (param_3 - iVar9);
        param_6 = param_6 + (param_7 - iVar9);
      }
    }
  }
  return;
}

