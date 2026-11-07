// 0x080305d2 FUN_080305d2 size=156

void FUN_080305d2(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = FUN_0802f0a8(param_1);
  if (iVar1 == 0) {
    iVar1 = FUN_0802f0bc(param_1);
    iVar2 = FUN_0802ee28(param_1);
    if ((iVar2 < 0) && (0 < iVar1)) {
      if (-iVar2 <= iVar1) {
        iVar1 = -iVar2;
      }
      FUN_08030f80(param_1,0,iVar1,param_2);
    }
  }
  iVar1 = FUN_0802f094(param_1);
  if (iVar1 == 0) {
    iVar1 = FUN_0802ef1c(param_1);
    iVar2 = FUN_0802efdc(param_1);
    iVar3 = FUN_0802f7ec(param_1,0);
    if (iVar3 == 1) {
      if ((iVar1 < 0) && (0 < iVar2)) {
        FUN_08030f80(param_1,iVar1,0,param_2);
        return;
      }
    }
    else if ((iVar2 < 0) && (0 < iVar1)) {
      if (-iVar2 <= iVar1) {
        iVar1 = -iVar2;
      }
      FUN_08030f80(param_1,iVar1,0,param_2);
      return;
    }
  }
  return;
}

