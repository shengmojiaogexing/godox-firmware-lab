// 0x0802c1e8 FUN_0802c1e8 size=1466

void FUN_0802c1e8(undefined4 param_1,undefined4 param_2)

{
  undefined1 uVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  char *pcVar5;
  uint uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  int iVar9;
  int iVar10;
  undefined4 *puVar11;
  int *piVar12;
  byte bVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  int iVar17;
  uint *local_a4;
  int local_a0;
  int local_9c;
  int local_98;
  int local_94;
  int local_90;
  int local_8c;
  int local_88;
  undefined2 local_84;
  undefined2 local_82;
  undefined4 local_80;
  undefined4 local_7c;
  byte local_6c;
  int local_68;
  int local_64;
  int local_60;
  int local_5c;
  int local_58;
  int local_54;
  undefined1 auStack_50 [16];
  int local_40;
  int local_3c;
  int local_38;
  int local_34;
  uint local_30;
  int local_2c;
  uint local_28;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  
  iVar9 = FUN_0802acf4(param_2);
  if (((iVar9 == 0x15) || (iVar9 == 0x18)) ||
     (iVar10 = FUN_0802eb88(DAT_0802c3c0,param_2), iVar10 == 1)) {
    iVar10 = FUN_0802ad70(param_2);
    if (iVar9 == 0x2a) {
      if ((*(byte *)(iVar10 + 0x4e) & 3) == 2) {
        FUN_0802c400(iVar10,*(undefined4 *)(iVar10 + 0x2c));
        return;
      }
      FUN_08030c20(iVar10);
      return;
    }
    if (iVar9 == 0x13) {
      piVar12 = (int *)FUN_0802ad6c(param_2);
      if ((*(short *)(iVar10 + 0x40) != 0) || (*(short *)(iVar10 + 0x4c) != 0x100)) {
        iVar3 = FUN_0802fc5e(iVar10);
        iVar15 = FUN_0802ed88(iVar10);
        local_2c = iVar10 + 0x44;
        local_30 = (uint)*(ushort *)(iVar10 + 0x4c);
        FUN_08017880(&local_24,iVar3,iVar15,(int)*(short *)(iVar10 + 0x40));
        iVar9 = *piVar12;
        if (*piVar12 <= -local_24) {
          iVar9 = -local_24;
        }
        *piVar12 = iVar9;
        if (iVar9 <= -local_20) {
          iVar9 = -local_20;
        }
        *piVar12 = iVar9;
        if (iVar9 <= local_1c - iVar3) {
          iVar9 = local_1c - iVar3;
        }
        *piVar12 = iVar9;
        if (iVar9 <= local_18 - iVar15) {
          iVar9 = local_18 - iVar15;
        }
        *piVar12 = iVar9;
      }
    }
    else if (iVar9 == 0x11) {
      puVar11 = (undefined4 *)FUN_0802ad6c(param_2);
      iVar9 = FUN_0802fc5e(iVar10);
      if (iVar9 == *(int *)(iVar10 + 0x38)) {
        iVar9 = FUN_0802ed88(iVar10);
        if ((iVar9 == *(int *)(iVar10 + 0x3c)) &&
           (((*(short *)(iVar10 + 0x4c) != 0x100 || (*(short *)(iVar10 + 0x40) != 0)) ||
            ((*(int *)(iVar10 + 0x38) / 2 != *(int *)(iVar10 + 0x44) ||
             (*(int *)(iVar10 + 0x3c) / 2 != *(int *)(iVar10 + 0x48))))))) {
          uVar2 = FUN_0802fc5e(iVar10);
          uVar7 = FUN_0802ed88(iVar10);
          local_2c = iVar10 + 0x44;
          local_30 = (uint)*(ushort *)(iVar10 + 0x4c);
          FUN_08017880(&local_24,uVar2,uVar7,(int)*(short *)(iVar10 + 0x40));
          local_24 = local_24 + *(int *)(iVar10 + 0x14);
          local_20 = local_20 + *(int *)(iVar10 + 0x18);
          local_1c = local_1c + *(int *)(iVar10 + 0x14);
          local_18 = local_18 + *(int *)(iVar10 + 0x18);
          uVar1 = FUN_0801761a(&local_24,*puVar11,0);
          *(undefined1 *)(puVar11 + 1) = uVar1;
          return;
        }
      }
      FUN_0802ec02(iVar10,&local_24);
      uVar1 = FUN_0801761a(&local_24,*puVar11,0);
      *(undefined1 *)(puVar11 + 1) = uVar1;
    }
    else if (iVar9 == 0x2c) {
      piVar12 = (int *)FUN_0802ad6c(param_2);
      if ((*(byte *)(iVar10 + 0x4f) & 3) == 1) {
        FUN_0802c3c4(&local_1c,iVar10);
        *piVar12 = local_1c;
        piVar12[1] = local_18;
      }
      else {
        *piVar12 = *(int *)(iVar10 + 0x38);
        piVar12[1] = *(int *)(iVar10 + 0x3c);
      }
    }
    else if (((iVar9 == 0x15) || (iVar9 == 0x18)) || (iVar9 == 0x12)) {
      iVar9 = FUN_0802acf4(param_2);
      iVar10 = FUN_0802ad70(param_2);
      if (iVar9 == 0x12) {
        pcVar5 = (char *)FUN_0802ad6c(param_2);
        if (*pcVar5 != '\x02') {
          uVar6 = (uint)*(byte *)(iVar10 + 0x4e);
          if (((~uVar6 & 3) == 0) || ((uVar6 & 3) == 2)) {
            *pcVar5 = '\x01';
          }
          else {
            uVar6 = (uVar6 & 0x7f) >> 2;
            if ((uVar6 == 4) || (uVar6 == 1)) {
              local_28 = FUN_0802f9bc(iVar10,0,0x46);
              if ((local_28 & 0xff) == 0xff) {
                if (*(short *)(iVar10 + 0x40) == 0) {
                  uVar2 = FUN_0802ad6c(param_2);
                  if (*(short *)(iVar10 + 0x4c) == 0x100) {
                    iVar9 = FUN_080174c4(uVar2,iVar10 + 0x14,0);
                    if (iVar9 == 0) {
                      *pcVar5 = '\x01';
                    }
                  }
                  else {
                    uVar7 = FUN_0802ed88(iVar10);
                    uVar8 = FUN_0802fc5e(iVar10);
                    local_a4 = (uint *)(iVar10 + 0x44);
                    FUN_08017880(&local_34,uVar8,uVar7,0,*(undefined2 *)(iVar10 + 0x4c));
                    local_34 = local_34 + *(int *)(iVar10 + 0x14);
                    local_30 = local_30 + *(int *)(iVar10 + 0x18);
                    local_2c = local_2c + *(int *)(iVar10 + 0x14);
                    local_28 = local_28 + *(int *)(iVar10 + 0x18);
                    iVar9 = FUN_080174c4(uVar2,&local_34,0);
                    if (iVar9 == 0) {
                      *pcVar5 = '\x01';
                    }
                  }
                }
                else {
                  *pcVar5 = '\x01';
                }
              }
              else {
                *pcVar5 = '\x01';
              }
            }
            else {
              *pcVar5 = '\x01';
            }
          }
        }
      }
      else if ((iVar9 == 0x15) || (iVar9 == 0x18)) {
        local_28 = FUN_0802fc5e(iVar10);
        uVar2 = FUN_0802ed88(iVar10);
        iVar3 = FUN_0802f9bc(iVar10,0,0x32);
        local_54 = iVar3;
        local_54 = FUN_0802f9bc(iVar10,0,0x12);
        iVar15 = local_54 + iVar3;
        local_54 = FUN_0802f9bc(iVar10,0,0x13);
        iVar17 = local_54 + iVar3;
        local_54 = FUN_0802f9bc(iVar10,0,0x10);
        iVar16 = local_54 + iVar3;
        local_54 = FUN_0802f9bc(iVar10,0,0x11);
        iVar3 = iVar3 + local_54;
        local_30 = *(int *)(iVar10 + 0x44) + iVar15;
        local_2c = *(int *)(iVar10 + 0x48) + iVar16;
        if ((*(byte *)(iVar10 + 0x4f) & 3) == 1) {
          FUN_0802ed04(iVar10,&local_40);
        }
        else {
          local_a4 = &local_30;
          FUN_08017880(&local_40,local_28,uVar2,(int)*(short *)(iVar10 + 0x40),
                       *(undefined2 *)(iVar10 + 0x4c));
          local_40 = *(int *)(iVar10 + 0x14) + local_40;
          local_3c = *(int *)(iVar10 + 0x18) + local_3c;
          local_38 = *(int *)(iVar10 + 0x14) + local_38;
          local_34 = *(int *)(iVar10 + 0x18) + local_34;
        }
        iVar14 = iVar10 + 0x14;
        FUN_08027454(auStack_50);
        FUN_08027454(iVar14,&local_40);
        iVar4 = FUN_0802eb88(DAT_0801d5e4,param_2);
        if ((((iVar4 == 1) && (FUN_08027454(iVar14,auStack_50), iVar9 == 0x15)) &&
            (*(int *)(iVar10 + 0x3c) != 0)) &&
           ((*(int *)(iVar10 + 0x38) != 0 && (*(short *)(iVar10 + 0x4c) != 0)))) {
          iVar9 = FUN_0802ad00(param_2);
          FUN_08027454(&local_60,iVar14);
          FUN_0802c3c4(&local_68,iVar10);
          if ((*(byte *)(iVar10 + 0x4f) & 3) == 1) {
            iVar4 = ((*(int *)(iVar10 + 0x38) - local_68) + 1) / 2;
            local_60 = local_60 - iVar4;
            local_58 = local_58 - iVar4;
            local_5c = local_5c - ((*(int *)(iVar10 + 0x3c) - local_64) + 1) / 2;
            local_54 = local_54 - ((*(int *)(iVar10 + 0x3c) - local_64) + 1) / 2;
          }
          else {
            local_58 = (local_38 - local_40) + local_60;
            local_54 = (local_34 - local_3c) + local_5c;
          }
          local_60 = local_60 + iVar15;
          local_5c = local_5c + iVar16;
          local_58 = local_58 - iVar17;
          local_54 = local_54 - iVar3;
          bVar13 = *(byte *)(iVar10 + 0x4e) & 3;
          if ((bVar13 == 1) || ((*(byte *)(iVar10 + 0x4e) & 3) == 0)) {
            FUN_080287cc(&local_84);
            FUN_0802fdd2(iVar10,0,&local_84);
            local_82 = *(undefined2 *)(iVar10 + 0x4c);
            local_84 = *(undefined2 *)(iVar10 + 0x40);
            local_80 = *(undefined4 *)(iVar10 + 0x44);
            local_7c = *(undefined4 *)(iVar10 + 0x48);
            local_6c = *(byte *)(iVar10 + 0x4e) >> 7;
            local_94 = local_40 + iVar15;
            local_90 = local_3c + iVar16;
            local_8c = local_38 - iVar17;
            local_88 = local_34 - iVar3;
            uVar2 = *(undefined4 *)(iVar9 + 8);
            iVar3 = FUN_08017484(&local_94,uVar2,&local_94);
            if (iVar3 != 0) {
              *(int **)(iVar9 + 8) = &local_94;
              iVar15 = *(int *)(iVar10 + 0x38);
              iVar3 = *(int *)(iVar10 + 0x30);
              local_a0 = (*(int *)(iVar10 + 0x34) -
                         *(int *)(iVar10 + 0x3c) *
                         (*(int *)(iVar10 + 0x34) / *(int *)(iVar10 + 0x3c))) + local_5c;
              if (local_5c < local_a0) {
                local_a0 = local_a0 - *(int *)(iVar10 + 0x3c);
              }
              local_98 = local_a0 + *(int *)(iVar10 + 0x3c) + -1;
              for (; local_a0 < local_54; local_a0 = local_a0 + local_64) {
                local_a4 = (uint *)(local_60 + (iVar3 - iVar15 * (iVar3 / iVar15)));
                if (local_60 < (int)local_a4) {
                  local_a4 = (uint *)((int)local_a4 - *(int *)(iVar10 + 0x38));
                }
                local_9c = (int)local_a4 + *(int *)(iVar10 + 0x38) + -1;
                for (; (int)local_a4 < local_58; local_a4 = (uint *)((int)local_a4 + local_68)) {
                  FUN_08028758(iVar9,&local_84,&local_a4,*(undefined4 *)(iVar10 + 0x2c));
                  local_9c = local_68 + local_9c;
                }
                local_98 = local_98 + local_64;
              }
              *(undefined4 *)(iVar9 + 8) = uVar2;
            }
          }
          else if (bVar13 == 2) {
            FUN_08028d04(&local_94);
            FUN_0802fe78(iVar10,0,&local_94);
            FUN_08028800(iVar9,&local_94,iVar14,*(undefined4 *)(iVar10 + 0x2c),0);
          }
          else {
            FUN_08028758(iVar9,0,iVar14);
          }
        }
      }
      return;
    }
  }
  return;
}

