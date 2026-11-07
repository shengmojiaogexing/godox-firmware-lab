// 0x0801801e FUN_0801801e size=136

void FUN_0801801e(int param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 extraout_r2;
  undefined4 extraout_r2_00;
  int extraout_r3;
  int extraout_r3_00;
  undefined4 extraout_r3_01;
  undefined8 uVar4;
  
  if (param_1 != 0) {
    uVar4 = FUN_08017f30(param_1,param_2,param_3,param_2);
    if ((int)uVar4 != extraout_r3) {
      iVar1 = FUN_08017f70(param_1,(int)((ulonglong)uVar4 >> 0x20),0);
      if (iVar1 != extraout_r3_00) {
        uVar2 = FUN_08017f68(param_1,extraout_r3_00);
        uVar3 = FUN_08017f5e(param_1,extraout_r3_01);
        FUN_08035c34(param_1,uVar2,uVar3);
        FUN_08035c46(param_1,uVar3,uVar2);
        return;
      }
      iVar1 = FUN_08017f68(param_1,extraout_r3_00);
      *(int *)(param_1 + 8) = iVar1;
      if (iVar1 == 0) {
        *(undefined4 *)(param_1 + 4) = extraout_r2_00;
        return;
      }
      FUN_08035c34(param_1,iVar1,0);
      return;
    }
    iVar1 = FUN_08017f5e(param_1,extraout_r3);
    *(int *)(param_1 + 4) = iVar1;
    if (iVar1 != 0) {
      FUN_08035c46(param_1,iVar1,0);
      return;
    }
    *(undefined4 *)(param_1 + 8) = extraout_r2;
  }
  return;
}

