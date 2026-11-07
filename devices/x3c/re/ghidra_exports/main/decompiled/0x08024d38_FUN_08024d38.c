// 0x08024d38 FUN_08024d38 size=290

void FUN_08024d38(int param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  short sVar1;
  int iVar2;
  int iVar3;
  short sVar4;
  int iVar5;
  int local_28;
  undefined4 local_24;
  undefined4 local_20;
  
  iVar3 = DAT_08024e5c;
  local_24 = param_3;
  local_20 = param_4;
  if ((int)((uint)*(byte *)(param_1 + 1) << 0x1c) < 0) {
    FUN_0802ad74(*(undefined4 *)(param_1 + 0x44),3,*(undefined4 *)(DAT_08024e5c + 8));
    iVar2 = FUN_08024ea0(param_1);
    if (iVar2 != 0) {
      return;
    }
    *(undefined4 *)(param_1 + 0x44) = 0;
    *(undefined4 *)(param_1 + 0x48) = 0;
    *(undefined4 *)(param_1 + 0x70) = 0;
    *(undefined4 *)(param_1 + 0x74) = 0;
    *(byte *)(param_1 + 1) = *(byte *)(param_1 + 1) & 0xf7;
  }
  iVar2 = *(int *)(param_1 + 0x44);
  *(int *)(iVar3 + 4) = iVar2;
  iVar5 = *(int *)(param_1 + 0x4c);
  local_28 = param_2;
  if (iVar2 != 0) {
    FUN_0802ad74(iVar2,8,*(undefined4 *)(iVar3 + 8));
    iVar2 = FUN_08024ea0(param_1);
    if (iVar2 != 0) {
      return;
    }
    if (iVar5 == 0) {
      if ((*(byte *)(param_1 + 1) & 1) == 0) {
        FUN_0802ad74(*(undefined4 *)(iVar3 + 4),4,*(undefined4 *)(iVar3 + 8));
        iVar2 = FUN_08024ea0(param_1);
        if (iVar2 != 0) {
          return;
        }
      }
      FUN_0802ad74(*(undefined4 *)(iVar3 + 4),7,*(undefined4 *)(iVar3 + 8));
      iVar3 = FUN_08024ea0(param_1);
      if (iVar3 != 0) {
        return;
      }
    }
    *(undefined4 *)(param_1 + 0x44) = 0;
    *(undefined4 *)(param_1 + 0x70) = 0;
    *(undefined4 *)(param_1 + 0x74) = 0;
    if (iVar5 != 0) {
      sVar4 = 0;
      iVar2 = 0x100;
      local_24 = 0;
      local_20 = 0;
      for (iVar3 = iVar5; iVar3 != 0; iVar3 = FUN_0802ee0a(iVar3)) {
        sVar1 = FUN_0802f9bc(iVar3,0,0x6d);
        sVar4 = sVar1 + sVar4;
        local_28 = FUN_0802f9bc(iVar3,0,0x6c);
        iVar2 = (int)(short)((short)iVar2 *
                            (short)(local_28 + ((uint)(local_28 >> 0x1f) >> 0x18) >> 8));
      }
      if ((sVar4 != 0) || (iVar2 != 0x100)) {
        iVar3 = (int)(short)(0x10000 / iVar2);
        FUN_080318c8(param_1 + 0x34,(int)-sVar4,iVar3,&local_24);
        FUN_080318c8(param_1 + 0x3c,(int)-sVar4,iVar3,&local_24);
      }
    }
  }
  if (iVar5 == 0) {
    return;
  }
  FUN_08017c32(param_1);
  FUN_08024ea0(param_1,local_28,local_24,local_20);
  return;
}

