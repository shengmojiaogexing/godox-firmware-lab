// 0x0802f76a FUN_0802f76a size=20

byte FUN_0802f76a(int param_1)

{
  if (*(int *)(param_1 + 8) == 0) {
    return 3;
  }
  return *(byte *)(*(int *)(param_1 + 8) + 0x20) & 3;
}

