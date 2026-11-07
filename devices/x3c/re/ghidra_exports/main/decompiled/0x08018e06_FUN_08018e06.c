// 0x08018e06 FUN_08018e06 size=52

void FUN_08018e06(int param_1,uint param_2,uint param_3)

{
  if (param_2 < 0x10) {
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xff7fffff;
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xfff0ffff | (param_2 & 0xf) << 0x10;
  }
  else {
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) | 0x800000;
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xfff0ffff | (param_2 & 0xf) << 0x10;
  }
  *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xffcfffff | (param_3 & 3) << 0x14;
  return;
}

