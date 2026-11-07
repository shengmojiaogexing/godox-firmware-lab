// 0x08018c7c FUN_08018c7c size=12

void FUN_08018c7c(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0x80) = *(uint *)(param_1 + 0x80) & 0xfffffdff | (param_2 & 1) << 9;
  return;
}

