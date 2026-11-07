// 0x0800cf68 FUN_0800cf68 size=46

void FUN_0800cf68(void)

{
  if (*(int *)(DAT_0800cf98 + 0xc) == DAT_0800cf9c) {
    FUN_08035bc4(0x29);
    FUN_0801c118(2);
    *DAT_0800cfa0 = *DAT_0800cfa0 | 0x10;
    return;
  }
  FUN_080143c0(0x29);
  return;
}

