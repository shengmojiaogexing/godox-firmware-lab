// 0x08003fa8 FUN_08003fa8 size=18

void FUN_08003fa8(int param_1,uint param_2)

{
  *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & 0xffffefff | (param_2 & 1) << 0xc;
  return;
}

