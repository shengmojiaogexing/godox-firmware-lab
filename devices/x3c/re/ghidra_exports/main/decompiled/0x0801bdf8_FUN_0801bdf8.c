// 0x0801bdf8 FUN_0801bdf8 size=12

void FUN_0801bdf8(uint param_1)

{
  *DAT_0801be04 = *DAT_0801be04 & 0xfffffffc | param_1 & 3;
  return;
}

