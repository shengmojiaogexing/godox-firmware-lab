// 0x0803a87e FUN_0803a87e size=62

void FUN_0803a87e(int param_1,int param_2,int param_3)

{
  uint uVar1;
  uint uVar2;
  
  uVar1 = 0;
  if (param_2 == 0) {
    *(uint *)(param_1 + 0x28) = *(uint *)(param_1 + 0x24) | param_3 << 0x10;
    return;
  }
  uVar2 = *(uint *)(param_1 + 0x24) + (*(uint *)(param_1 + 0x28) >> 0x10);
  for (; (int)uVar1 < param_2 + -1; uVar1 = uVar1 + 1 & 0xff) {
    uVar2 = uVar2 + (*(uint *)(param_1 + uVar1 * 4 + 0x104) >> 0x10);
  }
  *(uint *)(param_1 + (param_2 + -1) * 4 + 0x104) = uVar2 | param_3 << 0x10;
  return;
}

