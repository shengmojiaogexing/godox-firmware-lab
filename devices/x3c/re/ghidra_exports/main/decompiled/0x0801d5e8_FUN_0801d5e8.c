// 0x0801d5e8 FUN_0801d5e8 size=1184

void FUN_0801d5e8(undefined4 param_1)

{
  bool bVar1;
  byte bVar2;
  short sVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  int *piVar8;
  int *piVar9;
  uint uVar10;
  int *piVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  undefined1 auStack_1b8 [4];
  undefined4 local_1b4;
  undefined4 local_1b0;
  int *local_1ac;
  undefined4 local_184;
  uint local_174;
  undefined1 auStack_170 [36];
  int local_14c;
  int local_148;
  int local_144;
  int local_140;
  int local_13c;
  int local_138;
  int local_134;
  int local_130;
  undefined1 auStack_12c [36];
  undefined1 auStack_108 [4];
  undefined4 local_104;
  undefined4 local_100;
  int *local_fc;
  int *local_f8;
  undefined4 local_d4;
  int local_c0;
  undefined1 local_bb;
  undefined1 local_a2;
  byte local_98;
  undefined1 local_78;
  undefined1 auStack_74 [16];
  code *local_64;
  int local_60;
  int local_5c;
  int local_58;
  int local_54;
  int local_50;
  int local_4c;
  int local_48;
  int local_44;
  int local_40;
  int local_3c;
  int local_38;
  int local_34;
  int local_30;
  undefined4 local_2c;
  int *local_28;
  
  iVar4 = FUN_0802ad70(param_1);
  local_2c = FUN_0802ad00(param_1);
  FUN_0802ed04(iVar4,&local_3c);
  iVar5 = FUN_0802f9bc(iVar4,0,0x68);
  local_174 = iVar5;
  local_174 = FUN_0802f9bc(iVar4,0,0x69);
  local_3c = local_3c - iVar5;
  local_34 = local_34 + iVar5;
  local_38 = local_38 - local_174;
  local_30 = local_174 + local_30;
  iVar13 = (local_34 - local_3c) + 1;
  iVar14 = (local_30 - local_38) + 1;
  iVar5 = *(int *)(iVar4 + 0x30);
  iVar12 = *(int *)(iVar4 + 0x34) - iVar5;
  bVar1 = iVar14 <= iVar13;
  local_40 = 0;
  if (((((*(byte *)(iVar4 + 0x6c) & 3) == 1) && (iVar5 < 0)) && (0 < *(int *)(iVar4 + 0x34))) &&
     (*(int *)(iVar4 + 0x38) == iVar5)) {
    local_40 = 1;
  }
  local_44 = FUN_0802f94a(iVar4,0);
  local_48 = FUN_0802f974(iVar4,0);
  local_4c = FUN_0802f9aa(iVar4,0);
  local_50 = FUN_0802f914(iVar4,0);
  local_28 = (int *)(iVar4 + 0x3c);
  FUN_08027442(local_28,&local_3c);
  *(int *)(iVar4 + 0x3c) = *(int *)(iVar4 + 0x3c) + local_44;
  *(int *)(iVar4 + 0x44) = *(int *)(iVar4 + 0x44) - local_48;
  *(int *)(iVar4 + 0x40) = *(int *)(iVar4 + 0x40) + local_4c;
  iVar5 = *(int *)(iVar4 + 0x48) - local_50;
  *(int *)(iVar4 + 0x48) = iVar5;
  if ((bVar1) && ((iVar5 - *(int *)(iVar4 + 0x40)) + 1 < 4)) {
    iVar5 = iVar14 / 2 + -2 + *(int *)(iVar4 + 0x18);
    *(int *)(iVar4 + 0x40) = iVar5;
    *(int *)(iVar4 + 0x48) = iVar5 + 4;
  }
  else if ((!bVar1) && ((*(int *)(iVar4 + 0x44) - *(int *)(iVar4 + 0x3c)) + 1 < 4)) {
    iVar5 = iVar13 / 2 + -2 + *(int *)(iVar4 + 0x14);
    *(int *)(iVar4 + 0x3c) = iVar5;
    *(int *)(iVar4 + 0x44) = iVar5 + 4;
  }
  local_54 = (*(int *)(iVar4 + 0x44) - *(int *)(iVar4 + 0x3c)) + 1;
  local_58 = (*(int *)(iVar4 + 0x48) - *(int *)(iVar4 + 0x40)) + 1;
  if (bVar1) {
    piVar11 = (int *)(iVar4 + 0x44);
    local_64 = DAT_0801da88;
    piVar8 = local_28;
    iVar5 = local_54;
  }
  else {
    piVar8 = (int *)(iVar4 + 0x40);
    piVar11 = (int *)(iVar4 + 0x48);
    local_64 = DAT_0801da8c;
    iVar5 = local_58;
  }
  if (*(int *)(iVar4 + 0x68) == -1) {
    local_60 = ((*(int *)(iVar4 + 0x38) - *(int *)(iVar4 + 0x30)) * iVar5) / iVar12;
  }
  else {
    local_60 = ((*(int *)(iVar4 + 0x60) - *(int *)(iVar4 + 0x30)) * iVar5) / iVar12;
    iVar6 = *(int *)(iVar4 + 0x68) *
            (((*(int *)(iVar4 + 100) - *(int *)(iVar4 + 0x30)) * iVar5) / iVar12 - local_60);
    local_60 = ((int)(iVar6 + ((uint)(iVar6 >> 0x1f) >> 0x18)) >> 8) + local_60;
  }
  if (*(int *)(iVar4 + 0x58) == -1) {
    local_5c = ((*(int *)(iVar4 + 0x2c) - *(int *)(iVar4 + 0x30)) * iVar5) / iVar12;
  }
  else {
    local_5c = ((*(int *)(iVar4 + 0x50) - *(int *)(iVar4 + 0x30)) * iVar5) / iVar12;
    iVar6 = *(int *)(iVar4 + 0x58) *
            (((*(int *)(iVar4 + 0x54) - *(int *)(iVar4 + 0x30)) * iVar5) / iVar12 - local_5c);
    local_5c = local_5c + ((int)(iVar6 + ((uint)(iVar6 >> 0x1f) >> 0x18)) >> 8);
  }
  local_174 = FUN_0802f9bc(iVar4,0,0x16);
  piVar9 = piVar8;
  if ((bVar1) && ((local_174 & 0xff) == 1)) {
    local_5c = -local_5c;
    local_60 = -local_60;
    piVar9 = piVar11;
    piVar11 = piVar8;
  }
  if (bVar1) {
    *piVar11 = local_5c + *piVar9;
    *piVar9 = local_60 + *piVar9;
  }
  else {
    *piVar9 = (*piVar11 - local_5c) + 1;
    *piVar11 = *piVar11 - local_60;
  }
  if (local_40 != 0) {
    iVar12 = (-*(int *)(iVar4 + 0x30) * iVar5) / iVar12;
    if (bVar1) {
      iVar12 = iVar12 + *piVar9;
      if (iVar12 < *piVar11) {
        *piVar9 = iVar12;
      }
      else {
        *piVar9 = *piVar11;
        *piVar11 = iVar12;
      }
    }
    else {
      iVar5 = (*piVar11 - iVar12) + 1;
      if (iVar5 < *piVar9) {
        *piVar11 = iVar5;
      }
      else {
        *piVar11 = *piVar9;
        *piVar9 = iVar5;
      }
      iVar5 = *piVar9;
      if (*piVar11 < iVar5) {
        *piVar9 = *piVar11;
        *piVar11 = iVar5;
      }
    }
  }
  uVar7 = DAT_0801da90;
  if ((local_40 == 0) && (iVar5 = (*local_64)(local_28), iVar5 < 2)) {
    FUN_0802e788(auStack_1b8,local_2c);
    local_184 = 0x20000;
    local_1b4 = uVar7;
    local_1b0 = 0;
    local_1ac = local_28;
    FUN_0802ad74(iVar4,0x1a,auStack_1b8);
    FUN_0802ad74(iVar4,0x1b,auStack_1b8);
  }
  else {
    FUN_08027442(auStack_74,local_28);
    FUN_08029a38(&local_c0);
    FUN_0802fff4(iVar4,0x20000,&local_c0);
    FUN_0802e788(auStack_108,local_2c);
    local_d4 = 0x20000;
    local_104 = uVar7;
    local_100 = 0;
    local_f8 = &local_c0;
    local_fc = local_28;
    FUN_0802ad74(iVar4,0x1a,auStack_108);
    local_174 = FUN_0802f9bc(iVar4,0,0xb);
    if (iVar13 < iVar14) {
      iVar5 = iVar13;
    }
    else {
      iVar5 = iVar14;
    }
    uVar10 = local_174;
    if (iVar5 >> 1 < (int)local_174) {
      uVar10 = iVar5 >> 1;
    }
    if (local_54 < local_58) {
      iVar5 = local_54;
    }
    else {
      iVar5 = local_58;
    }
    iVar12 = local_c0;
    if (iVar5 >> 1 < local_c0) {
      iVar12 = iVar5 >> 1;
    }
    if (((bVar1) && (iVar12 * 2 < (*(int *)(iVar4 + 0x44) - *(int *)(iVar4 + 0x3c)) + 1)) ||
       ((!bVar1 && (iVar12 * 2 < (*(int *)(iVar4 + 0x48) - *(int *)(iVar4 + 0x40)) + 1)))) {
      local_174 = (uint)local_98;
      local_bb = 0;
      local_a2 = 0;
      local_98 = 0;
      FUN_08029a14(local_2c,&local_c0,local_28);
      local_98 = (byte)local_174;
    }
    local_13c = local_44 + *(int *)(iVar4 + 0x14);
    local_134 = *(int *)(iVar4 + 0x1c) - local_48;
    local_138 = local_4c + *(int *)(iVar4 + 0x18);
    local_130 = *(int *)(iVar4 + 0x20) - local_50;
    FUN_080298a0(auStack_12c,&local_13c,uVar10,0);
    sVar3 = FUN_08028e78(auStack_12c,0);
    bVar2 = local_98;
    local_98 = 0;
    local_78 = 0;
    FUN_08027442(&local_14c,&local_3c);
    local_14c = local_44 + local_14c;
    local_148 = local_4c + local_148;
    local_144 = local_144 - local_48;
    local_140 = local_140 - local_50;
    if ((bVar1) && ((local_140 - local_148) + 1 < 4)) {
      local_148 = iVar14 / 2 + -2 + *(int *)(iVar4 + 0x18);
      local_140 = local_148 + 4;
    }
    else if ((!bVar1) && ((local_144 - local_14c) + 1 < 4)) {
      local_14c = iVar13 / 2 + -2 + *(int *)(iVar4 + 0x14);
      local_144 = local_14c + 4;
    }
    FUN_080298a0(auStack_170,local_28,local_c0,0);
    uVar7 = FUN_08028e78(auStack_170,0);
    FUN_08029a14(local_2c,&local_c0,&local_14c);
    local_bb = 0;
    local_a2 = 0;
    local_78 = 0;
    local_98 = bVar2;
    FUN_08029a14(local_2c,&local_c0,local_28);
    FUN_08029358(auStack_170);
    FUN_08029358(auStack_12c);
    FUN_080299f4(uVar7);
    FUN_080299f4((int)sVar3);
    FUN_0802ad74(iVar4,0x1b,auStack_108);
  }
  return;
}

