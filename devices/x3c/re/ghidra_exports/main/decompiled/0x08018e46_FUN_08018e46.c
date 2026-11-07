// 0x08018e46 FUN_08018e46 size=10

void FUN_08018e46(int param_1,uint param_2)

{
  *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xffbfffff | (param_2 & 1) << 0x16;
  return;
}

