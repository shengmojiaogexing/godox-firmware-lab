// 0x080398ba FUN_080398ba size=14

void FUN_080398ba(int param_1,undefined4 param_2,undefined4 param_3)

{
  *(undefined4 *)(param_1 + 0x2c) = param_2;
  *(undefined4 *)(param_1 + 0x28) = param_3;
  *(uint *)(param_1 + 0x14) = *(uint *)(param_1 + 0x14) | 1;
  return;
}

