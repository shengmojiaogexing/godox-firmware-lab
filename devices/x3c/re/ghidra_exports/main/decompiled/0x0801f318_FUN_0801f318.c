// 0x0801f318 FUN_0801f318 size=2626

void FUN_0801f318(int param_1,int *param_2,int *param_3)

{
  undefined1 uVar1;
  int iVar2;
  undefined1 *puVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined1 *puVar8;
  undefined1 *puVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  undefined1 *puVar13;
  byte *pbVar14;
  bool bVar15;
  int *local_f4 [2];
  undefined2 local_ec;
  undefined1 *local_e8;
  undefined1 local_e4;
  int *local_e0;
  byte local_dc;
  undefined1 local_db;
  int local_d8;
  undefined1 *local_d4;
  int local_d0;
  int local_cc;
  int local_c8;
  int local_c4;
  int local_c0;
  int local_bc;
  int local_b8;
  int local_b4;
  undefined4 local_b0;
  undefined1 auStack_ac [36];
  undefined1 *local_88;
  int local_84;
  int local_80;
  int local_7c;
  int local_78;
  int local_74;
  undefined1 auStack_70 [16];
  uint local_60;
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
  int *piStack_2c;
  int *piStack_28;
  
  iVar2 = param_2[0xe];
  bVar15 = iVar2 == 0;
  local_30 = param_1;
  piStack_2c = param_2;
  piStack_28 = param_3;
LAB_0801f326:
  do {
    if (bVar15) {
      return;
    }
    if (*(byte *)(param_2 + 0x12) < 3) {
      return;
    }
    if (((iVar2 == 1) && (param_2[0x11] < 1)) && (param_2[0xf] == 0)) {
      bVar15 = true;
      iVar2 = 0;
      if (param_2[0x10] == 0) goto LAB_0801f326;
    }
    local_4c = (*param_3 + param_2[0xf]) - param_2[0x11];
    local_44 = param_2[0x11] + param_2[0xf] + param_3[2];
    local_48 = (param_3[1] + param_2[0x10]) - param_2[0x11];
    local_40 = param_3[3] + param_2[0x10] + param_2[0x11];
    local_5c = (local_4c - param_2[0xe] / 2) + -1;
    local_54 = param_2[0xe] / 2 + 1 + local_44;
    local_58 = (local_48 - param_2[0xe] / 2) + -1;
    local_50 = param_2[0xe] / 2 + 1 + local_40;
    local_60 = (uint)*(byte *)(param_2 + 0x12);
    if (0xfd < local_60) {
      local_60 = 0xff;
    }
    iVar2 = FUN_08017484(auStack_70,&local_5c,*(undefined4 *)(local_30 + 8));
    bVar15 = iVar2 == 0;
    iVar2 = 0;
    if (!bVar15) {
      FUN_0802748a(&local_80,param_3);
      FUN_080274e4(&local_80,0xffffffff,0xffffffff);
      iVar11 = *param_2;
      iVar2 = (local_78 - local_80) + 1;
      iVar7 = (local_74 - local_7c) + 1;
      if (iVar7 <= iVar2) {
        iVar2 = iVar7;
      }
      local_84 = iVar11;
      if (iVar2 >> 1 < iVar11) {
        local_84 = iVar2 >> 1;
      }
      iVar2 = (local_44 - local_4c) + 1;
      iVar7 = (local_40 - local_48) + 1;
      if (iVar7 <= iVar2) {
        iVar2 = iVar7;
      }
      if (iVar2 >> 1 < iVar11) {
        iVar11 = iVar2 >> 1;
      }
      iVar7 = param_2[0xe] + iVar11;
      local_88 = (undefined1 *)FUN_0802d624(iVar7 * iVar7 * 2);
      FUN_08037e7c(&local_4c,local_88,param_2[0xe],iVar11);
      iVar2 = FUN_08029394(&local_5c);
      iVar11 = 1;
      if (((iVar2 != 0) || (*(char *)((int)param_2 + 5) != -1)) || ((char)param_2[1] != '\0')) {
        iVar11 = 0;
      }
      local_b0 = 0xffffffff;
      if (iVar11 == 0) {
        FUN_080298a0(auStack_ac,&local_80,local_84,1);
        local_b0 = FUN_08028e78(auStack_ac,0);
      }
      puVar3 = (undefined1 *)FUN_0802d624((local_54 - local_5c) + 1);
      FUN_0802d8d6(local_f4,0x1c);
      local_f4[0] = &local_c0;
      local_ec = *(undefined2 *)((int)param_2 + 0x36);
      local_dc = *(byte *)(param_2 + 0x12);
      local_db = (undefined1)param_2[1];
      iVar12 = local_5c + ((local_54 - local_5c) + 1) / 2;
      iVar2 = local_58 + ((local_50 - local_58) + 1) / 2;
      local_b8 = local_54;
      local_c0 = (local_54 - iVar7) + 1;
      local_bc = local_58;
      local_34 = iVar7 + -1;
      if (local_c0 <= iVar12) {
        local_c0 = iVar12;
      }
      if (local_34 + local_58 < iVar2) {
        local_b4 = local_34 + local_58;
      }
      else {
        local_b4 = iVar2;
      }
      local_e8 = puVar3;
      local_e0 = local_f4[0];
      iVar4 = FUN_08017484(&local_d0,&local_c0,*(undefined4 *)(local_30 + 8));
      if ((iVar4 != 0) && (iVar4 = FUN_080174c4(&local_d0,&local_80,local_84), iVar4 == 0)) {
        iVar4 = (local_c8 - local_d0) + 1;
        puVar13 = local_88 + (local_d0 - (local_54 - iVar7)) + (local_cc - local_58) * iVar7 + -1;
        if ((iVar11 == 0) || (iVar5 = FUN_08017586(&local_d0,&local_80,local_84), iVar5 == 0)) {
          local_d8 = iVar11;
        }
        else {
          local_d8 = 1;
        }
        if (0 < iVar4) {
          local_c0 = local_d0;
          local_b8 = local_c8;
          local_e4 = 2;
          local_e8 = puVar3;
          for (iVar5 = local_cc; iVar5 <= local_c4; iVar5 = iVar5 + 1) {
            local_b4 = iVar5;
            local_bc = iVar5;
            if (local_d8 == 0) {
              FUN_0802d764(puVar3,puVar13,iVar7);
              iVar10 = FUN_08029220(puVar3,local_d0,iVar5,iVar4);
              local_e4 = (undefined1)iVar10;
              if (iVar10 == 1) {
                local_e4 = 2;
              }
            }
            else {
              local_e8 = puVar13;
            }
            FUN_08029de4(local_30,local_f4);
            puVar13 = puVar13 + iVar7;
          }
        }
      }
      local_b8 = local_54;
      local_c0 = (local_54 - iVar7) + 1;
      local_bc = (local_50 - iVar7) + 1;
      local_b4 = local_50;
      if (local_c0 <= iVar12) {
        local_c0 = iVar12;
      }
      local_38 = iVar2 + 1;
      if (local_bc <= local_38) {
        local_bc = iVar2 + 1;
      }
      iVar4 = FUN_08017484(&local_d0,&local_c0,*(undefined4 *)(local_30 + 8));
      if ((iVar4 != 0) && (iVar4 = FUN_080174c4(&local_d0,&local_80,local_84), iVar4 == 0)) {
        iVar4 = (local_c8 - local_d0) + 1;
        puVar13 = local_88 + (local_d0 - (local_54 - iVar7)) + (local_b4 - local_c4) * iVar7 + -1;
        if ((iVar11 == 0) || (iVar5 = FUN_08017586(&local_d0,&local_80,local_84), iVar5 == 0)) {
          local_d8 = iVar11;
        }
        else {
          local_d8 = 1;
        }
        if (0 < iVar4) {
          local_c0 = local_d0;
          local_b8 = local_c8;
          local_e4 = 2;
          local_e8 = puVar3;
          for (iVar5 = local_c4; local_cc <= iVar5; iVar5 = iVar5 + -1) {
            local_b4 = iVar5;
            local_bc = iVar5;
            if (local_d8 == 0) {
              FUN_0802d764(puVar3,puVar13,iVar7);
              iVar10 = FUN_08029220(puVar3,local_d0,iVar5,iVar4);
              local_e4 = (undefined1)iVar10;
              if (iVar10 == 1) {
                local_e4 = 2;
              }
            }
            else {
              local_e8 = puVar13;
            }
            FUN_08029de4(local_30,local_f4);
            puVar13 = puVar13 + iVar7;
          }
        }
      }
      local_c0 = local_5c + iVar7;
      local_b8 = local_54 - iVar7;
      local_bc = local_58;
      if (local_34 + local_58 < iVar2) {
        local_b4 = local_34 + local_58;
      }
      else {
        local_b4 = iVar2;
      }
      iVar4 = FUN_08017484(&local_d0,&local_c0,*(undefined4 *)(local_30 + 8));
      if ((iVar4 != 0) && (iVar4 = FUN_080174c4(&local_d0,&local_80,local_84), iVar4 == 0)) {
        iVar4 = (local_c8 - local_d0) + 1;
        pbVar14 = local_88 + (local_cc - local_bc) * iVar7;
        if ((iVar11 == 0) || (iVar5 = FUN_08017586(&local_d0,&local_80,local_84), iVar5 == 0)) {
          local_d8 = iVar11;
        }
        else {
          local_d8 = 1;
        }
        if (0 < iVar4) {
          if (local_d8 == 0) {
            local_e8 = puVar3;
          }
          else {
            local_e8 = (undefined1 *)0x0;
          }
          local_c0 = local_d0;
          local_b8 = local_c8;
          for (iVar5 = local_cc; iVar5 <= local_c4; iVar5 = iVar5 + 1) {
            local_b4 = iVar5;
            local_bc = iVar5;
            if (local_d8 == 0) {
              FUN_0802d87c(puVar3,*pbVar14,iVar4);
              iVar10 = FUN_08029220(puVar3,local_d0,iVar5,iVar4);
              local_e4 = (undefined1)iVar10;
              if (iVar10 == 1) {
                local_e4 = 2;
              }
              FUN_08029de4(local_30,local_f4);
            }
            else {
              if (local_60 == 0xff) {
                local_dc = *pbVar14;
              }
              else {
                local_dc = (byte)((uint)((int)(short)(ushort)*pbVar14 *
                                        (int)(short)(ushort)*(byte *)(param_2 + 0x12)) >> 8);
              }
              FUN_08029de4(local_30,local_f4);
            }
            pbVar14 = pbVar14 + iVar7;
          }
        }
      }
      local_dc = *(byte *)(param_2 + 0x12);
      local_c0 = local_5c + iVar7;
      local_b8 = local_54 - iVar7;
      local_bc = (local_50 - iVar7) + 1;
      local_b4 = local_50;
      if (local_bc <= local_38) {
        local_bc = iVar2 + 1;
      }
      iVar4 = FUN_08017484(&local_d0,&local_c0,*(undefined4 *)(local_30 + 8));
      if ((iVar4 != 0) && (iVar4 = FUN_080174c4(&local_d0,&local_80,local_84), iVar4 == 0)) {
        iVar4 = (local_c8 - local_d0) + 1;
        pbVar14 = local_88 + (local_b4 - local_c4) * iVar7;
        if (0 < iVar4) {
          if ((iVar11 == 0) || (iVar5 = FUN_08017586(&local_d0,&local_80,local_84), iVar5 == 0)) {
            iVar5 = iVar11;
          }
          else {
            iVar5 = 1;
          }
          if (iVar5 == 0) {
            local_e8 = puVar3;
          }
          else {
            local_e8 = (undefined1 *)0x0;
          }
          local_c0 = local_d0;
          local_b8 = local_c8;
          for (iVar5 = local_c4; local_cc <= iVar5; iVar5 = iVar5 + -1) {
            local_bc = iVar5;
            local_b4 = iVar5;
            if ((iVar11 == 0) || (iVar10 = FUN_08017586(&local_d0,&local_80,local_84), iVar10 == 0))
            {
              iVar10 = iVar11;
            }
            else {
              iVar10 = 1;
            }
            if (iVar10 == 0) {
              FUN_0802d87c(puVar3,*pbVar14,iVar4);
              iVar10 = FUN_08029220(puVar3,local_d0,iVar5,iVar4);
              local_e4 = (undefined1)iVar10;
              if (iVar10 == 1) {
                local_e4 = 2;
              }
              FUN_08029de4(local_30,local_f4);
            }
            else {
              if (local_60 == 0xff) {
                local_dc = *pbVar14;
              }
              else {
                local_dc = (byte)((uint)((int)(short)(ushort)*pbVar14 *
                                        (int)(short)(ushort)*(byte *)(param_2 + 0x12)) >> 8);
              }
              FUN_08029de4(local_30,local_f4);
            }
            pbVar14 = pbVar14 + iVar7;
          }
        }
      }
      local_dc = (byte)param_2[0x12];
      local_c0 = (local_54 - iVar7) + 1;
      local_b8 = local_54;
      local_bc = local_58 + iVar7;
      if (local_38 <= local_bc) {
        local_bc = iVar2 + 1;
      }
      if (iVar2 < local_50 - iVar7) {
        local_b4 = local_50 - iVar7;
      }
      else {
        local_b4 = iVar2;
      }
      if (local_c0 <= iVar12) {
        local_c0 = iVar12;
      }
      iVar4 = FUN_08017484(&local_d0,&local_c0,*(undefined4 *)(local_30 + 8));
      if ((iVar4 != 0) && (iVar4 = FUN_080174c4(&local_d0,&local_80,local_84), iVar4 == 0)) {
        iVar4 = (local_c8 - local_d0) + 1;
        local_d4 = local_88 + (local_d0 - (local_54 - iVar7)) + local_34 * iVar7 + -1;
        if ((iVar11 == 0) || (iVar5 = FUN_08017586(&local_d0,&local_80,local_84), iVar5 == 0)) {
          iVar5 = iVar11;
        }
        else {
          iVar5 = 1;
        }
        if (iVar5 == 0) {
          local_e8 = puVar3;
        }
        else {
          local_e8 = local_d4;
        }
        if (0 < iVar4) {
          local_c0 = local_d0;
          local_b8 = local_c8;
          local_e4 = 2;
          for (iVar10 = local_cc; iVar10 <= local_c4; iVar10 = iVar10 + 1) {
            local_bc = iVar10;
            local_b4 = iVar10;
            if (iVar5 == 0) {
              FUN_0802d764(puVar3,local_d4,iVar4);
              iVar6 = FUN_08029220(puVar3,local_d0,iVar10,iVar4);
              local_e4 = (undefined1)iVar6;
              if (iVar6 == 1) {
                local_e4 = 2;
              }
            }
            FUN_08029de4(local_30,local_f4);
          }
        }
      }
      puVar13 = local_88;
      for (iVar4 = 0; iVar4 < iVar7; iVar4 = iVar4 + 1) {
        puVar8 = puVar13 + iVar7;
        puVar9 = puVar8;
        for (iVar5 = 0; puVar9 = puVar9 + -1, iVar5 < iVar7 / 2; iVar5 = iVar5 + 1) {
          uVar1 = *puVar13;
          *puVar13 = *puVar9;
          *puVar9 = uVar1;
          puVar13 = puVar13 + 1;
        }
        puVar13 = puVar8;
      }
      local_c0 = local_5c;
      local_b8 = local_5c + local_34;
      local_bc = local_58 + iVar7;
      if (local_38 <= local_bc) {
        local_bc = iVar2 + 1;
      }
      if (iVar2 < local_50 - iVar7) {
        local_b4 = local_50 - iVar7;
      }
      else {
        local_b4 = iVar2;
      }
      local_3c = iVar12 + -1;
      if (local_3c <= local_b8) {
        local_b8 = iVar12 + -1;
      }
      iVar4 = FUN_08017484(&local_d0,&local_c0,*(undefined4 *)(local_30 + 8));
      if ((iVar4 != 0) && (iVar4 = FUN_080174c4(&local_d0,&local_80,local_84), iVar4 == 0)) {
        iVar4 = (local_c8 - local_d0) + 1;
        local_d4 = local_88 + (local_d0 - local_c0) + local_34 * iVar7;
        if ((iVar11 == 0) || (iVar5 = FUN_08017586(&local_d0,&local_80,local_84), iVar5 == 0)) {
          iVar5 = iVar11;
        }
        else {
          iVar5 = 1;
        }
        if (iVar5 == 0) {
          local_e8 = puVar3;
        }
        else {
          local_e8 = local_d4;
        }
        if (0 < iVar4) {
          local_c0 = local_d0;
          local_b8 = local_c8;
          local_e4 = 2;
          for (iVar10 = local_cc; iVar10 <= local_c4; iVar10 = iVar10 + 1) {
            local_bc = iVar10;
            local_b4 = iVar10;
            if (iVar5 == 0) {
              FUN_0802d764(puVar3,local_d4,iVar4);
              iVar6 = FUN_08029220(puVar3,local_d0,iVar10,iVar4);
              local_e4 = (undefined1)iVar6;
              if (iVar6 == 1) {
                local_e4 = 2;
              }
            }
            FUN_08029de4(local_30,local_f4);
          }
        }
      }
      local_c0 = local_5c;
      local_b8 = local_5c + local_34;
      local_bc = local_58;
      if (local_3c <= local_b8) {
        local_b8 = iVar12 + -1;
      }
      if (local_34 + local_58 < iVar2) {
        local_b4 = local_34 + local_58;
      }
      else {
        local_b4 = iVar2;
      }
      iVar4 = FUN_08017484(&local_d0,&local_c0,*(undefined4 *)(local_30 + 8));
      if (iVar4 != 0) {
        iVar4 = FUN_080174c4(&local_d0,&local_80,local_84);
        if (iVar4 == 0) {
          iVar4 = (local_c8 - local_d0) + 1;
          puVar13 = local_88 + (local_d0 - local_c0) + (local_cc - local_bc) * iVar7;
          if ((iVar11 == 0) || (iVar5 = FUN_08017586(&local_d0,&local_80,local_84), iVar5 == 0)) {
            iVar5 = iVar11;
          }
          else {
            iVar5 = 1;
          }
          local_e8 = puVar3;
          if (0 < iVar4) {
            local_c0 = local_d0;
            local_b8 = local_c8;
            local_e4 = 2;
            for (iVar10 = local_cc; iVar10 <= local_c4; iVar10 = iVar10 + 1) {
              local_b4 = iVar10;
              local_bc = iVar10;
              if (iVar5 == 0) {
                FUN_0802d764(puVar3,puVar13,iVar7);
                iVar6 = FUN_08029220(puVar3,local_d0,iVar10,iVar4);
                local_e4 = (undefined1)iVar6;
                if (iVar6 == 1) {
                  local_e4 = 2;
                }
              }
              else {
                local_e8 = puVar13;
              }
              FUN_08029de4(local_30,local_f4);
              puVar13 = puVar13 + iVar7;
            }
          }
        }
      }
      local_c0 = local_5c;
      local_b8 = local_5c + local_34;
      local_bc = (local_50 - iVar7) + 1;
      local_b4 = local_50;
      if (local_bc <= local_38) {
        local_bc = iVar2 + 1;
      }
      if (local_3c <= local_b8) {
        local_b8 = iVar12 + -1;
      }
      iVar2 = FUN_08017484(&local_d0,&local_c0,*(undefined4 *)(local_30 + 8));
      if ((iVar2 != 0) && (iVar2 = FUN_080174c4(&local_d0,&local_80,local_84), iVar2 == 0)) {
        iVar2 = (local_c8 - local_d0) + 1;
        puVar13 = local_88 + (local_d0 - local_c0) + (local_b4 - local_c4) * iVar7;
        if ((iVar11 == 0) || (iVar12 = FUN_08017586(&local_d0,&local_80,local_84), iVar12 == 0)) {
          iVar12 = iVar11;
        }
        else {
          iVar12 = 1;
        }
        if (0 < iVar2) {
          local_c0 = local_d0;
          local_b8 = local_c8;
          local_e4 = 2;
          local_e8 = puVar3;
          for (iVar4 = local_c4; local_cc <= iVar4; iVar4 = iVar4 + -1) {
            local_b4 = iVar4;
            local_bc = iVar4;
            if (iVar12 == 0) {
              FUN_0802d764(puVar3,puVar13,iVar7);
              iVar5 = FUN_08029220(puVar3,local_d0,iVar4,iVar2);
              local_e4 = (undefined1)iVar5;
              if (iVar5 == 1) {
                local_e4 = 2;
              }
            }
            else {
              local_e8 = puVar13;
            }
            FUN_08029de4(local_30,local_f4);
            puVar13 = puVar13 + iVar7;
          }
        }
      }
      local_c0 = local_5c + iVar7;
      local_b8 = local_54 - iVar7;
      local_bc = local_58 + iVar7;
      local_b4 = local_50 - iVar7;
      local_e8 = puVar3;
      iVar2 = FUN_08017484(&local_d0,&local_c0,*(undefined4 *)(local_30 + 8));
      if (((iVar2 != 0) && (iVar2 = FUN_080174c4(&local_d0,&local_80,local_84), iVar2 == 0)) &&
         (iVar2 = (local_c8 - local_d0) + 1, 0 < iVar2)) {
        local_c0 = local_d0;
        local_b8 = local_c8;
        for (iVar7 = local_cc; iVar7 <= local_c4; iVar7 = iVar7 + 1) {
          local_bc = iVar7;
          local_b4 = iVar7;
          FUN_0802d926(puVar3,iVar2);
          local_e4 = FUN_08029220(puVar3,local_d0,iVar7,iVar2);
          FUN_08029de4(local_30,local_f4);
        }
      }
      if (iVar11 == 0) {
        FUN_08029358(auStack_ac);
        FUN_080299f4(local_b0);
      }
      FUN_0802d6c0(local_88);
      FUN_0802d6c0(puVar3);
      return;
    }
  } while( true );
}

