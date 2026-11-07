// 0x080f8fb6 FUN_080f8fb6 size=10

bool FUN_080f8fb6(int param_1,uint param_2)

{
  return (*(uint *)(param_1 + 0x10) & param_2) != 0;
}

