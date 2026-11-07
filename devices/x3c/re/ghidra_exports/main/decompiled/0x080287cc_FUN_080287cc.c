// 0x080287cc FUN_080287cc size=50

void FUN_080287cc(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined2 uVar1;
  
  FUN_0802d8d6(param_1,0x1c,param_3,param_4,param_3,param_4);
  uVar1 = FUN_08027b80(0,0);
  *(undefined2 *)(param_1 + 0xc) = uVar1;
  *(undefined1 *)(param_1 + 0xf) = 0xff;
  *(undefined2 *)(param_1 + 2) = 0x100;
  *(undefined1 *)(param_1 + 0x18) = 1;
  return;
}

