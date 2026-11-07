// 0x0802eec8 FUN_0802eec8 size=20

byte FUN_0802eec8(int param_1)

{
  if (*(int *)(param_1 + 8) == 0) {
    return 0xf;
  }
  return *(byte *)(*(int *)(param_1 + 8) + 0x21) & 0xf;
}

