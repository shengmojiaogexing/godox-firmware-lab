// 0x0801c33c FUN_0801c33c size=28

undefined4 FUN_0801c33c(uint param_1)

{
  uint uVar1;
  
  if (param_1 < 0x10000001) {
    uVar1 = *DAT_0801c35c;
  }
  else {
    uVar1 = *DAT_0801c358;
  }
  if ((uVar1 & param_1) == 0) {
    return 0;
  }
  return 1;
}

