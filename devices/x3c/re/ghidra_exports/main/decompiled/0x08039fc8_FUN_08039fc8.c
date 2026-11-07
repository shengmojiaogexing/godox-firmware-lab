// 0x08039fc8 FUN_08039fc8 size=232

void FUN_08039fc8(int param_1,undefined4 param_2,uint param_3,undefined4 param_4)

{
  short sVar1;
  short sVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  uint local_24;
  uint local_20;
  
  local_24 = param_3;
  local_20 = param_4;
  sVar1 = FUN_0802f9bc(param_1,0,0x6d,param_4,param_2);
  iVar5 = (int)sVar1;
  sVar2 = FUN_0802f9bc(param_1,0,0x6c);
  iVar6 = (int)sVar2;
  if ((iVar5 != 0) || (iVar6 != 0x100)) {
    local_24 = FUN_0802f9bc(param_1,0,0x6e);
    uVar3 = FUN_0802f9bc(param_1,0,0x6f);
    if (((local_24 & 0x7fffffff) >> 0x1d == 1) &&
       (uVar4 = local_24 & 0x9fffffff, (int)uVar4 < 0x7d1)) {
      if (1000 < (int)uVar4) {
        uVar4 = 1000 - uVar4;
      }
      local_24 = (int)(((*(int *)(param_1 + 0x1c) - *(int *)(param_1 + 0x14)) + 1) * uVar4) / 100;
    }
    local_20 = uVar3;
    if (((uVar3 & 0x7fffffff) >> 0x1d == 1) && (uVar4 = uVar3 & 0x9fffffff, (int)uVar4 < 0x7d1)) {
      if (1000 < (int)uVar4) {
        uVar4 = 1000 - uVar4;
      }
      local_20 = (int)(uVar4 * ((*(int *)(param_1 + 0x20) - *(int *)(param_1 + 0x18)) + 1)) / 100;
    }
    local_24 = *(int *)(param_1 + 0x14) + local_24;
    local_20 = *(int *)(param_1 + 0x18) + local_20;
    if (param_3 != 0) {
      iVar5 = (int)-sVar1;
      iVar6 = (int)(short)(0x10000 / iVar6);
    }
    FUN_080318c8(param_2,iVar5,iVar6,&local_24,uVar3);
  }
  return;
}

