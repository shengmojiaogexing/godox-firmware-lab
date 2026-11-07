// 0x0802afdc FUN_0802afdc size=64

undefined4 FUN_0802afdc(undefined4 *param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = param_1[1];
  if (iVar1 == 0) {
    return 0xb;
  }
  if (*(code **)(iVar1 + 0xc) == (code *)0x0) {
    return 9;
  }
  uVar2 = (**(code **)(iVar1 + 0xc))(iVar1,*param_1);
  if ((*(short *)(param_1[1] + 2) != 0) && (param_1[2] != 0)) {
    if (*(int *)(param_1[2] + 0xc) != 0) {
      FUN_0802d6e4();
    }
    FUN_0802d6e4(param_1[2]);
  }
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  return uVar2;
}

