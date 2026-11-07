// 0x0802074c FUN_0802074c size=38

undefined4 FUN_0802074c(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  int extraout_r2;
  int extraout_r3;
  undefined8 uVar2;
  
  iVar1 = 0x360000;
  while( true ) {
    uVar2 = FUN_0802052c(param_1,param_2,iVar1);
    param_2 = (undefined4)((ulonglong)uVar2 >> 0x20);
    if ((int)uVar2 != extraout_r3) {
      return 1;
    }
    if (extraout_r2 == 0) break;
    iVar1 = extraout_r2 + -1;
  }
  FUN_080207b4();
  return 0;
}

