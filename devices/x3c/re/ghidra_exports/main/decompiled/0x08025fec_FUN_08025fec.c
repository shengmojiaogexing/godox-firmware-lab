// 0x08025fec FUN_08025fec size=394

undefined4 FUN_08025fec(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  undefined1 uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  
  iVar2 = *(int *)(param_5 + 0x28);
  iVar4 = iVar2 * param_2 >> 10;
  if (iVar2 < 1) {
    if (iVar4 < param_3) {
      if ((int)((uint)*(byte *)(param_5 + 0x34) << 0x1e) < 0) {
        return 1;
      }
      return 0;
    }
  }
  else if (param_3 < iVar4) {
    if ((int)((uint)*(byte *)(param_5 + 0x34) << 0x1e) < 0) {
      return 1;
    }
    return 0;
  }
  iVar4 = iVar2 * (param_2 + param_4) >> 10;
  if (iVar2 < 1) {
    if (param_3 < iVar4) {
      if ((int)((uint)*(byte *)(param_5 + 0x34) << 0x1e) < 0) {
        return 0;
      }
      return 1;
    }
  }
  else if (iVar4 < param_3) {
    if ((int)((uint)*(byte *)(param_5 + 0x34) << 0x1e) < 0) {
      return 0;
    }
    return 1;
  }
  if (iVar2 < 1) {
    param_3 = (param_3 + 1) * *(int *)(param_5 + 0x24);
  }
  else {
    param_3 = param_3 * *(int *)(param_5 + 0x24);
  }
  uVar3 = param_3 * 0x100 >> 10 & 0xff;
  if (uVar3 == 0) {
    iVar2 = 0xff;
  }
  else {
    iVar2 = 0xff - ((int)((0xff - uVar3) * *(int *)(param_5 + 0x30)) >> 8);
  }
  param_2 = (param_3 * 0x100 >> 0x12) - param_2;
  iVar4 = param_2;
  if (uVar3 != 0) {
    if ((-1 < param_2) && (param_2 < param_4)) {
      uVar3 = 0xffU - ((int)((0xff - iVar2) * (0xff - uVar3)) >> 9) & 0xff;
      if ((int)((uint)*(byte *)(param_5 + 0x34) << 0x1e) < 0) {
        uVar3 = 0xff - uVar3;
      }
      uVar1 = FUN_08035164(*(undefined1 *)(param_1 + param_2),uVar3);
      *(undefined1 *)(param_1 + param_2) = uVar1;
    }
    iVar4 = param_2 + 1;
  }
  do {
    if (iVar2 <= *(int *)(param_5 + 0x30)) break;
    if ((-1 < iVar4) && (iVar4 < param_4)) {
      uVar3 = iVar2 - (*(int *)(param_5 + 0x30) >> 1) & 0xff;
      if ((int)((uint)*(byte *)(param_5 + 0x34) << 0x1e) < 0) {
        uVar3 = 0xff - uVar3;
      }
      uVar1 = FUN_08035164(*(undefined1 *)(param_1 + iVar4),uVar3);
      *(undefined1 *)(param_1 + iVar4) = uVar1;
    }
    iVar2 = iVar2 - *(int *)(param_5 + 0x30);
    iVar4 = iVar4 + 1;
  } while (iVar4 < param_4);
  if ((iVar4 < param_4) && (-1 < iVar4)) {
    uVar3 = (iVar2 * (iVar2 * *(int *)(param_5 + 0x24) >> 10) & 0x1ffffU) >> 9;
    if (*(int *)(param_5 + 0x28) < 0) {
      uVar3 = 0xff - uVar3;
    }
    if ((int)((uint)*(byte *)(param_5 + 0x34) << 0x1e) < 0) {
      uVar3 = 0xff - uVar3;
    }
    uVar1 = FUN_08035164(*(undefined1 *)(param_1 + iVar4),uVar3);
    *(undefined1 *)(param_1 + iVar4) = uVar1;
  }
  if ((int)((uint)*(byte *)(param_5 + 0x34) << 0x1e) < 0) {
    if (param_4 < param_2) {
      return 0;
    }
    if (-1 < param_2) {
      FUN_0802d8d6(param_1);
    }
  }
  else {
    iVar4 = iVar4 + 1;
    if (iVar4 < 0) {
      return 0;
    }
    if (iVar4 <= param_4) {
      FUN_0802d8d6(param_1 + iVar4,param_4 - iVar4);
    }
  }
  return 2;
}

