// 0x08018c06 FUN_08018c06 size=10

void FUN_08018c06(int param_1,uint param_2)

{
  *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xfffffffe | param_2 & 1;
  return;
}

