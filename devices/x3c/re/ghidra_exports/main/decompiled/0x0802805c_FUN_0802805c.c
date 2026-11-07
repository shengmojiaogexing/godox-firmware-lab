// 0x0802805c FUN_0802805c size=82

void FUN_0802805c(undefined4 *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  
  FUN_0802d8d6(param_1,0x5c,param_3,param_4,param_3,param_4);
  *param_1 = 0x140;
  param_1[1] = 0xf0;
  param_1[2] = 0xffffffff;
  param_1[3] = 0xffffffff;
  param_1[4] = 0;
  param_1[5] = 0;
  param_1[7] = param_1[7] & 0xfffe003f | 0x4108;
  *(undefined2 *)(param_1 + 0x11) = 0;
  uVar1 = DAT_080280b0;
  param_1[0x13] = DAT_080280b0;
  param_1[0x14] = uVar1;
  param_1[0x15] = 0x58;
  return;
}

