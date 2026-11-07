// 0x0802a01e FUN_0802a01e size=828

void FUN_0802a01e(int param_1,short *param_2,int *param_3,int param_4,int param_5)

{
  byte bVar1;
  ushort uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  int iVar11;
  ushort *puVar12;
  ushort uVar13;
  undefined1 auStack_9c [16];
  uint local_8c;
  uint local_88;
  int local_84;
  int local_80;
  int *local_7c;
  int local_78;
  short local_74;
  int local_70;
  undefined1 local_6c;
  int *local_68;
  undefined1 local_64;
  byte local_63;
  int local_60;
  int local_5c;
  int local_58;
  int local_54;
  int local_50;
  int local_4c;
  undefined1 auStack_48 [16];
  int local_38;
  int local_34;
  short *psStack_30;
  int *piStack_2c;
  int local_28;
  
  local_34 = param_1;
  psStack_30 = param_2;
  piStack_2c = param_3;
  local_28 = param_4;
  FUN_08027478(auStack_48,*(undefined4 *)(param_1 + 8));
  local_4c = FUN_08029394(auStack_48);
  if ((*param_2 == 0) && (param_2[1] == 0x100)) {
    local_50 = 0;
  }
  else {
    local_50 = 1;
  }
  FUN_0802d8d6(&local_7c,0x1c);
  local_64 = *(undefined1 *)((int)param_2 + 0xf);
  local_63 = *(byte *)(param_2 + 8) & 0xf;
  if (((local_4c == 0 && local_50 == 0) && (param_5 == 4)) && ((char)param_2[7] == '\0')) {
    local_78 = local_28;
    local_7c = param_3;
    FUN_08029de4(local_34,&local_7c);
  }
  else {
    local_7c = &local_60;
    local_68 = param_3;
    if ((local_4c == 0 && local_50 == 0) && (param_5 == 0xe)) {
      iVar3 = FUN_08017484(&local_8c,param_3,*(undefined4 *)(local_34 + 8));
      if (iVar3 != 0) {
        local_70 = local_28;
        local_78 = 0;
        local_74 = param_2[6];
        local_6c = 2;
        local_7c = param_3;
        FUN_08029de4(local_34,&local_7c);
      }
    }
    else if (((local_4c == 0 && local_50 == 0) && (param_5 == 0x14)) && ((char)param_2[7] == '\0'))
    {
      local_78 = local_28;
      local_70 = ((param_3[2] - *param_3) + 1) * 2 * ((param_3[3] - param_3[1]) + 1) + local_28;
      local_6c = 2;
      local_7c = param_3;
      FUN_08029de4(local_34,&local_7c);
    }
    else {
      local_60 = **(int **)(local_34 + 8);
      local_58 = *(int *)(*(int *)(local_34 + 8) + 8);
      local_5c = *(int *)(*(int *)(local_34 + 8) + 4);
      local_54 = *(int *)(*(int *)(local_34 + 8) + 0xc);
      local_80 = (param_3[2] - *param_3) + 1;
      local_84 = (param_3[3] - param_3[1]) + 1;
      uVar10 = (local_54 - local_5c) + 1;
      local_88 = (local_58 - local_60) + 1;
      FUN_08018528();
      uVar4 = FUN_08028248();
      uVar5 = FUN_080274b0(&local_60);
      if (uVar4 < uVar5) {
        uVar10 = uVar4 / local_88;
      }
      local_8c = local_88 * uVar10;
      iVar6 = FUN_0802d624(local_8c * 2);
      iVar7 = FUN_0802d624(local_8c);
      iVar3 = local_54;
      local_38 = uVar10 - 1;
      if (((param_5 == 4) && (*param_2 == 0)) && (param_2[1] == 0x100)) {
        local_6c = 1;
      }
      else {
        local_6c = 2;
      }
      local_78 = iVar6;
      local_70 = iVar7;
      local_68 = &local_60;
      local_54 = local_5c + local_38;
      while (local_5c <= iVar3) {
        FUN_08027478(auStack_9c,&local_60);
        FUN_080274fe(auStack_9c,-*param_3,-param_3[1]);
        if (local_50 == 0) {
          FUN_0801b8cc(auStack_9c,local_28,local_80,local_84,local_80,param_2,param_5,iVar6,iVar7);
        }
        else {
          FUN_0802acb4(local_34,auStack_9c,local_28,local_80,local_84,local_80,param_2,param_5,iVar6
                       ,iVar7);
        }
        if (2 < *(byte *)(param_2 + 7)) {
          bVar1 = *(byte *)(param_2 + 7);
          uVar4 = *(uint *)(param_2 + 6);
          uVar13 = (ushort)uVar4;
          iVar9 = 0xff - (uint)bVar1;
          for (uVar5 = 0; uVar5 < local_8c; uVar5 = uVar5 + 1) {
            puVar12 = (ushort *)(iVar6 + uVar5 * 2);
            uVar2 = *puVar12;
            *puVar12 = (ushort)((((uint)(uVar2 >> 0xb) * iVar9 +
                                 (uint)(ushort)((uVar13 >> 0xb) * (ushort)bVar1)) * 0x80810 >> 0x1b)
                               << 0xb) |
                       (ushort)(((((uVar2 & 0x7ff) >> 5) * iVar9 +
                                 (uint)(ushort)((short)((uVar4 & 0x7ff) >> 5) * (ushort)bVar1)) *
                                 0x40408 >> 0x1a) << 5) |
                       (ushort)(((uVar2 & 0x1f) * iVar9 +
                                (uint)(ushort)((uVar13 & 0x1f) * (ushort)bVar1)) * 0x80810 >> 0x1b);
          }
        }
        if (local_4c != 0) {
          iVar11 = iVar7;
          for (iVar9 = local_5c; iVar9 <= local_54; iVar9 = iVar9 + 1) {
            iVar8 = FUN_08029220(iVar11,local_60,iVar9,local_88);
            if (iVar8 == 0) {
              FUN_0802d8d6(iVar11,local_88);
              local_6c = 2;
            }
            else if (iVar8 == 2) {
              local_6c = 2;
            }
            iVar11 = iVar11 + local_88;
          }
        }
        FUN_08029de4(local_34,&local_7c);
        local_5c = local_54 + 1;
        local_54 = local_38 + local_5c;
        if (iVar3 < local_38 + local_5c) {
          local_54 = iVar3;
        }
      }
      FUN_0802d6c0(iVar7);
      FUN_0802d6c0(iVar6);
    }
  }
  return;
}

