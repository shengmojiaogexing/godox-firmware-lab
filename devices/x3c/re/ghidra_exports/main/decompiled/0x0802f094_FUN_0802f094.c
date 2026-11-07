// 0x0802f094 FUN_0802f094 size=20

uint FUN_0802f094(int param_1)

{
  if (*(int *)(param_1 + 8) == 0) {
    return 0;
  }
  return (*(byte *)(*(int *)(param_1 + 8) + 0x20) & 0xf) >> 2;
}

