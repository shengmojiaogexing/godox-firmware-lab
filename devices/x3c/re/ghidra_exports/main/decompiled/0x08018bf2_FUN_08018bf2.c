// 0x08018bf2 FUN_08018bf2 size=10

void FUN_08018bf2(int param_1,uint param_2)

{
  *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xfffffeff | (param_2 & 1) << 8;
  return;
}

