// 0x080293f0 FUN_080293f0 size=204

undefined4 FUN_080293f0(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  byte bVar1;
  int iVar2;
  undefined4 uVar3;
  
  param_3 = param_3 - *(int *)(param_5 + 0x20);
  param_2 = param_2 - *(int *)(param_5 + 0x1c);
  if (*(int *)(param_5 + 0x2c) != 0) {
    if ((*(byte *)(param_5 + 0x34) & 1) == 0) {
      uVar3 = FUN_08026176();
      return uVar3;
    }
    uVar3 = FUN_08025fec();
    return uVar3;
  }
  if ((*(byte *)(param_5 + 0x34) & 1) != 0) {
    bVar1 = *(byte *)(param_5 + 0x18);
    if (((bVar1 & 3) == 0) || ((bVar1 & 3) == 1)) {
      return 1;
    }
    if (((bVar1 & 3) == 2) && (param_3 + 1 < 0)) {
      return 1;
    }
    if (((~bVar1 & 3) == 0) && (0 < param_3)) {
      return 1;
    }
    return 0;
  }
  bVar1 = *(byte *)(param_5 + 0x18);
  if (((bVar1 & 3) == 2) || ((~bVar1 & 3) == 0)) {
    return 1;
  }
  if (((bVar1 & 3) == 1) && (0 < param_2)) {
    return 1;
  }
  if ((bVar1 & 3) != 0) {
    if (param_2 + param_4 < 0) {
      return 0;
    }
    param_2 = -param_2;
    if (param_2 < 0) {
      param_2 = 0;
    }
    if (param_4 <= param_2) {
      return 0;
    }
    if (-1 < param_2) {
      FUN_0802d8d6();
    }
    return 2;
  }
  if (param_2 + param_4 < 0) {
    return 1;
  }
  iVar2 = -param_2;
  if (iVar2 < 0) {
    return 0;
  }
  if (iVar2 < param_4) {
    FUN_0802d8d6(param_1 + iVar2,param_4 + param_2,iVar2,param_4,param_4);
  }
  return 2;
}

