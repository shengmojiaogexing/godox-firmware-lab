// 0x08018c70 FUN_08018c70 size=12

void FUN_08018c70(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0x80) = *(uint *)(param_1 + 0x80) & 0xfffffbff | (param_2 & 1) << 10;
  return;
}

