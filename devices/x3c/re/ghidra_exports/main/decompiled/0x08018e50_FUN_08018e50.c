// 0x08018e50 FUN_08018e50 size=10

void FUN_08018e50(int param_1,uint param_2)

{
  *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & 0xfcffffff | (param_2 & 3) << 0x18;
  return;
}

