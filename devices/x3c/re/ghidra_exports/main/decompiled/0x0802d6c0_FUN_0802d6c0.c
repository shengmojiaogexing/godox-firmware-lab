// 0x0802d6c0 FUN_0802d6c0 size=32

void FUN_0802d6c0(int param_1)

{
  uint uVar1;
  
  uVar1 = 0;
  do {
    if (*(int *)(DAT_0802d6e0 + uVar1 * 8) == param_1) {
      *(undefined1 *)(DAT_0802d6e0 + uVar1 * 8 + 6) = 0;
      return;
    }
    uVar1 = uVar1 + 1 & 0xff;
  } while (uVar1 < 0x10);
  return;
}

