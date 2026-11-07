// 0x0801dd34 FUN_0801dd34 size=674

void FUN_0801dd34(int param_1,int param_2,int *param_3,int param_4,int param_5)

{
  byte bVar1;
  uint uVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  byte bVar11;
  int iVar12;
  uint uVar13;
  byte *pbVar14;
  int iVar15;
  uint uVar16;
  int iVar17;
  int local_90;
  int local_8c;
  int local_88;
  int local_84;
  int local_80;
  int local_7c;
  int local_78;
  int local_74;
  int local_70;
  uint local_6c;
  int *local_68 [2];
  undefined2 local_60;
  int local_5c;
  undefined1 local_58;
  int *local_54;
  undefined1 local_50;
  undefined1 local_4f;
  uint local_4c;
  int local_48;
  uint local_44;
  uint local_40;
  int local_3c;
  int local_38;
  int local_34;
  int iStack_30;
  int *local_2c;
  int iStack_28;
  
  iVar8 = DAT_0801dfe0;
  pbVar14 = DAT_0801dfdc;
  uVar13 = *(byte *)(param_4 + 0xe) & 0xf;
  bVar1 = *(byte *)(param_2 + 0x24);
  uVar7 = (uint)bVar1;
  if (uVar13 == 3) {
    uVar13 = 4;
  }
  if (uVar13 == 1) {
    iVar15 = DAT_0801dfd8 + -0x16;
    uVar16 = 0x80;
    uVar10 = 2;
  }
  else if (uVar13 == 2) {
    iVar15 = DAT_0801dfd8 + -0x14;
    uVar16 = 0xc0;
    uVar10 = 4;
  }
  else if (uVar13 == 4) {
    iVar15 = DAT_0801dfd8 + -0x10;
    uVar16 = 0xf0;
    uVar10 = 0x10;
  }
  else {
    if (uVar13 != 8) {
      return;
    }
    uVar16 = 0xff;
    uVar10 = 0x100;
    iVar15 = DAT_0801dfd8;
  }
  if (uVar7 < 0xfd) {
    if ((*DAT_0801dfdc != uVar7) || (*(uint *)(DAT_0801dfdc + 4) != uVar13)) {
      for (uVar2 = 0; uVar2 < uVar10; uVar2 = uVar2 + 1) {
        if (*(byte *)(iVar15 + uVar2) == 0xff) {
          bVar11 = bVar1;
        }
        else {
          bVar11 = (byte)(uVar7 * *(byte *)(iVar15 + uVar2) >> 8);
        }
        *(byte *)(iVar8 + uVar2) = bVar11;
      }
    }
    iVar15 = DAT_0801dfe0;
    *pbVar14 = bVar1;
    *(uint *)(pbVar14 + 4) = uVar13;
  }
  local_40 = (uint)*(ushort *)(param_4 + 6);
  uVar7 = (uint)*(ushort *)(param_4 + 8);
  iVar8 = *param_3;
  piVar3 = *(int **)(param_1 + 8);
  if (iVar8 < *piVar3) {
    iVar17 = *piVar3 - iVar8;
  }
  else {
    iVar17 = 0;
  }
  if (piVar3[2] < (int)(iVar8 + local_40)) {
    local_44 = (piVar3[2] - iVar8) + 1;
  }
  else {
    local_44 = local_40;
  }
  iVar8 = param_3[1];
  if (iVar8 < piVar3[1]) {
    local_48 = piVar3[1] - iVar8;
  }
  else {
    local_48 = 0;
  }
  if (piVar3[3] < (int)(iVar8 + uVar7)) {
    local_4c = (piVar3[3] - iVar8) + 1;
  }
  else {
    local_4c = uVar7;
  }
  uVar10 = iVar17 * uVar13 + uVar13 * local_40 * local_48;
  pbVar14 = (byte *)(param_5 + (uVar10 >> 3));
  local_34 = param_1;
  iStack_30 = param_2;
  local_2c = param_3;
  iStack_28 = param_4;
  FUN_0802d8d6(local_68,0x1c);
  local_60 = *(undefined2 *)(param_2 + 0xc);
  local_50 = *(undefined1 *)(param_2 + 0x24);
  local_4f = (undefined1)((*(byte *)(param_2 + 0x28) & 0x3f) >> 3);
  FUN_08018528();
  local_6c = FUN_08028248();
  if ((int)(local_40 * uVar7) <= (int)local_6c) {
    local_6c = local_40 * uVar7;
  }
  iVar4 = FUN_0802d624(local_6c);
  local_90 = *local_2c + iVar17;
  local_88 = (local_44 - 1) + *local_2c;
  local_8c = local_48 + local_2c[1];
  local_80 = (local_88 - local_90) + 1;
  local_84 = local_4c + local_8c;
  local_7c = local_90;
  local_78 = local_8c;
  local_74 = local_88;
  local_70 = local_8c;
  local_5c = iVar4;
  iVar5 = FUN_08029394(&local_90);
  local_68[0] = &local_7c;
  iVar6 = (local_40 + iVar17) - local_44;
  iVar8 = 0;
  local_54 = local_68[0];
  for (local_3c = local_48; uVar10 = uVar10 & 7, local_3c < (int)local_4c; local_3c = local_3c + 1)
  {
    uVar7 = uVar16 >> uVar10;
    iVar12 = iVar8;
    for (iVar9 = iVar17; iVar9 < (int)local_44; iVar9 = iVar9 + 1) {
      uVar2 = (*pbVar14 & uVar7) >> ((8 - uVar13) - uVar10 & 0xff);
      if (uVar2 == 0) {
        *(undefined1 *)(iVar4 + iVar12) = 0;
      }
      else {
        *(undefined1 *)(iVar4 + iVar12) = *(undefined1 *)(iVar15 + uVar2);
      }
      if (uVar10 < 8 - uVar13) {
        uVar10 = uVar10 + uVar13;
        uVar7 = uVar7 >> uVar13;
      }
      else {
        uVar10 = 0;
        pbVar14 = pbVar14 + 1;
        uVar7 = uVar16;
      }
      iVar12 = iVar12 + 1;
    }
    if (iVar5 != 0) {
      local_38 = iVar4 + iVar8;
      iVar8 = FUN_08029220(local_38,local_7c,local_70,local_80);
      local_58 = (undefined1)iVar8;
      if (iVar8 == 0) {
        FUN_0802d8d6(local_38,local_80);
      }
    }
    if ((local_44 - iVar17) + iVar12 < local_6c) {
      local_70 = local_70 + 1;
    }
    else {
      local_58 = 2;
      FUN_08029de4(local_34,local_68);
      local_78 = local_70 + 1;
      iVar12 = 0;
      local_70 = local_78;
    }
    uVar10 = uVar13 * iVar6 + uVar10;
    pbVar14 = pbVar14 + (uVar10 >> 3);
    iVar8 = iVar12;
  }
  if (local_78 != local_70) {
    local_70 = local_70 + -1;
    local_58 = 2;
    FUN_08029de4(local_34,local_68);
  }
  FUN_0802d6c0(iVar4);
  return;
}

