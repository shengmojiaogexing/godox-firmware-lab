// 0x08029644 FUN_08029644 size=602

undefined4 FUN_08029644(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  undefined1 uVar1;
  undefined1 uVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int local_3c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  
  iVar8 = (uint)*(byte *)(param_5 + 0x1c) << 0x1f;
  iVar5 = *(int *)(param_5 + 0x18);
  FUN_08027430(&local_34,param_5 + 8);
  if (iVar8 < 0) {
    if ((param_3 < local_30) || (local_28 < param_3)) {
      return 1;
    }
  }
  else if ((param_3 < local_30) || (local_28 < param_3)) {
    return 0;
  }
  if (((param_2 < local_34 + iVar5) || (local_2c - iVar5 < param_2 + param_4)) &&
     ((param_3 < local_30 + iVar5 || (local_28 - iVar5 < param_3)))) {
    param_3 = param_3 - local_30;
    if (param_3 < iVar5) {
      param_3 = (iVar5 - param_3) + -1;
    }
    else {
      param_3 = param_3 - (((local_28 - local_30) + 1) - iVar5);
    }
    iVar4 = FUN_08022510(*(undefined4 *)(param_5 + 0x20),param_3,&local_38,&local_3c);
    iVar7 = (((local_34 - param_2) + (local_2c - local_34) + 1) - iVar5) + local_3c;
    iVar5 = ((local_34 - param_2) + iVar5) - local_3c;
    iVar6 = iVar5 + -1;
    if (iVar8 < 0) {
      for (iVar8 = 0; iVar8 < local_38; iVar8 = iVar8 + 1) {
        iVar10 = 0xff - (uint)*(byte *)((local_38 - iVar8) + iVar4 + -1);
        iVar9 = iVar7 + iVar8;
        if ((-1 < iVar9) && (iVar9 < param_4)) {
          uVar2 = FUN_08035164(iVar10,*(undefined1 *)(param_1 + iVar9));
          *(undefined1 *)(param_1 + iVar9) = uVar2;
        }
        iVar9 = iVar6 - iVar8;
        if ((-1 < iVar9) && (iVar9 < param_4)) {
          uVar2 = FUN_08035164(iVar10,*(undefined1 *)(param_1 + iVar9));
          *(undefined1 *)(param_1 + iVar9) = uVar2;
        }
      }
      iVar8 = iVar5;
      if (param_4 <= iVar5) {
        iVar8 = param_4;
      }
      if (iVar8 < 0) {
        iVar5 = 0;
      }
      else if (iVar5 < param_4) {
      }
      else {
        iVar5 = param_4;
      }
      iVar7 = iVar7 - iVar5;
      param_4 = param_4 - iVar5;
      if (iVar7 < param_4) {
        iVar8 = iVar7;
      }
      else {
        iVar8 = param_4;
      }
      if (iVar8 < 0) {
        iVar7 = 0;
      }
      else if (param_4 <= iVar7) {
        iVar7 = param_4;
      }
      FUN_0802d8d6(param_1 + iVar5,iVar7);
    }
    else {
      for (iVar5 = 0; iVar5 < local_38; iVar5 = iVar5 + 1) {
        uVar2 = *(undefined1 *)((local_38 - iVar5) + iVar4 + -1);
        iVar8 = iVar7 + iVar5;
        if ((-1 < iVar8) && (iVar8 < param_4)) {
          uVar1 = FUN_08035164(uVar2,*(undefined1 *)(param_1 + iVar8));
          *(undefined1 *)(param_1 + iVar8) = uVar1;
        }
        iVar8 = iVar6 - iVar5;
        if ((-1 < iVar8) && (iVar8 < param_4)) {
          uVar2 = FUN_08035164(uVar2,*(undefined1 *)(param_1 + iVar8));
          *(undefined1 *)(param_1 + iVar8) = uVar2;
        }
      }
      iVar7 = iVar7 + iVar5;
      if (iVar7 < param_4) {
        iVar5 = iVar7;
      }
      else {
        iVar5 = param_4;
      }
      if (iVar5 < 0) {
        iVar7 = 0;
      }
      else if (param_4 <= iVar7) {
        iVar7 = param_4;
      }
      FUN_0802d8d6(iVar7 + param_1,param_4 - iVar7);
      iVar6 = iVar6 - local_38;
      if (iVar6 + 1 < param_4) {
        iVar5 = iVar6 + 1;
      }
      else {
        iVar5 = param_4;
      }
      if (iVar5 < 0) {
        param_4 = 0;
      }
      else if (iVar6 + 1 < param_4) {
        param_4 = iVar6 + 1;
      }
      FUN_0802d8d6(param_1,param_4);
    }
    uVar3 = 2;
  }
  else if (iVar8 < 0) {
    local_34 = local_34 - param_2;
    if (local_34 < 0) {
      local_34 = 0;
    }
    if (local_34 <= param_4) {
      iVar5 = ((local_2c - param_2) - local_34) + 1;
      if (param_4 < local_34 + iVar5) {
        iVar5 = param_4 - local_34;
      }
      if (-1 < iVar5) {
        FUN_0802d8d6(local_34 + param_1);
      }
    }
    uVar3 = 2;
  }
  else {
    iVar5 = local_34 - param_2;
    if (param_4 < iVar5) {
      uVar3 = 0;
    }
    else {
      if (-1 < iVar5) {
        FUN_0802d8d6(param_1,iVar5);
      }
      iVar8 = (local_2c - param_2) + 1;
      if (iVar8 < 1) {
        uVar3 = 0;
      }
      else {
        if (iVar8 < param_4) {
          FUN_0802d8d6(param_1 + iVar8,param_4 - iVar8);
        }
        if ((iVar5 == 0) && (iVar8 == param_4)) {
          uVar3 = 1;
        }
        else {
          uVar3 = 2;
        }
      }
    }
  }
  return uVar3;
}

