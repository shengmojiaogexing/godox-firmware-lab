// 0x0801be28 FUN_0801be28 size=24

void FUN_0801be28(uint param_1)

{
  if (param_1 == 1) {
    FUN_0801bce8();
  }
  *DAT_0801be40 = *DAT_0801be40 & 0xffffdfff | (param_1 & 1) << 0xd;
  return;
}

