// 0x0802ca36 FUN_0802ca36 size=38

void FUN_0802ca36(int param_1)

{
  if (((int)((uint)*(byte *)(param_1 + 0x54) << 0x19) < 0) && (*(int *)(param_1 + 0x30) != 0)) {
    FUN_0802d6e4();
  }
  *(byte *)(param_1 + 0x54) = *(byte *)(param_1 + 0x54) & 0xbf;
  *(undefined4 *)(param_1 + 0x30) = 0;
  return;
}

