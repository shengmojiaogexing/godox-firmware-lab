// 0x0802f0a8 FUN_0802f0a8 size=20

uint FUN_0802f0a8(int param_1)

{
  if (*(int *)(param_1 + 8) == 0) {
    return 0;
  }
  return (*(byte *)(*(int *)(param_1 + 8) + 0x20) & 0x3f) >> 4;
}

