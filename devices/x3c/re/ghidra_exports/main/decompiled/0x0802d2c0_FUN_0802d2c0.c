// 0x0802d2c0 FUN_0802d2c0 size=114

void FUN_0802d2c0(int param_1,int param_2)

{
  FUN_08026938(param_1,DAT_0802d334);
  FUN_08026938(param_1,DAT_0802d338);
  *(undefined4 *)(param_1 + 0x4c) = 0;
  *(undefined4 *)(param_1 + 0x50) = 0;
  if (((param_2 == 2) || (param_2 == 3)) || (param_2 == 4)) {
    *(byte *)(param_1 + 0x54) = *(byte *)(param_1 + 0x54) | 0x20;
  }
  else {
    *(byte *)(param_1 + 0x54) = *(byte *)(param_1 + 0x54) & 0xdf;
  }
  if (((*(byte *)(param_1 + 0x54) & 7) == 1) && (*(int *)(param_1 + 0x34) != -1)) {
    FUN_0802d20c(param_1);
  }
  *(byte *)(param_1 + 0x54) = *(byte *)(param_1 + 0x54) & 0xf8 | (byte)param_2 & 7;
  FUN_0802cd80(param_1);
  return;
}

