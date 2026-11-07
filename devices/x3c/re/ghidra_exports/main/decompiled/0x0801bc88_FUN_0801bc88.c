// 0x0801bc88 FUN_0801bc88 size=30

bool FUN_0801bc88(uint param_1)

{
  return (1 << (param_1 & 0x1f) & ~*(uint *)(DAT_0801bca8 + (param_1 >> 0x10) + 0x800)) == 0;
}

