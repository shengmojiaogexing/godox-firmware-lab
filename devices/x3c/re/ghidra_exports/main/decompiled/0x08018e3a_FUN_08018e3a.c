// 0x08018e3a FUN_08018e3a size=12

void FUN_08018e3a(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0x80) = *(uint *)(param_1 + 0x80) & 0xfffffffd | (param_2 & 1) << 1;
  return;
}

