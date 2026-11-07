// 0x0803a86c FUN_0803a86c size=14

void FUN_0803a86c(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0x800) = *(uint *)(param_1 + 0x800) & 0xfffff80f | (param_2 & 0x7f) << 4;
  return;
}

