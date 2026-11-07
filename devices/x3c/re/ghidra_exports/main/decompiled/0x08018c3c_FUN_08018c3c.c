// 0x08018c3c FUN_08018c3c size=52

void FUN_08018c3c(int param_1,uint param_2,uint param_3)

{
  if (param_2 < 0x10) {
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0x7fffffff;
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xf0ffffff | (param_2 & 0xf) << 0x18;
  }
  else {
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) | 0x80000000;
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xf0ffffff | (param_2 & 0xf) << 0x18;
  }
  *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xcfffffff | (param_3 & 3) << 0x1c;
  return;
}

