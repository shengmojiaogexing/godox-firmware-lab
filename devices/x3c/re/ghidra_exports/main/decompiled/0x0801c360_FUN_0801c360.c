// 0x0801c360 FUN_0801c360 size=834

void FUN_0801c360(int param_1,int *param_2,undefined4 param_3)

{
  byte *pbVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  byte bVar5;
  int iVar6;
  byte bVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int local_bc;
  int local_b8;
  int local_b4;
  int local_b0;
  int local_ac;
  int local_a8;
  int local_a4;
  int local_a0;
  undefined1 auStack_9c [36];
  int local_78;
  int local_74;
  int *local_70;
  int local_6c;
  undefined2 local_68;
  int local_64;
  undefined1 local_60;
  int *local_5c;
  byte local_58;
  undefined1 local_57;
  uint local_54;
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
  undefined4 uStack_28;
  
  if (2 < *(byte *)((int)param_2 + 5)) {
    local_30 = param_1;
    piStack_2c = param_2;
    uStack_28 = param_3;
    FUN_0802748a(&local_40,param_3);
    if (((1 < param_2[9]) && (0xfc < *(byte *)(param_2 + 10))) && (*param_2 != 0)) {
      pbVar1 = (byte *)((int)param_2 + 0x29);
      local_40 = local_40 - ((int)((uint)(*pbVar1 >> 1) << 0x1d) >> 0x1f);
      local_3c = local_3c - ((int)((uint)(*pbVar1 >> 1) << 0x1e) >> 0x1f);
      local_38 = local_38 + ((int)((uint)(*pbVar1 >> 1) << 0x1c) >> 0x1f);
      local_34 = local_34 + ((int)((uint)*pbVar1 << 0x1e) >> 0x1f);
    }
    iVar2 = FUN_08017484(&local_50,&local_40,*(undefined4 *)(local_30 + 8));
    if (iVar2 != 0) {
      bVar5 = *(byte *)((int)param_2 + 0x11) & 7;
      if ((*(byte *)((int)param_2 + 0x11) & 7) == 0) {
        local_54 = (uint)*(ushort *)((int)param_2 + 6);
      }
      else {
        local_54 = param_2[2];
      }
      if ((local_54 & 0xffff) == (uint)*(ushort *)(param_2 + 3)) {
        bVar5 = 0;
      }
      iVar2 = FUN_08029394(&local_40);
      FUN_08004e9a(&local_70,0x1c);
      local_57 = (undefined1)param_2[1];
      local_68 = (undefined2)local_54;
      if (((iVar2 == 0) && (*param_2 == 0)) && (bVar5 == 0)) {
        local_70 = &local_40;
        local_58 = *(undefined1 *)((int)param_2 + 5);
        FUN_08029de4(local_30,&local_70);
      }
      else {
        bVar7 = *(byte *)((int)param_2 + 5);
        if (0xfc < bVar7) {
          bVar7 = 0xff;
        }
        iVar6 = (local_38 - local_40) + 1;
        iVar10 = (local_34 - local_3c) + 1;
        if (iVar6 < iVar10) {
          iVar8 = iVar6;
        }
        else {
          iVar8 = iVar10;
        }
        iVar4 = *param_2;
        if (iVar8 >> 1 <= *param_2) {
          iVar4 = iVar8 >> 1;
        }
        local_74 = (local_48 - local_50) + 1;
        local_78 = -1;
        iVar8 = 0;
        if ((0 < iVar4) || (iVar2 != 0)) {
          iVar8 = FUN_0802d624(local_74);
          FUN_080298a0(auStack_9c,&local_40,iVar4,0);
          local_78 = FUN_08028e78(auStack_9c,0);
        }
        local_ac = local_50;
        local_a4 = local_48;
        local_70 = &local_ac;
        local_58 = 0xff;
        local_64 = iVar8;
        local_5c = local_70;
        iVar6 = FUN_0802b448(param_2 + 2,iVar6,iVar10);
        if ((iVar6 != 0) && (bVar5 == 2)) {
          local_6c = *(int *)(iVar6 + 8) + local_50 * 2 + local_40 * -2;
        }
        if (iVar2 == 0) {
          for (iVar2 = 0; iVar2 < iVar4; iVar2 = iVar2 + 1) {
            iVar10 = local_3c + iVar2;
            iVar9 = local_34 - iVar2;
            if ((local_4c <= iVar10) || (iVar9 <= local_44)) {
              FUN_0802d87c(iVar8,bVar7,local_74);
              iVar3 = FUN_08029220(iVar8,local_ac,iVar10,local_74);
              local_60 = (undefined1)iVar3;
              if (iVar3 == 1) {
                local_60 = 2;
              }
              if (local_4c <= iVar10) {
                if (bVar5 == 1) {
                  local_68 = *(undefined2 *)(*(int *)(iVar6 + 8) + (iVar10 - local_3c) * 2);
                }
                local_a8 = iVar10;
                local_a0 = iVar10;
                FUN_08029de4(local_30,&local_70);
              }
              if (iVar9 <= local_44) {
                if (bVar5 == 1) {
                  local_68 = *(undefined2 *)(*(int *)(iVar6 + 8) + (iVar9 - local_3c) * 2);
                }
                local_a8 = iVar9;
                local_a0 = iVar9;
                FUN_08029de4(local_30,&local_70);
              }
            }
          }
          local_bc = local_40;
          local_b4 = local_38;
          local_b8 = local_3c + iVar4;
          local_b0 = local_34 - iVar4;
          iVar2 = FUN_08029394(&local_bc);
          local_58 = bVar7;
          if (iVar2 == 0 && bVar5 == 0) {
            local_a8 = local_3c + iVar4;
            local_a0 = local_34 - iVar4;
            local_64 = 0;
            FUN_08029de4(local_30,&local_70);
          }
          else {
            local_60 = 1;
            iVar10 = local_34 - iVar4;
            for (iVar4 = local_3c + iVar4; iVar4 <= iVar10; iVar4 = iVar4 + 1) {
              if (iVar2 != 0) {
                FUN_0802d87c(iVar8,bVar7,local_74);
                local_60 = FUN_08029220(iVar8,local_50,iVar4,local_74);
              }
              if (bVar5 == 1) {
                local_68 = *(undefined2 *)(*(int *)(iVar6 + 8) + (iVar4 - local_3c) * 2);
              }
              local_a8 = iVar4;
              local_a0 = iVar4;
              FUN_08029de4(local_30,&local_70);
            }
          }
        }
        else {
          for (iVar2 = local_4c; iVar2 <= local_44; iVar2 = iVar2 + 1) {
            local_a8 = iVar2;
            local_a0 = iVar2;
            FUN_0802d87c(iVar8,bVar7,local_74);
            iVar10 = FUN_08029220(iVar8,local_50,iVar2,local_74);
            local_60 = (undefined1)iVar10;
            if (iVar10 == 1) {
              local_60 = 2;
            }
            if (bVar5 == 1) {
              local_68 = *(undefined2 *)(*(int *)(iVar6 + 8) + (iVar2 - local_3c) * 2);
            }
            FUN_08029de4(local_30,&local_70);
          }
        }
        if (iVar8 != 0) {
          FUN_0802d6c0(iVar8);
        }
        if (local_78 != -1) {
          FUN_080299f4(local_78);
          FUN_08029358(auStack_9c);
        }
        if (iVar6 != 0) {
          FUN_0802b43c(iVar6);
        }
      }
    }
  }
  return;
}

