// 0x08018ca6 FUN_08018ca6 size=10

void FUN_08018ca6(int param_1,uint param_2)

{
  *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & 0xfffffbff | (param_2 & 1) << 10;
  return;
}

