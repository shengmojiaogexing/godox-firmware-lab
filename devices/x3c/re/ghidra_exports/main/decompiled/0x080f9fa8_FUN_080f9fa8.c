// 0x080f9fa8 FUN_080f9fa8 size=18

void FUN_080f9fa8(int param_1,uint param_2)

{
  *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & 0xffffefff | (param_2 & 1) << 0xc;
  return;
}

