// 0x0802e024 FUN_0802e024 size=212

int FUN_0802e024(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  iVar7 = 0;
  iVar1 = FUN_0802fa70(param_1,param_2,param_3,param_4,param_4);
  if ((iVar1 != 0) && (uVar2 = FUN_0802fa5e(param_1,param_2), 2 < uVar2)) {
    iVar3 = FUN_0802fa6a(param_1,param_2);
    iVar4 = FUN_0802fa52(param_1,param_2);
    iVar7 = FUN_0802fa58(param_1,param_2);
    if (iVar4 < 1) {
      iVar5 = -iVar4;
    }
    else {
      iVar5 = iVar4;
    }
    if (iVar7 < 1) {
      iVar6 = -iVar7;
    }
    else {
      iVar6 = iVar7;
    }
    if (iVar6 < iVar5) {
      if (iVar4 < 1) {
        iVar7 = -iVar4;
      }
      else {
        iVar7 = iVar4;
      }
    }
    else if (iVar7 < 1) {
      iVar7 = -iVar7;
    }
    iVar7 = iVar7 + iVar3 + iVar1 / 2 + 1;
    if (iVar7 < 0) {
      iVar7 = 0;
    }
  }
  iVar1 = FUN_0802f8f6(param_1,param_2);
  if (iVar1 != 0) {
    uVar2 = FUN_0802f8e4(param_1,param_2);
    if (2 < uVar2) {
      iVar3 = FUN_0802f8f0(param_1,param_2);
      if (iVar7 <= iVar3 + iVar1) {
        iVar7 = iVar3 + iVar1;
      }
    }
  }
  iVar3 = FUN_0802f9bc(param_1,param_2,0x68);
  iVar1 = FUN_0802f9bc(param_1,param_2,0x69);
  if (iVar1 < iVar3) {
    iVar1 = iVar3;
  }
  if (0 < iVar1) {
    iVar7 = iVar7 + iVar1;
  }
  return iVar7;
}

