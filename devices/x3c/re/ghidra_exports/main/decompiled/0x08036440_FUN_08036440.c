// 0x08036440 FUN_08036440 size=534

void FUN_08036440(int param_1,int param_2)

{
  int iVar1;
  undefined1 *puVar2;
  int iVar3;
  undefined4 *puVar4;
  uint uVar5;
  uint uVar6;
  char local_58 [4];
  undefined1 *local_54;
  short local_50;
  undefined2 local_4e;
  int local_4c;
  int local_48;
  undefined1 local_41;
  undefined1 local_40;
  undefined1 local_38;
  uint local_34;
  uint local_30;
  undefined1 auStack_2c [12];
  int local_20;
  
  iVar1 = FUN_0802fc9e(param_2,1);
  if (iVar1 == 0) {
    iVar1 = FUN_080180d0(param_2);
    if (iVar1 == 0) {
      FUN_0803066e(param_1,param_2);
      return;
    }
    puVar2 = (undefined1 *)FUN_0802f9bc(param_2,0,0x60);
    if ((1 < ((uint)puVar2 & 0xff)) &&
       (local_54 = puVar2, iVar3 = FUN_080258ee(param_1,param_2,iVar1,auStack_2c), iVar3 == 1)) {
      uVar6 = 1;
      iVar3 = FUN_080174c4(auStack_2c,param_2 + 0x14,0);
      if (iVar3 != 0) {
        local_58[0] = '\0';
        local_54 = auStack_2c;
        FUN_0802ad74(param_2,0x12,local_58);
        if (local_58[0] == '\0') {
          uVar6 = 0;
        }
      }
      if (iVar1 == 1) {
        uVar6 = uVar6 | 2;
      }
      puVar4 = (undefined4 *)FUN_08028d68(param_1,auStack_2c,uVar6);
      if (puVar4 != (undefined4 *)0x0) {
        local_54 = (undefined1 *)FUN_0802f9bc(param_2,0,0x6e);
        local_34 = (uint)local_54;
        local_54 = (undefined1 *)FUN_0802f9bc(param_2,0,0x6f);
        if (((local_34 & 0x7fffffff) >> 0x1d == 1) &&
           (uVar5 = local_34 & 0x9fffffff, (int)uVar5 < 0x7d1)) {
          if (1000 < (int)uVar5) {
            uVar5 = 1000 - uVar5;
          }
          local_34 = (int)(uVar5 * ((*(int *)(param_2 + 0x1c) - *(int *)(param_2 + 0x14)) + 1)) /
                     100;
        }
        local_30 = (uint)local_54;
        if ((((uint)local_54 & 0x7fffffff) >> 0x1d == 1) &&
           (uVar5 = (uint)local_54 & 0x9fffffff, (int)uVar5 < 0x7d1)) {
          if (1000 < (int)uVar5) {
            uVar5 = 1000 - uVar5;
          }
          local_30 = (int)(uVar5 * ((*(int *)(param_2 + 0x20) - *(int *)(param_2 + 0x18)) + 1)) /
                     100;
        }
        FUN_080287cc(&local_50);
        local_41 = SUB41(puVar2,0);
        local_54 = (undefined1 *)FUN_0802f9bc(param_2,0,0x6d);
        local_50 = (short)local_54;
        if (local_50 < 0xe11) {
          if (local_50 < 0) {
            local_50 = local_50 + 0xe10;
          }
        }
        else {
          local_50 = local_50 + -0xe10;
        }
        local_54 = (undefined1 *)FUN_0802f9bc(param_2,0,0x6c);
        local_4e = SUB42(local_54,0);
        local_54 = (undefined1 *)FUN_0802f9bc(param_2,0,0x67);
        local_40 = SUB41(local_54,0);
        local_38 = (undefined1)((*(byte *)(**(int **)(DAT_08036658 + 4) + 0x1c) & 0xf) >> 3);
        if ((int)(uVar6 << 0x1e) < 0) {
          puVar4[4] = *puVar4;
          puVar4[5] = puVar4[1];
          puVar4[6] = puVar4[2];
          iVar1 = puVar4[1] + puVar4[9] + -1;
          puVar4[7] = iVar1;
          if ((int)puVar4[3] < iVar1) {
            puVar4[7] = puVar4[3];
          }
        }
        while ((int)puVar4[5] <= local_20) {
          if ((int)(uVar6 << 0x1e) < 0) {
            FUN_08025888(param_2,param_1,puVar4,uVar6);
          }
          FUN_0803066e(param_1,param_2);
          local_4c = (*(int *)(param_2 + 0x14) + local_34) - **(int **)(param_1 + 4);
          local_48 = (*(int *)(param_2 + 0x18) + local_30) - *(int *)(*(int *)(param_1 + 4) + 4);
          FUN_08028d5e(param_1,puVar4,&local_50);
          if (-1 < (int)(uVar6 << 0x1e)) break;
          puVar4[5] = puVar4[7] + 1;
          puVar4[7] = puVar4[7] + 1 + puVar4[9] + -1;
        }
        FUN_08028dce(param_1,puVar4);
      }
    }
  }
  return;
}

