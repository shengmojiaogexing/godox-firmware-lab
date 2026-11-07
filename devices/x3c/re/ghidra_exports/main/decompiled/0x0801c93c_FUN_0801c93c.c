// 0x0801c93c FUN_0801c93c size=1024

void FUN_0801c93c(int param_1,int *param_2,int *param_3,int param_4,int param_5,undefined4 param_6,
                 uint param_7,undefined1 param_8)

{
  bool bVar1;
  bool bVar2;
  bool bVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  int local_c4;
  int local_c0;
  int local_bc;
  int local_b8;
  undefined4 local_b4;
  undefined1 auStack_b0 [36];
  undefined1 auStack_8c [36];
  int local_68;
  int *local_64 [2];
  undefined2 local_5c;
  undefined4 local_58;
  undefined1 local_54;
  int *local_50;
  undefined1 local_4c;
  undefined1 local_4b;
  int local_48;
  int local_44;
  int local_40;
  int local_3c;
  int local_38;
  int local_34;
  int *piStack_30;
  int *piStack_2c;
  int iStack_28;
  
  if (0xfe < param_7) {
    param_7 = 0xff;
  }
  local_34 = param_1;
  piStack_30 = param_2;
  piStack_2c = param_3;
  iStack_28 = param_4;
  iVar4 = FUN_08029394(param_2);
  if ((iVar4 == 0 && param_4 == 0) && param_5 == 0) {
    FUN_0801cd3c(local_34,param_2,param_3,param_6,param_7);
  }
  else {
    iVar5 = FUN_08017484(&local_44,param_2,*(undefined4 *)(local_34 + 8));
    if (iVar5 != 0) {
      local_48 = (local_3c - local_44) + 1;
      FUN_0802d8d6(local_64,0x1c);
      local_58 = FUN_0802d624(local_48);
      local_68 = -1;
      if (0 < param_4) {
        FUN_080298a0(auStack_8c,param_2,param_4,0);
        local_68 = FUN_08028e78(auStack_8c,0);
      }
      FUN_080298a0(auStack_b0,param_3,param_5,1);
      local_b4 = FUN_08028e78(auStack_b0,0);
      local_64[0] = &local_c4;
      local_5c = (undefined2)param_6;
      local_4c = (undefined1)param_7;
      local_4b = param_8;
      iVar14 = *param_2;
      iVar7 = *param_3;
      iVar5 = iVar14 + param_4;
      if (iVar14 + param_4 <= iVar7) {
        iVar5 = iVar7;
      }
      iVar6 = param_3[2];
      iVar8 = param_2[2] - param_4;
      if (iVar6 <= iVar8) {
        iVar8 = iVar6;
      }
      iVar13 = param_2[1];
      iVar9 = param_3[1];
      iVar10 = iVar13 + param_4;
      if (iVar13 + param_4 <= iVar9) {
        iVar10 = iVar9;
      }
      iVar11 = param_3[3];
      iVar12 = param_2[3] - param_4;
      if (iVar11 <= iVar12) {
        iVar12 = iVar11;
      }
      bVar1 = iVar11 <= param_2[3];
      local_50 = local_64[0];
      if (iVar4 == 0) {
        bVar2 = iVar6 <= param_2[2];
        bVar3 = true;
        if ((((iVar14 <= iVar7 && bVar2) && iVar13 <= iVar9) && bVar1) &&
           ((iVar8 - iVar5) + 1 < 0x32)) {
          bVar3 = false;
        }
        local_54 = 1;
        if ((bool)(iVar13 <= iVar9 & bVar3)) {
          local_c0 = param_2[1];
          local_b8 = param_3[1] + -1;
          local_c4 = iVar5;
          local_bc = iVar8;
          FUN_08029de4(local_34,local_64);
        }
        if ((bool)(bVar1 & bVar3)) {
          local_c0 = param_3[3] + 1;
          local_b8 = param_2[3];
          local_c4 = iVar5;
          local_bc = iVar8;
          FUN_08029de4(local_34,local_64);
        }
        if (iVar14 <= iVar7) {
          local_c4 = *param_2;
          local_bc = *param_3 + -1;
          local_c0 = iVar10;
          local_b8 = iVar12;
          FUN_08029de4(local_34,local_64);
        }
        if (bVar2) {
          local_c4 = param_3[2] + 1;
          local_bc = param_2[2];
          local_c0 = iVar10;
          local_b8 = iVar12;
          FUN_08029de4(local_34,local_64);
        }
        if (bVar3) {
          local_c4 = local_44;
          if (local_3c < iVar5 + -1) {
            local_bc = local_3c;
          }
          else {
            local_bc = iVar5 + -1;
          }
          iVar4 = (local_bc - local_44) + 1;
          if (0 < iVar4) {
            if (iVar14 <= iVar7 || iVar13 <= iVar9) {
              for (iVar5 = local_40; iVar5 < iVar10; iVar5 = iVar5 + 1) {
                local_c0 = iVar5;
                local_b8 = iVar5;
                FUN_0802d926(local_58,iVar4);
                local_54 = FUN_08029220(local_58,local_c4,iVar5,iVar4);
                FUN_08029de4(local_34,local_64);
              }
            }
            if (iVar14 <= iVar7 || bVar1) {
              iVar5 = iVar12;
              while (iVar5 = iVar5 + 1, iVar5 <= local_38) {
                local_c0 = iVar5;
                local_b8 = iVar5;
                FUN_0802d926(local_58,iVar4);
                local_54 = FUN_08029220(local_58,local_c4,iVar5,iVar4);
                FUN_08029de4(local_34,local_64);
              }
            }
          }
          if (local_44 <= iVar8 + 1) {
            local_44 = iVar8 + 1;
          }
          local_bc = local_3c;
          iVar4 = (local_3c - local_44) + 1;
          local_c4 = local_44;
          if (0 < iVar4) {
            if (bVar2 || iVar13 <= iVar9) {
              for (iVar5 = local_40; iVar5 < iVar10; iVar5 = iVar5 + 1) {
                local_c0 = iVar5;
                local_b8 = iVar5;
                FUN_0802d926(local_58,iVar4);
                local_54 = FUN_08029220(local_58,local_c4,iVar5,iVar4);
                FUN_08029de4(local_34,local_64);
              }
            }
            if (bVar2 || bVar1) {
              while (iVar12 = iVar12 + 1, iVar12 <= local_38) {
                local_c0 = iVar12;
                local_b8 = iVar12;
                FUN_0802d926(local_58,iVar4);
                local_54 = FUN_08029220(local_58,local_c4,iVar12,iVar4);
                FUN_08029de4(local_34,local_64);
              }
            }
          }
        }
        else {
          local_c4 = local_44;
          local_bc = local_3c;
          if (param_4 <= param_3[1] - param_2[1]) {
            param_4 = param_3[1] - param_2[1];
          }
          for (iVar4 = 0; iVar4 < param_4; iVar4 = iVar4 + 1) {
            iVar5 = param_2[1] + iVar4;
            iVar7 = param_2[3] - iVar4;
            if ((local_40 <= iVar5) || (iVar7 <= local_38)) {
              FUN_0802d926(local_58,local_48);
              local_54 = FUN_08029220(local_58,local_c4,iVar5,local_48);
              if (local_40 <= iVar5) {
                local_c0 = iVar5;
                local_b8 = iVar5;
                FUN_08029de4(local_34,local_64);
              }
              if (iVar7 <= local_38) {
                local_c0 = iVar7;
                local_b8 = iVar7;
                FUN_08029de4(local_34,local_64);
              }
            }
          }
        }
        FUN_08029358(auStack_b0);
        FUN_080299f4(local_b4);
        FUN_08029358(auStack_8c);
        FUN_080299f4(local_68);
        FUN_0802d6c0(local_58);
      }
      else {
        local_c4 = local_44;
        local_bc = local_3c;
        for (iVar4 = local_40; iVar4 <= local_38; iVar4 = iVar4 + 1) {
          if ((iVar13 <= iVar9) || (iVar10 <= iVar4)) {
            if ((!bVar1) && (iVar12 < iVar4)) break;
            local_c0 = iVar4;
            local_b8 = iVar4;
            FUN_0802d926(local_58,local_48);
            local_54 = FUN_08029220(local_58,local_44,iVar4,local_48);
            FUN_08029de4(local_34,local_64);
          }
        }
        FUN_08029358(auStack_b0);
        FUN_080299f4(local_b4);
        if (local_68 != -1) {
          FUN_08029358(auStack_8c);
          FUN_080299f4(local_68);
        }
        FUN_0802d6c0(local_58);
      }
    }
  }
  return;
}

