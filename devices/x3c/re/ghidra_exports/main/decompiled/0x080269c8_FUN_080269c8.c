// 0x080269c8 FUN_080269c8 size=50

void FUN_080269c8(int param_1)

{
  FUN_0802d8d6(param_1,0x44);
  *(undefined4 *)(param_1 + 0x2c) = 500;
  *(undefined4 *)(param_1 + 0x20) = 0;
  *(undefined4 *)(param_1 + 0x28) = 100;
  *(undefined2 *)(param_1 + 0x40) = 1;
  *(undefined4 *)(param_1 + 0x1c) = DAT_080269fc;
  *(byte *)(param_1 + 0x42) = *(byte *)(param_1 + 0x42) | 1;
  return;
}

