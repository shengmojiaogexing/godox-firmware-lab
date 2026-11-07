// 0x080f9fc4 FUN_080f9fc4 size=12

void FUN_080f9fc4(int param_1,byte param_2)

{
  *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & (uint)(byte)~param_2;
  return;
}

