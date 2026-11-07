// 0x080381da FUN_080381da size=60

undefined8 FUN_080381da(undefined4 *param_1,int param_2)

{
  undefined4 uVar1;
  
  if (param_2 == 0x10) {
    param_1[2] = 0xffffffef;
    return CONCAT44(0xffffffef,param_1);
  }
  if (param_2 == 1) {
    return CONCAT44(1,param_1[3]);
  }
  if (param_2 == 8) {
    return CONCAT44(8,param_1[2]);
  }
  if (param_2 == 0x100) {
    return CONCAT44(0x100,param_1[2]);
  }
  if (param_2 == 0x20) {
    uVar1 = *param_1;
    *param_1 = uVar1;
    return CONCAT44(uVar1,param_1);
  }
  if (param_2 == 0x40) {
    param_2 = param_1[3];
    param_1 = (undefined4 *)param_1[2];
  }
  return CONCAT44(param_2,param_1);
}

