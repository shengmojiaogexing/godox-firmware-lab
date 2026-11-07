// 0x08029ab0 FUN_08029ab0 size=788

void FUN_08029ab0(int param_1,undefined2 *param_2,int *param_3,uint param_4,uint param_5,
                 uint param_6)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  int iVar4;
  uint local_1b8 [4];
  int local_1a8;
  int local_1a4;
  int local_1a0;
  int local_19c;
  undefined1 auStack_198 [4];
  int *local_194;
  uint local_190;
  short local_18c;
  short local_18a;
  ushort local_188;
  ushort local_186;
  uint local_184;
  undefined4 *local_180;
  int *local_17c;
  int local_178;
  undefined4 local_174;
  undefined4 local_170;
  undefined1 auStack_16c [140];
  undefined4 local_e0;
  undefined1 auStack_dc [36];
  int local_b8;
  undefined1 auStack_b4 [36];
  int local_90;
  int local_8c;
  int local_88;
  int local_84;
  int local_80;
  int local_7c;
  int local_78;
  int local_74;
  undefined4 local_70;
  byte local_6c;
  undefined1 local_6b;
  undefined2 local_6a;
  undefined4 local_5c;
  undefined1 local_52;
  
  if (((2 < *(byte *)(param_2 + 8)) && (uVar3 = *(uint *)(param_2 + 2), uVar3 != 0)) &&
     (param_5 != param_6)) {
    if ((int)param_4 < (int)uVar3) {
      uVar3 = param_4;
    }
    FUN_08029a38(&local_70);
    local_6c = *(byte *)((int)param_2 + 0x11) & 3;
    if (*(int *)(param_2 + 6) == 0) {
      local_6b = *(undefined1 *)(param_2 + 8);
      local_6a = *param_2;
    }
    else {
      local_6b = 0;
      local_5c = *(undefined4 *)(param_2 + 6);
      local_52 = *(undefined1 *)(param_2 + 8);
    }
    local_80 = *param_3 - param_4;
    local_7c = param_3[1] - param_4;
    local_78 = *param_3 + (param_4 - 1);
    local_74 = (param_4 - 1) + param_3[1];
    local_90 = local_80 + *(int *)(param_2 + 2);
    local_8c = local_7c + *(int *)(param_2 + 2);
    local_88 = local_78 - *(int *)(param_2 + 2);
    local_84 = local_74 - *(int *)(param_2 + 2);
    iVar4 = -1;
    local_b8 = 0;
    if ((0 < (local_88 - local_90) + 1) && (0 < (local_84 - local_8c) + 1)) {
      FUN_080298a0(auStack_b4,&local_90,0x7fff,1);
      local_b8 = 1;
      iVar4 = FUN_08028e78(auStack_b4,0);
    }
    FUN_080298a0(auStack_dc,&local_80,0x7fff,0);
    local_e0 = FUN_08028e78(auStack_dc,0);
    if ((param_5 + 0x168 == param_6) || (param_5 == param_6 + 0x168)) {
      local_70 = 0x7fff;
      FUN_08029a14(param_1,&local_70,&local_80);
      FUN_080299f4(local_e0);
      if (iVar4 != -1) {
        FUN_080299f4(iVar4);
      }
      FUN_08029358(auStack_dc);
      if (local_b8 != 0) {
        FUN_08029358(auStack_b4);
      }
    }
    else {
      for (; 0x167 < param_5; param_5 = param_5 - 0x168 & 0xffff) {
      }
      for (; 0x167 < param_6; param_6 = param_6 - 0x168 & 0xffff) {
      }
      local_1b8[0] = param_6;
      FUN_0802916c(auStack_16c,*param_3,param_3[1],param_5);
      local_170 = FUN_08028e78(auStack_16c,0);
      if (param_5 < param_6) {
        iVar1 = 0x168 - (param_6 - param_5);
      }
      else {
        iVar1 = param_5 - param_6;
      }
      local_174 = *(undefined4 *)(param_1 + 8);
      if ((iVar1 < 0x3d) || (param_4 < 0xb)) {
        FUN_08029a14(param_1,&local_70,&local_80);
      }
      else {
        local_188 = (ushort)(param_5 / 0x5a) & 3;
        local_186 = (ushort)(param_6 / 0x5a) & 3;
        local_180 = &local_70;
        local_17c = &local_80;
        local_194 = param_3;
        local_190 = param_4;
        local_18c = (short)param_5;
        local_18a = (short)param_6;
        local_184 = uVar3;
        local_178 = param_1;
        FUN_0801eb1c(&local_194);
        FUN_0801ecea(&local_194);
        FUN_0801eec0(&local_194);
        FUN_0801f098(&local_194);
      }
      FUN_08029358(auStack_16c);
      FUN_08029358(auStack_dc);
      if (local_b8 != 0) {
        FUN_08029358(auStack_b4);
      }
      FUN_080299f4(local_170);
      FUN_080299f4(local_e0);
      if (iVar4 != -1) {
        FUN_080299f4(iVar4);
      }
      if ((int)((uint)*(byte *)((int)param_2 + 0x11) << 0x1d) < 0) {
        FUN_08022658((int)(short)param_5,param_4,uVar3 & 0xff,&local_1a8);
        local_1a8 = local_1a8 + *param_3;
        local_1a0 = local_1a0 + *param_3;
        local_1a4 = local_1a4 + param_3[1];
        local_19c = local_19c + param_3[1];
        iVar4 = FUN_08017484(local_1b8,local_174,&local_1a8);
        if (iVar4 != 0) {
          FUN_080298a0(auStack_198,&local_1a8,0x7fff,0);
          uVar2 = FUN_08028e78(auStack_198,0);
          *(uint **)(param_1 + 8) = local_1b8;
          FUN_08029a14(param_1,&local_70,&local_80);
          FUN_080299f4(uVar2);
          FUN_08029358(auStack_198);
        }
        FUN_08022658((int)(short)param_6,param_4,uVar3 & 0xff,&local_1a8);
        local_1a8 = local_1a8 + *param_3;
        local_1a0 = local_1a0 + *param_3;
        local_1a4 = local_1a4 + param_3[1];
        local_19c = local_19c + param_3[1];
        iVar4 = FUN_08017484(local_1b8,local_174,&local_1a8);
        if (iVar4 != 0) {
          FUN_080298a0(auStack_198,&local_1a8,0x7fff,0);
          uVar2 = FUN_08028e78(auStack_198,0);
          *(uint **)(param_1 + 8) = local_1b8;
          FUN_08029a14(param_1,&local_70,&local_80);
          FUN_080299f4(uVar2);
          FUN_08029358(auStack_198);
        }
        *(undefined4 *)(param_1 + 8) = local_174;
      }
    }
  }
  return;
}

