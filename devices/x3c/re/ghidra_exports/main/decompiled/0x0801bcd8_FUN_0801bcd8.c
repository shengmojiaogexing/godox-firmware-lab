// 0x0801bcd8 FUN_0801bcd8 size=12

void FUN_0801bcd8(uint param_1)

{
  *DAT_0801bce4 = *DAT_0801bce4 & 0xffffefff | (param_1 & 1) << 0xc;
  return;
}

