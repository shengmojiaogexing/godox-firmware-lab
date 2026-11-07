// 0x08028d04 FUN_08028d04 size=76

void FUN_08028d04(undefined4 *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined2 uVar1;
  
  FUN_0802d8d6(param_1,0x2c,param_3,param_4,param_4);
  *(undefined1 *)(param_1 + 9) = 0xff;
  uVar1 = FUN_08027a3c();
  *(undefined2 *)(param_1 + 3) = uVar1;
  *param_1 = DAT_08028d50;
  param_1[1] = 0xffff;
  param_1[2] = 0xffff;
  uVar1 = FUN_08027a3c();
  *(undefined2 *)((int)param_1 + 0xe) = uVar1;
  uVar1 = FUN_080318a8(5);
  *(undefined2 *)(param_1 + 4) = uVar1;
  *(undefined1 *)((int)param_1 + 0x25) = 0;
  return;
}

