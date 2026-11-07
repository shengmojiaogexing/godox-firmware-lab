// 0x080180d0 FUN_080180d0 size=18

byte FUN_080180d0(int param_1)

{
  if (*(int *)(param_1 + 8) == 0) {
    return 0;
  }
  return *(byte *)(*(int *)(param_1 + 8) + 0x22) >> 6;
}

