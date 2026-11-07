// 0x0801a774 FUN_0801a774 size=934

void FUN_0801a774(undefined4 param_1,undefined1 *param_2,int param_3,int param_4,int param_5,
                 int param_6,undefined4 param_7,int param_8,int *param_9)

{
  code *pcVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  bool bVar14;
  bool bVar15;
  int *local_5c;
  undefined4 local_58;
  undefined4 local_54;
  uint local_50;
  int local_4c;
  int local_48;
  int local_44;
  code *local_40;
  code *local_3c;
  int local_38;
  undefined4 local_34;
  undefined1 *puStack_30;
  int local_2c;
  int iStack_28;
  
  local_3c = DAT_0801ab20;
  pcVar1 = DAT_0801ab28;
  local_40 = DAT_0801ab24;
  if ((param_2[3] & 1) != 0) {
    local_3c = DAT_0801ab1c;
    pcVar1 = DAT_0801ab24;
    local_40 = DAT_0801ab28;
  }
  do {
    bVar14 = false;
    iVar2 = 0;
    iVar7 = param_9[1] - param_9[2];
    for (uVar8 = 0; uVar8 < (uint)param_9[5]; uVar8 = uVar8 + 1) {
      iVar3 = param_9[4];
      if ((*(byte *)(iVar3 + uVar8 * 0x18 + 0x14) & 1) == 0) {
        iVar2 = iVar2 + *(int *)(iVar3 + uVar8 * 0x18 + 0x10);
      }
      else {
        iVar7 = iVar7 - *(int *)(iVar3 + uVar8 * 0x18 + 0xc);
      }
    }
    for (uVar8 = 0; uVar8 < (uint)param_9[5]; uVar8 = uVar8 + 1) {
      iVar3 = param_9[4];
      local_38 = uVar8 * 0x18 + 0x14;
      if ((*(byte *)(iVar3 + local_38) & 1) == 0) {
        if (iVar2 == 0) {
          do {
                    /* WARNING: Do nothing block with infinite loop */
          } while( true );
        }
        iVar9 = uVar8 * 0x18 + 0x10;
        iVar10 = *(int *)(iVar3 + iVar9) * (iVar7 / iVar2);
        iVar12 = *(int *)(iVar3 + uVar8 * 0x18 + 8);
        iVar11 = *(int *)(iVar3 + uVar8 * 0x18 + 4);
        if (iVar10 < iVar12) {
          iVar13 = iVar10;
        }
        else {
          iVar13 = iVar12;
        }
        if (iVar11 <= iVar13) {
          if (iVar10 < iVar12) {
            iVar11 = iVar10;
          }
          else {
            iVar11 = iVar12;
          }
        }
        if (iVar11 != iVar10) {
          bVar14 = true;
          *(undefined4 *)(iVar3 + local_38) = 1;
        }
        *(int *)(param_9[4] + uVar8 * 0x18 + 0xc) = iVar11;
        iVar2 = iVar2 - *(int *)(param_9[4] + iVar9);
        iVar7 = iVar7 - iVar11;
      }
    }
  } while (bVar14);
  local_34 = param_1;
  puStack_30 = param_2;
  local_2c = param_3;
  iStack_28 = param_4;
  iVar2 = FUN_0802f7f8(param_1,0);
  bVar14 = iVar2 == 1;
  local_44 = 0;
  local_48 = 0;
  local_5c = &local_48;
  FUN_08035eb6(*param_2,param_7,param_9[1],param_9[3],&local_44);
  if ((param_2[3] & bVar14) != 0) {
    iVar2 = FUN_0802ecd0(local_34);
    local_44 = iVar2 + local_44;
  }
  iVar2 = FUN_0802ebc8(local_34,local_2c);
  do {
    while( true ) {
      if ((iVar2 == 0) || (local_2c == iStack_28)) {
        return;
      }
      iVar7 = FUN_0802fcac(iVar2,DAT_0801ab2c);
      if (iVar7 == 0) break;
      iVar2 = FUN_080224dc(local_34,((byte)param_2[3] & 7) >> 2,&local_2c);
    }
    iVar7 = FUN_0802f888(iVar2,0);
    if (iVar7 == 0) {
      *(ushort *)(iVar2 + 0x2a) = *(ushort *)(iVar2 + 0x2a) & 63999;
    }
    else {
      iVar7 = 0;
      for (uVar8 = 0; uVar8 < (uint)param_9[5]; uVar8 = uVar8 + 1) {
        if (*(int *)(param_9[4] + uVar8 * 0x18) == iVar2) {
          iVar7 = *(int *)(param_9[4] + uVar8 * 0x18 + 0xc);
          break;
        }
      }
      if ((param_2[3] & 1) == 0) {
        *(ushort *)(iVar2 + 0x2a) = *(ushort *)(iVar2 + 0x2a) & 0xfbff | 0x200;
      }
      else {
        *(ushort *)(iVar2 + 0x2a) = *(ushort *)(iVar2 + 0x2a) & 0xfdff | 0x400;
      }
      iVar3 = (*pcVar1)();
      if (iVar3 != iVar7) {
        FUN_080302d0(iVar2);
        local_5c = *(int **)(iVar2 + 0x14);
        local_58 = *(undefined4 *)(iVar2 + 0x18);
        local_54 = *(undefined4 *)(iVar2 + 0x1c);
        local_50 = *(uint *)(iVar2 + 0x20);
        (*local_3c)(iVar2 + 0x14,iVar7);
        FUN_0802ad74(iVar2,0x29,&local_5c);
        uVar4 = FUN_0802ee0a(iVar2);
        FUN_0802ad74(uVar4,0x22,iVar2);
        FUN_080302d0(iVar2);
      }
    }
    iVar7 = 0;
    if (param_2[1] == '\x01') {
      iVar7 = (*local_40)(iVar2 + 0x14);
      iVar7 = *param_9 - iVar7;
    }
    else if (param_2[1] == '\x02') {
      iVar7 = (*local_40)(iVar2 + 0x14);
      iVar7 = (int)((*param_9 + 1U & 0xfffffffe) - iVar7) / 2;
    }
    if ((param_2[3] & bVar14) != 0) {
      iVar3 = (*pcVar1)(iVar2 + 0x14);
      local_44 = local_44 - iVar3;
    }
    uVar8 = FUN_0802f9bc(iVar2,0,0x6a);
    local_50 = uVar8;
    uVar5 = FUN_0802f9bc(iVar2,0,0x6b);
    local_50 = uVar5;
    local_4c = FUN_0802fc5e(iVar2);
    iVar3 = FUN_0802ed88(iVar2);
    if (((uVar8 & 0x7fffffff) >> 0x1d == 1) && (uVar6 = uVar8 & 0x9fffffff, (int)uVar6 < 0x7d1)) {
      if (1000 < (int)uVar6) {
        uVar6 = 1000 - uVar6;
      }
      uVar8 = (int)(local_4c * uVar6) / 100;
    }
    if (((uVar5 & 0x7fffffff) >> 0x1d == 1) && (uVar6 = uVar5 & 0x9fffffff, (int)uVar6 < 0x7d1)) {
      if (1000 < (int)uVar6) {
        uVar6 = 1000 - uVar6;
      }
      uVar5 = (int)(iVar3 * uVar6) / 100;
    }
    bVar15 = (param_2[3] & 1) == 0;
    if (bVar15) {
      iVar3 = iVar7;
    }
    else {
      iVar3 = local_44;
    }
    iVar3 = iVar3 + (param_5 - *(int *)(iVar2 + 0x14)) + uVar8;
    if (bVar15) {
      iVar7 = local_44;
    }
    iVar7 = iVar7 + (param_6 - *(int *)(iVar2 + 0x18)) + uVar5;
    if (iVar3 != 0 || iVar7 != 0) {
      FUN_080302d0(iVar2);
      *(int *)(iVar2 + 0x14) = *(int *)(iVar2 + 0x14) + iVar3;
      *(int *)(iVar2 + 0x1c) = *(int *)(iVar2 + 0x1c) + iVar3;
      *(int *)(iVar2 + 0x18) = *(int *)(iVar2 + 0x18) + iVar7;
      *(int *)(iVar2 + 0x20) = *(int *)(iVar2 + 0x20) + iVar7;
      FUN_080302d0(iVar2);
      FUN_08030460(iVar2,iVar3,iVar7,0);
    }
    if ((param_2[3] & bVar14) == 0) {
      iVar2 = (*pcVar1)(iVar2 + 0x14);
      local_44 = iVar2 + param_8 + local_48 + local_44;
    }
    else {
      local_44 = local_44 - (param_8 + local_48);
    }
    iVar2 = FUN_080224dc(local_34,((byte)param_2[3] & 7) >> 2,&local_2c);
  } while( true );
}

