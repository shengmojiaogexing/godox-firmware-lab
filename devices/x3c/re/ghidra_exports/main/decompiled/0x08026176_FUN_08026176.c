// 0x08026176 FUN_08026176 size=538

undefined4 FUN_08026176(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  undefined1 uVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  
  iVar7 = *(int *)(param_5 + 0x24);
  iVar2 = iVar7 * param_3 >> 10;
  iVar3 = iVar2;
  if (0 < iVar7) {
    iVar3 = iVar2 + 1;
  }
  uVar6 = (uint)*(byte *)(param_5 + 0x34);
  if (iVar3 < param_2) {
    if ((int)(uVar6 << 0x1e) < 0) {
      uVar4 = 1;
    }
    else {
      uVar4 = 0;
    }
  }
  else if (param_2 + param_4 < iVar2) {
    if ((int)(uVar6 << 0x1e) < 0) {
      uVar4 = 0;
    }
    else {
      uVar4 = 1;
    }
  }
  else {
    iVar2 = iVar7 * param_3 * 0x100;
    iVar3 = iVar2 >> 0x12;
    uVar5 = iVar2 >> 10 & 0xff;
    iVar8 = iVar7 * (param_3 + 1) * 0x100;
    iVar2 = iVar8 >> 0x12;
    iVar9 = iVar3 - param_2;
    if (((iVar3 != iVar2) && (iVar7 < 0)) && (uVar5 == 0)) {
      uVar5 = 0xff;
      iVar9 = iVar9 + -1;
      iVar3 = iVar2;
    }
    if (iVar3 == iVar2) {
      if ((-1 < iVar9) && (iVar9 < param_4)) {
        uVar5 = uVar5 + (iVar8 >> 10 & 0xffU) >> 1;
        if ((int)(uVar6 << 0x1e) < 0) {
          uVar5 = 0xff - uVar5;
        }
        uVar1 = FUN_08035164(*(undefined1 *)(param_1 + iVar9),uVar5);
        *(undefined1 *)(param_1 + iVar9) = uVar1;
      }
      if ((int)((uint)*(byte *)(param_5 + 0x34) << 0x1e) < 0) {
        if (param_4 <= iVar3 - param_2) {
          return 0;
        }
        if (-1 < iVar3 - param_2) {
          FUN_0802d8d6(param_1);
        }
      }
      else {
        iVar3 = iVar9 + 1;
        if (param_4 < iVar9 + 1) {
          iVar3 = param_4;
        }
        if (iVar3 == 0) {
          return 0;
        }
        if (0 < iVar3) {
          FUN_0802d8d6(param_1 + iVar3,param_4 - iVar3);
        }
      }
    }
    else if (iVar7 < 0) {
      iVar2 = (int)(uVar5 * -*(int *)(param_5 + 0x28)) >> 10;
      if ((-1 < iVar9) && (iVar9 < param_4)) {
        uVar5 = (iVar2 * uVar5 & 0x1ffff) >> 9;
        if ((int)(uVar6 << 0x1e) < 0) {
          uVar5 = 0xff - uVar5;
        }
        uVar1 = FUN_08035164(*(undefined1 *)(param_1 + iVar9),uVar5);
        *(undefined1 *)(param_1 + iVar9) = uVar1;
      }
      iVar7 = iVar9 + -1;
      if ((-1 < iVar7) && (iVar7 < param_4)) {
        uVar6 = 0xffU - (((iVar2 + -0xff) * *(int *)(param_5 + 0x24) >> 10) * (0xff - iVar2) >> 9) &
                0xff;
        if ((int)((uint)*(byte *)(param_5 + 0x34) << 0x1e) < 0) {
          uVar6 = 0xff - uVar6;
        }
        uVar1 = FUN_08035164(*(undefined1 *)(param_1 + iVar7),uVar6);
        *(undefined1 *)(param_1 + iVar7) = uVar1;
      }
      iVar9 = iVar9 + 1;
      if ((int)((uint)*(byte *)(param_5 + 0x34) << 0x1e) < 0) {
        iVar3 = (iVar3 - param_2) + -1;
        if ((iVar3 <= param_4) && (0 < iVar3)) {
          FUN_0802d8d6(param_1);
        }
      }
      else {
        if (param_4 < iVar9) {
          return 1;
        }
        if (-1 < iVar9) {
          FUN_0802d8d6(param_1 + iVar9,param_4 - iVar9);
        }
      }
    }
    else {
      iVar2 = (int)((0xff - uVar5) * *(int *)(param_5 + 0x28)) >> 10;
      if ((-1 < iVar9) && (iVar9 < param_4)) {
        uVar5 = 0xffU - ((int)((0xff - uVar5) * iVar2) >> 9) & 0xff;
        if ((int)(uVar6 << 0x1e) < 0) {
          uVar5 = 0xff - uVar5;
        }
        uVar1 = FUN_08035164(*(undefined1 *)(param_1 + iVar9),uVar5);
        *(undefined1 *)(param_1 + iVar9) = uVar1;
      }
      iVar7 = iVar9 + 1;
      iVar2 = 0xff - iVar2;
      if ((-1 < iVar7) && (iVar7 < param_4)) {
        uVar6 = ((iVar2 * *(int *)(param_5 + 0x24) >> 10) * iVar2 & 0x1ffffU) >> 9;
        if ((int)((uint)*(byte *)(param_5 + 0x34) << 0x1e) < 0) {
          uVar6 = 0xff - uVar6;
        }
        uVar1 = FUN_08035164(*(undefined1 *)(param_1 + iVar7),uVar6);
        *(undefined1 *)(param_1 + iVar7) = uVar1;
      }
      if ((int)((uint)*(byte *)(param_5 + 0x34) << 0x1e) < 0) {
        if (param_4 < iVar3 - param_2) {
          return 0;
        }
        if (-1 < iVar3 - param_2) {
          FUN_0802d8d6(param_1);
        }
      }
      else {
        iVar3 = iVar9 + 2;
        if (param_4 < iVar9 + 2) {
          iVar3 = param_4;
        }
        if (iVar3 == 0) {
          return 0;
        }
        if (0 < iVar3) {
          FUN_0802d8d6(param_1 + iVar3,param_4 - iVar3);
        }
      }
    }
    uVar4 = 2;
  }
  return uVar4;
}

