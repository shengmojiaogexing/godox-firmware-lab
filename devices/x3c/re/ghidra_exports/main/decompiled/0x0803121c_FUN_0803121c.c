// 0x0803121c FUN_0803121c size=38

void FUN_0803121c(undefined4 param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  FUN_08026938(param_1,DAT_08031244);
  iVar5 = FUN_0802f0dc(param_1);
  if (iVar5 - param_2 != 0) {
    FUN_080317f8(param_1);
    iVar1 = FUN_0802f0cc(param_1);
    iVar6 = -iVar1;
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
    iVar2 = FUN_0802f0dc(param_1);
    iVar5 = (iVar5 - param_2) - iVar2;
    if (0 < iVar5) {
      iVar5 = 0;
    }
    if (iVar5 < 0) {
      iVar3 = FUN_0802f0bc(param_1);
      iVar4 = FUN_0802ee28(param_1);
      iVar3 = iVar3 + iVar4;
      if (iVar3 < 0) {
        iVar3 = 0;
      }
      if (iVar5 < -iVar3) {
        iVar5 = -iVar3;
      }
    }
    if (iVar6 + iVar1 != 0 || iVar5 + iVar2 != 0) {
      FUN_08030f80(param_1,iVar6 + iVar1,iVar5 + iVar2,param_3);
      return;
    }
  }
  return;
}

