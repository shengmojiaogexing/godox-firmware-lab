// 0x080336ca FUN_080336ca size=24

int FUN_080336ca(uint *param_1)

{
  uint uVar1;
  
  uVar1 = FUN_080334c0(param_1[1]);
  if (uVar1 < *param_1) {
    return *param_1 - uVar1;
  }
  return 0;
}

