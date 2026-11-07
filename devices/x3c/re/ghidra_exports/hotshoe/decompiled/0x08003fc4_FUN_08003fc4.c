// 0x08003fc4 FUN_08003fc4 size=12

void FUN_08003fc4(int param_1,byte param_2)

{
  *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & (uint)(byte)~param_2;
  return;
}

