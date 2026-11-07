// 0x0802b744 FUN_0802b744 size=16

uint FUN_0802b744(int param_1)

{
  if (param_1 == 0) {
    return 0;
  }
  return (*(byte *)(param_1 + 0x1c) & 3) >> 1;
}

