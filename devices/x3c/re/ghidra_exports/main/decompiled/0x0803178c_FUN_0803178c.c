// 0x0803178c FUN_0803178c size=106

void FUN_0803178c(int param_1,undefined4 param_2,int param_3,int param_4)

{
  int iVar1;
  undefined4 uVar2;
  
  while( true ) {
    if (param_1 == 0) {
      return;
    }
    iVar1 = FUN_080180d0(param_1);
    if (param_4 != 0) break;
    if (iVar1 == 2) {
      FUN_08039fc8(param_1,param_2,0);
    }
    if (param_3 == 0) {
      return;
    }
    param_1 = FUN_0802ee0a(param_1);
  }
  if (param_3 != 0) {
    uVar2 = FUN_0802ee0a(param_1);
    FUN_0803178c(uVar2,param_2,param_3,param_4);
  }
  if (iVar1 == 2) {
    FUN_08039fc8(param_1,param_2,param_4);
    return;
  }
  return;
}

