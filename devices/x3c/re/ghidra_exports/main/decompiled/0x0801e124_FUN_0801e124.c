// 0x0801e124 FUN_0801e124 size=792

void FUN_0801e124(int param_1,undefined2 *param_2,int *param_3,int *param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int *local_15c [2];
  undefined2 local_154;
  int local_150;
  undefined1 local_14c;
  int *local_148;
  undefined1 local_144;
  int local_140;
  undefined4 local_13c;
  undefined4 local_138;
  undefined1 auStack_134 [56];
  undefined1 auStack_fc [56];
  undefined1 auStack_c4 [56];
  undefined1 auStack_8c [56];
  int local_54;
  int local_50;
  int local_4c;
  int local_48;
  int local_44;
  int local_40;
  int local_3c;
  int local_38;
  int local_34;
  undefined2 *puStack_30;
  int *piStack_2c;
  int *piStack_28;
  
  iVar1 = param_3[1];
  local_38 = param_4[1];
  if (iVar1 < local_38) {
    local_3c = *param_3;
    local_44 = *param_4;
    local_40 = local_38;
    local_38 = iVar1;
  }
  else {
    local_3c = *param_4;
    local_44 = *param_3;
    local_40 = iVar1;
  }
  iVar1 = local_44 - local_3c;
  iVar7 = local_40 - local_38;
  if (iVar1 < 1) {
    iVar9 = -iVar1;
  }
  else {
    iVar9 = iVar1;
  }
  if (iVar7 < 1) {
    iVar8 = -iVar7;
  }
  else {
    iVar8 = iVar7;
  }
  if (iVar8 < iVar9) {
    if (iVar7 < 1) {
      iVar3 = -iVar7;
    }
    else {
      iVar3 = iVar7;
    }
    if (iVar1 < 1) {
      iVar2 = -iVar1;
    }
    else {
      iVar2 = iVar1;
    }
    iVar2 = (iVar3 << 5) / iVar2;
  }
  else {
    if (iVar1 < 1) {
      iVar3 = -iVar1;
    }
    else {
      iVar3 = iVar1;
    }
    if (iVar7 < 1) {
      iVar2 = -iVar7;
    }
    else {
      iVar2 = iVar7;
    }
    iVar2 = (iVar3 << 5) / iVar2;
  }
  iVar3 = *(int *)(param_2 + 2) * (uint)*(byte *)(DAT_0801e43c + iVar2) + 0x3f;
  uVar4 = iVar3 >> 7;
  iVar3 = iVar3 >> 8;
  iVar2 = (uVar4 & 1) + iVar3;
  if (local_3c < local_44) {
    local_54 = local_3c;
  }
  else {
    local_54 = local_44;
  }
  local_54 = local_54 - uVar4;
  if (local_44 < local_3c) {
    local_4c = local_3c;
  }
  else {
    local_4c = local_44;
  }
  local_4c = local_4c + uVar4;
  if (local_38 < local_40) {
    local_50 = local_38;
  }
  else {
    local_50 = local_40;
  }
  local_50 = local_50 - uVar4;
  if (local_40 < local_38) {
    local_48 = local_38;
  }
  else {
    local_48 = local_40;
  }
  local_48 = uVar4 + local_48;
  local_34 = param_1;
  puStack_30 = param_2;
  piStack_2c = param_3;
  piStack_28 = param_4;
  iVar5 = FUN_08017484(&local_54,&local_54,*(undefined4 *)(param_1 + 8));
  if (iVar5 != 0) {
    if (iVar8 < iVar9) {
      if (iVar1 < 1) {
        FUN_08029500(auStack_8c,local_3c,local_38 + iVar2,local_44,local_40 + iVar2,0);
        FUN_08029500(auStack_c4,local_3c,local_38 - iVar3,local_44,local_40 - iVar3,1);
      }
      else {
        FUN_08029500(auStack_8c,local_3c,local_38 - iVar3,local_44,local_40 - iVar3,0);
        FUN_08029500(auStack_c4,local_3c,local_38 + iVar2,local_44,local_40 + iVar2,1);
      }
    }
    else {
      FUN_08029500(auStack_8c,local_3c + iVar2,local_38,local_44 + iVar2,local_40,0);
      FUN_08029500(auStack_c4,local_3c - iVar3,local_38,local_44 - iVar3,local_40,1);
    }
    local_138 = FUN_08028e78(auStack_8c,0);
    local_13c = FUN_08028e78(auStack_c4,0);
    iVar9 = -1;
    local_140 = -1;
    if (-1 < (int)((uint)*(byte *)((int)param_2 + 0x11) << 0x1b)) {
      FUN_08029500(auStack_fc,local_3c,local_38,local_3c - iVar7,local_38 + iVar1,3);
      FUN_08029500(auStack_134,local_44,local_40,local_44 - iVar7,local_40 + iVar1,2);
      local_140 = FUN_08028e78(auStack_fc,0);
      iVar9 = FUN_08028e78(auStack_134,0);
    }
    iVar1 = (local_4c - local_54) + 1;
    FUN_08018528();
    uVar4 = FUN_08028248();
    uVar6 = FUN_080274b0(&local_54);
    if (uVar6 < uVar4) {
      uVar4 = FUN_080274b0(&local_54);
    }
    iVar3 = FUN_0802d624(uVar4);
    iVar7 = local_48;
    local_48 = local_50;
    iVar2 = 0;
    FUN_0802d926(iVar3,uVar4);
    FUN_0802d8d6(local_15c,0x1c);
    local_15c[0] = &local_54;
    local_154 = *param_2;
    local_144 = *(undefined1 *)(param_2 + 8);
    local_150 = iVar3;
    local_148 = local_15c[0];
    for (iVar8 = local_50; iVar8 <= iVar7; iVar8 = iVar8 + 1) {
      iVar5 = FUN_08029220(iVar3 + iVar2,local_54,iVar8,iVar1);
      local_14c = (undefined1)iVar5;
      if (iVar5 == 0) {
        FUN_0802d8d6(iVar3 + iVar2,iVar1);
      }
      iVar2 = iVar2 + iVar1;
      if ((uint)(iVar2 + iVar1) < uVar4) {
        local_48 = local_48 + 1;
      }
      else {
        local_14c = 2;
        FUN_08029de4(local_34,local_15c);
        local_50 = local_48 + 1;
        iVar2 = 0;
        local_48 = local_50;
        FUN_0802d926(iVar3,uVar4);
      }
    }
    if (local_50 != local_48) {
      local_48 = local_48 + -1;
      local_14c = 2;
      FUN_08029de4(local_34,local_15c);
    }
    FUN_0802d6c0(iVar3);
    FUN_08029358(auStack_8c);
    FUN_08029358(auStack_c4);
    if (local_140 != -1) {
      FUN_08029358(auStack_fc);
    }
    if (iVar9 != -1) {
      FUN_08029358(auStack_134);
    }
    FUN_080299f4(local_138);
    FUN_080299f4(local_13c);
    FUN_080299f4(local_140);
    FUN_080299f4(iVar9);
  }
  return;
}

