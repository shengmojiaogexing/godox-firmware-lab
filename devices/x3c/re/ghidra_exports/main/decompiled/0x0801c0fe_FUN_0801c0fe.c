// 0x0801c0fe FUN_0801c0fe size=24

void FUN_0801c0fe(int param_1,undefined4 param_2)

{
  undefined4 extraout_r1;
  int extraout_r2;
  undefined4 uVar1;
  undefined4 extraout_r3;
  
  uVar1 = 1000;
  while (param_1 != 0) {
    FUN_0801c118(uVar1,param_2,param_1 - 1U & 0xffff);
    param_1 = extraout_r2;
    uVar1 = extraout_r3;
    param_2 = extraout_r1;
  }
  return;
}

