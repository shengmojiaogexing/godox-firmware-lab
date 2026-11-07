// 0x0802cd80 FUN_0802cd80 size=1132

void FUN_0802cd80(uint param_1)

{
  bool bVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined1 uVar4;
  byte bVar5;
  char cVar6;
  ushort uVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  int iVar15;
  uint local_80;
  undefined4 local_7c;
  int local_60;
  undefined4 local_5c;
  int local_58;
  uint local_54;
  int local_50;
  undefined4 local_4c;
  uint local_48;
  int local_44;
  undefined4 local_40;
  int local_3c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  
  if (*(int *)(param_1 + 0x2c) != 0) {
    *(undefined4 *)(param_1 + 0x38) = 0xffffffff;
    FUN_0802ec40(param_1,&local_34);
    iVar15 = local_2c - local_34;
    iVar8 = FUN_0802fa9a(param_1,0);
    iVar9 = FUN_0802faac(param_1,0);
    iVar10 = FUN_0802faa6(param_1,0);
    local_80 = (uint)((int)((uint)*(byte *)(param_1 + 0x54) << 0x1b) < 0);
    if ((int)((uint)*(byte *)(param_1 + 0x54) << 0x1a) < 0) {
      local_80 = local_80 | 2;
    }
    iVar11 = FUN_0802faee(param_1,0);
    if ((iVar11 == DAT_0802d1e0) && (-1 < (int)((uint)*(ushort *)(param_1 + 0x2a) << 0x15))) {
      local_80 = local_80 | 4;
    }
    FUN_08033a84(&local_3c,*(undefined4 *)(param_1 + 0x2c),iVar8,iVar10,iVar9,iVar15 + 1);
    FUN_08030c70(param_1);
    bVar5 = *(byte *)(param_1 + 0x54) & 7;
    if (bVar5 == 2) {
      uVar7 = FUN_0802f7d4(param_1,0);
      uVar12 = (uint)uVar7;
      if (uVar12 == 0) {
        FUN_0802ed10(param_1);
        iVar9 = FUN_08028228();
        uVar12 = iVar9 / 3 & 0xffff;
      }
      FUN_080269c8(&local_80);
      local_40 = CONCAT22(local_40._2_2_,0xffff);
      local_4c = 300;
      local_44 = 300;
      bVar1 = false;
      iVar9 = (local_2c - local_34) + 1;
      if (iVar9 < local_3c) {
        local_58 = iVar9 - local_3c;
        local_60 = 0;
        local_5c = 0;
        local_7c = DAT_0802d1e4;
        local_80 = param_1;
        iVar10 = FUN_08026998(param_1,DAT_0802d1e4);
        iVar9 = local_60;
        iVar15 = 0;
        uVar13 = 0;
        if (iVar10 != 0) {
          iVar15 = *(int *)(iVar10 + 0x30);
          uVar13 = (*(byte *)(iVar10 + 0x42) & 3) >> 1;
        }
        if (iVar15 < (int)local_54) {
          uVar4 = local_40._2_1_;
          local_40 = local_40 & 0xfffeffff;
          local_50 = iVar15;
          if (uVar13 != 0) {
            local_40 = CONCAT13(local_40._3_1_,CONCAT12(uVar4,(undefined2)local_40)) & 0xfffeffff |
                       0x20000;
            local_60 = local_58;
            local_58 = iVar9;
          }
        }
        local_54 = FUN_08026a90(uVar12,local_60,local_58);
        local_48 = local_54;
        FUN_08026aac(&local_80);
        bVar1 = true;
      }
      else {
        local_80 = param_1;
        FUN_08026938(param_1,DAT_0802d1e4);
        *(undefined4 *)(param_1 + 0x4c) = 0;
      }
      iVar9 = (local_28 - local_30) + 1;
      if ((local_38 <= iVar9) || (bVar1)) {
        FUN_08026938(param_1,DAT_0802d1e8);
        *(undefined4 *)(param_1 + 0x50) = 0;
      }
      else {
        local_58 = (iVar9 - local_38) - *(int *)(iVar8 + 8);
        local_60 = 0;
        local_5c = 0;
        local_7c = DAT_0802d1e8;
        iVar9 = FUN_08026998(param_1,DAT_0802d1e8);
        iVar8 = local_60;
        iVar10 = 0;
        uVar13 = 0;
        if (iVar9 != 0) {
          iVar10 = *(int *)(iVar9 + 0x30);
          uVar13 = (*(byte *)(iVar9 + 0x42) & 3) >> 1;
        }
        if (iVar10 < (int)local_54) {
          uVar4 = local_40._2_1_;
          local_40 = local_40 & 0xfffeffff;
          local_50 = iVar10;
          if (uVar13 != 0) {
            local_40 = CONCAT13(local_40._3_1_,CONCAT12(uVar4,(undefined2)local_40)) & 0xfffeffff |
                       0x20000;
            local_60 = local_58;
            local_58 = iVar8;
          }
        }
        local_54 = FUN_08026a90(uVar12,local_60,local_58);
        local_48 = local_54;
        FUN_08026aac(&local_80);
      }
    }
    else if (bVar5 == 3) {
      iVar9 = FUN_0802f9bc(param_1,0,99);
      uVar7 = FUN_0802f7d4(param_1,0);
      uVar12 = (uint)uVar7;
      if (uVar12 == 0) {
        FUN_0802ed10(param_1);
        iVar10 = FUN_08028228();
        uVar12 = iVar10 / 3 & 0xffff;
      }
      FUN_080269c8(&local_80);
      local_40 = CONCAT22(local_40._2_2_,0xffff);
      bVar1 = false;
      if ((local_2c - local_34) + 1 < local_3c) {
        local_80 = param_1;
        iVar10 = FUN_0802afc2(iVar8,0x20);
        local_58 = -(iVar10 * 3 + local_3c);
        local_60 = 0;
        local_5c = 0;
        local_7c = DAT_0802d1e4;
        local_54 = FUN_08026a90(uVar12,0,local_58);
        iVar10 = FUN_08026998(param_1,DAT_0802d1e4);
        if (iVar10 == 0) {
          iVar10 = 0;
        }
        else {
          iVar10 = *(int *)(iVar10 + 0x30);
        }
        if (iVar9 == 0) {
          if (iVar10 < (int)local_54) {
            local_40 = local_40 & 0xfffeffff;
            local_50 = iVar10;
          }
        }
        else {
          local_44 = *(int *)(iVar9 + 0x3c);
          local_50 = *(int *)(iVar9 + 0x30);
        }
        FUN_08026aac(&local_80);
        bVar1 = true;
      }
      else {
        local_80 = param_1;
        FUN_08026938(param_1,DAT_0802d1e4);
        *(undefined4 *)(param_1 + 0x4c) = 0;
      }
      if ((local_38 <= (local_28 - local_30) + 1) || (bVar1)) {
        FUN_08026938(param_1,DAT_0802d1e8);
        *(undefined4 *)(param_1 + 0x50) = 0;
      }
      else {
        local_58 = -(*(int *)(iVar8 + 8) + local_38);
        local_60 = 0;
        local_5c = 0;
        local_7c = DAT_0802d1e8;
        local_54 = FUN_08026a90(uVar12,0,local_58);
        iVar8 = FUN_08026998(param_1,DAT_0802d1e8);
        if (iVar8 == 0) {
          iVar8 = 0;
        }
        else {
          iVar8 = *(int *)(iVar8 + 0x30);
        }
        if (iVar9 == 0) {
          if (iVar8 < (int)local_54) {
            local_40 = local_40 & 0xfffeffff;
            local_50 = iVar8;
          }
        }
        else {
          local_44 = *(int *)(iVar9 + 0x3c);
          local_50 = *(int *)(iVar9 + 0x30);
        }
        FUN_08026aac(&local_80);
      }
    }
    else if (bVar5 == 1) {
      if ((local_28 - local_30) + 1 < local_38) {
        if (*(int *)(iVar8 + 8) < local_38) {
          uVar12 = (*(code *)*DAT_0802d1ec)(*(undefined4 *)(param_1 + 0x2c));
          if (uVar12 < 4) {
            *(undefined4 *)(param_1 + 0x34) = 0xffffffff;
          }
          else {
            iVar15 = FUN_0802afc2(iVar8,0x2e);
            local_44 = (iVar15 + iVar10) * -3 + (local_2c - local_34) + 1;
            iVar10 = (local_28 - local_30) + 1;
            iVar15 = *(int *)(iVar8 + 8) + iVar9;
            iVar15 = iVar10 - iVar15 * (iVar10 / iVar15);
            if (iVar15 < *(int *)(iVar8 + 8)) {
              local_40 = (iVar10 - iVar15) - iVar9;
            }
            else {
              local_40 = *(int *)(iVar8 + 8) + (iVar10 - iVar15);
            }
            iVar8 = FUN_0802cb88(param_1,&local_44);
            uVar12 = FUN_08004df8(*(undefined4 *)(param_1 + 0x2c));
            local_48 = (*(code *)*DAT_0802d1f0)(*(undefined4 *)(param_1 + 0x2c),iVar8);
            puVar2 = DAT_0802d1f4;
            while (uVar13 = local_48, puVar3 = DAT_0802d1f8, uVar12 < local_48 + 3) {
              (*(code *)*puVar2)(*(undefined4 *)(param_1 + 0x2c),&local_48);
              iVar8 = iVar8 + -1;
            }
            bVar5 = 0;
            uVar14 = 0;
            do {
              cVar6 = (*(code *)*puVar3)(*(int *)(param_1 + 0x2c) + local_48);
              bVar5 = cVar6 + bVar5;
              (*(code *)*DAT_0802d1fc)(*(undefined4 *)(param_1 + 0x2c),&local_48);
              if ((3 < bVar5) || (uVar12 < local_48)) break;
              uVar14 = uVar14 + 1;
            } while (uVar14 < 4);
            iVar9 = FUN_0802d268(param_1,*(int *)(param_1 + 0x2c) + uVar13,bVar5);
            if (iVar9 != 0) {
              uVar12 = 0;
              do {
                *(undefined1 *)(*(int *)(param_1 + 0x2c) + uVar13 + uVar12) = 0x2e;
                uVar12 = uVar12 + 1;
              } while (uVar12 < 3);
              *(undefined1 *)(*(int *)(param_1 + 0x2c) + uVar13 + 3) = 0;
              *(int *)(param_1 + 0x34) = iVar8 + 3;
            }
          }
        }
        else {
          *(undefined4 *)(param_1 + 0x34) = 0xffffffff;
        }
      }
      else {
        *(undefined4 *)(param_1 + 0x34) = 0xffffffff;
      }
    }
    FUN_080302d0(param_1);
  }
  return;
}

