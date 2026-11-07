// 0x08028800 FUN_08028800 size=1278

void FUN_08028800(int param_1,int *param_2,uint *param_3,char *param_4,int *param_5)

{
  char cVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  char *pcVar7;
  char *pcVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  uint local_130;
  uint local_12c;
  undefined4 local_128;
  undefined4 local_124;
  uint local_120;
  uint local_11c;
  uint local_118;
  char *local_114;
  uint local_110;
  undefined1 auStack_10c [6];
  undefined2 local_106;
  int local_c0;
  uint local_bc;
  uint local_b8;
  uint local_b4;
  uint local_b0;
  undefined2 local_ac [2];
  int local_a8;
  undefined1 local_9c;
  byte local_9b;
  int local_98;
  uint local_94;
  uint local_90;
  int local_8c;
  uint local_88;
  uint local_84;
  undefined1 auStack_80 [16];
  uint local_70;
  int local_6c;
  undefined1 auStack_68 [12];
  undefined2 local_5c;
  uint local_3c;
  uint local_38;
  int local_34;
  int *piStack_30;
  uint *puStack_2c;
  char *pcStack_28;
  
  if (((2 < *(byte *)(param_2 + 9)) && (*param_2 != 0)) && (*(int *)(param_1 + 0x20) != 0)) {
    local_34 = param_1;
    piStack_30 = param_2;
    puStack_2c = param_3;
    pcStack_28 = param_4;
    FUN_08004e36(auStack_68,param_2,0x2c);
    local_6c = *param_2;
    if (((param_4 != (char *)0x0) && (*param_4 != '\0')) &&
       (iVar3 = FUN_08017484(auStack_80,param_3,*(undefined4 *)(local_34 + 8)), iVar3 != 0)) {
      local_84 = (uint)*(byte *)((int)param_2 + 0x26);
      local_88 = (uint)*(byte *)((int)param_2 + 0x25);
      if (local_84 == 0) {
        local_84 = 1;
      }
      if ((int)((uint)*(byte *)((int)param_2 + 0x27) << 0x1e) < 0) {
        local_128 = (uint)*(byte *)((int)param_2 + 0x27);
        local_130 = param_2[5];
        local_12c = 0x1fffffff;
        FUN_08033a84(&local_118,param_4,*param_2,param_2[6]);
        local_70 = local_118;
      }
      else {
        local_70 = (param_3[2] - *param_3) + 1;
      }
      iVar9 = *(int *)(local_6c + 8);
      local_8c = param_2[5] + iVar9;
      local_94 = *param_3;
      local_98 = param_2[7];
      local_90 = param_3[1] + param_2[8];
      iVar3 = -1;
      if (((param_5 != (int *)0x0) && (param_2[8] == 0)) && (uVar6 = param_3[1], (int)uVar6 < 0)) {
        iVar3 = param_5[2] - uVar6;
        if (iVar3 < 1) {
          iVar3 = uVar6 - param_5[2];
        }
        if (local_8c * -2 + 0x400 < iVar3) {
          *param_5 = -1;
        }
        iVar3 = *param_5;
      }
      iVar5 = 0;
      if ((param_5 != (int *)0x0) && (-1 < iVar3)) {
        local_90 = local_90 + param_5[1];
        iVar5 = iVar3;
      }
      local_12c = (uint)*(byte *)((int)param_2 + 0x27);
      local_130 = 0;
      iVar3 = FUN_08018918(param_4 + iVar5,local_6c,param_2[6],local_70);
      iVar3 = iVar3 + iVar5;
      do {
        if (*(int *)(*(int *)(local_34 + 8) + 4) <= (int)(local_90 + iVar9)) {
          local_38 = local_84 & 0xff;
          if (local_38 == 2) {
            local_130 = (uint)*(byte *)((int)param_2 + 0x27);
            iVar9 = FUN_08033b40(param_4 + iVar5,iVar3 - iVar5,local_6c,param_2[6]);
            local_94 = local_94 + (int)(((param_3[2] - *param_3) + 1) - iVar9) / 2;
          }
          else if (local_38 == 3) {
            local_130 = (uint)*(byte *)((int)param_2 + 0x27);
            iVar9 = FUN_08033b40(param_4 + iVar5,iVar3 - iVar5,local_6c,param_2[6]);
            local_94 = (((param_3[2] - *param_3) + 1) - iVar9) + local_94;
          }
          uVar10 = param_2[1];
          uVar11 = param_2[2];
          uVar6 = uVar10;
          if (uVar11 < uVar10) {
            uVar6 = uVar11;
            uVar11 = uVar10;
          }
          if ((*(byte *)(param_2 + 10) & 3) != 0) {
            FUN_08028e30(local_ac);
            local_ac[0] = (undefined2)param_2[3];
            local_a8 = (int)*(char *)(local_6c + 0x12);
            if (local_a8 == 0) {
              local_a8 = 1;
            }
            local_9c = (undefined1)param_2[9];
            local_9b = local_9b & 0xfc | *(byte *)(param_2 + 10) >> 3 & 3;
          }
          local_b4 = 0;
          local_b8 = FUN_08027a3c();
          local_bc = FUN_08027a3c();
          FUN_08029a38(auStack_10c);
          local_106 = (undefined2)param_2[4];
          local_110 = local_94;
          goto LAB_08028cee;
        }
        local_12c = (uint)*(byte *)((int)param_2 + 0x27);
        local_130 = 0;
        iVar4 = FUN_08018918(param_4 + iVar3,local_6c,param_2[6],local_70);
        local_90 = local_8c + local_90;
        if (((param_5 != (int *)0x0) && (-0x401 < (int)local_90)) && (*param_5 < 0)) {
          *param_5 = iVar3;
          param_5[1] = local_90 - param_3[1];
          param_5[2] = param_3[1];
        }
        pcVar8 = param_4 + iVar3;
        iVar5 = iVar3;
        iVar3 = iVar4 + iVar3;
      } while (*pcVar8 != '\0');
    }
  }
  return;
LAB_08028cee:
  if (param_4[iVar5] == '\0') {
    return;
  }
  local_94 = local_98 + local_94;
  iVar9 = 0;
  local_b0 = 0;
  local_114 = param_4 + iVar5;
  local_3c = iVar3 - iVar5;
LAB_08028bce:
  if (local_3c <= local_b0) goto code_r0x08028bd8;
  local_118 = 0;
  if ((uVar6 != 0xffff) && (uVar11 != 0xffff)) {
    local_118 = (*(code *)*DAT_08028d00)(param_4,iVar5 + local_b0);
  }
  FUN_080188e4(local_114,&local_11c,&local_120,&local_b0);
  if ((*(byte *)((int)param_2 + 0x27) & 1) == 0) {
LAB_08028ab8:
    local_bc = param_2[3];
    if (iVar9 == 2) {
      local_bc = local_b8;
    }
    local_c0 = FUN_0802afc2(local_6c,local_11c,local_120);
    if ((((uVar6 != 0xffff) && (uVar11 != 0xffff)) && (uVar6 <= local_118)) && (local_118 < uVar11))
    {
      local_130 = local_94;
      local_12c = local_90;
      local_128 = local_c0 + -1 + param_2[6] + local_94;
      local_124 = local_8c + -1 + local_90;
      FUN_08029a14(local_34,auStack_10c,&local_130);
      local_bc = (uint)*(ushort *)((int)param_2 + 0xe);
    }
    local_5c = (undefined2)local_bc;
    FUN_08028e0c(local_34,auStack_68,&local_94,local_11c);
    if (0 < local_c0) {
      local_94 = local_c0 + param_2[6] + local_94;
    }
    goto LAB_08028bce;
  }
  if (local_11c == 0x23) {
    if (iVar9 == 0) {
      local_b4 = local_b0;
      iVar9 = 1;
      goto LAB_08028bce;
    }
    if (iVar9 != 1) {
      if (iVar9 != 2) goto LAB_08028ab4;
      iVar9 = 0;
      goto LAB_08028bce;
    }
    iVar9 = 0;
  }
LAB_08028ab4:
  if (iVar9 != 1) goto LAB_08028ab8;
  if (local_11c == 0x20) {
    if (local_b0 - local_b4 == 7) {
      pcVar8 = (char *)&local_128;
      pcVar7 = local_114 + local_b4;
      for (iVar9 = 6; iVar9 != 0; iVar9 = iVar9 + -1) {
        *pcVar8 = *pcVar7;
        pcVar8 = pcVar8 + 1;
        pcVar7 = pcVar7 + 1;
      }
      local_124._0_3_ = (uint3)(ushort)local_124;
      iVar9 = FUN_08023ac0(local_128._1_1_);
      iVar4 = FUN_08023ac0(local_128 & 0xff);
      local_12c = iVar9 + iVar4 * 0x10;
      iVar9 = FUN_08023ac0(local_128._3_1_);
      iVar4 = FUN_08023ac0(local_128._2_1_);
      local_130 = iVar9 + iVar4 * 0x10;
      cVar1 = FUN_08023ac0(local_124._1_1_);
      cVar2 = FUN_08023ac0(local_124 & 0xff);
      local_b8 = FUN_08027bac(local_12c & 0xff,local_130 & 0xff,cVar1 + cVar2 * '\x10');
    }
    else {
      local_b8 = (uint)*(ushort *)(param_2 + 3);
    }
    iVar9 = 2;
  }
  goto LAB_08028bce;
code_r0x08028bd8:
  if ((int)((uint)*(byte *)(param_2 + 10) << 0x1e) < 0) {
    local_11c = local_110;
    local_120 = *(int *)(*param_2 + 8) / 2 + local_a8 / 2 + local_90;
    local_124 = local_94;
    local_ac[0] = (undefined2)local_bc;
    local_118 = local_120;
    FUN_08028e16(local_34,local_ac,&local_11c,&local_124);
  }
  if ((*(byte *)(param_2 + 10) & 1) != 0) {
    local_11c = local_110;
    local_120 = ((local_90 + *(int *)(*param_2 + 8)) - *(int *)(*param_2 + 0xc)) -
                (int)*(char *)(local_6c + 0x11);
    local_124 = local_94;
    local_ac[0] = (undefined2)local_bc;
    local_118 = local_120;
    FUN_08028e16(local_34,local_ac,&local_11c,&local_124);
  }
  local_12c = (uint)*(byte *)((int)param_2 + 0x27);
  local_130 = 0;
  pcVar8 = param_4 + iVar3;
  iVar9 = FUN_08018918(pcVar8,local_6c,param_2[6],local_70);
  iVar9 = iVar9 + iVar3;
  local_94 = *param_3;
  if (local_38 == 2) {
    local_130 = (uint)*(byte *)((int)param_2 + 0x27);
    iVar5 = FUN_08033b40(pcVar8,iVar9 - iVar3,local_6c,param_2[6]);
    local_94 = local_94 + (int)(((param_3[2] - *param_3) + 1) - iVar5) / 2;
  }
  else if (local_38 == 3) {
    local_130 = (uint)*(byte *)((int)param_2 + 0x27);
    iVar5 = FUN_08033b40(pcVar8,iVar9 - iVar3,local_6c,param_2[6]);
    local_94 = (((param_3[2] - *param_3) + 1) - iVar5) + local_94;
  }
  local_90 = local_8c + local_90;
  iVar5 = iVar3;
  iVar3 = iVar9;
  if (*(int *)(*(int *)(local_34 + 8) + 0xc) < (int)local_90) {
    return;
  }
  goto LAB_08028cee;
}

