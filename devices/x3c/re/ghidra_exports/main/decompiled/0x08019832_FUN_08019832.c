// 0x08019832 FUN_08019832 size=12

void FUN_08019832(int param_1,uint param_2)

{
  *(uint *)(param_1 + 4) = *(byte *)(param_1 + 4) & 3 | param_2;
  return;
}

