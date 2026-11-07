// 0x08018c88 FUN_08018c88 size=10

void FUN_08018c88(int param_1,uint param_2)

{
  *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xbfffffff | (param_2 & 1) << 0x1e;
  return;
}

