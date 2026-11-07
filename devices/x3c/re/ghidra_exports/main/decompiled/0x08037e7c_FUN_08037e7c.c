// 0x08037e7c FUN_08037e7c size=376

void FUN_08037e7c(int *param_1,undefined2 *param_2,uint param_3,int param_4)

{
  byte *pbVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  undefined2 *puVar7;
  int iVar8;
  code *local_54 [9];
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  
  iVar6 = param_3 + param_4;
  FUN_0802748a(&local_30,param_1);
  local_28 = ((param_4 + (int)param_3 / 2) - ((int)(param_3 << 0x1f) >> 0x1f)) + -2;
  local_2c = (int)param_3 / 2 + 1;
  local_30 = local_28 - ((param_1[2] - *param_1) + 1);
  local_24 = (param_1[3] - param_1[1]) + 1 + local_2c;
  FUN_080298a0(local_54,&local_30,param_4,0);
  iVar4 = 1;
  if (param_3 != 1) {
    iVar4 = (int)param_3 >> 1;
  }
  pbVar1 = (byte *)FUN_0802d624(iVar6);
  puVar7 = param_2;
  for (iVar8 = 0; iVar8 < iVar6; iVar8 = iVar8 + 1) {
    FUN_0802d926(pbVar1,iVar6);
    iVar2 = (*local_54[0])(pbVar1,0,iVar8,iVar6,local_54);
    if (iVar2 == 0) {
      FUN_0802d8d6(puVar7,iVar6 * 2);
    }
    else {
      *puVar7 = (short)((int)((uint)*pbVar1 << 6) / iVar4);
      for (iVar2 = 1; iVar2 < iVar6; iVar2 = iVar2 + 1) {
        if ((uint)pbVar1[iVar2] == (uint)pbVar1[iVar2 + -1]) {
          puVar7[iVar2] = puVar7[iVar2 + -1];
        }
        else {
          puVar7[iVar2] = (short)((int)((uint)pbVar1[iVar2] << 6) / iVar4);
        }
      }
    }
    puVar7 = puVar7 + iVar6;
  }
  FUN_0802d6c0(pbVar1);
  FUN_08029358(local_54);
  if (iVar4 == 1) {
    for (iVar4 = 0; iVar4 < iVar6 * iVar6; iVar4 = iVar4 + 1) {
      *(char *)((int)param_2 + iVar4) = (char)((ushort)param_2[iVar4] >> 6);
    }
  }
  else {
    FUN_08037d3c(iVar6,iVar4,param_2);
    iVar4 = (param_3 & 1) + iVar4;
    if (1 < iVar4) {
      for (uVar3 = 0; uVar3 < (uint)(iVar6 * iVar6); uVar3 = uVar3 + 1) {
        uVar5 = (uint)(ushort)param_2[uVar3];
        if (uVar5 != 0) {
          if (uVar5 == 0xff) {
            param_2[uVar3] = (short)(0x3fc0 / iVar4);
          }
          else {
            param_2[uVar3] = (short)((int)(uVar5 << 6) / iVar4);
          }
        }
      }
      FUN_08037d3c(iVar6,iVar4,param_2);
    }
    for (iVar4 = 0; iVar4 < iVar6 * iVar6; iVar4 = iVar4 + 1) {
      *(undefined1 *)((int)param_2 + iVar4) = *(undefined1 *)(param_2 + iVar4);
    }
  }
  return;
}

