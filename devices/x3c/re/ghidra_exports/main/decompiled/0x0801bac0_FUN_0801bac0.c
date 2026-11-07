// 0x0801bac0 FUN_0801bac0 size=12

void FUN_0801bac0(uint param_1)

{
  *DAT_0801bacc = *DAT_0801bacc & 0xfffeffff | (param_1 & 1) << 0x10;
  return;
}

