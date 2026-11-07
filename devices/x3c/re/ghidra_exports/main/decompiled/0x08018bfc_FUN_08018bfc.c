// 0x08018bfc FUN_08018bfc size=10

void FUN_08018bfc(int param_1,uint param_2)

{
  *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xfffffdff | (param_2 & 1) << 9;
  return;
}

