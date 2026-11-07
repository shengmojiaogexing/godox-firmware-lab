// 0x080311f0 FUN_080311f0 size=38

void FUN_080311f0(undefined4 param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  FUN_08026938(param_1,DAT_08031218);
  iVar6 = FUN_0802f0cc(param_1);
  if (iVar6 - param_2 != 0) {
    FUN_080317f8(param_1);
    iVar1 = FUN_0802f0cc(param_1);
    iVar6 = (iVar6 - param_2) - iVar1;
    iVar2 = FUN_0802f7ec(param_1,0);
    if (iVar2 == 1) {
      if (iVar6 < 0) {
        iVar6 = 0;
      }
      if (0 < iVar6) {
        iVar2 = FUN_0802ef1c(param_1);
        iVar3 = FUN_0802efdc(param_1);
        iVar2 = iVar2 + iVar3;
        if (iVar2 < 0) {
          iVar2 = 0;
        }
        if (iVar2 < iVar6) {
          iVar6 = iVar2;
        }
      }
    }
    else {
      if (0 < iVar6) {
        iVar6 = 0;
      }
      if (iVar6 < 0) {
        iVar2 = FUN_0802ef1c(param_1);
        iVar3 = FUN_0802efdc(param_1);
        iVar2 = iVar2 + iVar3;
        if (iVar2 < 0) {
          iVar2 = 0;
        }
        if (iVar6 < -iVar2) {
          iVar6 = -iVar2;
        }
      }
    }
    iVar3 = FUN_0802f0dc(param_1);
    iVar2 = -iVar3;
    if (0 < iVar2) {
      iVar2 = 0;
    }
    if (iVar2 < 0) {
      iVar4 = FUN_0802f0bc(param_1);
      iVar5 = FUN_0802ee28(param_1);
      iVar4 = iVar4 + iVar5;
      if (iVar4 < 0) {
        iVar4 = 0;
      }
      if (iVar2 < -iVar4) {
        iVar2 = -iVar4;
      }
    }
    if (iVar6 + iVar1 != 0 || iVar2 + iVar3 != 0) {
      FUN_08030f80(param_1,iVar6 + iVar1,iVar2 + iVar3,param_3);
      return;
    }
  }
  return;
}

