// 0x08039908 FUN_08039908 size=8

void FUN_08039908(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0x14) = *(uint *)(param_1 + 0x14) | param_2;
  return;
}

